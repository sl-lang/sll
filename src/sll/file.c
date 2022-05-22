#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/_internal/file.h>
#include <sll/api/hash.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/memory.h>
#include <sll/platform/file.h>
#include <sll/platform/lock.h>
#include <sll/platform/memory.h>
#include <sll/platform/path.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



#define LOCK SLL_CRITICAL_ERROR(sll_platform_lock_acquire(f->_lock))
#define UNLOCK SLL_CRITICAL(sll_platform_lock_release(f->_lock))



static sll_file_t _file_stdin;
static sll_file_t _file_stdout;
static sll_file_t _file_stderr;
static void* _file_buffer_pool[FILE_BUFFER_POOL_SIZE];
static sll_array_length_t _file_buffer_pool_len=0;



__SLL_EXTERNAL sll_file_t* sll_stdin=&_file_stdin;
__SLL_EXTERNAL sll_file_t* sll_stdout=&_file_stdout;
__SLL_EXTERNAL sll_file_t* sll_stderr=&_file_stderr;



static void _free_buffer(void* pg){
	if (_file_buffer_pool_len==FILE_BUFFER_POOL_SIZE){
		SLL_CRITICAL_ERROR(sll_platform_free_page(pg,SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE)));
		return;
	}
	_file_buffer_pool[_file_buffer_pool_len]=pg;
	_file_buffer_pool_len++;
}



static void* _get_buffer(void){
	if (!_file_buffer_pool_len){
		return sll_platform_allocate_page(SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE),1,NULL);
	}
	_file_buffer_pool_len--;
	return _file_buffer_pool[_file_buffer_pool_len];
}



void _file_end_hash(sll_file_t* f){
	if (!(f->flags&SLL_FILE_FLAG_READ)||f->_hash.buffer_offset==DISABLE_FILE_HASH){
		return;
	}
	sll_char_t tmp[128];
	sll_set_memory(tmp,128,0);
	sll_copy_data(f->_hash.buffer,f->_hash.buffer_offset,tmp);
	__SLL_U8 offset=(f->_hash.buffer_offset<56?56:120);
	tmp[f->_hash.buffer_offset]=128;
	tmp[offset]=(f->_offset>>53)&0xff;
	tmp[offset+1]=(f->_offset>>45)&0xff;
	tmp[offset+2]=(f->_offset>>37)&0xff;
	tmp[offset+3]=(f->_offset>>29)&0xff;
	tmp[offset+4]=(f->_offset>>21)&0xff;
	tmp[offset+5]=(f->_offset>>13)&0xff;
	tmp[offset+6]=(f->_offset>>5)&0xff;
	tmp[offset+7]=(f->_offset<<3)&0xff;
	sll_hash_sha256(&(f->_hash.hash),tmp,(f->_hash.buffer_offset<56?64:128));
}



void _file_init_std_streams(void){
	sll_file_open_descriptor(SLL_CHAR("sll_stdin"),sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_INPUT),SLL_FILE_FLAG_READ|SLL_FILE_FLAG_NO_BUFFER,sll_stdin);
	sll_file_open_descriptor(SLL_CHAR("sll_stdout"),sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_OUTPUT),SLL_FILE_FLAG_WRITE|SLL_FILE_FLUSH_ON_NEWLINE,sll_stdout);
	sll_file_open_descriptor(SLL_CHAR("sll_stderr"),sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_ERROR),SLL_FILE_FLAG_WRITE|SLL_FILE_FLUSH_ON_NEWLINE,sll_stderr);
	(*((sll_file_flags_t*)(&(sll_stdin->flags))))|=FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stdout->flags))))|=FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stderr->flags))))|=FILE_FLAG_NO_RELEASE;
}



void _file_release_std_streams(void){
	(*((sll_file_flags_t*)(&(sll_stdin->flags))))&=~FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stdout->flags))))&=~FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stderr->flags))))&=~FILE_FLAG_NO_RELEASE;
	sll_file_close(sll_stdin);
	sll_file_close(sll_stdout);
	sll_file_close(sll_stderr);
	while (_file_buffer_pool_len){
		_file_buffer_pool_len--;
		SLL_CRITICAL_ERROR(sll_platform_free_page(_file_buffer_pool[_file_buffer_pool_len],SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE)));
	}
}



void _file_start_hash(sll_file_t* f){
	if (!(f->flags&SLL_FILE_FLAG_READ)||(!(f->flags&FILE_FLAG_MEMORY)&&f->source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR)){
		return;
	}
	SLL_INIT_SHA256(&(f->_hash.hash));
	f->_hash.buffer_offset=0;
}



__SLL_EXTERNAL void sll_file_close(sll_file_t* f){
	sll_file_flush(f);
	if (f->flags&FILE_FLAG_NO_RELEASE){
		return;
	}
	SLL_CRITICAL(sll_platform_lock_delete(f->_lock));
	if (!(f->flags&FILE_FLAG_MEMORY)){
		sll_free_string((sll_string_t*)(&(f->source.file.path)));
		if (f->source.file.fd!=SLL_UNKNOWN_FILE_DESCRIPTOR){
			SLL_CRITICAL_ERROR(sll_platform_file_close(f->source.file.fd));
			if (!(f->flags&SLL_FILE_FLAG_NO_BUFFER)){
				if (f->flags&SLL_FILE_FLAG_READ){
					_free_buffer(f->_read_buffer);
				}
				if (f->flags&SLL_FILE_FLAG_WRITE){
					_free_buffer(f->_write_buffer.static_.pointer);
				}
			}
		}
	}
	else{
		SLL_ASSERT(f->flags&SLL_FILE_FLAG_NO_BUFFER);
		if (f->flags&FILE_FLAG_DYNAMIC_BUFFERS){
			dynamic_buffer_chunk_t* c=f->_write_buffer.dynamic.start;
			do{
				dynamic_buffer_chunk_t* n=c->next;
				SLL_CRITICAL_ERROR(sll_platform_free_page(c,c->size));
				c=n;
			} while (c);
		}
	}
	*((sll_file_flags_t*)(&(f->flags)))=FILE_FLAG_NO_RELEASE;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_file_data_available(sll_file_t* f){
	if ((f->flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_ASYNC))!=(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_ASYNC)){
		return 1;
	}
	return sll_platform_file_data_available(f->source.file.fd);
}



__SLL_EXTERNAL sll_error_t sll_file_flush(sll_file_t* f){
	if (!(f->flags&SLL_FILE_FLAG_WRITE)||(f->flags&SLL_FILE_FLAG_NO_BUFFER)||!f->_write_buffer.static_.offset){
		return SLL_NO_ERROR;
	}
	sll_error_t err=SLL_NO_ERROR;
	sll_platform_file_write(f->source.file.fd,f->_write_buffer.static_.pointer,f->_write_buffer.static_.offset,&err);
	f->_write_buffer.static_.offset=0;
	return err;
}



__SLL_EXTERNAL void sll_file_from_data(const void* ptr,sll_size_t sz,sll_file_flags_t flags,sll_file_t* o){
	flags&=~FILE_FLAG_DYNAMIC_BUFFERS;
	if (flags&SLL_FILE_FLAG_APPEND){
		SLL_UNIMPLEMENTED();
	}
	if (flags&SLL_FILE_FLAG_WRITE){
		flags|=FILE_FLAG_DYNAMIC_BUFFERS;
		dynamic_buffer_chunk_t* bf=sll_platform_allocate_page(SLL_ROUND_PAGE(FILE_DYNAMIC_BUFFER_ALLOC_SIZE),0,NULL);
		bf->size=SLL_ROUND_PAGE(FILE_DYNAMIC_BUFFER_ALLOC_SIZE);
		bf->next=NULL;
		o->_write_buffer.dynamic.start=bf;
		o->_write_buffer.dynamic.end=bf;
		o->_write_buffer.dynamic.size=0;
		o->_write_buffer.dynamic.offset=0;
	}
	*((const void**)(&(o->source.memory.pointer)))=ptr;
	*((sll_size_t*)(&(o->source.memory.size)))=sz;
	*((sll_file_flags_t*)(&(o->flags)))=(flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|FILE_FLAG_DYNAMIC_BUFFERS))|SLL_FILE_FLAG_NO_BUFFER|FILE_FLAG_MEMORY;
	o->_line_number=0;
	o->_offset=0;
	o->_hash.buffer_offset=DISABLE_FILE_HASH;
	o->_lock=sll_platform_lock_create(NULL);
}



__SLL_EXTERNAL void sll_file_get_buffer(sll_file_t* f,sll_string_t* o){
	if (!(f->flags&FILE_FLAG_DYNAMIC_BUFFERS)){
		SLL_INIT_STRING(o);
		return;
	}
	sll_string_create((sll_string_length_t)(f->_write_buffer.dynamic.size),o);
	dynamic_buffer_chunk_t* c=f->_write_buffer.dynamic.start;
	addr_t ptr=ADDR(o->data);
	sll_string_length_t l=o->length;
	do{
		sll_copy_data(c->data,(l>c->size?c->size:l),PTR(ptr));
		l-=(sll_string_length_t)(c->size);
		ptr+=c->size;
		c=c->next;
	} while (c);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL sll_error_t sll_file_open(const sll_char_t* name,sll_file_flags_t flags,sll_file_t* o){
	if ((flags&SLL_FILE_FLAG_READ)&&!sll_platform_path_exists(name)){
		return SLL_ERROR_NO_FILE_PATH;
	}
	sll_error_t err=SLL_NO_ERROR;
	sll_file_descriptor_t fd=sll_platform_file_open(name,flags,&err);
	if (fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		return err;
	}
	sll_file_open_descriptor(name,fd,flags,o);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL void sll_file_open_descriptor(const sll_char_t* nm,sll_file_descriptor_t fd,sll_file_flags_t flags,sll_file_t* o){
	*((sll_file_descriptor_t*)(&(o->source.file.fd)))=fd;
	sll_string_from_pointer(nm,(sll_string_t*)(&(o->source.file.path)));
	if (!(flags&SLL_FILE_FLAG_WRITE)){
		flags&=~SLL_FILE_FLUSH_ON_NEWLINE;
	}
	*((sll_file_flags_t*)(&(o->flags)))=flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|SLL_FILE_FLAG_NO_BUFFER|SLL_FILE_FLUSH_ON_NEWLINE);
	if (fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		*((sll_file_flags_t*)(&(o->flags)))=flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE);
		return;
	}
	if (sll_platform_file_async_read(fd)){
		*((sll_file_flags_t*)(&(o->flags)))|=SLL_FILE_FLAG_ASYNC;
	}
	o->_line_number=0;
	o->_offset=0;
	if (!(flags&SLL_FILE_FLAG_NO_BUFFER)){
		if (flags&SLL_FILE_FLAG_READ){
			o->_read_buffer=_get_buffer();
			o->_read_buffer_offset=0;
			o->_read_buffer_size=0;
		}
		if (flags&SLL_FILE_FLAG_WRITE){
			o->_write_buffer.static_.pointer=_get_buffer();
			o->_write_buffer.static_.offset=0;
		}
	}
	o->_hash.buffer_offset=DISABLE_FILE_HASH;
	o->_lock=sll_platform_lock_create(NULL);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_read_char_t sll_file_peek_char(sll_file_t* f,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->flags&SLL_FILE_FLAG_READ)){
		return SLL_END_OF_DATA;
	}
	if (f->flags&FILE_FLAG_MEMORY){
		LOCK;
		sll_read_char_t o=(f->_offset==f->source.memory.size?SLL_END_OF_DATA:*((sll_char_t*)(PTR(ADDR(f->source.memory.pointer)+f->_offset))));
		UNLOCK;
		return o;
	}
	if (f->source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return SLL_END_OF_DATA;
	}
	if (f->flags&SLL_FILE_FLAG_NO_BUFFER){
		SLL_UNIMPLEMENTED();
	}
	LOCK;
	if (!f->_read_buffer_offset&&!f->_read_buffer_size){
		f->_read_buffer_size=sll_platform_file_read(f->source.file.fd,f->_read_buffer,FILE_BUFFER_SIZE,err);
		if (!f->_read_buffer_size){
			UNLOCK;
			return SLL_END_OF_DATA;
		}
	}
	sll_char_t o=f->_read_buffer[f->_read_buffer_offset];
	UNLOCK;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_file_read(sll_file_t* f,void* ptr,sll_size_t sz,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->flags&SLL_FILE_FLAG_READ)||!sz){
		return 0;
	}
	sll_size_t o;
	if (f->flags&FILE_FLAG_MEMORY){
		LOCK;
		if (f->_offset+sz>=f->source.memory.size){
			sz=f->source.memory.size-f->_offset;
		}
		sll_copy_data(PTR(ADDR(f->source.memory.pointer)+f->_offset),sz,ptr);
		f->_offset+=sz;
		o=sz;
		goto _hash_data;
	}
	if (f->source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	LOCK;
	if (f->flags&SLL_FILE_FLAG_NO_BUFFER){
		sz=sll_platform_file_read(f->source.file.fd,ptr,sz,err);
		f->_offset+=sz;
		o=sz;
		goto _hash_data;
	}
	if (!f->_read_buffer_offset&&!f->_read_buffer_size){
		f->_read_buffer_size=sll_platform_file_read(f->source.file.fd,f->_read_buffer,FILE_BUFFER_SIZE,err);
		if (!f->_read_buffer_size){
			UNLOCK;
			return 0;
		}
	}
	if (f->_read_buffer_offset+sz<=f->_read_buffer_size){
		sll_copy_data(f->_read_buffer+f->_read_buffer_offset,sz,ptr);
		f->_read_buffer_offset+=sz;
		if (f->_read_buffer_offset==f->_read_buffer_size){
			f->_read_buffer_offset=0;
			f->_read_buffer_size=sll_platform_file_read(f->source.file.fd,f->_read_buffer,FILE_BUFFER_SIZE,err);
		}
		o=sz;
		goto _hash_data;
	}
	o=f->_read_buffer_size-f->_read_buffer_offset;
	sz-=o;
	sll_copy_data(f->_read_buffer+f->_read_buffer_offset,o,ptr);
	ptr=PTR(ADDR(ptr)+o);
	while (sz>FILE_BUFFER_SIZE){
		f->_read_buffer_size=sll_platform_file_read(f->source.file.fd,ptr,FILE_BUFFER_SIZE,err);
		if (f->_read_buffer_size!=FILE_BUFFER_SIZE){
			SLL_UNIMPLEMENTED();
			f->_read_buffer_offset=0;
			f->_read_buffer_size=0;
			goto _hash_data;
		}
		o+=FILE_BUFFER_SIZE;
		sz-=FILE_BUFFER_SIZE;
		ptr=PTR(ADDR(ptr)+FILE_BUFFER_SIZE);
	}
	f->_read_buffer_size=sll_platform_file_read(f->source.file.fd,f->_read_buffer,FILE_BUFFER_SIZE,err);
	if (f->_read_buffer_size<sz){
		o+=f->_read_buffer_size;
		sll_copy_data(f->_read_buffer,f->_read_buffer_size,ptr);
		f->_read_buffer_offset=0;
		f->_read_buffer_size=0;
		goto _hash_data;
	}
	o+=sz;
	f->_read_buffer_offset=sz;
	sll_copy_data(f->_read_buffer,sz,ptr);
_hash_data:
	if (f->_hash.buffer_offset!=DISABLE_FILE_HASH){
		SLL_UNIMPLEMENTED();
	}
	UNLOCK;
	return ZERO_IF_ERROR_PTR(o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_file_read_all(sll_file_t* f,sll_string_t* out){
	SLL_INIT_STRING(out);
	if (!(f->flags&SLL_FILE_FLAG_READ)){
		return SLL_NO_ERROR;
	}
	if (f->_hash.buffer_offset!=DISABLE_FILE_HASH){
		SLL_UNIMPLEMENTED();
	}
	if (f->flags&FILE_FLAG_MEMORY){
		SLL_UNIMPLEMENTED();
	}
	if (f->source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		return SLL_ERROR_UNKNOWN_FD;
	}
	LOCK;
	if (f->flags&SLL_FILE_FLAG_NO_BUFFER){
		SLL_UNIMPLEMENTED();
	}
	if (f->_read_buffer_size){
		SLL_UNIMPLEMENTED();
	}
	sll_error_t err;
	sll_string_length_t sz;
	do{
		sz=(sll_string_length_t)sll_platform_file_read(f->source.file.fd,f->_read_buffer,FILE_BUFFER_SIZE,&err);
		if (err!=SLL_NO_ERROR){
			sll_free_string(out);
			UNLOCK;
			return err;
		}
		sll_string_increase(out,sz);
		out->length+=sz;
		sll_string_insert_pointer_length(f->_read_buffer,sz,out->length-sz,out);
	} while (sz==FILE_BUFFER_SIZE);
	f->_read_buffer_offset=0;
	f->_read_buffer_size=0;
	UNLOCK;
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL sll_read_char_t sll_file_read_char(sll_file_t* f,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->flags&SLL_FILE_FLAG_READ)){
		return SLL_END_OF_DATA;
	}
	LOCK;
	sll_char_t o;
	if (f->flags&FILE_FLAG_MEMORY){
		if (f->_offset==f->source.memory.size){
			UNLOCK;
			return SLL_END_OF_DATA;
		}
		o=*((sll_char_t*)(PTR(ADDR(f->source.memory.pointer)+f->_offset)));
	}
	else if (f->source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return SLL_END_OF_DATA;
	}
	else if (f->flags&SLL_FILE_FLAG_NO_BUFFER){
		if (!sll_platform_file_read(f->source.file.fd,&o,sizeof(sll_char_t),err)){
			UNLOCK;
			return SLL_END_OF_DATA;
		}
	}
	else{
		if (!f->_read_buffer_offset&&!f->_read_buffer_size){
			f->_read_buffer_size=sll_platform_file_read(f->source.file.fd,f->_read_buffer,FILE_BUFFER_SIZE,err);
			if (!f->_read_buffer_size){
				UNLOCK;
				return SLL_END_OF_DATA;
			}
		}
		o=f->_read_buffer[f->_read_buffer_offset];
		f->_read_buffer_offset++;
		if (f->_read_buffer_offset==f->_read_buffer_size){
			f->_read_buffer_offset=0;
			f->_read_buffer_size=sll_platform_file_read(f->source.file.fd,f->_read_buffer,FILE_BUFFER_SIZE,err);
		}
	}
	f->_offset++;
	if (o=='\n'){
		f->_line_number++;
	}
	if (f->_hash.buffer_offset!=DISABLE_FILE_HASH){
		f->_hash.buffer[f->_hash.buffer_offset]=o;
		if (f->_hash.buffer_offset==63){
			f->_hash.buffer_offset=0;
			sll_hash_sha256(&(f->_hash.hash),f->_hash.buffer,64);
		}
		else{
			f->_hash.buffer_offset++;
		}
	}
	UNLOCK;
	return DATA_IF_ERROR_PTR(o,SLL_END_OF_DATA);
}



__SLL_EXTERNAL sll_error_t sll_file_reset(sll_file_t* f){
	sll_error_t err=SLL_NO_ERROR;
	LOCK;
	if (!(f->flags&FILE_FLAG_MEMORY)){
		if (f->source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
			UNLOCK;
			return SLL_ERROR_UNKNOWN_FD;
		}
		err=sll_platform_file_seek(f->source.file.fd,0);
		if (err!=SLL_NO_ERROR){
			UNLOCK;
			return err;
		}
		f->_read_buffer_offset=0;
		f->_read_buffer_size=sll_platform_file_read(f->source.file.fd,f->_read_buffer,FILE_BUFFER_SIZE,&err);
	}
	f->_line_number=0;
	f->_offset=0;
	if (f->_hash.buffer_offset!=DISABLE_FILE_HASH){
		_file_start_hash(f);
	}
	UNLOCK;
	return err;
}



__SLL_EXTERNAL sll_size_t sll_file_write(sll_file_t* f,const void* ptr,sll_size_t sz,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->flags&SLL_FILE_FLAG_WRITE)||!sz){
		return 0;
	}
	if (f->flags&FILE_FLAG_MEMORY){
		LOCK;
		if (f->flags&FILE_FLAG_DYNAMIC_BUFFERS){
			dynamic_buffer_chunk_t* c=f->_write_buffer.dynamic.end;
			if (c->size-f->_write_buffer.dynamic.offset>=sz){
				sll_copy_data(ptr,sz,c->data+f->_write_buffer.dynamic.offset);
				f->_write_buffer.dynamic.size+=sz;
				f->_write_buffer.dynamic.offset+=sz;
				UNLOCK;
				return sz;
			}
			SLL_UNIMPLEMENTED();
			UNLOCK;
			return sz;
		}
		if (f->_offset+sz>=f->source.memory.size){
			sz=f->source.memory.size-f->_offset;
		}
		sll_copy_data(ptr,sz,((sll_char_t*)(f->source.memory.pointer))+f->_offset);
		f->_offset+=sz;
		UNLOCK;
		return sz;
	}
	if (f->source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	if (f->flags&SLL_FILE_FLAG_NO_BUFFER){
		sll_size_t o=sll_platform_file_write(f->source.file.fd,ptr,sz,err);
		return o;
	}
	sll_bool_t flush=((f->flags&SLL_FILE_FLUSH_ON_NEWLINE)?sll_contains_character(ptr,sz,'\n'):0);
	LOCK;
	if (sz+f->_write_buffer.static_.offset<=FILE_BUFFER_SIZE){
		sll_copy_data(ptr,sz,f->_write_buffer.static_.pointer+f->_write_buffer.static_.offset);
		f->_write_buffer.static_.offset+=sz;
		if (f->_write_buffer.static_.offset==FILE_BUFFER_SIZE){
			sll_platform_file_write(f->source.file.fd,f->_write_buffer.static_.pointer,FILE_BUFFER_SIZE,err);
			f->_write_buffer.static_.offset=0;
		}
		UNLOCK;
		if (flush){
			sll_file_flush(f);
		}
		return ZERO_IF_ERROR_PTR(sz);
	}
	sll_size_t i=FILE_BUFFER_SIZE-f->_write_buffer.static_.offset;
	sll_copy_data(ptr,i,f->_write_buffer.static_.pointer+f->_write_buffer.static_.offset);
	sll_platform_file_write(f->source.file.fd,f->_write_buffer.static_.pointer,FILE_BUFFER_SIZE,err);
	const sll_char_t* v=((const sll_char_t*)ptr)+i;
	sll_size_t o=sz;
	sz-=i;
	i=sz&(FILE_BUFFER_SIZE-1);
	sz-=i;
	if (sz){
		sll_platform_file_write(f->source.file.fd,v,sz,err);
		v+=sz;
	}
	if (i){
		sll_copy_data(v,i,f->_write_buffer.static_.pointer);
	}
	f->_write_buffer.static_.offset=i;
	UNLOCK;
	if (flush){
		sll_file_flush(f);
	}
	return ZERO_IF_ERROR_PTR(o);
}



__SLL_EXTERNAL sll_bool_t sll_file_write_char(sll_file_t* f,sll_char_t chr,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->flags&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	if (f->flags&FILE_FLAG_MEMORY){
		LOCK;
		if (f->flags&FILE_FLAG_DYNAMIC_BUFFERS){
			dynamic_buffer_chunk_t* k=f->_write_buffer.dynamic.end;
			if (k->size>f->_write_buffer.dynamic.offset){
				k->data[f->_write_buffer.dynamic.offset]=chr;
				f->_write_buffer.dynamic.size++;
				f->_write_buffer.dynamic.offset++;
				UNLOCK;
				return 1;
			}
			SLL_UNIMPLEMENTED();
			UNLOCK;
			return 1;
		}
		if (f->_offset+1>=f->source.memory.size){
			UNLOCK;
			return 0;
		}
		*(((sll_char_t*)(f->source.memory.pointer))+f->_offset)=chr;
		f->_offset++;
		UNLOCK;
		return 1;
	}
	if (f->source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	if (f->flags&SLL_FILE_FLAG_NO_BUFFER){
		sll_bool_t o=sll_platform_file_write(f->source.file.fd,&chr,sizeof(sll_char_t),err);
		return o;
	}
	LOCK;
	SLL_ASSERT(f->_write_buffer.static_.offset+1<=FILE_BUFFER_SIZE);
	f->_write_buffer.static_.pointer[f->_write_buffer.static_.offset]=chr;
	f->_write_buffer.static_.offset++;
	if (f->_write_buffer.static_.offset==FILE_BUFFER_SIZE){
		sll_platform_file_write(f->source.file.fd,f->_write_buffer.static_.pointer,FILE_BUFFER_SIZE,err);
		f->_write_buffer.static_.offset=0;
	}
	UNLOCK;
	if ((f->flags&SLL_FILE_FLUSH_ON_NEWLINE)&&chr=='\n'){
		sll_file_flush(f);
	}
	return ZERO_IF_ERROR_PTR(1);
}



__SLL_EXTERNAL sll_size_t sll_file_write_char_count(sll_file_t* f,sll_char_t chr,sll_size_t cnt,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->flags&SLL_FILE_FLAG_WRITE)||!cnt){
		return 0;
	}
	if (f->flags&FILE_FLAG_MEMORY){
		if (f->flags&FILE_FLAG_DYNAMIC_BUFFERS){
			SLL_UNIMPLEMENTED();
		}
		LOCK;
		if (f->_offset+cnt>=f->source.memory.size){
			cnt=f->source.memory.size-f->_offset;
		}
		sll_set_memory(((sll_char_t*)(f->source.memory.pointer))+f->_offset,cnt,chr);
		f->_offset+=cnt;
		UNLOCK;
		return cnt;
	}
	if (f->source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	if (f->flags&SLL_FILE_FLAG_NO_BUFFER){
		sll_size_t o=0;
		while (cnt){
			o+=sll_platform_file_write(f->source.file.fd,&chr,sizeof(sll_char_t),err);
			cnt--;
		}
		return ZERO_IF_ERROR_PTR(o);
	}
	LOCK;
	if (cnt+f->_write_buffer.static_.offset<=FILE_BUFFER_SIZE){
		sll_set_memory(f->_write_buffer.static_.pointer+f->_write_buffer.static_.offset,cnt,chr);
		f->_write_buffer.static_.offset+=cnt;
		if (f->_write_buffer.static_.offset==FILE_BUFFER_SIZE){
			sll_platform_file_write(f->source.file.fd,f->_write_buffer.static_.pointer,FILE_BUFFER_SIZE,err);
			f->_write_buffer.static_.offset=0;
		}
		UNLOCK;
		if ((f->flags&SLL_FILE_FLUSH_ON_NEWLINE)&&chr=='\n'){
			sll_file_flush(f);
		}
		return ZERO_IF_ERROR_PTR(cnt);
	}
	sll_size_t i=FILE_BUFFER_SIZE-f->_write_buffer.static_.offset;
	sll_set_memory(f->_write_buffer.static_.pointer+f->_write_buffer.static_.offset,i,chr);
	sll_platform_file_write(f->source.file.fd,f->_write_buffer.static_.pointer,FILE_BUFFER_SIZE,err);
	sll_size_t o=cnt;
	cnt-=i;
	i=cnt&(FILE_BUFFER_SIZE-1);
	cnt-=i;
	if (cnt){
		sll_set_memory(f->_write_buffer.static_.pointer,FILE_BUFFER_SIZE,chr);
		while (cnt){
			sll_platform_file_write(f->source.file.fd,f->_write_buffer.static_.pointer,FILE_BUFFER_SIZE,err);
			cnt-=FILE_BUFFER_SIZE;
		}
	}
	if (i){
		sll_set_memory(f->_write_buffer.static_.pointer,i,chr);
	}
	f->_write_buffer.static_.offset=i;
	UNLOCK;
	if ((f->flags&SLL_FILE_FLUSH_ON_NEWLINE)&&chr=='\n'){
		sll_file_flush(f);
	}
	return ZERO_IF_ERROR_PTR(o);
}



__SLL_EXTERNAL sll_size_t sll_file_write_format(sll_file_t* f,const sll_char_t* fmt,sll_error_t* err,...){
	ERROR_PTR_RESET;
	if (!(f->flags&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	if (!(f->flags&FILE_FLAG_MEMORY)&&f->source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	va_list va;
	va_start(va,err);
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_C(&dt,&va);
	sll_string_t str;
	sll_string_format_list(fmt,sll_string_length(fmt),&dt,&str);
	va_end(va);
	sll_size_t o=sll_file_write(f,str.data,str.length,err);
	sll_free_string(&str);
	return o;
}



__SLL_EXTERNAL sll_size_t sll_file_write_string(sll_file_t* f,const sll_char_t* str,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->flags&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	if (!(f->flags&FILE_FLAG_MEMORY)&&f->source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	return sll_file_write(f,str,sll_string_length(str),err);
}

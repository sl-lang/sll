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



#define LOCK SLL_CRITICAL_ERROR(sll_platform_lock_acquire(f->_lck))
#define UNLOCK SLL_CRITICAL(sll_platform_lock_release(f->_lck))



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
	if (!(f->f&SLL_FILE_FLAG_READ)||f->_h.bfl==DISABLE_FILE_HASH){
		return;
	}
	sll_char_t tmp[128];
	sll_set_memory(tmp,128,0);
	sll_copy_data(f->_h.bf,f->_h.bfl,tmp);
	__SLL_U8 offset=(f->_h.bfl<56?56:120);
	tmp[f->_h.bfl]=128;
	tmp[offset]=(f->_off>>53)&0xff;
	tmp[offset+1]=(f->_off>>45)&0xff;
	tmp[offset+2]=(f->_off>>37)&0xff;
	tmp[offset+3]=(f->_off>>29)&0xff;
	tmp[offset+4]=(f->_off>>21)&0xff;
	tmp[offset+5]=(f->_off>>13)&0xff;
	tmp[offset+6]=(f->_off>>5)&0xff;
	tmp[offset+7]=(f->_off<<3)&0xff;
	sll_hash_sha256(&(f->_h.h),tmp,(f->_h.bfl<56?64:128));
}



void _file_init_std_streams(void){
	sll_file_open_descriptor(SLL_CHAR("sll_stdin"),sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_INPUT),SLL_FILE_FLAG_READ|SLL_FILE_FLAG_NO_BUFFER,sll_stdin);
	sll_file_open_descriptor(SLL_CHAR("sll_stdout"),sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_OUTPUT),SLL_FILE_FLAG_WRITE|SLL_FILE_FLUSH_ON_NEWLINE,sll_stdout);
	sll_file_open_descriptor(SLL_CHAR("sll_stderr"),sll_platform_get_default_stream_descriptor(SLL_PLATFORM_STREAM_ERROR),SLL_FILE_FLAG_WRITE|SLL_FILE_FLUSH_ON_NEWLINE,sll_stderr);
	(*((sll_file_flags_t*)(&(sll_stdin->f))))|=FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stdout->f))))|=FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stderr->f))))|=FILE_FLAG_NO_RELEASE;
}



void _file_release_std_streams(void){
	(*((sll_file_flags_t*)(&(sll_stdin->f))))&=~FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stdout->f))))&=~FILE_FLAG_NO_RELEASE;
	(*((sll_file_flags_t*)(&(sll_stderr->f))))&=~FILE_FLAG_NO_RELEASE;
	sll_file_close(sll_stdin);
	sll_file_close(sll_stdout);
	sll_file_close(sll_stderr);
	while (_file_buffer_pool_len){
		_file_buffer_pool_len--;
		SLL_CRITICAL_ERROR(sll_platform_free_page(_file_buffer_pool[_file_buffer_pool_len],SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE)));
	}
}



void _file_start_hash(sll_file_t* f){
	if (!(f->f&SLL_FILE_FLAG_READ)||(!(f->f&FILE_FLAG_MEMORY)&&f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR)){
		return;
	}
	SLL_INIT_SHA256(&(f->_h.h));
	f->_h.bfl=0;
}



__SLL_EXTERNAL void sll_file_close(sll_file_t* f){
	sll_file_flush(f);
	if (f->f&FILE_FLAG_NO_RELEASE){
		return;
	}
	SLL_CRITICAL(sll_platform_lock_delete(f->_lck));
	if (!(f->f&FILE_FLAG_MEMORY)){
		sll_free_string((sll_string_t*)(&(f->dt.fl.nm)));
		if (f->dt.fl.fd!=SLL_UNKNOWN_FILE_DESCRIPTOR){
			SLL_CRITICAL_ERROR(sll_platform_file_close(f->dt.fl.fd));
			if (!(f->f&SLL_FILE_FLAG_NO_BUFFER)){
				if (f->f&SLL_FILE_FLAG_READ){
					_free_buffer(f->_r_bf);
				}
				if (f->f&SLL_FILE_FLAG_WRITE){
					_free_buffer(f->_w.bf.p);
				}
			}
		}
	}
	else{
		SLL_ASSERT(f->f&SLL_FILE_FLAG_NO_BUFFER);
		if (f->f&FILE_FLAG_DYNAMIC_BUFFERS){
			dynamic_buffer_chunk_t* c=f->_w.d.b;
			do{
				dynamic_buffer_chunk_t* n=c->n;
				SLL_CRITICAL_ERROR(sll_platform_free_page(c,c->sz));
				c=n;
			} while (c);
		}
	}
	*((sll_file_flags_t*)(&(f->f)))=FILE_FLAG_NO_RELEASE;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_file_data_available(sll_file_t* f){
	if ((f->f&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_ASYNC))!=(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_ASYNC)){
		return 1;
	}
	return sll_platform_file_data_available(f->dt.fl.fd);
}



__SLL_EXTERNAL sll_error_t sll_file_flush(sll_file_t* f){
	if (!(f->f&SLL_FILE_FLAG_WRITE)||(f->f&SLL_FILE_FLAG_NO_BUFFER)||!f->_w.bf.off){
		return SLL_NO_ERROR;
	}
	sll_error_t err=SLL_NO_ERROR;
	sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,f->_w.bf.off,&err);
	f->_w.bf.off=0;
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
		bf->sz=SLL_ROUND_PAGE(FILE_DYNAMIC_BUFFER_ALLOC_SIZE);
		bf->n=NULL;
		o->_w.d.b=bf;
		o->_w.d.t=bf;
		o->_w.d.sz=0;
		o->_w.d.off=0;
	}
	*((const void**)(&(o->dt.mm.p)))=ptr;
	*((sll_size_t*)(&(o->dt.mm.sz)))=sz;
	*((sll_file_flags_t*)(&(o->f)))=(flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|FILE_FLAG_DYNAMIC_BUFFERS))|SLL_FILE_FLAG_NO_BUFFER|FILE_FLAG_MEMORY;
	o->_l_num=0;
	o->_off=0;
	o->_h.bfl=DISABLE_FILE_HASH;
	o->_lck=sll_platform_lock_create(NULL);
}



__SLL_EXTERNAL void sll_file_get_buffer(sll_file_t* f,sll_string_t* o){
	if (!(f->f&FILE_FLAG_DYNAMIC_BUFFERS)){
		SLL_INIT_STRING(o);
		return;
	}
	sll_string_create((sll_string_length_t)(f->_w.d.sz),o);
	dynamic_buffer_chunk_t* c=f->_w.d.b;
	addr_t ptr=ADDR(o->v);
	sll_string_length_t l=o->l;
	do{
		sll_copy_data(c->dt,(l>c->sz?c->sz:l),PTR(ptr));
		l-=(sll_string_length_t)(c->sz);
		ptr+=c->sz;
		c=c->n;
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
	*((sll_file_descriptor_t*)(&(o->dt.fl.fd)))=fd;
	sll_string_from_pointer(nm,(sll_string_t*)(&(o->dt.fl.nm)));
	if (!(flags&SLL_FILE_FLAG_WRITE)){
		flags&=~SLL_FILE_FLUSH_ON_NEWLINE;
	}
	*((sll_file_flags_t*)(&(o->f)))=flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|SLL_FILE_FLAG_NO_BUFFER|SLL_FILE_FLUSH_ON_NEWLINE);
	if (fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		*((sll_file_flags_t*)(&(o->f)))=flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE);
		return;
	}
	if (sll_platform_file_async_read(fd)){
		*((sll_file_flags_t*)(&(o->f)))|=SLL_FILE_FLAG_ASYNC;
	}
	o->_l_num=0;
	o->_off=0;
	if (!(flags&SLL_FILE_FLAG_NO_BUFFER)){
		if (flags&SLL_FILE_FLAG_READ){
			o->_r_bf=_get_buffer();
			o->_r_bf_off=0;
			o->_r_bf_sz=0;
		}
		if (flags&SLL_FILE_FLAG_WRITE){
			o->_w.bf.p=_get_buffer();
			o->_w.bf.off=0;
		}
	}
	o->_h.bfl=DISABLE_FILE_HASH;
	o->_lck=sll_platform_lock_create(NULL);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_read_char_t sll_file_peek_char(sll_file_t* f,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->f&SLL_FILE_FLAG_READ)){
		return SLL_END_OF_DATA;
	}
	if (f->f&FILE_FLAG_MEMORY){
		LOCK;
		sll_read_char_t o=(f->_off==f->dt.mm.sz?SLL_END_OF_DATA:*((sll_char_t*)(PTR(ADDR(f->dt.mm.p)+f->_off))));
		UNLOCK;
		return o;
	}
	if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return SLL_END_OF_DATA;
	}
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		SLL_UNIMPLEMENTED();
	}
	LOCK;
	if (!f->_r_bf_off&&!f->_r_bf_sz){
		f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,err);
		if (!f->_r_bf_sz){
			UNLOCK;
			return SLL_END_OF_DATA;
		}
	}
	sll_char_t o=f->_r_bf[f->_r_bf_off];
	UNLOCK;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_file_read(sll_file_t* f,void* ptr,sll_size_t sz,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->f&SLL_FILE_FLAG_READ)||!sz){
		return 0;
	}
	sll_size_t o;
	if (f->f&FILE_FLAG_MEMORY){
		LOCK;
		if (f->_off+sz>=f->dt.mm.sz){
			sz=f->dt.mm.sz-f->_off;
		}
		sll_copy_data(PTR(ADDR(f->dt.mm.p)+f->_off),sz,ptr);
		f->_off+=sz;
		o=sz;
		goto _hash_data;
	}
	if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	LOCK;
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		sz=sll_platform_file_read(f->dt.fl.fd,ptr,sz,err);
		f->_off+=sz;
		o=sz;
		goto _hash_data;
	}
	if (!f->_r_bf_off&&!f->_r_bf_sz){
		f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,err);
		if (!f->_r_bf_sz){
			UNLOCK;
			return 0;
		}
	}
	if (f->_r_bf_off+sz<=f->_r_bf_sz){
		sll_copy_data(f->_r_bf+f->_r_bf_off,sz,ptr);
		f->_r_bf_off+=sz;
		if (f->_r_bf_off==f->_r_bf_sz){
			f->_r_bf_off=0;
			f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,err);
		}
		o=sz;
		goto _hash_data;
	}
	o=f->_r_bf_sz-f->_r_bf_off;
	sz-=o;
	sll_copy_data(f->_r_bf+f->_r_bf_off,o,ptr);
	ptr=PTR(ADDR(ptr)+o);
	while (sz>FILE_BUFFER_SIZE){
		f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,ptr,FILE_BUFFER_SIZE,err);
		if (f->_r_bf_sz!=FILE_BUFFER_SIZE){
			SLL_UNIMPLEMENTED();
			f->_r_bf_off=0;
			f->_r_bf_sz=0;
			goto _hash_data;
		}
		o+=FILE_BUFFER_SIZE;
		sz-=FILE_BUFFER_SIZE;
		ptr=PTR(ADDR(ptr)+FILE_BUFFER_SIZE);
	}
	f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,err);
	if (f->_r_bf_sz<sz){
		o+=f->_r_bf_sz;
		sll_copy_data(f->_r_bf,f->_r_bf_sz,ptr);
		f->_r_bf_off=0;
		f->_r_bf_sz=0;
		goto _hash_data;
	}
	o+=sz;
	f->_r_bf_off=sz;
	sll_copy_data(f->_r_bf,sz,ptr);
_hash_data:
	if (f->_h.bfl!=DISABLE_FILE_HASH){
		SLL_UNIMPLEMENTED();
	}
	UNLOCK;
	return ZERO_IF_ERROR_PTR(o);
}



__SLL_EXTERNAL sll_read_char_t sll_file_read_char(sll_file_t* f,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->f&SLL_FILE_FLAG_READ)){
		return SLL_END_OF_DATA;
	}
	LOCK;
	sll_char_t o;
	if (f->f&FILE_FLAG_MEMORY){
		if (f->_off==f->dt.mm.sz){
			UNLOCK;
			return SLL_END_OF_DATA;
		}
		o=*((sll_char_t*)(PTR(ADDR(f->dt.mm.p)+f->_off)));
	}
	else if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return SLL_END_OF_DATA;
	}
	else if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		if (!sll_platform_file_read(f->dt.fl.fd,&o,sizeof(sll_char_t),err)){
			UNLOCK;
			return SLL_END_OF_DATA;
		}
	}
	else{
		if (!f->_r_bf_off&&!f->_r_bf_sz){
			f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,err);
			if (!f->_r_bf_sz){
				UNLOCK;
				return SLL_END_OF_DATA;
			}
		}
		o=f->_r_bf[f->_r_bf_off];
		f->_r_bf_off++;
		if (f->_r_bf_off==f->_r_bf_sz){
			f->_r_bf_off=0;
			f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,err);
		}
	}
	f->_off++;
	if (o=='\n'){
		f->_l_num++;
	}
	if (f->_h.bfl!=DISABLE_FILE_HASH){
		f->_h.bf[f->_h.bfl]=o;
		if (f->_h.bfl==63){
			f->_h.bfl=0;
			sll_hash_sha256(&(f->_h.h),f->_h.bf,64);
		}
		else{
			f->_h.bfl++;
		}
	}
	UNLOCK;
	return DATA_IF_ERROR_PTR(o,SLL_END_OF_DATA);
}



__SLL_EXTERNAL sll_error_t sll_file_reset(sll_file_t* f){
	sll_error_t err=SLL_NO_ERROR;
	LOCK;
	if (!(f->f&FILE_FLAG_MEMORY)){
		if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
			UNLOCK;
			return SLL_ERROR_UNKNOWN_FD;
		}
		err=sll_platform_file_seek(f->dt.fl.fd,0);
		if (err!=SLL_NO_ERROR){
			UNLOCK;
			return err;
		}
		f->_r_bf_off=0;
		f->_r_bf_sz=sll_platform_file_read(f->dt.fl.fd,f->_r_bf,FILE_BUFFER_SIZE,&err);
	}
	f->_l_num=0;
	f->_off=0;
	if (f->_h.bfl!=DISABLE_FILE_HASH){
		_file_start_hash(f);
	}
	UNLOCK;
	return err;
}



__SLL_EXTERNAL sll_size_t sll_file_write(sll_file_t* f,const void* ptr,sll_size_t sz,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->f&SLL_FILE_FLAG_WRITE)||!sz){
		return 0;
	}
	if (f->f&FILE_FLAG_MEMORY){
		LOCK;
		if (f->f&FILE_FLAG_DYNAMIC_BUFFERS){
			dynamic_buffer_chunk_t* c=f->_w.d.t;
			if (c->sz-f->_w.d.off>=sz){
				sll_copy_data(ptr,sz,c->dt+f->_w.d.off);
				f->_w.d.sz+=sz;
				f->_w.d.off+=sz;
				UNLOCK;
				return sz;
			}
			SLL_UNIMPLEMENTED();
			UNLOCK;
			return sz;
		}
		if (f->_off+sz>=f->dt.mm.sz){
			sz=f->dt.mm.sz-f->_off;
		}
		sll_copy_data(ptr,sz,((sll_char_t*)(f->dt.mm.p))+f->_off);
		f->_off+=sz;
		UNLOCK;
		return sz;
	}
	if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		sll_size_t o=sll_platform_file_write(f->dt.fl.fd,ptr,sz,err);
		return o;
	}
	sll_bool_t flush=((f->f&SLL_FILE_FLUSH_ON_NEWLINE)?sll_contains_character(ptr,sz,'\n'):0);
	LOCK;
	if (sz+f->_w.bf.off<=FILE_BUFFER_SIZE){
		sll_copy_data(ptr,sz,f->_w.bf.p+f->_w.bf.off);
		f->_w.bf.off+=sz;
		if (f->_w.bf.off==FILE_BUFFER_SIZE){
			sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,FILE_BUFFER_SIZE,err);
			f->_w.bf.off=0;
		}
		UNLOCK;
		if (flush){
			sll_file_flush(f);
		}
		return ZERO_IF_ERROR_PTR(sz);
	}
	sll_size_t i=FILE_BUFFER_SIZE-f->_w.bf.off;
	sll_copy_data(ptr,i,f->_w.bf.p+f->_w.bf.off);
	sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,FILE_BUFFER_SIZE,err);
	const sll_char_t* v=((const sll_char_t*)ptr)+i;
	sll_size_t o=sz;
	sz-=i;
	i=sz&(FILE_BUFFER_SIZE-1);
	sz-=i;
	if (sz){
		sll_platform_file_write(f->dt.fl.fd,v,sz,err);
		v+=sz;
	}
	if (i){
		sll_copy_data(v,i,f->_w.bf.p);
	}
	f->_w.bf.off=i;
	UNLOCK;
	if (flush){
		sll_file_flush(f);
	}
	return ZERO_IF_ERROR_PTR(o);
}



__SLL_EXTERNAL sll_bool_t sll_file_write_char(sll_file_t* f,sll_char_t chr,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->f&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	if (f->f&FILE_FLAG_MEMORY){
		LOCK;
		if (f->f&FILE_FLAG_DYNAMIC_BUFFERS){
			dynamic_buffer_chunk_t* k=f->_w.d.t;
			if (k->sz>f->_w.d.off){
				k->dt[f->_w.d.off]=chr;
				f->_w.d.sz++;
				f->_w.d.off++;
				UNLOCK;
				return 1;
			}
			SLL_UNIMPLEMENTED();
			UNLOCK;
			return 1;
		}
		if (f->_off+1>=f->dt.mm.sz){
			UNLOCK;
			return 0;
		}
		*(((sll_char_t*)(f->dt.mm.p))+f->_off)=chr;
		f->_off++;
		UNLOCK;
		return 1;
	}
	if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		sll_bool_t o=sll_platform_file_write(f->dt.fl.fd,&chr,sizeof(sll_char_t),err);
		return o;
	}
	LOCK;
	SLL_ASSERT(f->_w.bf.off+1<=FILE_BUFFER_SIZE);
	f->_w.bf.p[f->_w.bf.off]=chr;
	f->_w.bf.off++;
	if (f->_w.bf.off==FILE_BUFFER_SIZE){
		sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,FILE_BUFFER_SIZE,err);
		f->_w.bf.off=0;
	}
	UNLOCK;
	if ((f->f&SLL_FILE_FLUSH_ON_NEWLINE)&&chr=='\n'){
		sll_file_flush(f);
	}
	return ZERO_IF_ERROR_PTR(1);
}



__SLL_EXTERNAL sll_size_t sll_file_write_char_count(sll_file_t* f,sll_char_t chr,sll_size_t cnt,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->f&SLL_FILE_FLAG_WRITE)||!cnt){
		return 0;
	}
	if (f->f&FILE_FLAG_MEMORY){
		if (f->f&FILE_FLAG_DYNAMIC_BUFFERS){
			SLL_UNIMPLEMENTED();
		}
		LOCK;
		if (f->_off+cnt>=f->dt.mm.sz){
			cnt=f->dt.mm.sz-f->_off;
		}
		sll_set_memory(((sll_char_t*)(f->dt.mm.p))+f->_off,cnt,chr);
		f->_off+=cnt;
		UNLOCK;
		return cnt;
	}
	if (f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	if (f->f&SLL_FILE_FLAG_NO_BUFFER){
		sll_size_t o=0;
		while (cnt){
			o+=sll_platform_file_write(f->dt.fl.fd,&chr,sizeof(sll_char_t),err);
			cnt--;
		}
		return ZERO_IF_ERROR_PTR(o);
	}
	LOCK;
	if (cnt+f->_w.bf.off<=FILE_BUFFER_SIZE){
		sll_set_memory(f->_w.bf.p+f->_w.bf.off,cnt,chr);
		f->_w.bf.off+=cnt;
		if (f->_w.bf.off==FILE_BUFFER_SIZE){
			sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,FILE_BUFFER_SIZE,err);
			f->_w.bf.off=0;
		}
		UNLOCK;
		if ((f->f&SLL_FILE_FLUSH_ON_NEWLINE)&&chr=='\n'){
			sll_file_flush(f);
		}
		return ZERO_IF_ERROR_PTR(cnt);
	}
	sll_size_t i=FILE_BUFFER_SIZE-f->_w.bf.off;
	sll_set_memory(f->_w.bf.p+f->_w.bf.off,i,chr);
	sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,FILE_BUFFER_SIZE,err);
	sll_size_t o=cnt;
	cnt-=i;
	i=cnt&(FILE_BUFFER_SIZE-1);
	cnt-=i;
	if (cnt){
		sll_set_memory(f->_w.bf.p,FILE_BUFFER_SIZE,chr);
		while (cnt){
			sll_platform_file_write(f->dt.fl.fd,f->_w.bf.p,FILE_BUFFER_SIZE,err);
			cnt-=FILE_BUFFER_SIZE;
		}
	}
	if (i){
		sll_set_memory(f->_w.bf.p,i,chr);
	}
	f->_w.bf.off=i;
	UNLOCK;
	if ((f->f&SLL_FILE_FLUSH_ON_NEWLINE)&&chr=='\n'){
		sll_file_flush(f);
	}
	return ZERO_IF_ERROR_PTR(o);
}



__SLL_EXTERNAL sll_size_t sll_file_write_format(sll_file_t* f,const sll_char_t* fmt,sll_error_t* err,...){
	ERROR_PTR_RESET;
	if (!(f->f&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	if (!(f->f&FILE_FLAG_MEMORY)&&f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
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
	sll_size_t o=sll_file_write(f,str.v,str.l,err);
	sll_free_string(&str);
	return o;
}



__SLL_EXTERNAL sll_size_t sll_file_write_string(sll_file_t* f,const sll_char_t* str,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(f->f&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	if (!(f->f&FILE_FLAG_MEMORY)&&f->dt.fl.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	return sll_file_write(f,str,sll_string_length(str),err);
}

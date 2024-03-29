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
#include <sll/platform/socket.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



#define LOCK SLL_CRITICAL_ERROR(sll_platform_lock_acquire(file->_lock))
#define UNLOCK SLL_CRITICAL(sll_platform_lock_release(file->_lock))



static sll_file_t _file_stdin;
static sll_file_t _file_stdout;
static sll_file_t _file_stderr;
static void* _file_buffer_pool[FILE_BUFFER_POOL_SIZE];
static sll_array_length_t _file_buffer_pool_len=0;



__SLL_EXTERNAL sll_file_t* sll_stdin=&_file_stdin;
__SLL_EXTERNAL sll_file_t* sll_stdout=&_file_stdout;
__SLL_EXTERNAL sll_file_t* sll_stderr=&_file_stderr;



static void _free_buffer(void* page_pointer){
	if (_file_buffer_pool_len==FILE_BUFFER_POOL_SIZE){
		SLL_CRITICAL_ERROR(sll_platform_free_page(page_pointer,SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE)));
		return;
	}
	_file_buffer_pool[_file_buffer_pool_len]=page_pointer;
	_file_buffer_pool_len++;
}



static void* _get_buffer(void){
	if (!_file_buffer_pool_len){
		return sll_platform_allocate_page(SLL_ROUND_LARGE_PAGE(FILE_BUFFER_SIZE),1,NULL);
	}
	_file_buffer_pool_len--;
	return _file_buffer_pool[_file_buffer_pool_len];
}



void _file_end_hash(sll_file_t* file){
	if ((file->flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_SOCKET))!=SLL_FILE_FLAG_READ||file->data.file._hash.buffer_offset==DISABLE_FILE_HASH){
		return;
	}
	sll_char_t buffer[128];
	sll_zero_memory(buffer,128);
	sll_copy_data(file->data.file._hash.buffer,file->data.file._hash.buffer_offset,buffer);
	__SLL_U8 offset=(file->data.file._hash.buffer_offset<56?56:120);
	buffer[file->data.file._hash.buffer_offset]=128;
	buffer[offset]=(file->data.file._offset>>53)&0xff;
	buffer[offset+1]=(file->data.file._offset>>45)&0xff;
	buffer[offset+2]=(file->data.file._offset>>37)&0xff;
	buffer[offset+3]=(file->data.file._offset>>29)&0xff;
	buffer[offset+4]=(file->data.file._offset>>21)&0xff;
	buffer[offset+5]=(file->data.file._offset>>13)&0xff;
	buffer[offset+6]=(file->data.file._offset>>5)&0xff;
	buffer[offset+7]=(file->data.file._offset<<3)&0xff;
	sll_hash_sha256(&(file->data.file._hash.hash),buffer,(file->data.file._hash.buffer_offset<56?64:128));
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



void _file_start_hash(sll_file_t* file){
	if ((file->flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_SOCKET))!=SLL_FILE_FLAG_READ||(!(file->flags&FILE_FLAG_MEMORY)&&file->data.file.source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR)){
		return;
	}
	SLL_INIT_SHA256(&(file->data.file._hash.hash));
	file->data.file._hash.buffer_offset=0;
}



__SLL_EXTERNAL void sll_file_close(sll_file_t* file){
	sll_file_flush(file);
	if (file->flags&FILE_FLAG_NO_RELEASE){
		return;
	}
	SLL_CRITICAL(sll_platform_lock_delete(file->_lock));
	if (file->flags&SLL_FILE_FLAG_SOCKET){
		SLL_CRITICAL_ERROR(sll_platform_socket_close(file->data.socket.fd));
	}
	else if (!(file->flags&FILE_FLAG_MEMORY)){
		sll_free_string((sll_string_t*)(&(file->data.file.source.file.path)));
		if (file->data.file.source.file.fd!=SLL_UNKNOWN_FILE_DESCRIPTOR){
			SLL_CRITICAL_ERROR(sll_platform_file_close(file->data.file.source.file.fd));
			if (!(file->flags&SLL_FILE_FLAG_NO_BUFFER)){
				if (file->flags&SLL_FILE_FLAG_READ){
					_free_buffer(file->data.file._read_buffer);
				}
				if (file->flags&SLL_FILE_FLAG_WRITE){
					_free_buffer(file->data.file._write_buffer.static_.pointer);
				}
			}
		}
	}
	else{
		SLL_ASSERT(file->flags&SLL_FILE_FLAG_NO_BUFFER);
		if (file->flags&FILE_FLAG_DYNAMIC_BUFFERS){
			dynamic_buffer_chunk_t* chunk=file->data.file._write_buffer.dynamic.start;
			do{
				dynamic_buffer_chunk_t* next=chunk->next;
				SLL_CRITICAL_ERROR(sll_platform_free_page(chunk,chunk->size+sizeof(dynamic_buffer_chunk_t)));
				chunk=next;
			} while (chunk);
		}
	}
	*((sll_file_flags_t*)(&(file->flags)))=FILE_FLAG_NO_RELEASE;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_file_data_available(sll_file_t* file){
	if (!(file->flags&SLL_FILE_FLAG_READ)){
		return 0;
	}
	if (!(file->flags&SLL_FILE_FLAG_ASYNC)){
		return 1;
	}
	if (file->flags&FILE_FLAG_MEMORY){
		SLL_UNIMPLEMENTED();
	}
	if (!(file->flags&SLL_FILE_FLAG_NO_BUFFER)){
		SLL_UNIMPLEMENTED();
	}
	return ((file->flags&SLL_FILE_FLAG_SOCKET)?sll_platform_socket_data_available(file->data.socket.fd):sll_platform_file_data_available(file->data.file.source.file.fd));
}



__SLL_EXTERNAL sll_error_t sll_file_flush(sll_file_t* file){
	if (!(file->flags&SLL_FILE_FLAG_WRITE)||(file->flags&(SLL_FILE_FLAG_NO_BUFFER|SLL_FILE_FLAG_SOCKET))||!file->data.file._write_buffer.static_.offset){
		return SLL_NO_ERROR;
	}
	sll_error_t err=SLL_NO_ERROR;
	sll_platform_file_write(file->data.file.source.file.fd,file->data.file._write_buffer.static_.pointer,file->data.file._write_buffer.static_.offset,&err);
	file->data.file._write_buffer.static_.offset=0;
	return err;
}



__SLL_EXTERNAL void sll_file_from_data(const void* pointer,sll_size_t size,sll_file_flags_t flags,sll_file_t* out){
	flags&=~FILE_FLAG_DYNAMIC_BUFFERS;
	if (flags&SLL_FILE_FLAG_APPEND){
		SLL_UNIMPLEMENTED();
	}
	if (flags&SLL_FILE_FLAG_WRITE){
		flags|=FILE_FLAG_DYNAMIC_BUFFERS;
		dynamic_buffer_chunk_t* buffer=sll_platform_allocate_page(SLL_ROUND_PAGE(FILE_DYNAMIC_BUFFER_ALLOC_SIZE),0,NULL);
		buffer->size=SLL_ROUND_PAGE(FILE_DYNAMIC_BUFFER_ALLOC_SIZE)-sizeof(dynamic_buffer_chunk_t);
		buffer->next=NULL;
		out->data.file._write_buffer.dynamic.start=buffer;
		out->data.file._write_buffer.dynamic.end=buffer;
		out->data.file._write_buffer.dynamic.size=0;
		out->data.file._write_buffer.dynamic.offset=0;
	}
	*((const void**)(&(out->data.file.source.memory.pointer)))=pointer;
	*((sll_size_t*)(&(out->data.file.source.memory.size)))=size;
	*((sll_file_flags_t*)(&(out->flags)))=(flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|FILE_FLAG_DYNAMIC_BUFFERS))|SLL_FILE_FLAG_NO_BUFFER|FILE_FLAG_MEMORY;
	out->data.file._line_number=0;
	out->data.file._offset=0;
	out->data.file._hash.buffer_offset=DISABLE_FILE_HASH;
	out->_lock=sll_platform_lock_create(NULL);
}



__SLL_EXTERNAL void sll_file_get_buffer(sll_file_t* file,sll_string_t* out){
	if (!(file->flags&FILE_FLAG_DYNAMIC_BUFFERS)){
		SLL_INIT_STRING(out);
		return;
	}
	sll_string_create((sll_string_length_t)(file->data.file._write_buffer.dynamic.size),out);
	dynamic_buffer_chunk_t* chunk=file->data.file._write_buffer.dynamic.start;
	addr_t ptr=ADDR(out->data);
	sll_string_length_t length=out->length;
	do{
		sll_copy_data(chunk->data,(length>chunk->size?chunk->size:length),PTR(ptr));
		length-=(sll_string_length_t)(chunk->size);
		ptr+=chunk->size;
		chunk=chunk->next;
	} while (chunk);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL sll_error_t sll_file_open(const sll_char_t* name,sll_file_flags_t flags,sll_file_t* out){
	if ((flags&SLL_FILE_FLAG_READ)&&!sll_platform_path_exists(name)){
		return SLL_ERROR_NO_FILE_PATH;
	}
	sll_error_t err=SLL_NO_ERROR;
	sll_file_descriptor_t fd=sll_platform_file_open(name,flags,&err);
	if (fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		return err;
	}
	sll_file_open_descriptor(name,fd,flags,out);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL void sll_file_open_descriptor(const sll_char_t* name,sll_file_descriptor_t fd,sll_file_flags_t flags,sll_file_t* out){
	*((sll_file_descriptor_t*)(&(out->data.file.source.file.fd)))=fd;
	sll_string_from_pointer(name,(sll_string_t*)(&(out->data.file.source.file.path)));
	if (!(flags&SLL_FILE_FLAG_WRITE)){
		flags&=~SLL_FILE_FLUSH_ON_NEWLINE;
	}
	*((sll_file_flags_t*)(&(out->flags)))=flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|SLL_FILE_FLAG_NO_BUFFER|SLL_FILE_FLUSH_ON_NEWLINE);
	if (fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		*((sll_file_flags_t*)(&(out->flags)))=flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE);
		return;
	}
	if (sll_platform_file_async_read(fd)){
		*((sll_file_flags_t*)(&(out->flags)))|=SLL_FILE_FLAG_ASYNC;
	}
	out->data.file._line_number=0;
	out->data.file._offset=0;
	if (!(flags&SLL_FILE_FLAG_NO_BUFFER)){
		if (flags&SLL_FILE_FLAG_READ){
			out->data.file._read_buffer=_get_buffer();
			out->data.file._read_buffer_offset=0;
			out->data.file._read_buffer_size=0;
		}
		if (flags&SLL_FILE_FLAG_WRITE){
			out->data.file._write_buffer.static_.pointer=_get_buffer();
			out->data.file._write_buffer.static_.offset=0;
		}
	}
	out->data.file._hash.buffer_offset=DISABLE_FILE_HASH;
	out->_lock=sll_platform_lock_create(NULL);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_read_char_t sll_file_peek_char(sll_file_t* file,sll_error_t* err){
	ERROR_PTR_RESET;
	if (file->flags&SLL_FILE_FLAG_SOCKET){
		SLL_UNIMPLEMENTED();
	}
	if (!(file->flags&SLL_FILE_FLAG_READ)){
		return SLL_END_OF_DATA;
	}
	if (file->flags&FILE_FLAG_MEMORY){
		LOCK;
		sll_read_char_t out=(file->data.file._offset==file->data.file.source.memory.size?SLL_END_OF_DATA:*((sll_char_t*)(PTR(ADDR(file->data.file.source.memory.pointer)+file->data.file._offset))));
		UNLOCK;
		return out;
	}
	if (file->data.file.source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return SLL_END_OF_DATA;
	}
	if (file->flags&SLL_FILE_FLAG_NO_BUFFER){
		SLL_UNIMPLEMENTED();
	}
	LOCK;
	if (!file->data.file._read_buffer_offset&&!file->data.file._read_buffer_size){
		file->data.file._read_buffer_size=sll_platform_file_read(file->data.file.source.file.fd,file->data.file._read_buffer,FILE_BUFFER_SIZE,err);
		if (!file->data.file._read_buffer_size){
			UNLOCK;
			return SLL_END_OF_DATA;
		}
	}
	sll_char_t out=file->data.file._read_buffer[file->data.file._read_buffer_offset];
	UNLOCK;
	return out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_file_read(sll_file_t* file,void* pointer,sll_size_t size,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(file->flags&SLL_FILE_FLAG_READ)||!size){
		return 0;
	}
	LOCK;
	if (file->flags&SLL_FILE_FLAG_SOCKET){
		sll_size_t out=sll_platform_socket_read(file->data.socket.fd,pointer,size,err);
		UNLOCK;
		return out;
	}
	sll_size_t out;
	if (file->flags&FILE_FLAG_MEMORY){
		if (file->data.file._offset+size>=file->data.file.source.memory.size){
			size=file->data.file.source.memory.size-file->data.file._offset;
		}
		sll_copy_data(PTR(ADDR(file->data.file.source.memory.pointer)+file->data.file._offset),size,pointer);
		file->data.file._offset+=size;
		out=size;
		goto _hash_data;
	}
	if (file->data.file.source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		UNLOCK;
		return 0;
	}
	if (file->flags&SLL_FILE_FLAG_NO_BUFFER){
		size=sll_platform_file_read(file->data.file.source.file.fd,pointer,size,err);
		file->data.file._offset+=size;
		out=size;
		goto _hash_data;
	}
	if (!file->data.file._read_buffer_offset&&!file->data.file._read_buffer_size){
		file->data.file._read_buffer_size=sll_platform_file_read(file->data.file.source.file.fd,file->data.file._read_buffer,FILE_BUFFER_SIZE,err);
		if (!file->data.file._read_buffer_size){
			UNLOCK;
			return 0;
		}
	}
	if (file->data.file._read_buffer_offset+size<=file->data.file._read_buffer_size){
		sll_copy_data(file->data.file._read_buffer+file->data.file._read_buffer_offset,size,pointer);
		file->data.file._read_buffer_offset+=size;
		if (file->data.file._read_buffer_offset==file->data.file._read_buffer_size){
			file->data.file._read_buffer_offset=0;
			file->data.file._read_buffer_size=sll_platform_file_read(file->data.file.source.file.fd,file->data.file._read_buffer,FILE_BUFFER_SIZE,err);
		}
		out=size;
		goto _hash_data;
	}
	out=file->data.file._read_buffer_size-file->data.file._read_buffer_offset;
	size-=out;
	sll_copy_data(file->data.file._read_buffer+file->data.file._read_buffer_offset,out,pointer);
	pointer=PTR(ADDR(pointer)+out);
	while (size>FILE_BUFFER_SIZE){
		file->data.file._read_buffer_size=sll_platform_file_read(file->data.file.source.file.fd,pointer,FILE_BUFFER_SIZE,err);
		if (file->data.file._read_buffer_size!=FILE_BUFFER_SIZE){
			SLL_UNIMPLEMENTED();
			file->data.file._read_buffer_offset=0;
			file->data.file._read_buffer_size=0;
			goto _hash_data;
		}
		out+=FILE_BUFFER_SIZE;
		size-=FILE_BUFFER_SIZE;
		pointer=PTR(ADDR(pointer)+FILE_BUFFER_SIZE);
	}
	file->data.file._read_buffer_size=sll_platform_file_read(file->data.file.source.file.fd,file->data.file._read_buffer,FILE_BUFFER_SIZE,err);
	if (file->data.file._read_buffer_size<size){
		out+=file->data.file._read_buffer_size;
		sll_copy_data(file->data.file._read_buffer,file->data.file._read_buffer_size,pointer);
		file->data.file._read_buffer_offset=0;
		file->data.file._read_buffer_size=0;
		goto _hash_data;
	}
	out+=size;
	file->data.file._read_buffer_offset=size;
	sll_copy_data(file->data.file._read_buffer,size,pointer);
_hash_data:
	if (file->data.file._hash.buffer_offset!=DISABLE_FILE_HASH){
		SLL_UNIMPLEMENTED();
	}
	UNLOCK;
	return ZERO_IF_ERROR_PTR(out);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_file_read_all(sll_file_t* file,sll_string_t* out){
	SLL_INIT_STRING(out);
	if (!(file->flags&SLL_FILE_FLAG_READ)){
		return SLL_NO_ERROR;
	}
	if (file->flags&SLL_FILE_FLAG_SOCKET){
		SLL_UNIMPLEMENTED();
	}
	if (file->data.file._hash.buffer_offset!=DISABLE_FILE_HASH){
		SLL_UNIMPLEMENTED();
	}
	if (file->flags&FILE_FLAG_MEMORY){
		SLL_UNIMPLEMENTED();
	}
	if (file->data.file.source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		return SLL_ERROR_UNKNOWN_FD;
	}
	LOCK;
	if (file->flags&SLL_FILE_FLAG_NO_BUFFER){
		SLL_UNIMPLEMENTED();
	}
	if (file->data.file._read_buffer_size){
		SLL_UNIMPLEMENTED();
	}
	sll_error_t err;
	sll_string_length_t sz;
	do{
		sz=(sll_string_length_t)sll_platform_file_read(file->data.file.source.file.fd,file->data.file._read_buffer,FILE_BUFFER_SIZE,&err);
		if (err!=SLL_NO_ERROR){
			sll_free_string(out);
			UNLOCK;
			return err;
		}
		sll_string_increase(out,sz);
		out->length+=sz;
		sll_string_insert_pointer_length(file->data.file._read_buffer,sz,out->length-sz,out);
	} while (sz==FILE_BUFFER_SIZE);
	file->data.file._read_buffer_offset=0;
	file->data.file._read_buffer_size=0;
	UNLOCK;
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL sll_read_char_t sll_file_read_char(sll_file_t* file,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(file->flags&SLL_FILE_FLAG_READ)){
		return SLL_END_OF_DATA;
	}
	LOCK;
	sll_char_t out;
	if (file->flags&SLL_FILE_FLAG_SOCKET){
		sll_read_char_t ret=(sll_platform_socket_read(file->data.socket.fd,&out,1,err)?out:SLL_END_OF_DATA);
		UNLOCK;
		return ret;
	}
	if (file->flags&FILE_FLAG_MEMORY){
		if (file->data.file._offset==file->data.file.source.memory.size){
			UNLOCK;
			return SLL_END_OF_DATA;
		}
		out=*((sll_char_t*)(PTR(ADDR(file->data.file.source.memory.pointer)+file->data.file._offset)));
	}
	else if (file->data.file.source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		UNLOCK;
		return SLL_END_OF_DATA;
	}
	else if (file->flags&SLL_FILE_FLAG_NO_BUFFER){
		if (!sll_platform_file_read(file->data.file.source.file.fd,&out,sizeof(sll_char_t),err)){
			UNLOCK;
			return SLL_END_OF_DATA;
		}
	}
	else{
		if (!file->data.file._read_buffer_offset&&!file->data.file._read_buffer_size){
			file->data.file._read_buffer_size=sll_platform_file_read(file->data.file.source.file.fd,file->data.file._read_buffer,FILE_BUFFER_SIZE,err);
			if (!file->data.file._read_buffer_size){
				UNLOCK;
				return SLL_END_OF_DATA;
			}
		}
		out=file->data.file._read_buffer[file->data.file._read_buffer_offset];
		file->data.file._read_buffer_offset++;
		if (file->data.file._read_buffer_offset==file->data.file._read_buffer_size){
			file->data.file._read_buffer_offset=0;
			file->data.file._read_buffer_size=sll_platform_file_read(file->data.file.source.file.fd,file->data.file._read_buffer,FILE_BUFFER_SIZE,err);
		}
	}
	file->data.file._offset++;
	if (out=='\n'){
		file->data.file._line_number++;
	}
	if (file->data.file._hash.buffer_offset!=DISABLE_FILE_HASH){
		file->data.file._hash.buffer[file->data.file._hash.buffer_offset]=out;
		if (file->data.file._hash.buffer_offset==63){
			file->data.file._hash.buffer_offset=0;
			sll_hash_sha256(&(file->data.file._hash.hash),file->data.file._hash.buffer,64);
		}
		else{
			file->data.file._hash.buffer_offset++;
		}
	}
	UNLOCK;
	return DATA_IF_ERROR_PTR(out,SLL_END_OF_DATA);
}



__SLL_EXTERNAL sll_error_t sll_file_reset(sll_file_t* file){
	if (file->flags&SLL_FILE_FLAG_SOCKET){
		return SLL_NO_ERROR;
	}
	sll_error_t err=SLL_NO_ERROR;
	LOCK;
	if (!(file->flags&FILE_FLAG_MEMORY)){
		if (file->data.file.source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
			UNLOCK;
			return SLL_ERROR_UNKNOWN_FD;
		}
		err=sll_platform_file_seek(file->data.file.source.file.fd,0);
		if (err!=SLL_NO_ERROR){
			UNLOCK;
			return err;
		}
		file->data.file._read_buffer_offset=0;
		file->data.file._read_buffer_size=sll_platform_file_read(file->data.file.source.file.fd,file->data.file._read_buffer,FILE_BUFFER_SIZE,&err);
	}
	file->data.file._line_number=0;
	file->data.file._offset=0;
	if (file->data.file._hash.buffer_offset!=DISABLE_FILE_HASH){
		_file_start_hash(file);
	}
	UNLOCK;
	return err;
}



__SLL_EXTERNAL sll_size_t sll_file_write(sll_file_t* file,const void* pointer,sll_size_t size,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(file->flags&SLL_FILE_FLAG_WRITE)||!size){
		return 0;
	}
	LOCK;
	if (file->flags&SLL_FILE_FLAG_SOCKET){
		sll_size_t out=sll_platform_socket_write(file->data.socket.fd,pointer,size,err);
		UNLOCK;
		return out;
	}
	if (file->flags&FILE_FLAG_MEMORY){
		if (file->flags&FILE_FLAG_DYNAMIC_BUFFERS){
			dynamic_buffer_chunk_t* c=file->data.file._write_buffer.dynamic.end;
			sll_size_t capacity=c->size-file->data.file._write_buffer.dynamic.offset;
			file->data.file._write_buffer.dynamic.size+=size;
			if (capacity>=size){
				sll_copy_data(pointer,size,c->data+file->data.file._write_buffer.dynamic.offset);
				file->data.file._write_buffer.dynamic.offset+=size;
			}
			else{
				sll_copy_data(pointer,capacity,c->data+file->data.file._write_buffer.dynamic.offset);
				size-=capacity;
				file->data.file._write_buffer.dynamic.offset=size;
				sll_size_t round_size=SLL_ROUND_PAGE((size+sizeof(dynamic_buffer_chunk_t)<FILE_DYNAMIC_BUFFER_ALLOC_SIZE?FILE_DYNAMIC_BUFFER_ALLOC_SIZE:size+sizeof(dynamic_buffer_chunk_t)));
				dynamic_buffer_chunk_t* buffer=sll_platform_allocate_page(round_size,0,NULL);
				buffer->size=round_size-sizeof(dynamic_buffer_chunk_t);
				buffer->next=NULL;
				c->next=buffer;
				file->data.file._write_buffer.dynamic.end=buffer;
				sll_copy_data(PTR(ADDR(pointer)+capacity),size,buffer->data);
			}
			UNLOCK;
			return size;
		}
		if (file->data.file._offset+size>=file->data.file.source.memory.size){
			size=file->data.file.source.memory.size-file->data.file._offset;
		}
		sll_copy_data(pointer,size,((sll_char_t*)(file->data.file.source.memory.pointer))+file->data.file._offset);
		file->data.file._offset+=size;
		UNLOCK;
		return size;
	}
	if (file->data.file.source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		UNLOCK;
		return 0;
	}
	if (file->flags&SLL_FILE_FLAG_NO_BUFFER){
		sll_size_t out=sll_platform_file_write(file->data.file.source.file.fd,pointer,size,err);
		UNLOCK;
		return out;
	}
	sll_bool_t flush=((file->flags&SLL_FILE_FLUSH_ON_NEWLINE)?sll_contains_character(pointer,size,'\n'):0);
	if (size+file->data.file._write_buffer.static_.offset<=FILE_BUFFER_SIZE){
		sll_copy_data(pointer,size,file->data.file._write_buffer.static_.pointer+file->data.file._write_buffer.static_.offset);
		file->data.file._write_buffer.static_.offset+=size;
		if (file->data.file._write_buffer.static_.offset==FILE_BUFFER_SIZE){
			sll_platform_file_write(file->data.file.source.file.fd,file->data.file._write_buffer.static_.pointer,FILE_BUFFER_SIZE,err);
			file->data.file._write_buffer.static_.offset=0;
		}
		UNLOCK;
		if (flush){
			sll_file_flush(file);
		}
		return ZERO_IF_ERROR_PTR(size);
	}
	sll_size_t i=FILE_BUFFER_SIZE-file->data.file._write_buffer.static_.offset;
	sll_copy_data(pointer,i,file->data.file._write_buffer.static_.pointer+file->data.file._write_buffer.static_.offset);
	sll_platform_file_write(file->data.file.source.file.fd,file->data.file._write_buffer.static_.pointer,FILE_BUFFER_SIZE,err);
	const sll_char_t* v=((const sll_char_t*)pointer)+i;
	sll_size_t out=size;
	size-=i;
	i=size&(FILE_BUFFER_SIZE-1);
	size-=i;
	if (size){
		sll_platform_file_write(file->data.file.source.file.fd,v,size,err);
		v+=size;
	}
	if (i){
		sll_copy_data(v,i,file->data.file._write_buffer.static_.pointer);
	}
	file->data.file._write_buffer.static_.offset=i;
	UNLOCK;
	if (flush){
		sll_file_flush(file);
	}
	return ZERO_IF_ERROR_PTR(out);
}



__SLL_EXTERNAL sll_bool_t sll_file_write_char(sll_file_t* file,sll_char_t char_,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(file->flags&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	if (file->flags&SLL_FILE_FLAG_SOCKET){
		SLL_UNIMPLEMENTED();
	}
	if (file->flags&FILE_FLAG_MEMORY){
		LOCK;
		if (file->flags&FILE_FLAG_DYNAMIC_BUFFERS){
			dynamic_buffer_chunk_t* k=file->data.file._write_buffer.dynamic.end;
			if (k->size>file->data.file._write_buffer.dynamic.offset){
				k->data[file->data.file._write_buffer.dynamic.offset]=char_;
				file->data.file._write_buffer.dynamic.size++;
				file->data.file._write_buffer.dynamic.offset++;
				UNLOCK;
				return 1;
			}
			SLL_UNIMPLEMENTED();
			UNLOCK;
			return 1;
		}
		if (file->data.file._offset+1>=file->data.file.source.memory.size){
			UNLOCK;
			return 0;
		}
		*(((sll_char_t*)(file->data.file.source.memory.pointer))+file->data.file._offset)=char_;
		file->data.file._offset++;
		UNLOCK;
		return 1;
	}
	if (file->data.file.source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	if (file->flags&SLL_FILE_FLAG_NO_BUFFER){
		sll_bool_t out=sll_platform_file_write(file->data.file.source.file.fd,&char_,sizeof(sll_char_t),err);
		return out;
	}
	LOCK;
	SLL_ASSERT(file->data.file._write_buffer.static_.offset+1<=FILE_BUFFER_SIZE);
	file->data.file._write_buffer.static_.pointer[file->data.file._write_buffer.static_.offset]=char_;
	file->data.file._write_buffer.static_.offset++;
	if (file->data.file._write_buffer.static_.offset==FILE_BUFFER_SIZE){
		sll_platform_file_write(file->data.file.source.file.fd,file->data.file._write_buffer.static_.pointer,FILE_BUFFER_SIZE,err);
		file->data.file._write_buffer.static_.offset=0;
	}
	UNLOCK;
	if ((file->flags&SLL_FILE_FLUSH_ON_NEWLINE)&&char_=='\n'){
		sll_file_flush(file);
	}
	return ZERO_IF_ERROR_PTR(1);
}



__SLL_EXTERNAL sll_size_t sll_file_write_char_count(sll_file_t* file,sll_char_t char_,sll_size_t count,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(file->flags&SLL_FILE_FLAG_WRITE)||!count){
		return 0;
	}
	if (file->flags&SLL_FILE_FLAG_SOCKET){
		SLL_UNIMPLEMENTED();
	}
	if (file->flags&FILE_FLAG_MEMORY){
		if (file->flags&FILE_FLAG_DYNAMIC_BUFFERS){
			SLL_UNIMPLEMENTED();
		}
		LOCK;
		if (file->data.file._offset+count>=file->data.file.source.memory.size){
			count=file->data.file.source.memory.size-file->data.file._offset;
		}
		sll_set_memory(((sll_char_t*)(file->data.file.source.memory.pointer))+file->data.file._offset,count,char_);
		file->data.file._offset+=count;
		UNLOCK;
		return count;
	}
	if (file->data.file.source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	if (file->flags&SLL_FILE_FLAG_NO_BUFFER){
		sll_size_t out=0;
		while (count){
			out+=sll_platform_file_write(file->data.file.source.file.fd,&char_,sizeof(sll_char_t),err);
			count--;
		}
		return ZERO_IF_ERROR_PTR(out);
	}
	LOCK;
	if (count+file->data.file._write_buffer.static_.offset<=FILE_BUFFER_SIZE){
		sll_set_memory(file->data.file._write_buffer.static_.pointer+file->data.file._write_buffer.static_.offset,count,char_);
		file->data.file._write_buffer.static_.offset+=count;
		if (file->data.file._write_buffer.static_.offset==FILE_BUFFER_SIZE){
			sll_platform_file_write(file->data.file.source.file.fd,file->data.file._write_buffer.static_.pointer,FILE_BUFFER_SIZE,err);
			file->data.file._write_buffer.static_.offset=0;
		}
		UNLOCK;
		if ((file->flags&SLL_FILE_FLUSH_ON_NEWLINE)&&char_=='\n'){
			sll_file_flush(file);
		}
		return ZERO_IF_ERROR_PTR(count);
	}
	sll_size_t i=FILE_BUFFER_SIZE-file->data.file._write_buffer.static_.offset;
	sll_set_memory(file->data.file._write_buffer.static_.pointer+file->data.file._write_buffer.static_.offset,i,char_);
	sll_platform_file_write(file->data.file.source.file.fd,file->data.file._write_buffer.static_.pointer,FILE_BUFFER_SIZE,err);
	sll_size_t out=count;
	count-=i;
	i=count&(FILE_BUFFER_SIZE-1);
	count-=i;
	if (count){
		sll_set_memory(file->data.file._write_buffer.static_.pointer,FILE_BUFFER_SIZE,char_);
		while (count){
			sll_platform_file_write(file->data.file.source.file.fd,file->data.file._write_buffer.static_.pointer,FILE_BUFFER_SIZE,err);
			count-=FILE_BUFFER_SIZE;
		}
	}
	if (i){
		sll_set_memory(file->data.file._write_buffer.static_.pointer,i,char_);
	}
	file->data.file._write_buffer.static_.offset=i;
	UNLOCK;
	if ((file->flags&SLL_FILE_FLUSH_ON_NEWLINE)&&char_=='\n'){
		sll_file_flush(file);
	}
	return ZERO_IF_ERROR_PTR(out);
}



__SLL_EXTERNAL sll_size_t sll_file_write_format(sll_file_t* file,const sll_char_t* format,sll_error_t* err,...){
	ERROR_PTR_RESET;
	if (!(file->flags&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	if (file->flags&SLL_FILE_FLAG_SOCKET){
		SLL_UNIMPLEMENTED();
	}
	if (!(file->flags&FILE_FLAG_MEMORY)&&file->data.file.source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	va_list va;
	va_start(va,err);
	sll_var_arg_list_t va_list_;
	SLL_VAR_ARG_INIT_C(&va_list_,&va);
	sll_string_t str;
	sll_string_format_list(format,sll_string_length(format),&va_list_,&str);
	va_end(va);
	sll_size_t out=sll_file_write(file,str.data,str.length,err);
	sll_free_string(&str);
	return out;
}



__SLL_EXTERNAL sll_size_t sll_file_write_string(sll_file_t* file,const sll_char_t* string,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!(file->flags&SLL_FILE_FLAG_WRITE)){
		return 0;
	}
	if (file->flags&SLL_FILE_FLAG_SOCKET){
		SLL_UNIMPLEMENTED();
	}
	if (!(file->flags&FILE_FLAG_MEMORY)&&file->data.file.source.file.fd==SLL_UNKNOWN_FILE_DESCRIPTOR){
		ERROR_PTR(SLL_ERROR_UNKNOWN_FD);
		return 0;
	}
	return sll_file_write(file,string,sll_string_length(string),err);
}

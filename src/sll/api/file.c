#include <sll/_internal/api/file.h>
#include <sll/_internal/file.h>
#include <sll/_internal/static_object.h>
#include <sll/api/file.h>
#include <sll/api/path.h>
#include <sll/api/string.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/container.h>
#include <sll/data.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/platform/path.h>
#include <sll/sandbox.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>



static sll_handle_container_t _file_data=SLL_HANDLE_CONTAINER_INIT_STRUCT;
static sll_bool_t _file_cleanup=0;



static void _release_data(void){
	SLL_HANDLE_CONTAINER_ITER_CLEAR(&_file_data,extended_file_t,file,{
		sll_file_close((file->is_pointer?file->data.pointer:&(file->data.struct_)));
		sll_deallocate(file);
	});
	_file_cleanup=0;
}



static sll_file_handle_t _alloc_file(void){
	sll_size_t o;
	SLL_HANDLE_CONTAINER_ALLOC(&_file_data,&o);
	extended_file_t* data=sll_allocate(sizeof(extended_file_t));
	data->reference_count=1;
	data->is_pointer=0;
	data->data_pointer=NULL;
	*(_file_data.data+o)=data;
	return (sll_file_handle_t)o;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_file_close(sll_file_handle_t handle){
	handle--;
	if (!SLL_HANDLE_CONTAINER_CHECK(&_file_data,handle)){
		return 0;
	}
	extended_file_t* file=*(_file_data.data+handle);
	file->reference_count--;
	if (file->reference_count){
		return 1;
	}
	sll_file_close((file->is_pointer?file->data.pointer:&(file->data.struct_)));
	if (file->data_pointer){
		sll_deallocate(file->data_pointer);
	}
	sll_deallocate(file);
	SLL_HANDLE_CONTAINER_DEALLOC(&_file_data,handle);
	return 1;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_copy(const sll_string_t* src,const sll_string_t* dst){
	if (src->length>SLL_API_MAX_FILE_PATH_LENGTH||dst->length>SLL_API_MAX_FILE_PATH_LENGTH){
		return SLL_ERROR_TOO_LONG;
	}
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_COPY)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
	}
	sll_audit(SLL_CHAR("sll.file.copy"),SLL_CHAR("ss"),src,dst);
	return sll_platform_path_copy(src->data,dst->data);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_delete(const sll_string_t* path){
	if (path->length>SLL_API_MAX_FILE_PATH_LENGTH){
		return SLL_ERROR_TOO_LONG;
	}
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_DELETE)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
	}
	sll_audit(SLL_CHAR("sll.file.delete"),SLL_CHAR("s"),path);
	return sll_platform_path_delete(path->data);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_file_flush(sll_file_handle_t handle){
	handle--;
	if (!SLL_HANDLE_CONTAINER_CHECK(&_file_data,handle)){
		return 0;
	}
	extended_file_t* data=*(_file_data.data+handle);
	sll_file_flush((data->is_pointer?data->data.pointer:&(data->data.struct_)));
	return 1;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_from_data(const sll_string_t* data,sll_file_flags_t flags,sll_file_handle_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_BUFFER_FILES)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_FILE_IO);
	}
	void* ptr=sll_allocate(data->length);
	sll_copy_data(data->data,data->length,ptr);
	sll_file_t f;
	sll_file_from_data(ptr,data->length,(sll_file_flags_t)(flags&(SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|SLL_FILE_FLAG_NO_BUFFER)),&f);
	sll_file_handle_t h=_alloc_file();
	extended_file_t* f_data=*(_file_data.data+h);
	sll_copy_data(&f,sizeof(sll_file_t),&(f_data->data.struct_));
	f_data->is_pointer=0;
	f_data->data_pointer=ptr;
	*out=h+1;
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_file_get_buffer(sll_file_handle_t handle,sll_string_t* out){
	SLL_INIT_STRING(out);
	handle--;
	if (!SLL_HANDLE_CONTAINER_CHECK(&_file_data,handle)){
		return;
	}
	extended_file_t* data=*(_file_data.data+handle);
	sll_file_get_buffer((data->is_pointer?data->data.pointer:&(data->data.struct_)),out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_file_get_temp_path(sll_string_t* out){
	sll_string_clone(sll_temporary_file_path,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_file_inc_handle(sll_file_handle_t handle){
	handle--;
	if (!SLL_HANDLE_CONTAINER_CHECK(&_file_data,handle)){
		return;
	}
	((extended_file_t*)(*(_file_data.data+handle)))->reference_count++;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_open(const sll_string_t* path,sll_file_flags_t flags,sll_file_handle_t* out){
	if (path->length>SLL_API_MAX_FILE_PATH_LENGTH){
		return SLL_ERROR_TOO_LONG;
	}
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_FILE_IO);
	}
	flags&=SLL_FILE_FLAG_READ|SLL_FILE_FLAG_WRITE|SLL_FILE_FLAG_APPEND|SLL_FILE_FLAG_NO_BUFFER|SLL_FILE_FLUSH_ON_NEWLINE;
	sll_audit(SLL_CHAR("sll.file.open"),SLL_CHAR("sh"),path,flags);
	sll_file_t f;
	sll_error_t err=sll_file_open(path->data,flags,&f);
	if (err!=SLL_NO_ERROR){
		return err;
	}
	sll_file_handle_t handle=_alloc_file();
	extended_file_t* data=*(_file_data.data+handle);
	sll_copy_data(&f,sizeof(sll_file_t),&(data->data.struct_));
	data->is_pointer=0;
	*out=handle+1;
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_peek(sll_file_handle_t handle,sll_char_t* out){
	handle--;
	if (!SLL_HANDLE_CONTAINER_CHECK(&_file_data,handle)){
		return SLL_ERROR_UNKNOWN_FD;
	}
	extended_file_t* data=*(_file_data.data+handle);
	sll_error_t err;
	sll_read_char_t o=sll_file_peek_char((data->is_pointer?data->data.pointer:&(data->data.struct_)),&err);
	if (o==SLL_END_OF_DATA){
		return (err==SLL_NO_ERROR?SLL_ERROR_EOF:err);
	}
	*out=(sll_char_t)o;
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_read(sll_file_handle_t handle,sll_string_length_t size,sll_string_t* out){
	handle--;
	if (!SLL_HANDLE_CONTAINER_CHECK(&_file_data,handle)){
		return SLL_ERROR_UNKNOWN_FD;
	}
	extended_file_t* data=*(_file_data.data+handle);
	sll_file_t* f=(data->is_pointer?data->data.pointer:&(data->data.struct_));
	if ((f->flags&SLL_FILE_FLAG_ASYNC)&&!sll_file_data_available(f)){
		SLL_INIT_STRING(out);
		return SLL_NO_ERROR;
	}
	if (!size){
		return sll_file_read_all(f,out);
	}
	sll_string_create(size,out);
	sll_error_t err;
	sll_size_t sz=sll_file_read(f,out->data,size,&err);
	if (!sz&&err!=SLL_NO_ERROR){
		sll_free_string(out);
		return err;
	}
	sll_string_decrease(out,(sll_string_length_t)sz);
	sll_string_calculate_checksum(out);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_read_char(sll_file_handle_t handle,sll_char_t* out){
	handle--;
	if (!SLL_HANDLE_CONTAINER_CHECK(&_file_data,handle)){
		return SLL_ERROR_UNKNOWN_FD;
	}
	extended_file_t* data=*(_file_data.data+handle);
	sll_error_t err;
	sll_read_char_t c=sll_file_read_char((data->is_pointer?data->data.pointer:&(data->data.struct_)),&err);
	if (c==SLL_END_OF_DATA){
		return (err==SLL_NO_ERROR?SLL_ERROR_EOF:err);
	}
	*out=(sll_char_t)c;
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_rename(const sll_string_t* src,const sll_string_t* dst){
	if (src->length>SLL_API_MAX_FILE_PATH_LENGTH||dst->length>SLL_API_MAX_FILE_PATH_LENGTH){
		return SLL_ERROR_TOO_LONG;
	}
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_PATH_API)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_FILE_RENAME)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_PATH_API);
	}
	sll_audit(SLL_CHAR("sll.file.rename"),SLL_CHAR("ss"),src,dst);
	return sll_platform_path_rename(src->data,dst->data);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_std_handle(sll_char_t id,sll_file_handle_t* out){
	if (!_file_cleanup){
		sll_register_cleanup(_release_data,SLL_CLEANUP_TYPE_VM);
		_file_cleanup=1;
	}
	sll_file_t* p=NULL;
	if (!id){
		if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDIN_IO)){
			return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_FILE_IO);
		}
		p=sll_current_vm_config->in;
	}
	else if (id==1){
		if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)&&!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_ENABLE_STDOUT_IO)){
			return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_FILE_IO);
		}
		p=sll_current_vm_config->out;
	}
	else{
		if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_FILE_IO)){
			return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_FILE_IO);
		}
		p=sll_current_vm_config->err;
	}
	sll_file_handle_t handle=_alloc_file();
	extended_file_t* data=*(_file_data.data+handle);
	data->data.pointer=p;
	data->is_pointer=1;
	*out=handle+1;
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_file_write(sll_file_handle_t handle,const sll_string_t* data,sll_size_t* out){
	handle--;
	if (!SLL_HANDLE_CONTAINER_CHECK(&_file_data,handle)){
		return SLL_ERROR_UNKNOWN_FD;
	}
	extended_file_t* f_data=*(_file_data.data+handle);
	sll_error_t err;
	*out=sll_file_write((f_data->is_pointer?f_data->data.pointer:&(f_data->data.struct_)),data->data,data->length*sizeof(sll_char_t),&err);
	return err;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_t* sll_file_from_handle(sll_file_handle_t handle){
	handle--;
	if (!SLL_HANDLE_CONTAINER_CHECK(&_file_data,handle)){
		return NULL;
	}
	extended_file_t* data=*(_file_data.data+handle);
	return (data->is_pointer?data->data.pointer:&(data->data.struct_));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_file_handle_t sll_file_to_handle(sll_file_t* f,sll_bool_t alloc){
	sll_file_handle_t out=_alloc_file();
	extended_file_t* data=*(_file_data.data+out);
	if (alloc){
		sll_copy_data(f,sizeof(sll_file_t),&(data->data.struct_));
	}
	else{
		data->data.pointer=f;// lgtm [cpp/stack-address-escape]
	}
	data->is_pointer=!alloc;
	return out+1;
}

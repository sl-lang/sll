#include <sll/_internal/common.h>
#include <sll/_internal/print.h>
#include <sll/_internal/static_object.h>
#include <sll/_internal/static_string.h>
#include <sll/_internal/string.h>
#include <sll/abi.h>
#include <sll/api/hash.h>
#include <sll/api/path.h>
#include <sll/api/sys.h>
#include <sll/array.h>
#include <sll/audit.h>
#include <sll/common.h>
#include <sll/container.h>
#include <sll/data.h>
#include <sll/environment.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/platform/file.h>
#include <sll/platform/library.h>
#include <sll/platform/path.h>
#include <sll/platform/thread.h>
#include <sll/platform/util.h>
#include <sll/sandbox.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/version.h>
#include <sll/vm.h>



#if SLL_VERSION_HAS_SHA
static __STATIC_STRING(_sys_full_commit,SLL_VERSION_FULL_SHA);
#endif
static sll_argc_t _sys_argc=0;
static sll_string_t* _sys_argv=NULL;
static sll_container_t _sys_library_data=SLL_CONTAINER_INIT_STRUCT;
static sll_bool_t _sys_init=0;
static sll_bool_t _sys_vm_init=0;



static void _cleanup_data(void){
	while (_sys_argc){
		_sys_argc--;
		sll_free_string(_sys_argv+_sys_argc);
	}
	sll_deallocate(_sys_argv);
	_sys_argv=NULL;
	_sys_init=0;
}



static void _cleanup_vm_data(void){
	SLL_CONTAINER_ITER_CLEAR(&_sys_library_data,sll_loaded_library_t*,library,{
		sll_free_string((sll_string_t*)&(library->name));
		void (*fn)(void)=sll_platform_lookup_symbol(library->handle,SLL_ABI_NAME(SLL_ABI_DEINIT));
		if (fn){
			fn();
		}
		SLL_CRITICAL_ERROR(sll_platform_unload_library(library->handle));
		sll_deallocate(library);
	});
	_sys_vm_init=0;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_args(sll_array_t* out){
	sll_object_t* obj=sll_new_object(SLL_CHAR("s+"),_sys_argv,_sys_argc);
	*out=obj->data.array;
	SLL_CRITICAL(sll_destroy_object(obj));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_cpu_t sll_api_sys_get_cpu_count(void){
	return *sll_platform_cpu_count;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_env(sll_map_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT)){
		SLL_INIT_MAP(out);
		return;
	}
	sll_map_create(sll_environment->length,out);
	for (sll_environment_length_t i=0;i<sll_environment->length;i++){
		const sll_environment_variable_t* kv=*(sll_environment->data+i);
		out->data[i<<1]=STRING_TO_OBJECT(&(kv->key));
		out->data[(i<<1)+1]=STRING_TO_OBJECT(&(kv->value));
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_executable(sll_string_t* out){
	sll_string_clone(sll_executable_file_path,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_library(sll_string_t* out){
	sll_string_clone(sll_library_file_path,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_platform(sll_string_t* out){
	sll_string_clone(sll_platform_string,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_sandbox_flags_t sll_api_sys_get_sandbox_flags(void){
	return sll_get_sandbox_flags();
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_version(sll_array_t* out){
#if SLL_VERSION_HAS_SHA
	sll_new_object_array(SLL_CHAR("hhhsi"),out,SLL_VERSION_MAJOR,SLL_VERSION_MINOR,SLL_VERSION_PATCH,&_sys_full_commit,SLL_VERSION_BUILD_TIME);
#else
	sll_new_object_array(SLL_CHAR("hhhZi"),out,SLL_VERSION_MAJOR,SLL_VERSION_MINOR,SLL_VERSION_PATCH,SLL_VERSION_BUILD_TIME);
#endif
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_sys_load_library(const sll_string_t* path){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_LOAD_LIBRARY)){
		return SLL_ERROR_FROM_SANDBOX(SLL_SANDBOX_FLAG_DISABLE_LOAD_LIBRARY);
	}
	if (path->length>=SLL_API_MAX_FILE_PATH_LENGTH){
		return SLL_ERROR_TOO_LONG;
	}
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH];
	sll_string_length_t bfl=sll_platform_absolute_path(path->data,bf,SLL_API_MAX_FILE_PATH_LENGTH);
	sll_string_t full_lib_path;
	if (bfl){
		sll_string_from_pointer_length(bf,bfl,&full_lib_path);
	}
	else{
		sll_string_clone(path,&full_lib_path);
	}
	sll_error_t err;
	if (!sll_platform_path_exists(path->data)){
		err=SLL_ERROR_NO_FILE_PATH;
		goto _error;
	}
	SLL_CONTAINER_ITER(&_sys_library_data,sll_loaded_library_t*,library,{
		if (STRING_EQUAL(&(library->name),&full_lib_path)){
			sll_audit(SLL_CHAR("sll.sys.library.load"),SLL_CHAR("si"),&full_lib_path,library->handle);
			sll_free_string(&full_lib_path);
			return SLL_NO_ERROR;
		}
	});
	sll_library_handle_t h=sll_platform_load_library(path->data,&err);
	if (!h){
		goto _error;
	}
	sll_bool_t (*fn)(sll_version_t)=sll_platform_lookup_symbol(h,SLL_ABI_NAME(SLL_ABI_INIT));
	if (!fn||!fn(SLL_VERSION)){
		SLL_CRITICAL_ERROR(sll_platform_unload_library(h));
		sll_free_string(&full_lib_path);
		SLL_UNIMPLEMENTED();
	}
	sll_loaded_library_t* data=sll_allocate(sizeof(sll_loaded_library_t));
	sll_copy_data(&full_lib_path,sizeof(sll_string_t),(sll_string_t*)(&(data->name)));
	data->handle=h;
	SLL_CONTAINER_PUSH(&_sys_library_data,sll_loaded_library_t*,data);
	sll_audit(SLL_CHAR("sll.sys.library.load"),SLL_CHAR("si"),&full_lib_path,h);
	if (!_sys_vm_init){
		sll_register_cleanup(_cleanup_vm_data,SLL_CLEANUP_TYPE_VM);
		_sys_vm_init=1;
	}
	const sll_internal_function_table_descriptor_t*const* ift_desc=sll_platform_lookup_symbol(h,SLL_ABI_NAME(SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR));
	if (ift_desc&&*ift_desc){
		sll_register_internal_functions(sll_current_runtime_data->internal_function_table,(*ift_desc)->data,(*ift_desc)->length);
	}
	return SLL_NO_ERROR;
_error:
	sll_audit(SLL_CHAR("sll.sys.library.load.error"),SLL_CHAR("si"),&full_lib_path,err);
	sll_free_string(&full_lib_path);
	return err;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_remove_env(const sll_string_t* key){
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT)){
		sll_audit(SLL_CHAR("sll.sys.env.delete"),SLL_CHAR("s"),key);
		sll_remove_environment_variable(key);
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_set_env(const sll_string_t* key,const sll_string_t* value){
	if (!sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_ENVIRONMENT)){
		sll_audit(SLL_CHAR("sll.sys.env.set"),SLL_CHAR("ss"),key,value);
		sll_set_environment_variable(key,value);
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_set_sandbox_flag(sll_sandbox_flag_t flag){
	sll_audit(SLL_CHAR("sll.sys.sandbox.set"),SLL_CHAR("u"),flag);
	sll_set_sandbox_flag(flag);
}



__SLL_EXTERNAL const sll_loaded_library_t*const* sll_get_loaded_libraries(sll_size_t* count){
	*count=_sys_library_data.size;
	return (const sll_loaded_library_t*const*)(_sys_library_data.data);
}



__SLL_EXTERNAL void sll_set_argument(sll_argc_t index,const sll_char_t* value){
	if (index>=_sys_argc){
		return;
	}
	sll_free_string(_sys_argv+index);
	sll_string_from_pointer(value,_sys_argv+index);
}



__SLL_EXTERNAL void sll_set_argument_count(sll_argc_t arg_count){
	SLL_ASSERT(arg_count>0);
	if (_sys_argv){
		for (sll_argc_t i=0;i<_sys_argc;i++){
			sll_free_string(_sys_argv+i);
		}
		sll_deallocate(_sys_argv);
	}
	else if (!_sys_init){
		sll_register_cleanup(_cleanup_data,SLL_CLEANUP_TYPE_GLOBAL);
		_sys_init=1;
	}
	_sys_argc=arg_count;
	_sys_argv=sll_allocate(arg_count*sizeof(sll_string_t));
	for (sll_argc_t i=0;i<arg_count;i++){
		SLL_INIT_STRING(_sys_argv+i);
	}
}

#include <sll/_internal/atexit.h>
#include <sll/_internal/common.h>
#include <sll/_internal/vm.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/memory.h>
#include <sll/platform/lock.h>
#include <sll/types.h>
#include <sll/vm.h>



static atexit_function_t** _atexit_data=NULL;
static sll_array_length_t _atexit_data_len=0;
static sll_lock_handle_t _atexit_lock=NULL;
static sll_bool_t _atexit_enable=1;



static void _cleanup_data(void){
	if (_atexit_lock){
		SLL_CRITICAL(sll_platform_lock_delete(_atexit_lock));
		_atexit_lock=NULL;
	}
}



static void _delete_atexit_function(atexit_function_t* function){
	sll_gc_remove_roots(function->args);
	while (function->arg_count){
		function->arg_count--;
		SLL_RELEASE(function->args[function->arg_count]);
	}
	sll_deallocate(function);
}



void _atexit_execute(void){
	if (!_atexit_lock){
		return;
	}
	_atexit_enable=0;
	while (_atexit_data_len){
		_atexit_data_len--;
		atexit_function_t* function=*(_atexit_data+_atexit_data_len);
		SLL_RELEASE(sll_execute_function(function->function,function->args,function->arg_count,EXECUTE_FUNCTION_NO_AUDIT_TERMINATE));
		_delete_atexit_function(function);
	}
	sll_deallocate(_atexit_data);
	_atexit_data=NULL;
	_atexit_enable=1;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_atexit_register(sll_integer_t function,sll_object_t*const* args,sll_arg_count_t arg_count){
	if (!function||!_atexit_enable){
		return;
	}
	if (!_atexit_lock){
		_atexit_lock=sll_platform_lock_create(NULL);
		sll_register_cleanup(_cleanup_data,SLL_CLEANUP_TYPE_GLOBAL);
	}
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_atexit_lock));
	_atexit_data_len++;
	_atexit_data=sll_reallocate(_atexit_data,_atexit_data_len*sizeof(atexit_function_t*));
	atexit_function_t* function_data=sll_allocate(sizeof(atexit_function_t)+arg_count*sizeof(sll_object_t*));
	function_data->function=function;
	function_data->arg_count=arg_count;
	for (sll_arg_count_t i=0;i<arg_count;i++){
		SLL_ACQUIRE(*(args+i));
		function_data->args[i]=*(args+i);
	}
	sll_gc_add_roots(function_data->args,arg_count);
	*(_atexit_data+_atexit_data_len-1)=function_data;
	SLL_CRITICAL(sll_platform_lock_release(_atexit_lock));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_atexit_unregister(sll_integer_t function){
	if (!function||!_atexit_lock||!_atexit_enable){
		return 0;
	}
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_atexit_lock));
	sll_bool_t o=0;
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<_atexit_data_len;j++){
		atexit_function_t* function_data=*(_atexit_data+j);
		if (function_data->function==function){
			o=1;
			_delete_atexit_function(function_data);
		}
		else{
			*(_atexit_data+i)=function_data;
			i++;
		}
	}
	_atexit_data_len=i;
	_atexit_data=sll_reallocate(_atexit_data,_atexit_data_len*sizeof(atexit_function_t*));
	SLL_CRITICAL(sll_platform_lock_release(_atexit_lock));
	return o;
}

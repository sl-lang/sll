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



static void _delete_atexit_function(atexit_function_t* af){
	while (af->arg_count){
		af->arg_count--;
		SLL_RELEASE(af->args[af->arg_count]);
	}
	sll_deallocate(af);
}



void _atexit_execute(void){
	if (!_atexit_lock){
		return;
	}
	_atexit_enable=0;
	while (_atexit_data_len){
		_atexit_data_len--;
		atexit_function_t* af=*(_atexit_data+_atexit_data_len);
		SLL_RELEASE(sll_execute_function(af->function,af->args,af->arg_count,EXECUTE_FUNCTION_NO_AUDIT_TERMINATE));
		_delete_atexit_function(af);
	}
	sll_deallocate(_atexit_data);
	_atexit_data=NULL;
	_atexit_enable=1;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_atexit_register(sll_integer_t fn,sll_object_t*const* args,sll_arg_count_t len){
	if (!fn||!_atexit_enable){
		return;
	}
	if (!_atexit_lock){
		_atexit_lock=sll_platform_lock_create(NULL);
		sll_register_cleanup(_cleanup_data,SLL_CLEANUP_TYPE_GLOBAL);
	}
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_atexit_lock));
	_atexit_data_len++;
	_atexit_data=sll_reallocate(_atexit_data,_atexit_data_len*sizeof(atexit_function_t*));
	atexit_function_t* af=sll_allocate(sizeof(atexit_function_t)+len*sizeof(sll_object_t*));
	af->function=fn;
	af->arg_count=len;
	for (sll_arg_count_t i=0;i<len;i++){
		SLL_ACQUIRE(*(args+i));
		af->args[i]=*(args+i);
	}
	*(_atexit_data+_atexit_data_len-1)=af;
	SLL_CRITICAL(sll_platform_lock_release(_atexit_lock));
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_atexit_unregister(sll_integer_t fn){
	if (!fn||!_atexit_lock||!_atexit_enable){
		return 0;
	}
	SLL_CRITICAL_ERROR(sll_platform_lock_acquire(_atexit_lock));
	sll_bool_t o=0;
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<_atexit_data_len;j++){
		atexit_function_t* af=*(_atexit_data+j);
		if (af->function==fn){
			o=1;
			_delete_atexit_function(af);
		}
		else{
			*(_atexit_data+i)=af;
			i++;
		}
	}
	_atexit_data_len=i;
	_atexit_data=sll_reallocate(_atexit_data,_atexit_data_len*sizeof(atexit_function_t*));
	SLL_CRITICAL(sll_platform_lock_release(_atexit_lock));
	return o;
}

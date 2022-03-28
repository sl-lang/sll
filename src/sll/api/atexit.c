#include <sll/_internal/api.h>
#include <sll/_internal/atexit.h>
#include <sll/_internal/common.h>
#include <sll/_internal/vm.h>
#include <sll/api.h>
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



static void _cleanup_data(void){
	if (_atexit_lock){
		SLL_CRITICAL(sll_platform_lock_delete(_atexit_lock));
	}
}



static void _delete_atexit_function(atexit_function_t* af){
	while (af->all){
		af->all--;
		sll_release_object(af->al[af->all]);
	}
	sll_deallocate(af);
}



void _atexit_execute(void){
	if (!_atexit_lock){
		return;
	}
	SLL_CRITICAL(sll_platform_lock_acquire(_atexit_lock));
	while (_atexit_data_len){
		_atexit_data_len--;
		atexit_function_t* af=*(_atexit_data+_atexit_data_len);
		sll_release_object(sll_execute_function(af->fn,af->al,af->all,EXECUTE_FUNCTION_NO_AUDIT_TERMINATE));
		_delete_atexit_function(af);
	}
	sll_deallocate(_atexit_data);
	_atexit_data=NULL;
	SLL_CRITICAL(sll_platform_lock_release(_atexit_lock));
}



__API_FUNC(atexit_register){
	if (!a){
		return;
	}
	if (!_atexit_lock){
		_atexit_lock=sll_platform_lock_create();
		sll_register_cleanup(_cleanup_data);
	}
	SLL_CRITICAL(sll_platform_lock_acquire(_atexit_lock));
	_atexit_data_len++;
	_atexit_data=sll_reallocate(_atexit_data,_atexit_data_len*sizeof(atexit_function_t*));
	atexit_function_t* af=sll_allocate(sizeof(atexit_function_t)+bc*sizeof(sll_object_t*));
	af->fn=a;
	af->all=bc;
	for (sll_arg_count_t i=0;i<bc;i++){
		SLL_ACQUIRE(*(b+i));
		af->al[i]=*(b+i);
	}
	*(_atexit_data+_atexit_data_len-1)=af;
	SLL_CRITICAL(sll_platform_lock_release(_atexit_lock));
}



__API_FUNC(atexit_unregister){
	if (!a||!_atexit_lock){
		return 0;
	}
	SLL_CRITICAL(sll_platform_lock_acquire(_atexit_lock));
	sll_bool_t o=0;
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<_atexit_data_len;j++){
		atexit_function_t* af=*(_atexit_data+j);
		if (af->fn==a){
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
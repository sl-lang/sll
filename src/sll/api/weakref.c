#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <sll/vm.h>
#include <sll/weakref.h>



static sll_object_t _weakref_no_object_ret=NULL;
static sll_integer_t _weakref_cb_func=0;



static void _cleanup_data(void){
	if (!_weakref_no_object_ret){
		return;
	}
	sll_gc_remove_root(_weakref_no_object_ret);
	SLL_RELEASE(_weakref_no_object_ret);
	_weakref_no_object_ret=NULL;
}



static void _call_user_array(sll_weak_reference_t weak_reference,sll_object_t obj,void* arg){
	sll_object_t dt=arg;
	if (!dt){
		return;
	}
	sll_object_t al[2]={
		dt,
		obj
	};
	sll_object_t ret=sll_execute_function(_weakref_cb_func,al,2,0);
	if (ret){
		SLL_RELEASE(ret);
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_weakref__init(sll_object_t no_object,sll_integer_t callback){
	if (_weakref_no_object_ret){
		return;
	}
	SLL_ACQUIRE(no_object);
	_weakref_no_object_ret=no_object;
	sll_gc_add_root(_weakref_no_object_ret,0);
	_weakref_cb_func=callback;
	sll_register_cleanup(_cleanup_data,SLL_CLEANUP_TYPE_VM);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_weak_reference_t sll_api_weakref_create(sll_object_t object){
	return sll_weakref_create(object);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_weakref_delete(sll_weak_reference_t weak_reference){
	return sll_weakref_delete(weak_reference);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t sll_api_weakref_get(sll_weak_reference_t weak_reference){
	sll_object_t o=sll_weakref_get(weak_reference);
	if (o){
		SLL_ACQUIRE(o);
		return o;
	}
	if (_weakref_no_object_ret){
		SLL_ACQUIRE(_weakref_no_object_ret);
		return _weakref_no_object_ret;
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_weakref_set_callback_data(sll_weak_reference_t weak_reference,sll_object_t callback){
	sll_weakref_set_callback(weak_reference,_call_user_array,callback);
}

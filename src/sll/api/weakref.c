#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/object.h>
#include <sll/types.h>
#include <sll/vm.h>
#include <sll/weakref.h>



static sll_object_t* _weakref_no_object_ret=NULL;
static sll_integer_t _weakref_cb_func=0;



static void _cleanup_data(void){
	if (!_weakref_no_object_ret){
		return;
	}
	GC_RELEASE(_weakref_no_object_ret);
	_weakref_no_object_ret=NULL;
}



static void _call_user_array(sll_weak_reference_t wr,sll_object_t* obj,void* arg){
	sll_object_t* dt=sll_weakref_get((sll_weak_reference_t)arg);
	SLL_CRITICAL(sll_weakref_delete((sll_weak_reference_t)arg));
	if (!dt){
		return;
	}
	sll_object_t* al[]={
		dt,
		obj
	};
	GC_RELEASE(sll_execute_function(_weakref_cb_func,al,2,0));
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_weakref__init(sll_object_t* a,sll_integer_t b){
	if (_weakref_no_object_ret){
		return;
	}
	_weakref_no_object_ret=a;
	SLL_ACQUIRE(a);
	_weakref_cb_func=b;
	sll_register_cleanup(_cleanup_data);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_weakref_create(sll_object_t* a){
	return (sll_integer_t)sll_weakref_create(a);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_weakref_delete(sll_integer_t a){
	return sll_weakref_delete((sll_weak_reference_t)a);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_weakref_get(sll_integer_t a){
	sll_object_t* o=sll_weakref_get((sll_weak_reference_t)a);
	if (o){
		SLL_ACQUIRE(o);
		return o;
	}
	SLL_ACQUIRE(_weakref_no_object_ret);
	return _weakref_no_object_ret;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_weakref_set_callback_data(sll_integer_t a,sll_object_t* b){
	sll_weakref_set_callback((sll_weak_reference_t)a,_call_user_array,sll_weakref_create(b));
}

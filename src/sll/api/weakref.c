#include <sll/_internal/api.h>
#include <sll/_internal/common.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/types.h>
#include <sll/weakref.h>



static sll_object_t* _weakref_no_object_ret=NULL;



static void _cleanup_data(void){
	sll_release_object(_weakref_no_object_ret);
}



__API_FUNC(weakref__init){
	_weakref_no_object_ret=a;
	SLL_ACQUIRE(a);
	sll_register_cleanup(_cleanup_data);
}



__API_FUNC(weakref_create){
	return (sll_integer_t)sll_weakref_create(a);
}



__API_FUNC(weakref_delete){
	return sll_weakref_delete((sll_weak_ref_t)a);
}



__API_FUNC(weakref_get){
	sll_object_t* o=sll_weakref_get((sll_weak_ref_t)a);
	if (o){
		SLL_ACQUIRE(o);
		return o;
	}
	SLL_ACQUIRE(_weakref_no_object_ret);
	return _weakref_no_object_ret;
}

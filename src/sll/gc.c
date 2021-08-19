#include <sll/common.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/types.h>



__SLL_FUNC void sll_acquire_object(sll_runtime_object_t* o){
	if (o->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
		SLL_ACQUIRE(o->dt.s);
	}
	else if (o->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
		SLL_ACQUIRE(o->dt.a);
	}
}



__SLL_FUNC void sll_release_object(sll_runtime_object_t* o){
	if (o->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
		SLL_RELEASE(o->dt.s);
	}
	else if (o->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
		SLL_RELEASE(o->dt.a);
	}
}

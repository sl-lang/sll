#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/constants.h>
#include <sll/types.h>



__API_FUNC(util_ref_count){
	o->t=SLL_RUNTIME_OBJECT_TYPE_INT;
	o->dt.i=0;
	if (!ac){
		return;
	}
	if (a->t==SLL_RUNTIME_OBJECT_TYPE_STRING){
		o->dt.i=a->dt.s->rc;
	}
	else if (a->t==SLL_RUNTIME_OBJECT_TYPE_ARRAY){
		o->dt.i=a->dt.a->rc;
	}
}

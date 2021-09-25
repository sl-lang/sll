#include <sll/_sll_internal.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdlib.h>
#include <string.h>



__SLL_FUNC void sll_array_create(sll_array_length_t l,sll_array_t* o){
	o->l=l;
	o->v=calloc(sizeof(sll_runtime_object_t*),l);
}



__SLL_FUNC void sll_array_join(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	o->l=a->l+b->l;
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	memcpy(o->v,a->v,a->l*sizeof(sll_runtime_object_t*));
	memcpy(o->v+a->l,b->v,b->l*sizeof(sll_runtime_object_t*));
	for (sll_array_length_t i=0;i<o->l;i++){
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_FUNC void sll_array_push(const sll_array_t* a,sll_runtime_object_t* v,sll_array_t* o){
	o->l=a->l+1;
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	memcpy(o->v,a->v,a->l*sizeof(sll_runtime_object_t*));
	o->v[a->l]=v;
	for (sll_array_length_t i=0;i<o->l;i++){
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_FUNC void sll_array_move(const sll_array_t* a,sll_integer_t v,sll_array_t* o){
	if (v<0){
		v=-v;
		if (v>a->l){
			SLL_ZERO_ARRAY(o);
			return;
		}
		o->l=(sll_array_length_t)(a->l-v);
		o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
		memcpy(o->v,a->v+v,(a->l-v)*sizeof(sll_runtime_object_t*));
		for (sll_array_length_t i=0;i<o->l;i++){
			SLL_ACQUIRE(o->v[i]);
		}
		return;
	}
	SLL_ASSERT(a->l+v<=SLL_MAX_ARRAY_LENGTH);
	o->l=(sll_array_length_t)(a->l+v);
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	for (sll_array_length_t i=0;i<(sll_array_length_t)v;i++){
		o->v[i]=SLL_ACQUIRE_STATIC_INT(0);
	}
	memcpy(o->v+v,a->v,a->l*sizeof(sll_runtime_object_t*));
	for (sll_array_length_t i=0;i<a->l;i++){
		SLL_ACQUIRE(a->v[i]);
	}
}

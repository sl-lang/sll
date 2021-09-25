#include <sll/_sll_internal.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdlib.h>
#include <string.h>



__SLL_FUNC void sll_array_clone(const sll_array_t* a,sll_array_t* o){
	if (!a->l){
		SLL_ZERO_ARRAY(o);
		return;
	}
	o->l=a->l;
	o->v=malloc(a->l*sizeof(sll_runtime_object_t*));
	memcpy(o->v,a->v,a->l*sizeof(sll_runtime_object_t*));
	for (sll_array_length_t i=0;i<a->l;i++){
		SLL_ACQUIRE(a->v[i]);
	}
}



__SLL_FUNC sll_array_length_t sll_array_count(const sll_array_t* a,sll_runtime_object_t* v){
	sll_array_length_t o=0;
	for (sll_array_length_t i=0;i<a->l;i++){
		if (sll_operator_equal(a->v[i],v)){
			o++;
		}
	}
	return o;
}



__SLL_FUNC sll_array_length_t sll_array_count_multiple(const sll_array_t* a,sll_runtime_object_t** v,sll_array_length_t vl){
	sll_array_length_t o=0;
	for (sll_array_length_t i=0;i<a->l;i++){
		for (sll_array_length_t j=0;j<vl;j++){
			if (sll_operator_equal(a->v[i],*(v+j))){
				o++;
				break;
			}
		}
	}
	return o;
}



__SLL_FUNC void sll_array_create(sll_array_length_t l,sll_array_t* o){
	if (!l){
		SLL_ZERO_ARRAY(o);
		return;
	}
	o->l=l;
	o->v=calloc(sizeof(sll_runtime_object_t*),l);
}



__SLL_FUNC void sll_array_join(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	o->l=a->l+b->l;
	if (!o->l){
		o->v=NULL;
		return;
	}
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	memcpy(o->v,a->v,a->l*sizeof(sll_runtime_object_t*));
	memcpy(o->v+a->l,b->v,b->l*sizeof(sll_runtime_object_t*));
	for (sll_array_length_t i=0;i<o->l;i++){
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_FUNC sll_runtime_object_t* sll_array_pop(const sll_array_t* a,sll_array_t* o){
	if (!a->l){
		SLL_ZERO_ARRAY(o);
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SLL_ACQUIRE(a->v[a->l-1]);
	if (a->l==1){
		SLL_ZERO_ARRAY(o);
	}
	else{
		o->l=a->l-1;
		o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
		memcpy(o->v,a->v,o->l*sizeof(sll_runtime_object_t*));
		for (sll_array_length_t i=0;i<o->l;i++){
			SLL_ACQUIRE(o->v[i]);
		}
	}
	return a->v[a->l-1];
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



__SLL_FUNC void sll_array_remove(const sll_array_t* a,sll_runtime_object_t* v,sll_array_t* o){
	if (!a->l){
		SLL_ZERO_ARRAY(o);
		return;
	}
	o->l=a->l;
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<a->l;j++){
		if (!sll_operator_equal(a->v[j],v)){
			o->v[i]=a->v[j];
			SLL_ACQUIRE(o->v[i]);
			i++;
		}
	}
	o->l=i;
	if (!o->l){
		free(o->v);
		o->v=NULL;
	}
	else{
		o->v=realloc(o->v,o->l*sizeof(sll_runtime_object_t*));
	}
}



__SLL_FUNC void sll_array_remove_multiple(const sll_array_t* a,sll_runtime_object_t** v,sll_array_length_t vl,sll_array_t* o){
	if (!a->l){
		SLL_ZERO_ARRAY(o);
		return;
	}
	o->l=a->l;
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<a->l;j++){
		for (sll_array_length_t k=0;k<vl;k++){
			if (sll_operator_equal(a->v[j],*(v+k))){
				goto _skip_value;
			}
		}
		o->v[i]=a->v[j];
		SLL_ACQUIRE(o->v[i]);
		i++;
_skip_value:;
	}
	o->l=i;
	if (!o->l){
		free(o->v);
		o->v=NULL;
	}
	else{
		o->v=realloc(o->v,o->l*sizeof(sll_runtime_object_t*));
	}
}



__SLL_FUNC void sll_array_resize(const sll_array_t* a,sll_integer_t v,sll_array_t* o){
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



__SLL_FUNC sll_runtime_object_t* sll_array_shift(const sll_array_t* a,sll_array_t* o){
	if (!a->l){
		SLL_ZERO_ARRAY(o);
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SLL_ACQUIRE(a->v[0]);
	if (a->l==1){
		SLL_ZERO_ARRAY(o);
	}
	else{
		o->l=a->l-1;
		o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
		memcpy(o->v,a->v+1,o->l*sizeof(sll_runtime_object_t*));
		for (sll_array_length_t i=0;i<o->l;i++){
			SLL_ACQUIRE(o->v[i]);
		}
	}
	return a->v[0];
}



__SLL_FUNC void sll_array_unshift(const sll_array_t* a,sll_runtime_object_t* v,sll_array_t* o){
	o->l=a->l+1;
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	memcpy(o->v+1,a->v,a->l*sizeof(sll_runtime_object_t*));
	o->v[0]=v;
	for (sll_array_length_t i=0;i<o->l;i++){
		SLL_ACQUIRE(o->v[i]);
	}
}

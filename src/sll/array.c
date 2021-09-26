#include <sll/_sll_internal.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
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



__SLL_FUNC void sll_array_combinations(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC sll_compare_result_t sll_array_compare(const sll_array_t* a,const sll_array_t* b){
	SLL_UNIMPLEMENTED();
	return SLL_COMPARE_RESULT_EQUAL;
}



__SLL_FUNC sll_compare_result_t sll_array_compare_map(const sll_array_t* a,const sll_map_t* m,sll_bool_t inv){
	SLL_UNIMPLEMENTED();
	return SLL_COMPARE_RESULT_EQUAL;
}



__SLL_FUNC void sll_array_create(sll_array_length_t l,sll_array_t* o){
	if (!l){
		SLL_ZERO_ARRAY(o);
		return;
	}
	o->l=l;
	o->v=calloc(sizeof(sll_runtime_object_t*),l);
}



__SLL_FUNC void sll_array_duplicate(const sll_array_t* a,sll_integer_t n,sll_array_length_t e,sll_array_t* o){
	SLL_ASSERT(e<a->l);
	sll_bool_t r=0;
	if (n<0){
		n=-n;
		r=1;
	}
	sll_ref_count_t m=(sll_ref_count_t)n;
	if (!n){
		if (!e||!a->l){
			SLL_ZERO_ARRAY(o);
			return;
		}
		o->l=e;
		o->v=malloc(e*sizeof(sll_runtime_object_t*));
		memcpy(o->v,a->v,e*sizeof(sll_runtime_object_t*));
		for (sll_array_length_t i=0;i<e;i++){
			SLL_ACQUIRE(o->v[i]);
		}
		return;
	}
	n*=a->l;
	SLL_ASSERT(n<SLL_MAX_ARRAY_LENGTH);
	o->l=((sll_array_length_t)n)+e;
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	if (r){
		sll_array_length_t i=a->l-1;
		for (sll_array_length_t j=0;j<i;i--,j++){
			o->v[i]=a->v[j];
			o->v[i]->rc+=m;
			o->v[j]=a->v[i];
			o->v[j]->rc+=m;
		}
		if (a->l&1){
			o->v[i]=a->v[i];
			o->v[i]->rc+=m;
		}
	}
	else{
		memcpy(o->v,a->v,a->l*sizeof(sll_runtime_object_t*));
		for (sll_array_length_t i=0;i<a->l;i++){
			a->v[i]->rc+=m;
		}
	}
	sll_array_length_t i=a->l;
	while (i<n){
		sll_array_length_t j=i<<1;
		if (j>n){
			j=(sll_array_length_t)n;
		}
		memcpy(o->v+i,o->v,(j-i)*sizeof(sll_runtime_object_t*));
		i=j;
	}
	if (e){
		memcpy(o->v+i,o->v,e*sizeof(sll_runtime_object_t*));
		for (sll_array_length_t i=0;i<e;i++){
			SLL_ACQUIRE(o->v[i]);
		}
	}
}



__SLL_FUNC sll_bool_t sll_array_equal(const sll_array_t* a,const sll_array_t* b){
	SLL_UNIMPLEMENTED();
	return 0;
}



__SLL_FUNC sll_bool_t sll_array_equal_map(const sll_array_t* a,const sll_map_t* m){
	SLL_UNIMPLEMENTED();
	return 0;
}



__SLL_FUNC void sll_array_from_length(sll_array_length_t l,sll_array_t* o){
	if (!l){
		SLL_ZERO_ARRAY(o);
		return;
	}
	o->l=l;
	o->v=malloc(l*sizeof(sll_runtime_object_t*));
	sll_static_int[0]->rc+=l;
	o->v[0]=sll_static_int[0];
	sll_array_length_t i=1;
	while (i<l){
		sll_array_length_t j=i<<1;
		if (j>l){
			j=l;
		}
		memcpy(o->v+i,o->v,(j-i)*sizeof(sll_runtime_object_t*));
		i=j;
	}
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



__SLL_FUNC void sll_array_op_array(const sll_array_t* a,const sll_array_t* b,sll_binary_operator_t f,sll_array_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_array_op_string(const sll_array_t* a,const sll_string_t* s,sll_binary_operator_t f,sll_array_t* o){
	sll_array_length_t e=a->l;
	o->l=s->l;
	if (s->l<a->l){
		e=s->l;
		o->l=a->l;
	}
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	for (sll_array_length_t i=0;i<e;i++){
		o->v[i]=f(a->v[i],sll_static_int[s->v[i]]);
	}
	if (s->l==a->l){
		return;
	}
	if (s->l>a->l){
		for (sll_array_length_t i=e;i<o->l;i++){
			o->v[i]=sll_static_int[s->v[i]];
		}
	}
	else{
		memcpy(o->v+e,a->v+e,(o->l-e)*sizeof(sll_runtime_object_t*));
	}
	for (sll_array_length_t i=e;i<o->l;i++){
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



__SLL_FUNC void sll_array_range(sll_integer_t s,sll_integer_t e,sll_integer_t n,sll_array_t* o){
	if (!n||s==e){
		SLL_ZERO_ARRAY(o);
		return;
	}
	if (e<s){
		SLL_ASSERT(n<0);
		SLL_UNIMPLEMENTED();
	}
	SLL_ASSERT(n>0);
	o->l=(sll_array_length_t)((e-s)/n);
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	sll_array_length_t i=0;
	for (sll_integer_t j=s;j<e;j+=n){
		o->v[i]=SLL_FROM_INT(j);
		i++;
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



__SLL_FUNC void sll_array_string_combinations(const sll_array_t* a,const sll_string_t* s,sll_array_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_array_to_map(const sll_array_t* a,sll_map_t* o){
	if (!a->l){
		SLL_ZERO_MAP(o);
		return;
	}
	o->l=a->l;
	sll_map_length_t e=a->l<<1;
	o->v=malloc(e*sizeof(sll_runtime_object_t*));
	sll_array_length_t i=0;
	for (sll_map_length_t j=0;j<e;j+=2){
		o->v[j]=SLL_FROM_INT(i);
		o->v[j+1]=a->v[i];
		SLL_ACQUIRE(a->v[i]);
		i++;
	}
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

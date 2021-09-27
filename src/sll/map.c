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



__SLL_FUNC void sll_map_add(const sll_map_t* m,sll_runtime_object_t* k,sll_runtime_object_t* v,sll_map_t* o){
	sll_map_length_t l=m->l<<1;
	SLL_ACQUIRE(v);
	o->l=m->l+1;
	o->v=malloc((l+2)*sizeof(sll_runtime_object_t*));
	memcpy(o->v,m->v,l*sizeof(sll_runtime_object_t*));
	for (sll_map_length_t i=0;i<l;i+=2){
		o->v[i]=m->v[i];
		SLL_ACQUIRE(o->v[i]);
		if (sll_operator_equal(m->v[i],k)){
			o->l--;
			o->v=realloc(o->v,l*sizeof(sll_runtime_object_t*));
			o->v[i+1]=v;
			i+=2;
			memcpy(o->v+i,m->v+i,(l-i)*sizeof(sll_runtime_object_t*));
			while (i<l){
				SLL_ACQUIRE(o->v[i]);
				i++;
			}
			return;
		}
		o->v[i+1]=m->v[i+1];
		SLL_ACQUIRE(o->v[i+1]);
	}
	o->v[l]=k;
	o->v[l+1]=v;
	SLL_ACQUIRE(k);
}



__SLL_FUNC void sll_map_add_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_map_add_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_map_and(const sll_map_t* a,const sll_map_t* b,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_map_and_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_map_and_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_map_clone(const sll_map_t* m,sll_map_t* o){
	if (!m->l){
		SLL_ZERO_MAP(o);
		return;
	}
	o->l=m->l;
	o->v=malloc((o->l<<1)*sizeof(sll_runtime_object_t*));
	memcpy(o->v,m->v,(m->l<<1)*sizeof(sll_runtime_object_t*));
	for (sll_map_length_t i=0;i<(o->l<<1);i++){
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_FUNC sll_compare_result_t sll_map_compare(const sll_map_t* a,const sll_map_t* b){
	SLL_UNIMPLEMENTED();
	return SLL_COMPARE_RESULT_EQUAL;
}



__SLL_FUNC void sll_map_create(sll_map_length_t l,sll_map_t* o){
	if (!l){
		SLL_ZERO_MAP(o);
		return;
	}
	o->l=l;
	o->v=calloc(sizeof(sll_runtime_object_t*),l<<1);
}



__SLL_FUNC sll_bool_t sll_map_equal(const sll_map_t* a,const sll_map_t* b){
	if (a->l!=b->l){
		return 0;
	}
	for (sll_map_length_t i=0;i<(a->l<<1);i+=2){
		sll_runtime_object_t* e=a->v[i];
		for (sll_map_length_t j=0;j<(b->l<<1);j+=2){
			if (sll_operator_equal(e,b->v[j])){
				if (!sll_operator_equal(a->v[i+1],b->v[j+1])){
					return 0;
				}
				goto _next_key;
			}
		}
		return 0;
_next_key:;
	}
	return 1;
}



__SLL_FUNC void sll_map_join(const sll_map_t* a,const sll_map_t* b,sll_map_t* o){
	o->l=a->l+b->l;
	if (!o->l){
		o->v=NULL;
		return;
	}
	o->v=malloc((o->l<<1)*sizeof(sll_runtime_object_t*));
	sll_map_length_t al=(a->l<<1);
	memcpy(o->v,a->v,al*sizeof(sll_runtime_object_t*));
	sll_map_length_t i=al;
	for (sll_map_length_t j=0;j<(b->l<<1);j+=2){
		sll_runtime_object_t* e=b->v[j];
		for (sll_map_length_t k=0;k<al;k+=2){
			if (sll_operator_equal(o->v[k],e)){
				o->v[k+1]=b->v[j+1];
				goto _next_key;
			}
		}
		o->v[i]=e;
		o->v[i+1]=b->v[j+1];
		i+=2;
_next_key:;
	}
	if (i!=(o->l<<1)){
		o->l=i>>1;
		o->v=realloc(o->v,i*sizeof(sll_runtime_object_t*));
	}
	while (i){
		i--;
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_FUNC void sll_map_keys(const sll_map_t* m,sll_array_t* o){
	if (!m->l){
		SLL_ZERO_ARRAY(o);
		return;
	}
	o->l=m->l;
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	for (sll_array_length_t i=0;i<o->l;i++){
		o->v[i]=m->v[i<<1];
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_FUNC sll_runtime_object_t* sll_map_remove(const sll_map_t* m,sll_runtime_object_t* k,sll_map_t* o){
	for (sll_map_length_t i=0;i<m->l;i++){
		SLL_ACQUIRE(m->v[(i<<1)+1]);
		if (sll_operator_equal(m->v[i<<1],k)){
			o->l=m->l-1;
			o->v=malloc((o->l<<1)*sizeof(sll_runtime_object_t*));
			memcpy(o->v,m->v,(i<<1)*sizeof(sll_runtime_object_t*));
			memcpy(o->v+(i<<1),m->v+((i+1)<<1),((m->l-i-1)<<1)*sizeof(sll_runtime_object_t*));
			sll_runtime_object_t* v=m->v[(i<<1)+1];
			i++;
			while (i<m->l){
				SLL_ACQUIRE(m->v[i<<1]);
				SLL_ACQUIRE(m->v[(i<<1)+1]);
				i++;
			}
			return v;
		}
		SLL_ACQUIRE(m->v[i<<1]);
	}
	o->l=m->l;
	o->v=malloc((o->l<<1)*sizeof(sll_runtime_object_t*));
	memcpy(o->v,m->v,(o->l<<1)*sizeof(sll_runtime_object_t*));
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_FUNC void sll_map_to_array(const sll_map_t* m,sll_array_t* o){
	if (!m->l){
		SLL_ZERO_ARRAY(o);
		return;
	}
	o->l=m->l<<1;
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	memcpy(o->v,m->v,o->l*sizeof(sll_runtime_object_t*));
	for (sll_array_length_t i=0;i<o->l;i++){
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_FUNC void sll_map_values(const sll_map_t* m,sll_array_t* o){
	if (!m->l){
		SLL_ZERO_ARRAY(o);
		return;
	}
	o->l=m->l;
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	for (sll_array_length_t i=0;i<o->l;i++){
		o->v[i]=m->v[(i<<1)+1];
		SLL_ACQUIRE(o->v[i]);
	}
}

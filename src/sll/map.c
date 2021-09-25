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
	o->l=m->l+1;
	o->v=malloc((o->l<<1)*sizeof(sll_runtime_object_t*));
	memcpy(o->v,m->v,(m->l<<1)*sizeof(sll_runtime_object_t*));
	o->v[m->l<<1]=k;
	o->v[(m->l<<1)+1]=v;
	for (sll_map_length_t i=0;i<(o->l<<1);i++){
		SLL_ACQUIRE(o->v[i]);
	}
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



__SLL_FUNC void sll_map_create(sll_map_length_t l,sll_map_t* o){
	if (!l){
		SLL_ZERO_MAP(o);
		return;
	}
	o->l=l;
	o->v=calloc(sizeof(sll_runtime_object_t*),l<<1);
}



__SLL_FUNC void sll_map_join(const sll_map_t* a,const sll_map_t* b,sll_map_t* o){
	o->l=a->l+b->l;
	if (!o->l){
		o->v=NULL;
		return;
	}
	o->v=malloc((o->l<<1)*sizeof(sll_runtime_object_t*));
	memcpy(o->v,a->v,(a->l<<1)*sizeof(sll_runtime_object_t*));
	memcpy(o->v+(a->l<<1),b->v,(b->l<<1)*sizeof(sll_runtime_object_t*));
	for (sll_map_length_t i=0;i<(o->l<<1);i++){
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

#include <sll/_sll_internal.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/operator.h>
#include <sll/runtime_object.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_FUNC void sll_map_add(const sll_map_t* m,sll_runtime_object_t* k,sll_runtime_object_t* v,sll_map_t* o){
	sll_map_length_t l=m->l<<1;
	SLL_ACQUIRE(v);
	o->l=m->l+1;
	o->v=sll_allocate((l+2)*sizeof(sll_runtime_object_t*));
	for (sll_map_length_t i=0;i<l;i+=2){
		o->v[i]=m->v[i];
		SLL_ACQUIRE(o->v[i]);
		if (sll_operator_equal(m->v[i],k)){
			o->l--;
			o->v=sll_rellocate(o->v,l*sizeof(sll_runtime_object_t*));
			o->v[i+1]=v;
			i+=2;
			while (i<l){
				o->v[i]=m->v[i];
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
	o->l=m->l+a->l;
	o->v=sll_allocate((o->l<<1)*sizeof(sll_runtime_object_t*));
	sll_map_length_t i=m->l<<1;
	uint64_t* sm=sll_zero_allocate(sizeof(uint64_t),(a->l+63)>>6);
	for (sll_map_length_t j=0;j<i;j+=2){
		sll_runtime_object_t* e=m->v[j];
		SLL_ACQUIRE(e);
		o->v[j]=e;
		sll_integer_t n=e->dt.i;
		if (SLL_RUNTIME_OBJECT_GET_TYPE(e)==SLL_RUNTIME_OBJECT_TYPE_INT&&n>=0&&n<a->l){
			o->v[j+1]=a->v[n];
			SLL_ACQUIRE(o->v[j+1]);
			(*(sm+(n>>6)))|=1ull<<(n&63);
			continue;
		}
		o->v[j+1]=m->v[j+1];
		SLL_ACQUIRE(o->v[j+1]);
	}
	for (sll_array_length_t j=0;j<((a->l+63)>>6);j++){
		uint64_t v=~(*(sm+j));
		while (v){
			sll_array_length_t k=(j<<6)|FIND_FIRST_SET_BIT(v);
			if (k>=a->l){
				break;
			}
			o->v[i]=SLL_FROM_INT(k);
			o->v[i+1]=a->v[k];
			SLL_ACQUIRE(o->v[i+1]);
			i+=2;
			v&=v-1;
		}
	}
	sll_deallocate(sm);
	if (i!=o->l){
		o->l=i>>1;
		o->v=sll_rellocate(o->v,i*sizeof(sll_runtime_object_t*));
	}
}



__SLL_FUNC void sll_map_add_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o){
	o->l=m->l+s->l;
	o->v=sll_allocate((o->l<<1)*sizeof(sll_runtime_object_t*));
	sll_map_length_t i=m->l<<1;
	uint64_t* sm=sll_zero_allocate(sizeof(uint64_t),(s->l+63)>>6);
	for (sll_map_length_t j=0;j<i;j+=2){
		sll_runtime_object_t* e=m->v[j];
		SLL_ACQUIRE(e);
		o->v[j]=e;
		sll_integer_t n=e->dt.i;
		if (SLL_RUNTIME_OBJECT_GET_TYPE(e)==SLL_RUNTIME_OBJECT_TYPE_INT&&n>=0&&n<s->l){
			o->v[j+1]=SLL_FROM_CHAR(s->v[n]);
			(*(sm+(n>>6)))|=1ull<<(n&63);
			continue;
		}
		o->v[j+1]=m->v[j+1];
		SLL_ACQUIRE(o->v[j+1]);
	}
	for (sll_string_length_t j=0;j<((s->l+63)>>6);j++){
		uint64_t v=~(*(sm+j));
		while (v){
			sll_string_length_t k=(j<<6)|FIND_FIRST_SET_BIT(v);
			if (k>=s->l){
				break;
			}
			o->v[i]=SLL_FROM_INT(k);
			o->v[i+1]=SLL_FROM_CHAR(s->v[k]);
			i+=2;
			v&=v-1;
		}
	}
	sll_deallocate(sm);
	if ((i>>1)!=o->l){
		o->l=i>>1;
		o->v=sll_rellocate(o->v,i*sizeof(sll_runtime_object_t*));
	}
}



__SLL_FUNC void sll_map_and(const sll_map_t* a,const sll_map_t* b,sll_map_t* o){
	if (b->l>a->l){
		const sll_map_t* c=a;
		a=b;
		b=c;
	}
	if (!b->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=b->l;
	o->v=sll_allocate((b->l<<1)*sizeof(sll_runtime_object_t*));
	sll_map_length_t i=0;
	for (sll_map_length_t j=0;j<(a->l<<1);j+=2){
		sll_runtime_object_t* e=a->v[j];
		for (sll_map_length_t k=0;k<(b->l<<1);k+=2){
			if (sll_operator_equal(e,b->v[k])){
				o->v[i]=e;
				SLL_ACQUIRE(e);
				o->v[i+1]=sll_operator_add(a->v[j+1],b->v[k+1]);
				i+=2;
				break;
			}
		}
	}
	if ((i>>1)!=o->l){
		o->l=i>>1;
		o->v=sll_rellocate(o->v,i*sizeof(sll_runtime_object_t*));
	}
}



__SLL_FUNC void sll_map_and_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o){
	o->l=(m->l<a->l?m->l:a->l);
	if (!o->l){
		o->v=NULL;
		return;
	}
	o->v=sll_allocate((o->l<<1)*sizeof(sll_runtime_object_t*));
	sll_map_length_t i=0;
	for (sll_map_length_t j=0;j<(m->l<<1);j+=2){
		sll_runtime_object_t* e=m->v[j];
		if (SLL_RUNTIME_OBJECT_GET_TYPE(e)==SLL_RUNTIME_OBJECT_TYPE_INT&&e->dt.i>=0&&e->dt.i<a->l){
			o->v[i]=e;
			SLL_ACQUIRE(e);
			o->v[i+1]=sll_operator_add(m->v[i+1],a->v[e->dt.i]);
			i+=2;
		}
	}
	if ((i>>1)!=o->l){
		o->l=i>>1;
		o->v=sll_rellocate(o->v,i*sizeof(sll_runtime_object_t*));
	}
}



__SLL_FUNC void sll_map_and_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o){
	o->l=(m->l<s->l?m->l:s->l);
	if (!o->l){
		o->v=NULL;
		return;
	}
	o->v=sll_allocate((o->l<<1)*sizeof(sll_runtime_object_t*));
	sll_map_length_t i=0;
	for (sll_map_length_t j=0;j<(m->l<<1);j+=2){
		sll_runtime_object_t* e=m->v[j];
		if (SLL_RUNTIME_OBJECT_GET_TYPE(e)==SLL_RUNTIME_OBJECT_TYPE_INT&&e->dt.i>=0&&e->dt.i<s->l){
			o->v[i]=e;
			SLL_ACQUIRE(e);
			o->v[i+1]=sll_operator_add(m->v[i+1],sll_static_char[s->v[e->dt.i]]);
			i+=2;
		}
	}
	if ((i>>1)!=o->l){
		o->l=i>>1;
		o->v=sll_rellocate(o->v,i*sizeof(sll_runtime_object_t*));
	}
}



__SLL_FUNC void sll_map_clone(const sll_map_t* m,sll_map_t* o){
	if (!m->l){
		SLL_INIT_MAP(o);
		return;
	}
	o->l=m->l;
	o->v=sll_allocate((o->l<<1)*sizeof(sll_runtime_object_t*));
	for (sll_map_length_t i=0;i<(o->l<<1);i++){
		o->v[i]=m->v[i];
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_compare_result_t sll_map_compare(const sll_map_t* a,const sll_map_t* b){
	SLL_UNIMPLEMENTED();
	return SLL_COMPARE_RESULT_EQUAL;
}



__SLL_FUNC void sll_map_create(sll_map_length_t l,sll_map_t* o){
	if (!l){
		SLL_INIT_MAP(o);
		return;
	}
	o->l=l;
	o->v=sll_zero_allocate(sizeof(sll_runtime_object_t*),l<<1);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_map_equal(const sll_map_t* a,const sll_map_t* b){
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
	o->v=sll_allocate((o->l<<1)*sizeof(sll_runtime_object_t*));
	sll_map_length_t al=(a->l<<1);
	for (sll_array_length_t i=0;i<al;i++){
		o->v[i]=a->v[i];
	}
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
		o->v=sll_rellocate(o->v,i*sizeof(sll_runtime_object_t*));
	}
	while (i){
		i--;
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_map_get(const sll_map_t* m,const sll_runtime_object_t* v){
	SLL_UNIMPLEMENTED();
	return NULL;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_map_get_key(const sll_map_t* m,sll_map_length_t i){
	if (i<m->l){
		SLL_ACQUIRE(m->v[i<<1]);
		return m->v[i<<1];
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_map_get_value(const sll_map_t* m,sll_map_length_t i){
	if (i<m->l){
		SLL_ACQUIRE(m->v[(i<<1)+1]);
		return m->v[(i<<1)+1];
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_FUNC void sll_map_keys(const sll_map_t* m,sll_array_t* o){
	if (!m->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=m->l;
	o->v=sll_allocate(o->l*sizeof(sll_runtime_object_t*));
	for (sll_array_length_t i=0;i<o->l;i++){
		o->v[i]=m->v[i<<1];
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_FUNC void sll_map_op(const sll_map_t* a,const sll_map_t* b,sll_binary_operator_t f,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_map_remove(const sll_map_t* m,sll_runtime_object_t* k,sll_map_t* o){
	for (sll_map_length_t i=0;i<m->l;i++){
		SLL_ACQUIRE(m->v[(i<<1)+1]);
		if (sll_operator_equal(m->v[i<<1],k)){
			o->l=m->l-1;
			o->v=sll_allocate((o->l<<1)*sizeof(sll_runtime_object_t*));
			for (sll_map_length_t j=0;j<(i<<1);j++){
				o->v[i]=m->v[i];
			}
			for (sll_map_length_t j=0;j<(i<<1);j++){
				o->v[i]=m->v[i];
			}
			sll_runtime_object_t* v=m->v[(i<<1)+1];
			for (i=(i+1)<<1;i<(m->l<<1);i++){
				o->v[i-2]=m->v[i];
				SLL_ACQUIRE(o->v[i-2]);
			}
			return v;
		}
		SLL_ACQUIRE(m->v[i<<1]);
	}
	o->l=m->l;
	o->v=sll_allocate((o->l<<1)*sizeof(sll_runtime_object_t*));
	for (sll_map_length_t i=0;i<(o->l<<1);i++){
		o->v[i]=m->v[i];
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_FUNC void sll_map_remove_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_map_remove_map(const sll_map_t* a,const sll_map_t* b,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_map_remove_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_map_set(const sll_map_t* m,sll_runtime_object_t* k,sll_runtime_object_t* v){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_map_set_key(const sll_map_t* m,sll_map_length_t i,sll_runtime_object_t* v){
	if (i<m->l){
		SLL_ACQUIRE(v);
		SLL_RELEASE(m->v[i<<1]);
		m->v[i<<1]=v;
	}
}



__SLL_FUNC void sll_map_set_value(const sll_map_t* m,sll_map_length_t i,sll_runtime_object_t* v){
	if (i<m->l){
		SLL_ACQUIRE(v);
		SLL_RELEASE(m->v[(i<<1)+1]);
		m->v[(i<<1)+1]=v;
	}
}



__SLL_FUNC void sll_map_to_array(const sll_map_t* m,sll_array_t* o){
	if (!m->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=m->l<<1;
	o->v=sll_allocate(o->l*sizeof(sll_runtime_object_t*));
	for (sll_array_length_t i=0;i<o->l;i++){
		o->v[i]=m->v[i];
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_FUNC void sll_map_values(const sll_map_t* m,sll_array_t* o){
	if (!m->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=m->l;
	o->v=sll_allocate(o->l*sizeof(sll_runtime_object_t*));
	for (sll_array_length_t i=0;i<o->l;i++){
		o->v[i]=m->v[(i<<1)+1];
		SLL_ACQUIRE(o->v[i]);
	}
}

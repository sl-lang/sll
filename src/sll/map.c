#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/allocator.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_free_map(sll_map_t* m){
	for (sll_map_length_t j=0;j<(m->length<<1);j++){
		SLL_RELEASE(*(m->data+j));
	}
	sll_deallocate(m->data);
	m->data=NULL;
	m->length=0;
}



__SLL_EXTERNAL void sll_map_add(const sll_map_t* m,sll_object_t* k,sll_object_t* v,sll_map_t* o){
	sll_map_length_t l=m->length<<1;
	SLL_ACQUIRE(v);
	o->length=m->length+1;
	o->data=sll_allocate((l+2)*sizeof(sll_object_t*));
	for (sll_map_length_t i=0;i<l;i+=2){
		o->data[i]=m->data[i];
		SLL_ACQUIRE(o->data[i]);
		if (sll_operator_strict_equal(m->data[i],k)){
			o->length--;
			o->data=sll_reallocate(o->data,l*sizeof(sll_object_t*));
			o->data[i+1]=v;
			i+=2;
			while (i<l){
				o->data[i]=m->data[i];
				SLL_ACQUIRE(o->data[i]);
				i++;
			}
			return;
		}
		o->data[i+1]=m->data[i+1];
		SLL_ACQUIRE(o->data[i+1]);
	}
	o->data[l]=k;
	o->data[l+1]=v;
	SLL_ACQUIRE(k);
}



__SLL_EXTERNAL void sll_map_add_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o){
	o->length=m->length+a->length;
	o->data=sll_allocate((o->length<<1)*sizeof(sll_object_t*));
	sll_map_length_t i=m->length<<1;
	bitmap_t* sm=sll_zero_allocate(((a->length+63)>>6)*sizeof(bitmap_t));
	for (sll_map_length_t j=0;j<i;j+=2){
		sll_object_t* e=m->data[j];
		SLL_ACQUIRE(e);
		o->data[j]=e;
		sll_integer_t n=e->dt.i;
		if (e->t==SLL_OBJECT_TYPE_INT&&n>=0&&n<a->length){
			o->data[j+1]=a->data[n];
			SLL_ACQUIRE(o->data[j+1]);
			(*(sm+(n>>6)))|=1ull<<(n&63);
			continue;
		}
		o->data[j+1]=m->data[j+1];
		SLL_ACQUIRE(o->data[j+1]);
	}
	for (sll_array_length_t j=0;j<((a->length+63)>>6);j++){
		bitmap_t v=~(*(sm+j));
		while (v){
			sll_array_length_t k=(j<<6)|FIND_FIRST_SET_BIT(v);
			if (k>=a->length){
				break;
			}
			o->data[i]=sll_int_to_object(k);
			o->data[i+1]=a->data[k];
			SLL_ACQUIRE(o->data[i+1]);
			i+=2;
			v&=v-1;
		}
	}
	sll_deallocate(sm);
	if (i!=o->length){
		o->length=i>>1;
		o->data=sll_reallocate(o->data,i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_map_add_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o){
	o->length=m->length+s->l;
	o->data=sll_allocate((o->length<<1)*sizeof(sll_object_t*));
	sll_map_length_t i=m->length<<1;
	bitmap_t* sm=sll_zero_allocate(((s->l+63)>>6)*sizeof(bitmap_t));
	for (sll_map_length_t j=0;j<i;j+=2){
		sll_object_t* e=m->data[j];
		SLL_ACQUIRE(e);
		o->data[j]=e;
		sll_integer_t n=e->dt.i;
		if (e->t==SLL_OBJECT_TYPE_INT&&n>=0&&n<s->l){
			o->data[j+1]=SLL_FROM_CHAR(s->v[n]);
			(*(sm+(n>>6)))|=1ull<<(n&63);
			continue;
		}
		o->data[j+1]=m->data[j+1];
		SLL_ACQUIRE(o->data[j+1]);
	}
	for (sll_string_length_t j=0;j<((s->l+63)>>6);j++){
		bitmap_t v=~(*(sm+j));
		while (v){
			sll_string_length_t k=(j<<6)|FIND_FIRST_SET_BIT(v);
			if (k>=s->l){
				break;
			}
			o->data[i]=sll_int_to_object(k);
			o->data[i+1]=SLL_FROM_CHAR(s->v[k]);
			i+=2;
			v&=v-1;
		}
	}
	sll_deallocate(sm);
	if ((i>>1)!=o->length){
		o->length=i>>1;
		o->data=sll_reallocate(o->data,i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_map_and(const sll_map_t* a,const sll_map_t* b,sll_map_t* o){
	if (b->length>a->length){
		const sll_map_t* c=a;
		a=b;
		b=c;
	}
	if (!b->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=b->length;
	o->data=sll_allocate((b->length<<1)*sizeof(sll_object_t*));
	sll_map_length_t i=0;
	for (sll_map_length_t j=0;j<(a->length<<1);j+=2){
		sll_object_t* e=a->data[j];
		for (sll_map_length_t k=0;k<(b->length<<1);k+=2){
			if (sll_operator_strict_equal(e,b->data[k])){
				o->data[i]=e;
				SLL_ACQUIRE(e);
				o->data[i+1]=sll_operator_add(a->data[j+1],b->data[k+1]);
				i+=2;
				break;
			}
		}
	}
	if ((i>>1)!=o->length){
		o->length=i>>1;
		o->data=sll_reallocate(o->data,i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_map_and_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o){
	o->length=(m->length<a->length?m->length:a->length);
	if (!o->length){
		o->data=NULL;
		return;
	}
	o->data=sll_allocate((o->length<<1)*sizeof(sll_object_t*));
	sll_map_length_t i=0;
	for (sll_map_length_t j=0;j<(m->length<<1);j+=2){
		sll_object_t* e=m->data[j];
		if (e->t==SLL_OBJECT_TYPE_INT&&e->dt.i>=0&&e->dt.i<a->length){
			o->data[i]=e;
			SLL_ACQUIRE(e);
			o->data[i+1]=sll_operator_add(m->data[j+1],a->data[e->dt.i]);
			i+=2;
		}
	}
	if ((i>>1)!=o->length){
		o->length=i>>1;
		o->data=sll_reallocate(o->data,i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_map_and_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o){
	o->length=(m->length<s->l?m->length:s->l);
	if (!o->length){
		o->data=NULL;
		return;
	}
	o->data=sll_allocate((o->length<<1)*sizeof(sll_object_t*));
	sll_map_length_t i=0;
	for (sll_map_length_t j=0;j<(m->length<<1);j+=2){
		sll_object_t* e=m->data[j];
		if (e->t==SLL_OBJECT_TYPE_INT&&e->dt.i>=0&&e->dt.i<s->l){
			o->data[i]=e;
			SLL_ACQUIRE(e);
			o->data[i+1]=sll_operator_add(m->data[i+1],sll_static_char[s->v[e->dt.i]]);
			i+=2;
		}
	}
	if ((i>>1)!=o->length){
		o->length=i>>1;
		o->data=sll_reallocate(o->data,i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_map_clone(const sll_map_t* m,sll_map_t* o){
	if (!m->length){
		SLL_INIT_MAP(o);
		return;
	}
	o->length=m->length;
	o->data=sll_allocate((o->length<<1)*sizeof(sll_object_t*));
	for (sll_map_length_t i=0;i<(o->length<<1);i++){
		o->data[i]=m->data[i];
		SLL_ACQUIRE(o->data[i]);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_map_compare(const sll_map_t* a,const sll_map_t* b){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_map_create(sll_map_length_t l,sll_map_t* o){
	if (!l){
		SLL_INIT_MAP(o);
		return;
	}
	o->length=l;
	o->data=sll_zero_allocate((l<<1)*sizeof(sll_object_t*));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_map_equal(const sll_map_t* a,const sll_map_t* b){
	if (a->length!=b->length){
		return 0;
	}
	for (sll_map_length_t i=0;i<(a->length<<1);i+=2){
		sll_object_t* e=a->data[i];
		for (sll_map_length_t j=0;j<(b->length<<1);j+=2){
			if (sll_operator_strict_equal(e,b->data[j])){
				if (!sll_operator_strict_equal(a->data[i+1],b->data[j+1])){
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_get(const sll_map_t* m,sll_object_t* k){
	for (sll_map_length_t i=0;i<m->length;i++){
		if (sll_operator_strict_equal(m->data[i<<1],k)){
			SLL_ACQUIRE(m->data[(i<<1)+1]);
			return m->data[(i<<1)+1];
		}
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_get_key(const sll_map_t* m,sll_map_length_t i){
	if (i<m->length){
		SLL_ACQUIRE(m->data[i<<1]);
		return m->data[i<<1];
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_get_value(const sll_map_t* m,sll_map_length_t i){
	if (i<m->length){
		SLL_ACQUIRE(m->data[(i<<1)+1]);
		return m->data[(i<<1)+1];
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_map_includes(const sll_map_t* m,sll_object_t* e){
	for (sll_map_length_t i=0;i<m->length;i++){
		if (sll_operator_strict_equal(m->data[i<<1],e)){
			return 1;
		}
	}
	return 0;
}



__SLL_EXTERNAL void sll_map_join(const sll_map_t* a,const sll_map_t* b,sll_map_t* o){
	o->length=a->length+b->length;
	if (!o->length){
		o->data=NULL;
		return;
	}
	o->data=sll_allocate((o->length<<1)*sizeof(sll_object_t*));
	sll_map_length_t al=(a->length<<1);
	for (sll_array_length_t i=0;i<al;i++){
		o->data[i]=a->data[i];
	}
	sll_map_length_t i=al;
	for (sll_map_length_t j=0;j<(b->length<<1);j+=2){
		sll_object_t* e=b->data[j];
		for (sll_map_length_t k=0;k<al;k+=2){
			if (sll_operator_strict_equal(o->data[k],e)){
				o->data[k+1]=b->data[j+1];
				goto _next_key;
			}
		}
		o->data[i]=e;
		o->data[i+1]=b->data[j+1];
		i+=2;
_next_key:;
	}
	if (i!=(o->length<<1)){
		o->length=i>>1;
		o->data=sll_reallocate(o->data,i*sizeof(sll_object_t*));
	}
	while (i){
		i--;
		SLL_ACQUIRE(o->data[i]);
	}
}



__SLL_EXTERNAL void sll_map_keys(const sll_map_t* m,sll_array_t* o){
	if (!m->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=m->length;
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<o->length;i++){
		o->data[i]=m->data[i<<1];
		SLL_ACQUIRE(o->data[i]);
	}
}



__SLL_EXTERNAL void sll_map_op(const sll_map_t* a,const sll_map_t* b,sll_binary_operator_t f,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_remove(const sll_map_t* m,sll_object_t* k,sll_map_t* o){
	if (!m){
		sll_map_length_t i=0;
		while (i<(o->length<<1)){
			if (sll_operator_strict_equal(o->data[i],k)){
				SLL_RELEASE(o->data[i]);
				sll_object_t* v=o->data[i+1];
				i+=2;
				while (i<(o->length<<1)){
					o->data[i-2]=o->data[i];
					i++;
				}
				o->length--;
				o->data=sll_reallocate(o->data,(o->length<<1)*sizeof(sll_object_t*));
				return v;
			}
			i+=2;
		}
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	for (sll_map_length_t i=0;i<m->length;i++){
		SLL_ACQUIRE(m->data[(i<<1)+1]);
		if (sll_operator_strict_equal(m->data[i<<1],k)){
			o->length=m->length-1;
			o->data=sll_allocate((o->length<<1)*sizeof(sll_object_t*));
			for (sll_map_length_t j=0;j<(i<<1);j++){
				o->data[i]=m->data[i];
			}
			sll_object_t* v=m->data[(i<<1)+1];
			for (sll_map_length_t j=(i+1)<<1;j<(m->length<<1);j++){
				o->data[j-2]=m->data[j];
				SLL_ACQUIRE(o->data[j-2]);
			}
			return v;
		}
		SLL_ACQUIRE(m->data[i<<1]);
	}
	o->length=m->length;
	o->data=sll_allocate((o->length<<1)*sizeof(sll_object_t*));
	for (sll_map_length_t i=0;i<(o->length<<1);i++){
		o->data[i]=m->data[i];
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL void sll_map_remove_array(const sll_map_t* m,const sll_array_t* a,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_map_remove_map(const sll_map_t* a,const sll_map_t* b,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_map_remove_string(const sll_map_t* m,const sll_string_t* s,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_map_set(sll_map_t* m,sll_object_t* k,sll_object_t* v){
	SLL_ACQUIRE(v);
	for (sll_map_length_t i=0;i<m->length;i++){
		if (sll_operator_strict_equal(m->data[i<<1],k)){
			SLL_RELEASE(m->data[(i<<1)+1]);
			m->data[(i<<1)+1]=v;
			return;
		}
	}
	m->length++;
	m->data=sll_reallocate(m->data,(m->length<<1)*sizeof(sll_object_t*));
	m->data[(m->length-1)<<1]=k;
	m->data[(m->length<<1)-1]=v;
	SLL_ACQUIRE(k);
}



__SLL_EXTERNAL void sll_map_set_key(const sll_map_t* m,sll_map_length_t i,sll_object_t* v){
	if (i<m->length){
		SLL_ACQUIRE(v);
		SLL_RELEASE(m->data[i<<1]);
		m->data[i<<1]=v;
	}
}



__SLL_EXTERNAL void sll_map_set_value(const sll_map_t* m,sll_map_length_t i,sll_object_t* v){
	if (i<m->length){
		SLL_ACQUIRE(v);
		SLL_RELEASE(m->data[(i<<1)+1]);
		m->data[(i<<1)+1]=v;
	}
}



__SLL_EXTERNAL void sll_map_to_array(const sll_map_t* m,sll_array_t* o){
	if (!m->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=m->length<<1;
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<o->length;i++){
		o->data[i]=m->data[i];
		SLL_ACQUIRE(o->data[i]);
	}
}



__SLL_EXTERNAL void sll_map_values(const sll_map_t* m,sll_array_t* o){
	if (!m->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=m->length;
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<o->length;i++){
		o->data[i]=m->data[(i<<1)+1];
		SLL_ACQUIRE(o->data[i]);
	}
}

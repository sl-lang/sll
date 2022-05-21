#include <sll/_internal/common.h>
#include <sll/_internal/util.h>
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



__SLL_EXTERNAL void sll_array_and(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	if (b->length>a->length){
		const sll_array_t* c=a;
		a=b;
		b=c;
	}
	if (!b->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=b->length;
	o->data=sll_allocator_init(b->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	bitmap_t* m=sll_zero_allocate(((b->length+63)>>6)*sizeof(bitmap_t));
	for (sll_array_length_t j=0;j<a->length;j++){
		sll_object_t* e=a->data[j];
		for (sll_array_length_t k=0;k<b->length;k++){
			if (sll_operator_strict_equal(e,b->data[k])){
				bitmap_t n=1ull<<(k&63);
				if (!((*(m+(k>>6)))&n)){
					(*(m+(k>>6)))|=n;
					o->data[i]=e;
					SLL_ACQUIRE(e);
					i++;
				}
				break;
			}
		}
	}
	sll_deallocate(m);
	if (o->length!=i){
		o->length=i;
		sll_allocator_resize((void**)(&(o->data)),i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_array_clone(const sll_array_t* a,sll_array_t* o){
	if (!a->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=a->length;
	o->data=sll_allocator_init(a->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<a->length;i++){
		o->data[i]=a->data[i];
		SLL_ACQUIRE(o->data[i]);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_array_count(const sll_array_t* a,sll_object_t* v){
	sll_array_length_t o=0;
	for (sll_array_length_t i=0;i<a->length;i++){
		if (sll_operator_strict_equal(a->data[i],v)){
			o++;
		}
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_array_count_multiple(const sll_array_t* a,sll_object_t*const* v,sll_array_length_t vl){
	sll_array_length_t o=0;
	for (sll_array_length_t i=0;i<a->length;i++){
		for (sll_array_length_t j=0;j<vl;j++){
			if (sll_operator_strict_equal(a->data[i],*(v+j))){
				o++;
				break;
			}
		}
	}
	return o;
}



__SLL_EXTERNAL void sll_array_combinations(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	if (!a->length||!b->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=a->length*b->length;
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<a->length;j++){
		for (sll_array_length_t k=0;k<b->length;k++){
			o->data[i]=sll_operator_add(a->data[j],b->data[k]);
			i++;
		}
	}
}



__SLL_EXTERNAL void sll_array_combinations_string(const sll_array_t* a,const sll_string_t* s,sll_array_t* o){
	if (!a->length||!s->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=a->length*s->l;
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_string_length_t j=0;j<a->length;j++){
		for (sll_array_length_t k=0;k<s->l;k++){
			o->data[i]=sll_operator_add(a->data[j],sll_static_char[s->v[k]]);
			i++;
		}
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_array_compare(const sll_array_t* a,const sll_array_t* b){
	for (sll_array_length_t i=0;i<(a->length<b->length?a->length:b->length);i++){
		sll_compare_result_t cmp=sll_operator_compare(a->data[i],b->data[i]);
		if (cmp!=SLL_COMPARE_RESULT_EQUAL){
			return cmp;
		}
	}
	return (a->length==b->length?SLL_COMPARE_RESULT_EQUAL:(a->length<b->length?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_array_compare_map(const sll_array_t* a,const sll_map_t* m,sll_bool_t inv){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_array_create(sll_array_length_t l,sll_array_t* o){
	if (!l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=l;
	o->data=sll_allocator_init(l*sizeof(sll_object_t*));
}



__SLL_EXTERNAL void sll_array_create_zero(sll_array_length_t l,sll_array_t* o){
	if (!l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=l;
	o->data=sll_allocator_init(l*sizeof(sll_object_t*));
	sll_static_int[0]->rc+=l;
	for (sll_array_length_t i=0;i<l;i++){
		o->data[i]=sll_static_int[0];
	}
}



__SLL_EXTERNAL void sll_array_duplicate(const sll_array_t* a,sll_integer_t n,sll_array_length_t e,sll_array_t* o){
	SLL_ASSERT(e<a->length);
	sll_bool_t r=0;
	if (n<0){
		n=-n;
		r=1;
	}
	sll_array_length_t m=(sll_array_length_t)n;
	if (!n){
		if (!e||!a->length){
			SLL_INIT_ARRAY(o);
			return;
		}
		o->length=e;
		o->data=sll_allocator_init(e*sizeof(sll_object_t*));
		for (sll_array_length_t i=0;i<e;i++){
			o->data[i]=a->data[i];
			SLL_ACQUIRE(o->data[i]);
		}
		return;
	}
	n*=a->length;
	SLL_ASSERT(n<SLL_MAX_ARRAY_LENGTH);
	o->length=((sll_array_length_t)n)+e;
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	if (r){
		sll_array_length_t i=a->length-1;
		for (sll_array_length_t j=0;j<i;j++){
			o->data[i]=a->data[j];
			o->data[i]->rc+=m;
			o->data[j]=a->data[i];
			o->data[j]->rc+=m;
			i--;
		}
		if (a->length&1){
			o->data[i]=a->data[i];
			o->data[i]->rc+=m;
		}
	}
	else{
		for (sll_array_length_t i=0;i<a->length;i++){
			o->data[i]=a->data[i];
			o->data[i]->rc+=m;
		}
	}
	sll_array_length_t i=a->length;
	sll_array_length_t j=0;
	while (i<o->length){
		o->data[i]=o->data[j];
		i++;
		j++;
	}
	if (e){
		for (i=0;i<e;i++){
			SLL_ACQUIRE(o->data[i]);
		}
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_array_equal(const sll_array_t* a,const sll_array_t* b){
	if (a->length!=b->length){
		return 0;
	}
	for (sll_array_length_t i=0;i<a->length;i++){
		if (!sll_operator_strict_equal(a->data[i],b->data[i])){
			return 0;
		}
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_array_equal_map(const sll_array_t* a,const sll_map_t* m){
	if (a->length!=m->length){
		return 0;
	}
	for (sll_map_length_t i=0;i<(m->length<<1);i+=2){
		sll_object_t* e=m->data[i];
		sll_bool_t st=(e->t!=SLL_OBJECT_TYPE_INT||e->dt.int_value<0||e->dt.int_value>=a->length||!sll_operator_strict_equal(m->data[i+1],a->data[e->dt.int_value]));
		if (st){
			return 0;
		}
	}
	return 1;
}



__SLL_EXTERNAL void sll_array_extend(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	if (!a){
		if (!b->length){
			return;
		}
		o->length+=b->length;
		sll_allocator_resize((void**)(&(o->data)),o->length*sizeof(sll_object_t*));
		sll_array_length_t i=o->length-b->length;
		for (sll_array_length_t j=0;j<b->length;j++){
			o->data[i]=b->data[j];
			SLL_ACQUIRE(b->data[j]);
			i++;
		}
		return;
	}
	if (!a->length){
		sll_array_clone(b,o);
		return;
	}
	if (!b->length){
		sll_array_clone(a,o);
		return;
	}
	sll_array_create(a->length+b->length,o);
	sll_array_length_t i=0;
	for (;i<a->length;i++){
		o->data[i]=a->data[i];
		SLL_ACQUIRE(a->data[i]);
	}
	for (sll_array_length_t j=0;j<b->length;j++){
		o->data[i]=b->data[j];
		SLL_ACQUIRE(b->data[j]);
		i++;
	}
}



__SLL_EXTERNAL void sll_array_from_length(sll_array_length_t l,sll_array_t* o){
	if (!l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=l;
	o->data=sll_allocator_init(l*sizeof(sll_object_t*));
	sll_static_int[0]->rc+=l;
	for (sll_array_length_t i=0;i<l;i++){
		o->data[i]=sll_static_int[0];
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_array_get(const sll_array_t* a,sll_array_length_t i){
	if (i<a->length){
		SLL_ACQUIRE(a->data[i]);
		return a->data[i];
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_array_includes(const sll_array_t* a,sll_object_t* e){
	for (sll_array_length_t i=0;i<a->length;i++){
		if (sll_operator_strict_equal(a->data[i],e)){
			return 1;
		}
	}
	return 0;
}



__SLL_EXTERNAL void sll_array_join(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	o->length=a->length+b->length;
	if (!o->length){
		o->data=NULL;
		return;
	}
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (;i<a->length;i++){
		o->data[i]=a->data[i];
		SLL_ACQUIRE(o->data[i]);
	}
	for (sll_array_length_t j=0;j<b->length;j++){
		o->data[i]=b->data[j];
		SLL_ACQUIRE(o->data[i]);
		i++;
	}
}



__SLL_EXTERNAL void sll_array_join_arrays(const sll_array_t*const* al,sll_array_length_t all,sll_object_t* e,sll_array_t* o){
	if (!all){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=all-1;
	e->rc+=o->length;
	for (sll_array_length_t i=0;i<all;i++){
		o->length+=(*(al+i))->length;
	}
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<all;j++){
		if (j){
			o->data[i]=e;
			i++;
		}
		const sll_array_t* a=*(al+j);
		for (sll_array_length_t k=0;k<a->length;k++){
			o->data[i]=a->data[k];
			SLL_ACQUIRE(a->data[k]);
			i++;
		}
	}
}



__SLL_EXTERNAL void sll_array_op(const sll_array_t* a,const sll_array_t* b,sll_binary_operator_t f,sll_array_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_array_op_map(const sll_array_t* a,const sll_map_t* m,sll_binary_operator_t f,sll_bool_t inv,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_array_or(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	if (!a->length&&!b->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=a->length+b->length;
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<a->length;i++){
		o->data[i]=a->data[i];
	}
	for (sll_array_length_t i=0;i<b->length;i++){
		o->data[a->length+i]=b->data[i];
	}
	sll_array_length_t i=0;
	bitmap_t* m=sll_zero_allocate(((o->length+63)>>6)*sizeof(bitmap_t));
	for (sll_array_length_t j=0;j<o->length;j++){
		sll_object_t* e=o->data[j];
		if ((*(m+(j>>6)))&(1ull<<(j&63))){
			continue;
		}
		o->data[i]=o->data[j];
		SLL_ACQUIRE(o->data[i]);
		i++;
		for (sll_array_length_t k=j+1;k<o->length;k++){
			if (sll_operator_strict_equal(e,o->data[k])){
				(*(m+(k>>6)))|=1ull<<(k&63);
			}
		}
	}
	sll_deallocate(m);
	if (o->length!=i){
		o->length=i;
		sll_allocator_resize((void**)(&(o->data)),i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_array_parse_char(const sll_array_t* a){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_array_parse_float(const sll_array_t* a){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_array_parse_int(const sll_array_t* a){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_array_pop(const sll_array_t* a,sll_array_t* o){
	if (!a){
		if (!o->length){
			return SLL_ACQUIRE_STATIC_INT(0);
		}
		sll_object_t* e=o->data[o->length-1];
		o->length--;
		sll_allocator_resize((void**)(&(o->data)),o->length*sizeof(sll_object_t*));
		return e;
	}
	if (!a->length){
		SLL_INIT_ARRAY(o);
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SLL_ACQUIRE(a->data[a->length-1]);
	if (a->length==1){
		SLL_INIT_ARRAY(o);
	}
	else{
		o->length=a->length-1;
		o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
		for (sll_array_length_t i=0;i<o->length;i++){
			o->data[i]=a->data[i];
			SLL_ACQUIRE(o->data[i]);
		}
	}
	return a->data[a->length-1];
}



__SLL_EXTERNAL void sll_array_push(const sll_array_t* a,sll_object_t* v,sll_array_t* o){
	if (!a){
		o->length++;
		sll_allocator_resize((void**)(&(o->data)),o->length*sizeof(sll_object_t*));
		SLL_ACQUIRE(v);
		o->data[o->length-1]=v;
		return;
	}
	o->length=a->length+1;
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<a->length;i++){
		o->data[i]=a->data[i];
		SLL_ACQUIRE(o->data[i]);
	}
	o->data[a->length]=v;
	SLL_ACQUIRE(v);
}



__SLL_EXTERNAL void sll_array_range(sll_integer_t s,sll_integer_t e,sll_integer_t n,sll_array_t* o){
	if (!n||s==e){
		SLL_INIT_ARRAY(o);
		return;
	}
	if (e<s){
		SLL_ASSERT(n<0);
		o->length=(sll_array_length_t)((e-s)/n);
		o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
		sll_array_length_t i=0;
		for (sll_integer_t j=s;j>e;j+=n){
			o->data[i]=sll_int_to_object(j);
			i++;
		}
		return;
	}
	SLL_ASSERT(n>0);
	o->length=(sll_array_length_t)((e-s)/n);
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_integer_t j=s;j<e;j+=n){
		o->data[i]=sll_int_to_object(j);
		i++;
	}
}



__SLL_EXTERNAL void sll_array_remove(const sll_array_t* a,sll_object_t* v,sll_array_t* o){
	if (!a){
		sll_array_length_t i=0;
		for (sll_array_length_t j=0;j<o->length;j++){
			if (!sll_operator_strict_equal(o->data[j],v)){
				o->data[i]=o->data[j];
				i++;
			}
			else{
				SLL_RELEASE(o->data[j]);
			}
		}
		o->length=i;
		sll_allocator_resize((void**)(&(o->data)),o->length*sizeof(sll_object_t*));
		return;
	}
	if (!a->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=a->length;
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<a->length;j++){
		if (!sll_operator_strict_equal(a->data[j],v)){
			o->data[i]=a->data[j];
			SLL_ACQUIRE(o->data[i]);
			i++;
		}
	}
	o->length=i;
	sll_allocator_resize((void**)(&(o->data)),o->length*sizeof(sll_object_t*));
}



__SLL_EXTERNAL void sll_array_remove_multiple(const sll_array_t* a,sll_object_t** v,sll_array_length_t vl,sll_array_t* o){
	if (!a->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=a->length;
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<a->length;j++){
		for (sll_array_length_t k=0;k<vl;k++){
			if (sll_operator_strict_equal(a->data[j],*(v+k))){
				goto _skip_value;
			}
		}
		o->data[i]=a->data[j];
		SLL_ACQUIRE(o->data[i]);
		i++;
_skip_value:;
	}
	o->length=i;
	sll_allocator_resize((void**)(&(o->data)),o->length*sizeof(sll_object_t*));
}



__SLL_EXTERNAL void sll_array_replace(const sll_array_t* a,sll_object_t* k,sll_object_t* v,sll_array_t* o){
	if (!a->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=a->length;
	o->data=sll_allocator_init(a->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<a->length;i++){
		o->data[i]=(sll_operator_strict_equal(a->data[i],k)?v:a->data[i]);
		SLL_ACQUIRE(o->data[i]);
	}
}



__SLL_EXTERNAL void sll_array_resize(const sll_array_t* a,sll_integer_t v,sll_array_t* o){
	if (v<0){
		v=-v;
		if (v>=a->length){
			SLL_INIT_ARRAY(o);
			return;
		}
		o->length=(sll_array_length_t)(a->length-v);
		o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
		for (sll_array_length_t i=(sll_array_length_t)v;i<a->length;i++){
			o->data[i-v]=a->data[i];
			SLL_ACQUIRE(a->data[i]);
		}
		return;
	}
	SLL_ASSERT(a->length+v<=SLL_MAX_ARRAY_LENGTH);
	o->length=(sll_array_length_t)(a->length+v);
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<(sll_array_length_t)v;i++){
		o->data[i]=SLL_ACQUIRE_STATIC_INT(0);
	}
	for (sll_array_length_t i=0;i<a->length;i++){
		o->data[i+v]=a->data[i];
		SLL_ACQUIRE(a->data[i]);
	}
}



__SLL_EXTERNAL void sll_array_reverse(const sll_array_t* a,sll_array_t* o){
	if (!a->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=a->length;
	o->data=sll_allocator_init(a->length*sizeof(sll_object_t*));
	sll_array_length_t i=a->length;
	for (sll_array_length_t j=0;j<a->length;j++){
		i--;
		o->data[j]=a->data[i];
		SLL_ACQUIRE(o->data[j]);
	}
}



__SLL_EXTERNAL void sll_array_select(const sll_array_t* s,sll_integer_t a,sll_integer_t b,sll_integer_t c,sll_array_t* o){
	WRAP_ARRAY_INDEX_VAR(a,s->length);
	WRAP_ARRAY_INDEX_VAR(b,s->length);
	if (!s->length||a==b||!c||(a<=b&&a>=s->length)||(b<=a&&b>=s->length)){
		SLL_INIT_ARRAY(o);
		return;
	}
	if (a<b){
		if (c<0){
			sll_array_select(s,0,a,1,o);
			return;
		}
		if (a+c>=s->length){
			o->length=1;
			o->data=sll_allocator_init(sizeof(sll_object_t*));
			o->data[0]=s->data[0];
			SLL_ACQUIRE(o->data[0]);
			return;
		}
		o->length=(sll_array_length_t)((b-a-1)/c+1);
		o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
		sll_array_length_t i=0;
		do{
			o->data[i]=s->data[a];
			SLL_ACQUIRE(o->data[i]);
			i++;
			a+=c;
		} while (a<b);
		SLL_ASSERT(i==o->length);
	}
	else{
		if (c>0){
			sll_array_select(s,a,s->length-1,1,o);
			return;
		}
		c=-c;
		if (c>a){
			o->length=1;
			o->data=sll_allocator_init(sizeof(sll_object_t*));
			o->data[0]=s->data[0];
			SLL_ACQUIRE(o->data[0]);
			return;
		}
		o->length=(sll_array_length_t)((a-b-1)/c+1);
		o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
		sll_array_length_t i=0;
		do{
			o->data[i]=s->data[a];
			SLL_ACQUIRE(o->data[i]);
			i++;
			a-=c;
		} while (a>b);
		SLL_ASSERT(i==o->length);
	}
}



__SLL_EXTERNAL void sll_array_split(const sll_array_t* a,sll_object_t* e,sll_array_t* o){
	o->length=1;
	sll_object_t** ptr=sll_allocate_stack(sizeof(sll_object_t*));
	sll_object_t** arr_ptr=sll_allocate_stack(1);
	sll_object_t* oa=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
	oa->dt.a.length=0;
	*ptr=oa;
	for (sll_array_length_t i=0;i<a->length;i++){
		if (sll_operator_strict_equal(a->data[i],e)){
			oa->dt.a.data=(oa->dt.a.length?sll_allocator_from_memory(arr_ptr,oa->dt.a.length*sizeof(sll_object_t*)):NULL);
			sll_deallocate(arr_ptr);
			arr_ptr=sll_allocate_stack(1);
			oa=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
			oa->dt.a.length=0;
			o->length++;
			ptr=sll_reallocate(ptr,o->length*sizeof(sll_object_t*));
			*(ptr+o->length-1)=oa;
		}
		else{
			oa->dt.a.length++;
			arr_ptr=sll_reallocate(arr_ptr,oa->dt.a.length*sizeof(sll_object_t*));
			*(arr_ptr+oa->dt.a.length-1)=a->data[i];
			SLL_ACQUIRE(a->data[i]);
		}
	}
	oa->dt.a.data=(oa->dt.a.length?sll_allocator_from_memory(arr_ptr,oa->dt.a.length*sizeof(sll_object_t*)):NULL);
	sll_deallocate(arr_ptr);
	o->data=sll_allocator_from_memory(ptr,o->length*sizeof(sll_object_t*));
	sll_deallocate(ptr);
}



__SLL_EXTERNAL void sll_array_set(const sll_array_t* a,sll_array_length_t i,sll_object_t* v){
	if (i<a->length){
		SLL_ACQUIRE(v);
		SLL_RELEASE(a->data[i]);
		a->data[i]=v;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_array_shift(const sll_array_t* a,sll_array_t* o){
	if (!a){
		if (!o->length){
			return SLL_ACQUIRE_STATIC_INT(0);
		}
		sll_object_t* e=o->data[0];
		o->length--;
		for (sll_array_length_t i=0;i<o->length;i++){
			o->data[i]=o->data[i+1];
		}
		sll_allocator_resize((void**)(&(o->data)),o->length*sizeof(sll_object_t*));
		return e;
	}
	if (!a->length){
		SLL_INIT_ARRAY(o);
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SLL_ACQUIRE(a->data[0]);
	if (a->length==1){
		SLL_INIT_ARRAY(o);
	}
	else{
		o->length=a->length-1;
		o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
		if (!o->data){
			return 0;
		}
		for (sll_array_length_t i=0;i<o->length;i++){
			o->data[i]=a->data[i+1];
			SLL_ACQUIRE(o->data[i]);
		}
	}
	return a->data[0];
}



__SLL_EXTERNAL void sll_array_to_map(const sll_array_t* a,sll_map_t* o){
	if (!a->length){
		SLL_INIT_MAP(o);
		return;
	}
	o->length=a->length;
	sll_map_length_t e=a->length<<1;
	o->data=sll_allocate(e*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_map_length_t j=0;j<e;j+=2){
		o->data[j]=sll_int_to_object(i);
		o->data[j+1]=a->data[i];
		SLL_ACQUIRE(a->data[i]);
		i++;
	}
}



__SLL_EXTERNAL void sll_array_unshift(const sll_array_t* a,sll_object_t* v,sll_array_t* o){
	if (!a){
		o->length++;
		sll_allocator_resize((void**)(&(o->data)),o->length*sizeof(sll_object_t*));
		for (sll_array_length_t i=o->length-1;i;i--){
			o->data[i]=o->data[i-1];
		}
		o->data[0]=v;
		SLL_ACQUIRE(v);
		return;
	}
	o->length=a->length+1;
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	o->data[0]=v;
	SLL_ACQUIRE(v);
	for (sll_array_length_t i=1;i<o->length;i++){
		o->data[i]=a->data[i-1];
		SLL_ACQUIRE(o->data[i]);
	}
}



__SLL_EXTERNAL void sll_array_xor(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	if (b->length>a->length){
		const sll_array_t* c=a;
		a=b;
		b=c;
	}
	if (!b->length){
		sll_array_clone(a,o);
		return;
	}
	o->length=a->length;
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (;i<b->length;i++){
		o->data[i]=sll_operator_xor(a->data[i],b->data[i]);
	}
	while (i<a->length){
		SLL_ACQUIRE(a->data[i]);
		o->data[i]=a->data[i];
		i++;
	}
}



__SLL_EXTERNAL void sll_free_array(sll_array_t* a){
	for (sll_array_length_t i=0;i<a->length;i++){
		SLL_RELEASE(a->data[i]);
	}
	sll_allocator_release(a->data);
	a->data=NULL;
	a->length=0;
}

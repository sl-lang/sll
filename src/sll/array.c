#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
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
#include <sll/types.h>



__SLL_EXTERNAL void sll_array_and(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	if (b->l>a->l){
		const sll_array_t* c=a;
		a=b;
		b=c;
	}
	if (!b->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=b->l;
	o->v=sll_allocator_init(b->l*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	bitmap_t* m=sll_zero_allocate(((b->l+63)>>6)*sizeof(bitmap_t));
	for (sll_array_length_t j=0;j<a->l;j++){
		sll_object_t* e=a->v[j];
		for (sll_array_length_t k=0;k<b->l;k++){
			if (sll_operator_strict_equal(e,b->v[k])){
				bitmap_t n=1ull<<(k&63);
				if (!((*(m+(k>>6)))&n)){
					(*(m+(k>>6)))|=n;
					o->v[i]=e;
					SLL_ACQUIRE(e);
					i++;
				}
				break;
			}
		}
	}
	sll_deallocate(m);
	if (o->l!=i){
		o->l=i;
		sll_allocator_resize((void**)(&(o->v)),i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_array_clone(const sll_array_t* a,sll_array_t* o){
	if (!a->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=a->l;
	o->v=sll_allocator_init(a->l*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<a->l;i++){
		o->v[i]=a->v[i];
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_array_count(const sll_array_t* a,sll_object_t* v){
	sll_array_length_t o=0;
	for (sll_array_length_t i=0;i<a->l;i++){
		if (sll_operator_strict_equal(a->v[i],v)){
			o++;
		}
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_array_count_multiple(const sll_array_t* a,sll_object_t*const* v,sll_array_length_t vl){
	sll_array_length_t o=0;
	for (sll_array_length_t i=0;i<a->l;i++){
		for (sll_array_length_t j=0;j<vl;j++){
			if (sll_operator_strict_equal(a->v[i],*(v+j))){
				o++;
				break;
			}
		}
	}
	return o;
}



__SLL_EXTERNAL void sll_array_combinations(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	if (!a->l||!b->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=a->l*b->l;
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<a->l;j++){
		for (sll_array_length_t k=0;k<b->l;k++){
			o->v[i]=sll_operator_add(a->v[j],b->v[k]);
			i++;
		}
	}
}



__SLL_EXTERNAL void sll_array_combinations_string(const sll_array_t* a,const sll_string_t* s,sll_array_t* o){
	if (!a->l||!s->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=a->l*s->l;
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_string_length_t j=0;j<a->l;j++){
		for (sll_array_length_t k=0;k<s->l;k++){
			o->v[i]=sll_operator_add(a->v[j],sll_static_char[s->v[k]]);
			i++;
		}
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_array_compare(const sll_array_t* a,const sll_array_t* b){
	for (sll_array_length_t i=0;i<(a->l<b->l?a->l:b->l);i++){
		sll_compare_result_t cmp=sll_operator_compare(a->v[i],b->v[i]);
		if (cmp!=SLL_COMPARE_RESULT_EQUAL){
			return cmp;
		}
	}
	return (a->l==b->l?SLL_COMPARE_RESULT_EQUAL:(a->l<b->l?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_array_compare_map(const sll_array_t* a,const sll_map_t* m,sll_bool_t inv){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_array_create(sll_array_length_t l,sll_array_t* o){
	if (!l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=l;
	o->v=sll_allocator_init(l*sizeof(sll_object_t*));
}



__SLL_EXTERNAL void sll_array_create_zero(sll_array_length_t l,sll_array_t* o){
	if (!l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=l;
	o->v=sll_allocator_init(l*sizeof(sll_object_t*));
	sll_static_int[0]->rc+=l;
	for (sll_array_length_t i=0;i<l;i++){
		o->v[i]=sll_static_int[0];
	}
}



__SLL_EXTERNAL void sll_array_duplicate(const sll_array_t* a,sll_integer_t n,sll_array_length_t e,sll_array_t* o){
	SLL_ASSERT(e<a->l);
	sll_bool_t r=0;
	if (n<0){
		n=-n;
		r=1;
	}
	sll_array_length_t m=(sll_array_length_t)n;
	if (!n){
		if (!e||!a->l){
			SLL_INIT_ARRAY(o);
			return;
		}
		o->l=e;
		o->v=sll_allocator_init(e*sizeof(sll_object_t*));
		for (sll_array_length_t i=0;i<e;i++){
			o->v[i]=a->v[i];
			SLL_ACQUIRE(o->v[i]);
		}
		return;
	}
	n*=a->l;
	SLL_ASSERT(n<SLL_MAX_ARRAY_LENGTH);
	o->l=((sll_array_length_t)n)+e;
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	if (r){
		sll_array_length_t i=a->l-1;
		for (sll_array_length_t j=0;j<i;j++){
			o->v[i]=a->v[j];
			o->v[i]->rc+=m;
			o->v[j]=a->v[i];
			o->v[j]->rc+=m;
			i--;
		}
		if (a->l&1){
			o->v[i]=a->v[i];
			o->v[i]->rc+=m;
		}
	}
	else{
		for (sll_array_length_t i=0;i<a->l;i++){
			o->v[i]=a->v[i];
			o->v[i]->rc+=m;
		}
	}
	sll_array_length_t i=a->l;
	sll_array_length_t j=0;
	while (i<o->l){
		o->v[i]=o->v[j];
		i++;
		j++;
	}
	if (e){
		for (i=0;i<e;i++){
			SLL_ACQUIRE(o->v[i]);
		}
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_array_equal(const sll_array_t* a,const sll_array_t* b){
	if (a->l!=b->l){
		return 0;
	}
	for (sll_array_length_t i=0;i<a->l;i++){
		if (!sll_operator_strict_equal(a->v[i],b->v[i])){
			return 0;
		}
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_array_equal_map(const sll_array_t* a,const sll_map_t* m){
	if (a->l!=m->l){
		return 0;
	}
	for (sll_map_length_t i=0;i<(m->l<<1);i+=2){
		sll_object_t* e=m->v[i];
		sll_bool_t st=(e->t!=SLL_OBJECT_TYPE_INT||e->dt.i<0||e->dt.i>=a->l||!sll_operator_strict_equal(m->v[i+1],a->v[e->dt.i]));
		if (st){
			return 0;
		}
	}
	return 1;
}



__SLL_EXTERNAL void sll_array_extend(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	if (!a){
		if (!b->l){
			return;
		}
		o->l+=b->l;
		sll_allocator_resize((void**)(&(o->v)),o->l*sizeof(sll_object_t*));
		sll_array_length_t i=o->l-b->l;
		for (sll_array_length_t j=0;j<b->l;j++){
			o->v[i]=b->v[j];
			SLL_ACQUIRE(b->v[j]);
			i++;
		}
		return;
	}
	if (!a->l){
		sll_array_clone(b,o);
		return;
	}
	if (!b->l){
		sll_array_clone(a,o);
		return;
	}
	sll_array_create(a->l+b->l,o);
	sll_array_length_t i=0;
	for (;i<a->l;i++){
		o->v[i]=a->v[i];
		SLL_ACQUIRE(a->v[i]);
	}
	for (sll_array_length_t j=0;j<b->l;j++){
		o->v[i]=b->v[j];
		SLL_ACQUIRE(b->v[j]);
		i++;
	}
}



__SLL_EXTERNAL void sll_array_from_length(sll_array_length_t l,sll_array_t* o){
	if (!l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=l;
	o->v=sll_allocator_init(l*sizeof(sll_object_t*));
	sll_static_int[0]->rc+=l;
	for (sll_array_length_t i=0;i<l;i++){
		o->v[i]=sll_static_int[0];
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_array_get(const sll_array_t* a,sll_array_length_t i){
	if (i<a->l){
		SLL_ACQUIRE(a->v[i]);
		return a->v[i];
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_array_includes(const sll_array_t* a,sll_object_t* e){
	for (sll_array_length_t i=0;i<a->l;i++){
		if (sll_operator_strict_equal(a->v[i],e)){
			return 1;
		}
	}
	return 0;
}



__SLL_EXTERNAL void sll_array_join(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	o->l=a->l+b->l;
	if (!o->l){
		o->v=NULL;
		return;
	}
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (;i<a->l;i++){
		o->v[i]=a->v[i];
		SLL_ACQUIRE(o->v[i]);
	}
	for (sll_array_length_t j=0;j<b->l;j++){
		o->v[i]=b->v[j];
		SLL_ACQUIRE(o->v[i]);
		i++;
	}
}



__SLL_EXTERNAL void sll_array_join_arrays(const sll_array_t*const* al,sll_array_length_t all,sll_object_t* e,sll_array_t* o){
	if (!all){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=all-1;
	e->rc+=o->l;
	for (sll_array_length_t i=0;i<all;i++){
		o->l+=(*(al+i))->l;
	}
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<all;j++){
		if (j){
			o->v[i]=e;
			i++;
		}
		const sll_array_t* a=*(al+j);
		for (sll_array_length_t k=0;k<a->l;k++){
			o->v[i]=a->v[k];
			SLL_ACQUIRE(a->v[k]);
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
	if (!a->l&&!b->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=a->l+b->l;
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<a->l;i++){
		o->v[i]=a->v[i];
	}
	for (sll_array_length_t i=0;i<b->l;i++){
		o->v[a->l+i]=b->v[i];
	}
	sll_array_length_t i=0;
	bitmap_t* m=sll_zero_allocate(((o->l+63)>>6)*sizeof(bitmap_t));
	for (sll_array_length_t j=0;j<o->l;j++){
		sll_object_t* e=o->v[j];
		if ((*(m+(j>>6)))&(1ull<<(j&63))){
			continue;
		}
		o->v[i]=o->v[j];
		SLL_ACQUIRE(o->v[i]);
		i++;
		for (sll_array_length_t k=j+1;k<o->l;k++){
			if (sll_operator_strict_equal(e,o->v[k])){
				(*(m+(k>>6)))|=1ull<<(k&63);
			}
		}
	}
	sll_deallocate(m);
	if (o->l!=i){
		o->l=i;
		sll_allocator_resize((void**)(&(o->v)),i*sizeof(sll_object_t*));
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
		if (!o->l){
			return SLL_ACQUIRE_STATIC_INT(0);
		}
		sll_object_t* e=o->v[o->l-1];
		o->l--;
		sll_allocator_resize((void**)(&(o->v)),o->l*sizeof(sll_object_t*));
		return e;
	}
	if (!a->l){
		SLL_INIT_ARRAY(o);
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SLL_ACQUIRE(a->v[a->l-1]);
	if (a->l==1){
		SLL_INIT_ARRAY(o);
	}
	else{
		o->l=a->l-1;
		o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
		for (sll_array_length_t i=0;i<o->l;i++){
			o->v[i]=a->v[i];
			SLL_ACQUIRE(o->v[i]);
		}
	}
	return a->v[a->l-1];
}



__SLL_EXTERNAL void sll_array_push(const sll_array_t* a,sll_object_t* v,sll_array_t* o){
	if (!a){
		o->l++;
		sll_allocator_resize((void**)(&(o->v)),o->l*sizeof(sll_object_t*));
		SLL_ACQUIRE(v);
		o->v[o->l-1]=v;
		return;
	}
	o->l=a->l+1;
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<a->l;i++){
		o->v[i]=a->v[i];
		SLL_ACQUIRE(o->v[i]);
	}
	o->v[a->l]=v;
	SLL_ACQUIRE(v);
}



__SLL_EXTERNAL void sll_array_range(sll_integer_t s,sll_integer_t e,sll_integer_t n,sll_array_t* o){
	if (!n||s==e){
		SLL_INIT_ARRAY(o);
		return;
	}
	if (e<s){
		SLL_ASSERT(n<0);
		o->l=(sll_array_length_t)((e-s)/n);
		o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
		sll_array_length_t i=0;
		for (sll_integer_t j=s;j>e;j+=n){
			o->v[i]=sll_int_to_object(j);
			i++;
		}
		return;
	}
	SLL_ASSERT(n>0);
	o->l=(sll_array_length_t)((e-s)/n);
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_integer_t j=s;j<e;j+=n){
		o->v[i]=sll_int_to_object(j);
		i++;
	}
}



__SLL_EXTERNAL void sll_array_remove(const sll_array_t* a,sll_object_t* v,sll_array_t* o){
	if (!a){
		sll_array_length_t i=0;
		for (sll_array_length_t j=0;j<o->l;j++){
			if (!sll_operator_strict_equal(o->v[j],v)){
				o->v[i]=o->v[j];
				i++;
			}
			else{
				SLL_RELEASE(o->v[j]);
			}
		}
		o->l=i;
		sll_allocator_resize((void**)(&(o->v)),o->l*sizeof(sll_object_t*));
		return;
	}
	if (!a->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=a->l;
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<a->l;j++){
		if (!sll_operator_strict_equal(a->v[j],v)){
			o->v[i]=a->v[j];
			SLL_ACQUIRE(o->v[i]);
			i++;
		}
	}
	o->l=i;
	sll_allocator_resize((void**)(&(o->v)),o->l*sizeof(sll_object_t*));
}



__SLL_EXTERNAL void sll_array_remove_multiple(const sll_array_t* a,sll_object_t** v,sll_array_length_t vl,sll_array_t* o){
	if (!a->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=a->l;
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<a->l;j++){
		for (sll_array_length_t k=0;k<vl;k++){
			if (sll_operator_strict_equal(a->v[j],*(v+k))){
				goto _skip_value;
			}
		}
		o->v[i]=a->v[j];
		SLL_ACQUIRE(o->v[i]);
		i++;
_skip_value:;
	}
	o->l=i;
	sll_allocator_resize((void**)(&(o->v)),o->l*sizeof(sll_object_t*));
}



__SLL_EXTERNAL void sll_array_replace(const sll_array_t* a,sll_object_t* k,sll_object_t* v,sll_array_t* o){
	if (!a->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=a->l;
	o->v=sll_allocator_init(a->l*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<a->l;i++){
		o->v[i]=(sll_operator_strict_equal(a->v[i],k)?v:a->v[i]);
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_EXTERNAL void sll_array_resize(const sll_array_t* a,sll_integer_t v,sll_array_t* o){
	if (v<0){
		v=-v;
		if (v>=a->l){
			SLL_INIT_ARRAY(o);
			return;
		}
		o->l=(sll_array_length_t)(a->l-v);
		o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
		for (sll_array_length_t i=(sll_array_length_t)v;i<a->l;i++){
			o->v[i-v]=a->v[i];
			SLL_ACQUIRE(a->v[i]);
		}
		return;
	}
	SLL_ASSERT(a->l+v<=SLL_MAX_ARRAY_LENGTH);
	o->l=(sll_array_length_t)(a->l+v);
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<(sll_array_length_t)v;i++){
		o->v[i]=SLL_ACQUIRE_STATIC_INT(0);
	}
	for (sll_array_length_t i=0;i<a->l;i++){
		o->v[i+v]=a->v[i];
		SLL_ACQUIRE(a->v[i]);
	}
}



__SLL_EXTERNAL void sll_array_reverse(const sll_array_t* a,sll_array_t* o){
	if (!a->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=a->l;
	o->v=sll_allocator_init(a->l*sizeof(sll_object_t*));
	sll_array_length_t i=a->l;
	for (sll_array_length_t j=0;j<a->l;j++){
		i--;
		o->v[j]=a->v[i];
		SLL_ACQUIRE(o->v[j]);
	}
}



__SLL_EXTERNAL void sll_array_select(const sll_array_t* s,sll_integer_t a,sll_integer_t b,sll_integer_t c,sll_array_t* o){
	WRAP_ARRAY_INDEX_VAR(a,s->l);
	WRAP_ARRAY_INDEX_VAR(b,s->l);
	if (!s->l||a==b||!c||(a<=b&&a>=s->l)||(b<=a&&b>=s->l)){
		SLL_INIT_ARRAY(o);
		return;
	}
	if (a<b){
		if (c<0){
			sll_array_select(s,0,a,1,o);
			return;
		}
		if (a+c>=s->l){
			o->l=1;
			o->v=sll_allocator_init(sizeof(sll_object_t*));
			o->v[0]=s->v[0];
			SLL_ACQUIRE(o->v[0]);
			return;
		}
		o->l=(sll_array_length_t)((b-a-1)/c+1);
		o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
		sll_array_length_t i=0;
		do{
			o->v[i]=s->v[a];
			SLL_ACQUIRE(o->v[i]);
			i++;
			a+=c;
		} while (a<b);
		SLL_ASSERT(i==o->l);
	}
	else{
		if (c>0){
			sll_array_select(s,a,s->l-1,1,o);
			return;
		}
		c=-c;
		if (c>a){
			o->l=1;
			o->v=sll_allocator_init(sizeof(sll_object_t*));
			o->v[0]=s->v[0];
			SLL_ACQUIRE(o->v[0]);
			return;
		}
		o->l=(sll_array_length_t)((a-b-1)/c+1);
		o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
		sll_array_length_t i=0;
		do{
			o->v[i]=s->v[a];
			SLL_ACQUIRE(o->v[i]);
			i++;
			a-=c;
		} while (a>b);
		SLL_ASSERT(i==o->l);
	}
}



__SLL_EXTERNAL void sll_array_split(const sll_array_t* a,sll_object_t* e,sll_array_t* o){
	o->l=1;
	sll_object_t** ptr=sll_allocate_stack(sizeof(sll_object_t*));
	sll_object_t** arr_ptr=sll_allocate_stack(1);
	sll_object_t* oa=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
	oa->dt.a.l=0;
	*ptr=oa;
	for (sll_array_length_t i=0;i<a->l;i++){
		if (sll_operator_strict_equal(a->v[i],e)){
			oa->dt.a.v=(oa->dt.a.l?sll_allocator_from_memory(arr_ptr,oa->dt.a.l*sizeof(sll_object_t*)):NULL);
			sll_deallocate(arr_ptr);
			arr_ptr=sll_allocate_stack(1);
			oa=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
			oa->dt.a.l=0;
			o->l++;
			ptr=sll_reallocate(ptr,o->l*sizeof(sll_object_t*));
			*(ptr+o->l-1)=oa;
		}
		else{
			oa->dt.a.l++;
			arr_ptr=sll_reallocate(arr_ptr,oa->dt.a.l*sizeof(sll_object_t*));
			*(arr_ptr+oa->dt.a.l-1)=a->v[i];
			SLL_ACQUIRE(a->v[i]);
		}
	}
	oa->dt.a.v=(oa->dt.a.l?sll_allocator_from_memory(arr_ptr,oa->dt.a.l*sizeof(sll_object_t*)):NULL);
	sll_deallocate(arr_ptr);
	o->v=sll_allocator_from_memory(ptr,o->l*sizeof(sll_object_t*));
	sll_deallocate(ptr);
}



__SLL_EXTERNAL void sll_array_set(const sll_array_t* a,sll_array_length_t i,sll_object_t* v){
	if (i<a->l){
		SLL_ACQUIRE(v);
		SLL_RELEASE(a->v[i]);
		a->v[i]=v;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_array_shift(const sll_array_t* a,sll_array_t* o){
	if (!a){
		if (!o->l){
			return SLL_ACQUIRE_STATIC_INT(0);
		}
		sll_object_t* e=o->v[0];
		o->l--;
		for (sll_array_length_t i=0;i<o->l;i++){
			o->v[i]=o->v[i+1];
		}
		sll_allocator_resize((void**)(&(o->v)),o->l*sizeof(sll_object_t*));
		return e;
	}
	if (!a->l){
		SLL_INIT_ARRAY(o);
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SLL_ACQUIRE(a->v[0]);
	if (a->l==1){
		SLL_INIT_ARRAY(o);
	}
	else{
		o->l=a->l-1;
		o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
		if (!o->v){
			return 0;
		}
		for (sll_array_length_t i=0;i<o->l;i++){
			o->v[i]=a->v[i+1];
			SLL_ACQUIRE(o->v[i]);
		}
	}
	return a->v[0];
}



__SLL_EXTERNAL void sll_array_to_map(const sll_array_t* a,sll_map_t* o){
	if (!a->l){
		SLL_INIT_MAP(o);
		return;
	}
	o->l=a->l;
	sll_map_length_t e=a->l<<1;
	o->v=sll_allocate(e*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_map_length_t j=0;j<e;j+=2){
		o->v[j]=sll_int_to_object(i);
		o->v[j+1]=a->v[i];
		SLL_ACQUIRE(a->v[i]);
		i++;
	}
}



__SLL_EXTERNAL void sll_array_unshift(const sll_array_t* a,sll_object_t* v,sll_array_t* o){
	if (!a){
		o->l++;
		sll_allocator_resize((void**)(&(o->v)),o->l*sizeof(sll_object_t*));
		for (sll_array_length_t i=o->l-1;i;i--){
			o->v[i]=o->v[i-1];
		}
		o->v[0]=v;
		SLL_ACQUIRE(v);
		return;
	}
	o->l=a->l+1;
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	o->v[0]=v;
	SLL_ACQUIRE(v);
	for (sll_array_length_t i=1;i<o->l;i++){
		o->v[i]=a->v[i-1];
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_EXTERNAL void sll_array_xor(const sll_array_t* a,const sll_array_t* b,sll_array_t* o){
	if (b->l>a->l){
		const sll_array_t* c=a;
		a=b;
		b=c;
	}
	if (!b->l){
		sll_array_clone(a,o);
		return;
	}
	o->l=a->l;
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (;i<b->l;i++){
		o->v[i]=sll_operator_xor(a->v[i],b->v[i]);
	}
	while (i<a->l){
		SLL_ACQUIRE(a->v[i]);
		o->v[i]=a->v[i];
		i++;
	}
}



__SLL_EXTERNAL void sll_free_array(sll_array_t* a){
	for (sll_array_length_t i=0;i<a->l;i++){
		SLL_RELEASE(a->v[i]);
	}
	sll_allocator_release(a->v);
	a->v=NULL;
	a->l=0;
}

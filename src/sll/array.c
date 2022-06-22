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



__SLL_EXTERNAL void sll_array_and(const sll_array_t* a,const sll_array_t* b,sll_array_t* out){
	if (b->length>a->length){
		const sll_array_t* c=a;
		a=b;
		b=c;
	}
	if (!b->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=b->length;
	out->data=sll_allocator_init(b->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	bitmap_t* m=sll_zero_allocate(((b->length+63)>>6)*sizeof(bitmap_t));
	for (sll_array_length_t j=0;j<a->length;j++){
		sll_object_t* e=a->data[j];
		for (sll_array_length_t k=0;k<b->length;k++){
			if (sll_operator_strict_equal(e,b->data[k])){
				bitmap_t n=1ull<<(k&63);
				if (!((*(m+(k>>6)))&n)){
					(*(m+(k>>6)))|=n;
					out->data[i]=e;
					SLL_ACQUIRE(e);
					i++;
				}
				break;
			}
		}
	}
	sll_deallocate(m);
	if (out->length!=i){
		out->length=i;
		sll_allocator_resize((void**)(&(out->data)),i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_array_clone(const sll_array_t* array,sll_array_t* out){
	if (!array->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=array->length;
	out->data=sll_allocator_init(array->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<array->length;i++){
		out->data[i]=array->data[i];
		SLL_ACQUIRE(out->data[i]);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_array_count(const sll_array_t* array,sll_object_t* object){
	sll_array_length_t o=0;
	for (sll_array_length_t i=0;i<array->length;i++){
		if (sll_operator_strict_equal(array->data[i],object)){
			o++;
		}
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_array_count_multiple(const sll_array_t* array,sll_object_t*const* object_data,sll_array_length_t object_count){
	sll_array_length_t o=0;
	for (sll_array_length_t i=0;i<array->length;i++){
		for (sll_array_length_t j=0;j<object_count;j++){
			if (sll_operator_strict_equal(array->data[i],*(object_data+j))){
				o++;
				break;
			}
		}
	}
	return o;
}



__SLL_EXTERNAL void sll_array_combinations(const sll_array_t* a,const sll_array_t* b,sll_array_t* out){
	if (!a->length||!b->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=a->length*b->length;
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<a->length;j++){
		for (sll_array_length_t k=0;k<b->length;k++){
			out->data[i]=sll_operator_add(a->data[j],b->data[k]);
			i++;
		}
	}
}



__SLL_EXTERNAL void sll_array_combinations_string(const sll_array_t* array,const sll_string_t* string,sll_array_t* out){
	if (!array->length||!string->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=array->length*string->length;
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_string_length_t j=0;j<array->length;j++){
		for (sll_array_length_t k=0;k<string->length;k++){
			out->data[i]=sll_operator_add(array->data[j],sll_static_char[string->data[k]]);
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_array_compare_map(const sll_array_t* array,const sll_map_t* map,sll_bool_t inv){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_array_create(sll_array_length_t length,sll_array_t* out){
	if (!length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=length;
	out->data=sll_allocator_init(length*sizeof(sll_object_t*));
}



__SLL_EXTERNAL void sll_array_create_zero(sll_array_length_t length,sll_array_t* out){
	if (!length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=length;
	out->data=sll_allocator_init(length*sizeof(sll_object_t*));
	sll_static_int[0]->rc+=length;
	for (sll_array_length_t i=0;i<length;i++){
		out->data[i]=sll_static_int[0];
	}
}



__SLL_EXTERNAL void sll_array_duplicate(const sll_array_t* array,sll_integer_t count,sll_array_length_t extra,sll_array_t* out){
	if (!array->length){
		SLL_INIT_ARRAY(out);
	}
	SLL_ASSERT(extra<array->length);
	sll_bool_t r=0;
	if (count<0){
		count=-count;
		r=1;
	}
	sll_array_length_t m=(sll_array_length_t)count;
	if (!count){
		if (!extra){
			SLL_INIT_ARRAY(out);
			return;
		}
		out->length=extra;
		out->data=sll_allocator_init(extra*sizeof(sll_object_t*));
		for (sll_array_length_t i=0;i<extra;i++){
			out->data[i]=array->data[i];
			SLL_ACQUIRE(out->data[i]);
		}
		return;
	}
	count*=array->length;
	SLL_ASSERT(count<SLL_MAX_ARRAY_LENGTH);
	out->length=((sll_array_length_t)count)+extra;
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	if (r){
		sll_array_length_t i=array->length-1;
		for (sll_array_length_t j=0;j<i;j++){
			out->data[i]=array->data[j];
			out->data[i]->rc+=m;
			out->data[j]=array->data[i];
			out->data[j]->rc+=m;
			i--;
		}
		if (array->length&1){
			out->data[i]=array->data[i];
			out->data[i]->rc+=m;
		}
	}
	else{
		for (sll_array_length_t i=0;i<array->length;i++){
			out->data[i]=array->data[i];
			out->data[i]->rc+=m;
		}
	}
	sll_array_length_t i=array->length;
	sll_array_length_t j=0;
	while (i<out->length){
		out->data[i]=out->data[j];
		i++;
		j++;
	}
	if (extra){
		for (i=0;i<extra;i++){
			SLL_ACQUIRE(out->data[i]);
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_array_equal_map(const sll_array_t* array,const sll_map_t* map){
	if (array->length!=map->length){
		return 0;
	}
	for (sll_map_length_t i=0;i<(map->length<<1);i+=2){
		sll_object_t* e=map->data[i];
		sll_bool_t st=(e->type!=SLL_OBJECT_TYPE_INT||e->data.int_<0||e->data.int_>=array->length||!sll_operator_strict_equal(map->data[i+1],array->data[e->data.int_]));
		if (st){
			return 0;
		}
	}
	return 1;
}



__SLL_EXTERNAL void sll_array_extend(const sll_array_t* base,const sll_array_t* new,sll_array_t* out){
	if (!base){
		if (!new->length){
			return;
		}
		out->length+=new->length;
		sll_allocator_resize((void**)(&(out->data)),out->length*sizeof(sll_object_t*));
		sll_array_length_t i=out->length-new->length;
		for (sll_array_length_t j=0;j<new->length;j++){
			out->data[i]=new->data[j];
			SLL_ACQUIRE(new->data[j]);
			i++;
		}
		return;
	}
	if (!base->length){
		sll_array_clone(new,out);
		return;
	}
	if (!new->length){
		sll_array_clone(base,out);
		return;
	}
	sll_array_create(base->length+new->length,out);
	sll_array_length_t i=0;
	for (;i<base->length;i++){
		out->data[i]=base->data[i];
		SLL_ACQUIRE(base->data[i]);
	}
	for (sll_array_length_t j=0;j<new->length;j++){
		out->data[i]=new->data[j];
		SLL_ACQUIRE(new->data[j]);
		i++;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_array_get(const sll_array_t* array,sll_array_length_t index){
	if (index<array->length){
		SLL_ACQUIRE(array->data[index]);
		return array->data[index];
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_array_includes(const sll_array_t* array,sll_object_t* object){
	for (sll_array_length_t i=0;i<array->length;i++){
		if (sll_operator_strict_equal(array->data[i],object)){
			return 1;
		}
	}
	return 0;
}



__SLL_EXTERNAL void sll_array_join_arrays(const sll_array_t*const* array_data,sll_array_length_t array_count,sll_object_t* infix,sll_array_t* out){
	if (!array_count){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=array_count-1;
	infix->rc+=out->length;
	for (sll_array_length_t i=0;i<array_count;i++){
		out->length+=(*(array_data+i))->length;
	}
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<array_count;j++){
		if (j){
			out->data[i]=infix;
			i++;
		}
		const sll_array_t* a=*(array_data+j);
		for (sll_array_length_t k=0;k<a->length;k++){
			out->data[i]=a->data[k];
			SLL_ACQUIRE(a->data[k]);
			i++;
		}
	}
}



__SLL_EXTERNAL void sll_array_op(const sll_array_t* a,const sll_array_t* b,sll_binary_operator_t operator,sll_array_t* out){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_array_op_map(const sll_array_t* array,const sll_map_t* map,sll_binary_operator_t operator,sll_bool_t inv,sll_map_t* out){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_array_or(const sll_array_t* a,const sll_array_t* b,sll_array_t* out){
	if (!a->length&&!b->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=a->length+b->length;
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<a->length;i++){
		out->data[i]=a->data[i];
	}
	for (sll_array_length_t i=0;i<b->length;i++){
		out->data[a->length+i]=b->data[i];
	}
	sll_array_length_t i=0;
	bitmap_t* m=sll_zero_allocate(((out->length+63)>>6)*sizeof(bitmap_t));
	for (sll_array_length_t j=0;j<out->length;j++){
		sll_object_t* e=out->data[j];
		if ((*(m+(j>>6)))&(1ull<<(j&63))){
			continue;
		}
		out->data[i]=out->data[j];
		SLL_ACQUIRE(out->data[i]);
		i++;
		for (sll_array_length_t k=j+1;k<out->length;k++){
			if (sll_operator_strict_equal(e,out->data[k])){
				(*(m+(k>>6)))|=1ull<<(k&63);
			}
		}
	}
	sll_deallocate(m);
	if (out->length!=i){
		out->length=i;
		sll_allocator_resize((void**)(&(out->data)),i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_array_parse_char(const sll_array_t* array){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_array_parse_float(const sll_array_t* array){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_array_parse_int(const sll_array_t* array){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_array_pop(const sll_array_t* array,sll_array_t* out){
	if (!array){
		if (!out->length){
			return SLL_ACQUIRE_STATIC_INT(0);
		}
		sll_object_t* e=out->data[out->length-1];
		out->length--;
		sll_allocator_resize((void**)(&(out->data)),out->length*sizeof(sll_object_t*));
		return e;
	}
	if (!array->length){
		SLL_INIT_ARRAY(out);
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SLL_ACQUIRE(array->data[array->length-1]);
	if (array->length==1){
		SLL_INIT_ARRAY(out);
	}
	else{
		out->length=array->length-1;
		out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
		for (sll_array_length_t i=0;i<out->length;i++){
			out->data[i]=array->data[i];
			SLL_ACQUIRE(out->data[i]);
		}
	}
	return array->data[array->length-1];
}



__SLL_EXTERNAL void sll_array_push(const sll_array_t* array,sll_object_t* object,sll_array_t* out){
	if (!array){
		out->length++;
		sll_allocator_resize((void**)(&(out->data)),out->length*sizeof(sll_object_t*));
		SLL_ACQUIRE(object);
		out->data[out->length-1]=object;
		return;
	}
	out->length=array->length+1;
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<array->length;i++){
		out->data[i]=array->data[i];
		SLL_ACQUIRE(out->data[i]);
	}
	out->data[array->length]=object;
	SLL_ACQUIRE(object);
}



__SLL_EXTERNAL void sll_array_range(sll_integer_t start,sll_integer_t end,sll_integer_t step,sll_array_t* out){
	if (!step||start==end){
		SLL_INIT_ARRAY(out);
		return;
	}
	if (end<start){
		SLL_ASSERT(step<0);
		out->length=(sll_array_length_t)((end-start)/step);
		out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
		sll_array_length_t i=0;
		for (sll_integer_t j=start;j>end;j+=step){
			out->data[i]=sll_int_to_object(j);
			i++;
		}
		return;
	}
	SLL_ASSERT(step>0);
	out->length=(sll_array_length_t)((end-start)/step);
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_integer_t j=start;j<end;j+=step){
		out->data[i]=sll_int_to_object(j);
		i++;
	}
}



__SLL_EXTERNAL void sll_array_remove(const sll_array_t* array,sll_object_t* object,sll_array_t* out){
	if (!array){
		sll_array_length_t i=0;
		for (sll_array_length_t j=0;j<out->length;j++){
			if (!sll_operator_strict_equal(out->data[j],object)){
				out->data[i]=out->data[j];
				i++;
			}
			else{
				SLL_RELEASE(out->data[j]);
			}
		}
		out->length=i;
		sll_allocator_resize((void**)(&(out->data)),out->length*sizeof(sll_object_t*));
		return;
	}
	if (!array->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=array->length;
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<array->length;j++){
		if (!sll_operator_strict_equal(array->data[j],object)){
			out->data[i]=array->data[j];
			SLL_ACQUIRE(out->data[i]);
			i++;
		}
	}
	out->length=i;
	sll_allocator_resize((void**)(&(out->data)),out->length*sizeof(sll_object_t*));
}



__SLL_EXTERNAL void sll_array_remove_multiple(const sll_array_t* array,sll_object_t** object_data,sll_array_length_t object_count,sll_array_t* out){
	if (!array->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=array->length;
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_array_length_t j=0;j<array->length;j++){
		for (sll_array_length_t k=0;k<object_count;k++){
			if (sll_operator_strict_equal(array->data[j],*(object_data+k))){
				goto _skip_value;
			}
		}
		out->data[i]=array->data[j];
		SLL_ACQUIRE(out->data[i]);
		i++;
_skip_value:;
	}
	out->length=i;
	sll_allocator_resize((void**)(&(out->data)),out->length*sizeof(sll_object_t*));
}



__SLL_EXTERNAL void sll_array_replace(const sll_array_t* array,sll_object_t* key,sll_object_t* value,sll_array_t* out){
	if (!array->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=array->length;
	out->data=sll_allocator_init(array->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<array->length;i++){
		out->data[i]=(sll_operator_strict_equal(array->data[i],key)?value:array->data[i]);
		SLL_ACQUIRE(out->data[i]);
	}
}



__SLL_EXTERNAL void sll_array_resize(const sll_array_t* array,sll_integer_t delta,sll_array_t* out){
	if (delta<0){
		delta=-delta;
		if (delta>=array->length){
			SLL_INIT_ARRAY(out);
			return;
		}
		out->length=(sll_array_length_t)(array->length-delta);
		out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
		for (sll_array_length_t i=(sll_array_length_t)delta;i<array->length;i++){
			out->data[i-delta]=array->data[i];
			SLL_ACQUIRE(array->data[i]);
		}
		return;
	}
	SLL_ASSERT(array->length+delta<=SLL_MAX_ARRAY_LENGTH);
	out->length=(sll_array_length_t)(array->length+delta);
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<(sll_array_length_t)delta;i++){
		out->data[i]=SLL_ACQUIRE_STATIC_INT(0);
	}
	for (sll_array_length_t i=0;i<array->length;i++){
		out->data[i+delta]=array->data[i];
		SLL_ACQUIRE(array->data[i]);
	}
}



__SLL_EXTERNAL void sll_array_reverse(const sll_array_t* array,sll_array_t* out){
	if (!array->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=array->length;
	out->data=sll_allocator_init(array->length*sizeof(sll_object_t*));
	sll_array_length_t i=array->length;
	for (sll_array_length_t j=0;j<array->length;j++){
		i--;
		out->data[j]=array->data[i];
		SLL_ACQUIRE(out->data[j]);
	}
}



__SLL_EXTERNAL void sll_array_select(const sll_array_t* array,sll_integer_t start,sll_integer_t end,sll_integer_t step,sll_array_t* out){
	WRAP_ARRAY_INDEX_VAR(start,array->length);
	WRAP_ARRAY_INDEX_VAR(end,array->length);
	SLL_INIT_ARRAY(out);
	if (!array->length||start==end||!step||(start<=end&&start>=array->length)||(end<=start&&end>=array->length)){
		return;
	}
	if (start<end){
		if (step<0){
			return;
		}
		if (start+step>=array->length){
			out->length=1;
			out->data=sll_allocator_init(sizeof(sll_object_t*));
			out->data[0]=array->data[0];
			SLL_ACQUIRE(out->data[0]);
			return;
		}
		out->length=(sll_array_length_t)((end-start-1)/step+1);
		out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
		sll_array_length_t i=0;
		do{
			out->data[i]=array->data[start];
			SLL_ACQUIRE(out->data[i]);
			i++;
			start+=step;
		} while (start<end);
		SLL_ASSERT(i==out->length);
	}
	else{
		if (step>0){
			return;
		}
		step=-step;
		if (step>start){
			out->length=1;
			out->data=sll_allocator_init(sizeof(sll_object_t*));
			out->data[0]=array->data[0];
			SLL_ACQUIRE(out->data[0]);
			return;
		}
		out->length=(sll_array_length_t)((start-end-1)/step+1);
		out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
		sll_array_length_t i=0;
		do{
			out->data[i]=array->data[start];
			SLL_ACQUIRE(out->data[i]);
			i++;
			start-=step;
		} while (start>end);
		SLL_ASSERT(i==out->length);
	}
}



__SLL_EXTERNAL void sll_array_split(const sll_array_t* array,sll_object_t* object,sll_array_t* out){
	out->length=1;
	sll_object_t** ptr=sll_allocate_stack(sizeof(sll_object_t*));
	sll_object_t** arr_ptr=sll_allocate_stack(1);
	sll_object_t* oa=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
	oa->data.array.length=0;
	*ptr=oa;
	for (sll_array_length_t i=0;i<array->length;i++){
		if (sll_operator_strict_equal(array->data[i],object)){
			oa->data.array.data=(oa->data.array.length?sll_allocator_from_memory(arr_ptr,oa->data.array.length*sizeof(sll_object_t*)):NULL);
			sll_deallocate(arr_ptr);
			arr_ptr=sll_allocate_stack(1);
			oa=sll_create_object(SLL_OBJECT_TYPE_ARRAY);
			oa->data.array.length=0;
			out->length++;
			ptr=sll_reallocate(ptr,out->length*sizeof(sll_object_t*));
			*(ptr+out->length-1)=oa;
		}
		else{
			oa->data.array.length++;
			arr_ptr=sll_reallocate(arr_ptr,oa->data.array.length*sizeof(sll_object_t*));
			*(arr_ptr+oa->data.array.length-1)=array->data[i];
			SLL_ACQUIRE(array->data[i]);
		}
	}
	oa->data.array.data=(oa->data.array.length?sll_allocator_from_memory(arr_ptr,oa->data.array.length*sizeof(sll_object_t*)):NULL);
	sll_deallocate(arr_ptr);
	out->data=sll_allocator_from_memory(ptr,out->length*sizeof(sll_object_t*));
	sll_deallocate(ptr);
}



__SLL_EXTERNAL void sll_array_set(const sll_array_t* array,sll_array_length_t index,sll_object_t* object){
	if (index<array->length){
		SLL_ACQUIRE(object);
		SLL_RELEASE(array->data[index]);
		array->data[index]=object;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_array_shift(const sll_array_t* array,sll_array_t* out){
	if (!array){
		if (!out->length){
			return SLL_ACQUIRE_STATIC_INT(0);
		}
		sll_object_t* e=out->data[0];
		out->length--;
		for (sll_array_length_t i=0;i<out->length;i++){
			out->data[i]=out->data[i+1];
		}
		sll_allocator_resize((void**)(&(out->data)),out->length*sizeof(sll_object_t*));
		return e;
	}
	if (!array->length){
		SLL_INIT_ARRAY(out);
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	SLL_ACQUIRE(array->data[0]);
	if (array->length==1){
		SLL_INIT_ARRAY(out);
	}
	else{
		out->length=array->length-1;
		out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
		if (!out->data){
			return 0;
		}
		for (sll_array_length_t i=0;i<out->length;i++){
			out->data[i]=array->data[i+1];
			SLL_ACQUIRE(out->data[i]);
		}
	}
	return array->data[0];
}



__SLL_EXTERNAL void sll_array_to_map(const sll_array_t* array,sll_map_t* out){
	if (!array->length){
		SLL_INIT_MAP(out);
		return;
	}
	out->length=array->length;
	sll_map_length_t e=array->length<<1;
	out->data=sll_allocate(e*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_map_length_t j=0;j<e;j+=2){
		out->data[j]=sll_int_to_object(i);
		out->data[j+1]=array->data[i];
		SLL_ACQUIRE(array->data[i]);
		i++;
	}
}



__SLL_EXTERNAL void sll_array_unshift(const sll_array_t* array,sll_object_t* object,sll_array_t* out){
	if (!array){
		out->length++;
		sll_allocator_resize((void**)(&(out->data)),out->length*sizeof(sll_object_t*));
		for (sll_array_length_t i=out->length-1;i;i--){
			out->data[i]=out->data[i-1];
		}
		out->data[0]=object;
		SLL_ACQUIRE(object);
		return;
	}
	out->length=array->length+1;
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	out->data[0]=object;
	SLL_ACQUIRE(object);
	for (sll_array_length_t i=1;i<out->length;i++){
		out->data[i]=array->data[i-1];
		SLL_ACQUIRE(out->data[i]);
	}
}



__SLL_EXTERNAL void sll_array_xor(const sll_array_t* a,const sll_array_t* b,sll_array_t* out){
	if (b->length>a->length){
		const sll_array_t* c=a;
		a=b;
		b=c;
	}
	if (!b->length){
		sll_array_clone(a,out);
		return;
	}
	out->length=a->length;
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (;i<b->length;i++){
		out->data[i]=sll_operator_xor(a->data[i],b->data[i]);
	}
	while (i<a->length){
		SLL_ACQUIRE(a->data[i]);
		out->data[i]=a->data[i];
		i++;
	}
}



__SLL_EXTERNAL void sll_free_array(sll_array_t* array){
	for (sll_array_length_t i=0;i<array->length;i++){
		SLL_RELEASE(array->data[i]);
	}
	sll_allocator_release(array->data);
	array->data=NULL;
	array->length=0;
}

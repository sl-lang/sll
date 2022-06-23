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



__SLL_EXTERNAL void sll_free_map(sll_map_t* map){
	for (sll_map_length_t j=0;j<(map->length<<1);j++){
		SLL_RELEASE(*(map->data+j));
	}
	sll_deallocate(map->data);
	map->data=NULL;
	map->length=0;
}



__SLL_EXTERNAL void sll_map_add(const sll_map_t* map,sll_object_t* key,sll_object_t* value,sll_map_t* out){
	sll_map_length_t l=map->length<<1;
	SLL_ACQUIRE(value);
	out->length=map->length+1;
	out->data=sll_allocate((l+2)*sizeof(sll_object_t*));
	for (sll_map_length_t i=0;i<l;i+=2){
		out->data[i]=map->data[i];
		SLL_ACQUIRE(out->data[i]);
		if (sll_operator_strict_equal(map->data[i],key)){
			out->length--;
			out->data=sll_reallocate(out->data,l*sizeof(sll_object_t*));
			out->data[i+1]=value;
			i+=2;
			while (i<l){
				out->data[i]=map->data[i];
				SLL_ACQUIRE(out->data[i]);
				i++;
			}
			return;
		}
		out->data[i+1]=map->data[i+1];
		SLL_ACQUIRE(out->data[i+1]);
	}
	out->data[l]=key;
	out->data[l+1]=value;
	SLL_ACQUIRE(key);
}



__SLL_EXTERNAL void sll_map_add_array(const sll_map_t* map,const sll_array_t* array,sll_map_t* out){
	out->length=map->length+array->length;
	out->data=sll_allocate((out->length<<1)*sizeof(sll_object_t*));
	sll_map_length_t i=map->length<<1;
	bitmap_t* sm=sll_zero_allocate(((array->length+63)>>6)*sizeof(bitmap_t));
	for (sll_map_length_t j=0;j<i;j+=2){
		sll_object_t* e=map->data[j];
		SLL_ACQUIRE(e);
		out->data[j]=e;
		sll_integer_t n=e->data.int_;
		if (e->type==SLL_OBJECT_TYPE_INT&&n>=0&&n<array->length){
			out->data[j+1]=array->data[n];
			SLL_ACQUIRE(out->data[j+1]);
			(*(sm+(n>>6)))|=1ull<<(n&63);
			continue;
		}
		out->data[j+1]=map->data[j+1];
		SLL_ACQUIRE(out->data[j+1]);
	}
	for (sll_array_length_t j=0;j<((array->length+63)>>6);j++){
		bitmap_t v=~(*(sm+j));
		while (v){
			sll_array_length_t k=(j<<6)|FIND_FIRST_SET_BIT(v);
			if (k>=array->length){
				break;
			}
			out->data[i]=sll_int_to_object(k);
			out->data[i+1]=array->data[k];
			SLL_ACQUIRE(out->data[i+1]);
			i+=2;
			v&=v-1;
		}
	}
	sll_deallocate(sm);
	if (i!=out->length){
		out->length=i>>1;
		out->data=sll_reallocate(out->data,i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_map_add_string(const sll_map_t* map,const sll_string_t* string,sll_map_t* out){
	out->length=map->length+string->length;
	out->data=sll_allocate((out->length<<1)*sizeof(sll_object_t*));
	sll_map_length_t i=map->length<<1;
	bitmap_t* sm=sll_zero_allocate(((string->length+63)>>6)*sizeof(bitmap_t));
	for (sll_map_length_t j=0;j<i;j+=2){
		sll_object_t* e=map->data[j];
		SLL_ACQUIRE(e);
		out->data[j]=e;
		sll_integer_t n=e->data.int_;
		if (e->type==SLL_OBJECT_TYPE_INT&&n>=0&&n<string->length){
			out->data[j+1]=SLL_FROM_CHAR(string->data[n]);
			(*(sm+(n>>6)))|=1ull<<(n&63);
			continue;
		}
		out->data[j+1]=map->data[j+1];
		SLL_ACQUIRE(out->data[j+1]);
	}
	for (sll_string_length_t j=0;j<((string->length+63)>>6);j++){
		bitmap_t v=~(*(sm+j));
		while (v){
			sll_string_length_t k=(j<<6)|FIND_FIRST_SET_BIT(v);
			if (k>=string->length){
				break;
			}
			out->data[i]=sll_int_to_object(k);
			out->data[i+1]=SLL_FROM_CHAR(string->data[k]);
			i+=2;
			v&=v-1;
		}
	}
	sll_deallocate(sm);
	if ((i>>1)!=out->length){
		out->length=i>>1;
		out->data=sll_reallocate(out->data,i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_map_and(const sll_map_t* a,const sll_map_t* b,sll_map_t* out){
	if (b->length>a->length){
		const sll_map_t* c=a;
		a=b;
		b=c;
	}
	if (!b->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=b->length;
	out->data=sll_allocate((b->length<<1)*sizeof(sll_object_t*));
	sll_map_length_t i=0;
	for (sll_map_length_t j=0;j<(a->length<<1);j+=2){
		sll_object_t* e=a->data[j];
		for (sll_map_length_t k=0;k<(b->length<<1);k+=2){
			if (sll_operator_strict_equal(e,b->data[k])){
				out->data[i]=e;
				SLL_ACQUIRE(e);
				out->data[i+1]=sll_operator_add(a->data[j+1],b->data[k+1]);
				i+=2;
				break;
			}
		}
	}
	if ((i>>1)!=out->length){
		out->length=i>>1;
		out->data=sll_reallocate(out->data,i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_map_and_array(const sll_map_t* map,const sll_array_t* array,sll_map_t* out){
	out->length=(map->length<array->length?map->length:array->length);
	if (!out->length){
		out->data=NULL;
		return;
	}
	out->data=sll_allocate((out->length<<1)*sizeof(sll_object_t*));
	sll_map_length_t i=0;
	for (sll_map_length_t j=0;j<(map->length<<1);j+=2){
		sll_object_t* e=map->data[j];
		if (e->type==SLL_OBJECT_TYPE_INT&&e->data.int_>=0&&e->data.int_<array->length){
			out->data[i]=e;
			SLL_ACQUIRE(e);
			out->data[i+1]=sll_operator_add(map->data[j+1],array->data[e->data.int_]);
			i+=2;
		}
	}
	if ((i>>1)!=out->length){
		out->length=i>>1;
		out->data=sll_reallocate(out->data,i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_map_and_string(const sll_map_t* map,const sll_string_t* string,sll_map_t* out){
	out->length=(map->length<string->length?map->length:string->length);
	if (!out->length){
		out->data=NULL;
		return;
	}
	out->data=sll_allocate((out->length<<1)*sizeof(sll_object_t*));
	sll_map_length_t i=0;
	for (sll_map_length_t j=0;j<(map->length<<1);j+=2){
		sll_object_t* e=map->data[j];
		if (e->type==SLL_OBJECT_TYPE_INT&&e->data.int_>=0&&e->data.int_<string->length){
			out->data[i]=e;
			SLL_ACQUIRE(e);
			out->data[i+1]=sll_operator_add(map->data[i+1],sll_static_char[string->data[e->data.int_]]);
			i+=2;
		}
	}
	if ((i>>1)!=out->length){
		out->length=i>>1;
		out->data=sll_reallocate(out->data,i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_map_clone(const sll_map_t* map,sll_map_t* out){
	if (!map->length){
		SLL_INIT_MAP(out);
		return;
	}
	out->length=map->length;
	out->data=sll_allocate((out->length<<1)*sizeof(sll_object_t*));
	for (sll_map_length_t i=0;i<(out->length<<1);i++){
		out->data[i]=map->data[i];
		SLL_ACQUIRE(out->data[i]);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_map_compare(const sll_map_t* a,const sll_map_t* b){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_map_create(sll_map_length_t length,sll_map_t* out){
	if (!length){
		SLL_INIT_MAP(out);
		return;
	}
	out->length=length;
	out->data=sll_zero_allocate((length<<1)*sizeof(sll_object_t*));
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_get(const sll_map_t* map,sll_object_t* key){
	for (sll_map_length_t i=0;i<map->length;i++){
		if (sll_operator_strict_equal(map->data[i<<1],key)){
			SLL_ACQUIRE(map->data[(i<<1)+1]);
			return map->data[(i<<1)+1];
		}
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_get_key(const sll_map_t* map,sll_map_length_t index){
	if (index<map->length){
		SLL_ACQUIRE(map->data[index<<1]);
		return map->data[index<<1];
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_get_value(const sll_map_t* map,sll_map_length_t index){
	if (index<map->length){
		SLL_ACQUIRE(map->data[(index<<1)+1]);
		return map->data[(index<<1)+1];
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_map_includes(const sll_map_t* map,sll_object_t* key){
	for (sll_map_length_t i=0;i<map->length;i++){
		if (sll_operator_strict_equal(map->data[i<<1],key)){
			return 1;
		}
	}
	return 0;
}



__SLL_EXTERNAL void sll_map_join(const sll_map_t* a,const sll_map_t* b,sll_map_t* out){
	if (!a){
		if (!b->length){
			return;
		}
		if (!out->length){
			sll_map_clone(b,out);
			return;
		}
		out->length+=b->length;
		SLL_UNIMPLEMENTED();
	}
	out->length=a->length+b->length;
	if (!out->length){
		out->data=NULL;
		return;
	}
	out->data=sll_allocate((out->length<<1)*sizeof(sll_object_t*));
	sll_map_length_t al=(a->length<<1);
	for (sll_array_length_t i=0;i<al;i++){
		out->data[i]=a->data[i];
	}
	sll_map_length_t i=al;
	for (sll_map_length_t j=0;j<(b->length<<1);j+=2){
		sll_object_t* e=b->data[j];
		for (sll_map_length_t k=0;k<al;k+=2){
			if (sll_operator_strict_equal(out->data[k],e)){
				out->data[k+1]=b->data[j+1];
				goto _next_key;
			}
		}
		out->data[i]=e;
		out->data[i+1]=b->data[j+1];
		i+=2;
_next_key:;
	}
	if (i!=(out->length<<1)){
		out->length=i>>1;
		out->data=sll_reallocate(out->data,i*sizeof(sll_object_t*));
	}
	while (i){
		i--;
		SLL_ACQUIRE(out->data[i]);
	}
}



__SLL_EXTERNAL void sll_map_keys(const sll_map_t* map,sll_array_t* out){
	if (!map->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=map->length;
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<out->length;i++){
		out->data[i]=map->data[i<<1];
		SLL_ACQUIRE(out->data[i]);
	}
}



__SLL_EXTERNAL void sll_map_op(const sll_map_t* a,const sll_map_t* b,sll_binary_operator_t operator,sll_map_t* out){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_map_remove(const sll_map_t* map,sll_object_t* key,sll_map_t* out){
	if (!map){
		sll_map_length_t i=0;
		while (i<(out->length<<1)){
			if (sll_operator_strict_equal(out->data[i],key)){
				SLL_RELEASE(out->data[i]);
				sll_object_t* v=out->data[i+1];
				i+=2;
				while (i<(out->length<<1)){
					out->data[i-2]=out->data[i];
					i++;
				}
				out->length--;
				out->data=sll_reallocate(out->data,(out->length<<1)*sizeof(sll_object_t*));
				return v;
			}
			i+=2;
		}
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	for (sll_map_length_t i=0;i<map->length;i++){
		SLL_ACQUIRE(map->data[(i<<1)+1]);
		if (sll_operator_strict_equal(map->data[i<<1],key)){
			out->length=map->length-1;
			out->data=sll_allocate((out->length<<1)*sizeof(sll_object_t*));
			for (sll_map_length_t j=0;j<(i<<1);j++){
				out->data[i]=map->data[i];
			}
			sll_object_t* v=map->data[(i<<1)+1];
			for (sll_map_length_t j=(i+1)<<1;j<(map->length<<1);j++){
				out->data[j-2]=map->data[j];
				SLL_ACQUIRE(out->data[j-2]);
			}
			return v;
		}
		SLL_ACQUIRE(map->data[i<<1]);
	}
	out->length=map->length;
	out->data=sll_allocate((out->length<<1)*sizeof(sll_object_t*));
	for (sll_map_length_t i=0;i<(out->length<<1);i++){
		out->data[i]=map->data[i];
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL void sll_map_remove_array(const sll_map_t* map,const sll_array_t* array,sll_map_t* out){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_map_remove_map(const sll_map_t* a,const sll_map_t* b,sll_map_t* out){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_map_remove_string(const sll_map_t* map,const sll_string_t* string,sll_map_t* out){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_map_set(sll_map_t* map,sll_object_t* key,sll_object_t* value){
	SLL_ACQUIRE(value);
	for (sll_map_length_t i=0;i<map->length;i++){
		if (sll_operator_strict_equal(map->data[i<<1],key)){
			SLL_RELEASE(map->data[(i<<1)+1]);
			map->data[(i<<1)+1]=value;
			return;
		}
	}
	map->length++;
	map->data=sll_reallocate(map->data,(map->length<<1)*sizeof(sll_object_t*));
	map->data[(map->length-1)<<1]=key;
	map->data[(map->length<<1)-1]=value;
	SLL_ACQUIRE(key);
}



__SLL_EXTERNAL void sll_map_set_key(const sll_map_t* map,sll_map_length_t index,sll_object_t* key){
	if (index<map->length){
		SLL_ACQUIRE(key);
		SLL_RELEASE(map->data[index<<1]);
		map->data[index<<1]=key;
	}
}



__SLL_EXTERNAL void sll_map_set_value(const sll_map_t* map,sll_map_length_t index,sll_object_t* value){
	if (index<map->length){
		SLL_ACQUIRE(value);
		SLL_RELEASE(map->data[(index<<1)+1]);
		map->data[(index<<1)+1]=value;
	}
}



__SLL_EXTERNAL void sll_map_to_array(const sll_map_t* map,sll_array_t* out){
	if (!map->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=map->length<<1;
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<out->length;i++){
		out->data[i]=map->data[i];
		SLL_ACQUIRE(out->data[i]);
	}
}



__SLL_EXTERNAL void sll_map_values(const sll_map_t* map,sll_array_t* out){
	if (!map->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=map->length;
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<out->length;i++){
		out->data[i]=map->data[(i<<1)+1];
		SLL_ACQUIRE(out->data[i]);
	}
}

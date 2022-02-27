#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>



__API_FUNC(array_count){
	return sll_array_count(a,b);
}



__API_FUNC(array_create){
	sll_array_create_zero((a<0?0:(a>SLL_MAX_ARRAY_LENGTH?SLL_MAX_ARRAY_LENGTH:(sll_array_length_t)a)),out);
}



__API_FUNC(array_extend){
	sll_array_extend(NULL,b,a);
}



__API_FUNC(array_join){
	sll_array_t** al=sll_allocate_stack(a->l*sizeof(sll_array_t*));
	sll_object_t** obj=sll_allocate_stack(a->l*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<a->l;i++){
		*(obj+i)=sll_operator_cast(a->v[i],sll_static_int[SLL_OBJECT_TYPE_ARRAY]);
		*(al+i)=&((*(obj+i))->dt.a);
	}
	sll_array_join_arrays((const sll_array_t*const*)al,a->l,b,out);
	for (sll_array_length_t i=0;i<a->l;i++){
		SLL_RELEASE(*(obj+i));
	}
	sll_deallocate(obj);
	sll_deallocate(al);
}




__API_FUNC(array_pop){
	return sll_array_pop(NULL,a);
}




__API_FUNC(array_push){
	sll_array_push(NULL,b,a);
}



__API_FUNC(array_remove){
	sll_array_remove(NULL,b,a);
}



__API_FUNC(array_replace){
	sll_array_replace(a,b,c,out);
}



__API_FUNC(array_reverse){
	sll_array_reverse(a,out);
}



__API_FUNC(array_shift){
	return sll_array_shift(NULL,a);
}



__API_FUNC(array_split){
	sll_array_split(a,b,out);
}



__API_FUNC(array_unshift){
	sll_array_unshift(NULL,b,a);
}

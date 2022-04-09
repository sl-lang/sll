#include <sll/_internal/api.h>
#include <sll/_internal/gc.h>
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



__API_FUNC(array_index){
	for (sll_array_length_t i=0;i<a->l;i++){
		if (sll_operator_strict_equal(a->v[i],b)){
			return i;
		}
	}
	return -1;
}



__API_FUNC(array_join){
	sll_array_join_arrays((const sll_array_t*const*)a,ac,b,out);
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

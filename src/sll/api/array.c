#include <sll/_internal/gc.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_array_count(sll_array_t* a,sll_object_t* b){
	return sll_array_count(a,b);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_create(sll_integer_t a,sll_array_t* out){
	sll_array_create_zero((a<0?0:(a>SLL_MAX_ARRAY_LENGTH?SLL_MAX_ARRAY_LENGTH:(sll_array_length_t)a)),out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_extend(sll_array_t* a,sll_array_t* b){
	sll_array_extend(NULL,b,a);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_array_index(sll_array_t* a,sll_object_t* b){
	for (sll_array_length_t i=0;i<a->l;i++){
		if (sll_operator_strict_equal(a->v[i],b)){
			return i;
		}
	}
	return -1;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_join(sll_array_t*const* a,sll_arg_count_t ac,sll_object_t* b,sll_array_t* out){
	sll_array_join_arrays((const sll_array_t*const*)a,ac,b,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_pop(sll_array_t* a){
	return sll_array_pop(NULL,a);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_push(sll_array_t* a,sll_object_t* b){
	sll_array_push(NULL,b,a);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_remove(sll_array_t* a,sll_object_t* b){
	sll_array_remove(NULL,b,a);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_replace(sll_array_t* a,sll_object_t* b,sll_object_t* c,sll_array_t* out){
	sll_array_replace(a,b,c,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_reverse(sll_array_t* a,sll_array_t* out){
	sll_array_reverse(a,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_shift(sll_array_t* a){
	return sll_array_shift(NULL,a);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_split(sll_array_t* a,sll_object_t* b,sll_array_t* out){
	sll_array_split(a,b,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_unshift(sll_array_t* a,sll_object_t* b){
	sll_array_unshift(NULL,b,a);
}

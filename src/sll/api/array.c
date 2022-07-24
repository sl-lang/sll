#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_array_length_t sll_api_array_count(const sll_array_t* array,sll_object_t elem){
	return sll_array_count(array,elem);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_create(sll_array_length_t length,sll_array_t* out){
	sll_array_create_zero(length,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_array_length_t sll_api_array_extend(sll_array_t* array,const sll_array_t* new){
	sll_array_extend(NULL,new,array);
	return array->length;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_array_index(const sll_array_t* array,sll_object_t elem){
	for (sll_array_length_t i=0;i<array->length;i++){
		if (sll_operator_strict_equal(array->data[i],elem)){
			return i;
		}
	}
	return -1;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_join(const sll_array_t*const* array,sll_arg_count_t length,sll_object_t elem,sll_array_t* out){
	sll_array_join_arrays(array,length,elem,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t sll_api_array_pop(sll_array_t* array){
	return sll_array_pop(NULL,array);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_array_length_t sll_api_array_push(sll_array_t* array,sll_object_t elem){
	sll_array_push(NULL,elem,array);
	return array->length;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_array_length_t sll_api_array_remove(sll_array_t* array,sll_object_t elem){
	sll_array_remove(NULL,elem,array);
	return array->length;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_replace(const sll_array_t* array,sll_object_t old,sll_object_t new,sll_array_t* out){
	sll_array_replace(array,old,new,out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_reverse(const sll_array_t* array,sll_array_t* out){
	sll_array_reverse(array,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t sll_api_array_shift(sll_array_t* array){
	return sll_array_shift(NULL,array);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_split(const sll_array_t* array,sll_object_t key,sll_array_t* out){
	sll_array_split(array,key,out);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_array_length_t sll_api_array_unshift(sll_array_t* array,sll_object_t elem){
	sll_array_unshift(NULL,elem,array);
	return array->length;
}

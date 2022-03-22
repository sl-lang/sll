#include <sll/common.h>
#include <sll/identifier.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_add_index(sll_identifier_index_t i,sll_identifier_index_t j){
	return SLL_IDENTIFIER_ADD_INDEX(i,j);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_create(sll_identifier_index_t i,sll_identifier_index_t j){
	return SLL_CREATE_IDENTIFIER(i,j);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_get_array_id(sll_identifier_index_t i){
	return SLL_IDENTIFIER_GET_ARRAY_ID(i);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_get_array_index(sll_identifier_index_t i){
	return SLL_IDENTIFIER_GET_ARRAY_INDEX(i);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_identifier_get_string_index(sll_identifier_t* v){
	return SLL_IDENTIFIER_GET_STRING_INDEX(v);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_identifier_is_tls(sll_identifier_t* v){
	return SLL_IDENTIFIER_IS_TLS(v);
}



__SLL_EXTERNAL void sll_identifier_set_string_index(sll_identifier_t* v,sll_string_index_t si,sll_bool_t tls){
	SLL_IDENTIFIER_SET_STRING_INDEX(v,si,tls);
}



__SLL_EXTERNAL void sll_identifier_update_string_index(sll_identifier_t* v,sll_string_index_t si){
	SLL_IDENTIFIER_UPDATE_STRING_INDEX(v,si);
}



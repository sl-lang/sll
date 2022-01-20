#include <sll/common.h>
#include <sll/identifier.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_add_index(sll_identifier_index_t i,sll_identifier_index_t j){
	return SLL_IDENTIFIER_ADD_INDEX(i,j);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_create_identifier(sll_identifier_index_t i,sll_identifier_index_t j){
	return SLL_CREATE_IDENTIFIER(i,j);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_get_array_id(sll_identifier_index_t i){
	return SLL_IDENTIFIER_GET_ARRAY_ID(i);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_get_array_index(sll_identifier_index_t i){
	return SLL_IDENTIFIER_GET_ARRAY_INDEX(i);
}

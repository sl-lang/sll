#include <sll/common.h>
#include <sll/identifier.h>
#include <sll/string_table.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_add_index(sll_identifier_index_t identifier_index,sll_identifier_index_t delta_index){
	return SLL_IDENTIFIER_ADD_INDEX(identifier_index,delta_index);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_create(sll_identifier_index_t index,sll_identifier_index_t id){
	return SLL_CREATE_IDENTIFIER(index,id);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_get_array_id(sll_identifier_index_t identifier_index){
	return SLL_IDENTIFIER_GET_ARRAY_ID(identifier_index);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_identifier_index_t sll_identifier_get_array_index(sll_identifier_index_t identifier_index){
	return SLL_IDENTIFIER_GET_ARRAY_INDEX(identifier_index);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_index_t sll_identifier_get_string_index(sll_identifier_t* identifier){
	return SLL_IDENTIFIER_GET_STRING_INDEX(identifier);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_identifier_is_tls(sll_identifier_t* identifier){
	return SLL_IDENTIFIER_IS_TLS(identifier);
}



__SLL_EXTERNAL void sll_identifier_set_string_index(sll_identifier_t* identifier,sll_string_index_t name_string_index,sll_bool_t tls){
	SLL_IDENTIFIER_SET_STRING_INDEX(identifier,name_string_index,tls);
}



__SLL_EXTERNAL void sll_identifier_update_string_index(sll_identifier_t* identifier,sll_string_index_t name_string_index){
	SLL_IDENTIFIER_UPDATE_STRING_INDEX(identifier,name_string_index);
}



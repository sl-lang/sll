#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/string.h>
#include <sll/types.h>



static const sll_char_t* _error_string_pointer_table[ERROR_STRING_POINTER_TABLE_SIZE];
static sll_array_length_t _error_string_pointer_table_index=0;



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_error_from_string_pointer(const sll_char_t* string){
	_error_string_pointer_table[_error_string_pointer_table_index]=string;
	sll_error_t out=SLL_ERROR_FROM_EXTRA(SLL_ERROR_STRING,_error_string_pointer_table_index);
	_error_string_pointer_table_index=(_error_string_pointer_table_index+1)%ERROR_STRING_POINTER_TABLE_SIZE;
	return out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_char_t* sll_error_get_string_pointer(sll_error_t error){
	SLL_ASSERT(SLL_ERROR_GET_TYPE(error)==SLL_ERROR_FLAG_SLL&&SLL_ERROR_GET_VALUE(error)==SLL_ERROR_STRING);
	return _error_string_pointer_table[SLL_ERROR_GET_EXTRA(error)];
}

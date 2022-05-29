#include <sll.h>



static const sll_internal_function_descriptor_t _internal_function_table[]={
	{
		SLL_CHAR("clib:null"),
		NULL,
		SLL_CHAR("")
	}
};



const sll_internal_function_descriptor_t* clib_internal_function_data=(const sll_internal_function_descriptor_t*)(&_internal_function_table);
const sll_function_index_t clib_internal_function_count=1;


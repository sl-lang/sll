#include <clib/library.h>
#include <sll.h>



static const sll_internal_function_descriptor_t _internal_function_table[]={
	{
		SLL_CHAR("clib:get_sll_library_handle"),
		clib_api_get_sll_library_handle,
		SLL_CHAR("|Q")
	},
	{
		SLL_CHAR("clib:load_library"),
		clib_api_load_library,
		SLL_CHAR("#s|~Q")
	}
};



const sll_internal_function_descriptor_t* clib_internal_function_data=(const sll_internal_function_descriptor_t*)(&_internal_function_table);
const sll_function_index_t clib_internal_function_count=2;


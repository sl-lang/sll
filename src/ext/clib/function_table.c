#include <clib/function.h>
#include <clib/library.h>
#include <sll.h>



static const sll_internal_function_descriptor_t _internal_function_table[]={
	{
		SLL_CHAR("clib:call_function"),
		clib_api_call_function,
		SLL_CHAR("Q#o!|o")
	},
	{
		SLL_CHAR("clib:get_sll_library_handle"),
		clib_api_get_sll_library_handle,
		SLL_CHAR("|Q")
	},
	{
		SLL_CHAR("clib:load_library"),
		clib_api_load_library,
		SLL_CHAR("#s|~Q")
	},
	{
		SLL_CHAR("clib:lookup_symbol"),
		clib_api_lookup_symbol,
		SLL_CHAR("Q#s|Q")
	},
	{
		SLL_CHAR("clib:unload_library"),
		clib_api_lookup_symbol,
		SLL_CHAR("Q|b")
	}
};



const sll_internal_function_descriptor_t* clib_internal_function_data=(const sll_internal_function_descriptor_t*)(&_internal_function_table);
const sll_function_index_t clib_internal_function_count=5;


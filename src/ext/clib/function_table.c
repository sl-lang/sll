#include <clib/common.h>
#include <clib/function.h>
#include <clib/library.h>
#include <sll.h>



static const sll_internal_function_descriptor_t _internal_function_table[]={
	{
		SLL_CHAR("clib:function_call"),
		clib_api_function_call,
		SLL_CHAR("QQ#a|o")
	},
	{
		SLL_CHAR("clib:library_get_loaded_list"),
		clib_api_library_get_loaded_list,
		SLL_CHAR("|a")
	},
	{
		SLL_CHAR("clib:library_get_sll_handle"),
		clib_api_library_get_sll_handle,
		SLL_CHAR("|Q")
	},
	{
		SLL_CHAR("clib:library_load"),
		clib_api_library_load,
		SLL_CHAR("#s|~a")
	},
	{
		SLL_CHAR("clib:library_lookup_symbol"),
		clib_api_library_lookup_symbol,
		SLL_CHAR("Q#s|Q")
	},
	{
		SLL_CHAR("clib:library_unload"),
		clib_api_library_unload,
		SLL_CHAR("Q|Q")
	}
};



static const sll_internal_function_table_descriptor_t _internal_function_table_descriptor_data={
	(const sll_internal_function_descriptor_t*)(&_internal_function_table),
	6
};



__CLIB_EXTERNAL const sll_internal_function_table_descriptor_t* SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR=&_internal_function_table_descriptor_data;

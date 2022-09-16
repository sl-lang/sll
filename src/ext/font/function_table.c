#include <font/common.h>
#include <font/true_type.h>
#include <sll.h>



static const sll_internal_function_descriptor_t _internal_function_table[]={
	{
		SLL_CHAR("font:true_type_create"),
		font_api_true_type_create,
		SLL_CHAR("#s|s")
	},
	{
		SLL_CHAR("font:true_type_find"),
		font_api_true_type_find,
		SLL_CHAR("#s|s")
	}
};



static const sll_internal_function_table_descriptor_t _internal_function_table_descriptor_data={
	(const sll_internal_function_descriptor_t*)(&_internal_function_table),
	2
};



__FONT_EXTERNAL const sll_internal_function_table_descriptor_t* SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR=&_internal_function_table_descriptor_data;

#include <window/common.h>
#include <window/display.h>
#include <window/window.h>
#include <sll.h>



static const sll_internal_function_descriptor_t _internal_function_table[]={
	{
		SLL_CHAR("window:display_enumerate"),
		window_api_display_enumerate,
		SLL_CHAR("|a")
	},
	{
		SLL_CHAR("window:create_window"),
		window_api_create_window,
		SLL_CHAR("iiii#si#s|")
	}
};



static const sll_internal_function_table_descriptor_t _internal_function_table_descriptor_data={
	(const sll_internal_function_descriptor_t*)(&_internal_function_table),
	2
};



__WINDOW_EXTERNAL const sll_internal_function_table_descriptor_t* SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR=&_internal_function_table_descriptor_data;

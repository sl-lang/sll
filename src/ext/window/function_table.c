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
		SLL_CHAR("window:window_create"),
		window_api_window_create,
		SLL_CHAR("DDDDQ|Q")
	},
	{
		SLL_CHAR("window:window_update_visibility"),
		window_api_window_update_visibility,
		SLL_CHAR("Qb|")
	}
};



static const sll_internal_function_table_descriptor_t _internal_function_table_descriptor_data={
	(const sll_internal_function_descriptor_t*)(&_internal_function_table),
	3
};



__WINDOW_EXTERNAL const sll_internal_function_table_descriptor_t* SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR=&_internal_function_table_descriptor_data;

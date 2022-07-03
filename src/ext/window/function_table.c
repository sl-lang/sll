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
		SLL_CHAR("window:window_destroy"),
		window_api_window_destroy,
		SLL_CHAR("Q|")
	},
	{
		SLL_CHAR("window:window_poll_events"),
		window_api_window_poll_events,
		SLL_CHAR("|")
	},
	{
		SLL_CHAR("window:window_set_title"),
		window_api_window_set_title,
		SLL_CHAR("Q#s|")
	},
	{
		SLL_CHAR("window:window_set_visibility"),
		window_api_window_set_visibility,
		SLL_CHAR("Qb|")
	}
};



static const sll_internal_function_table_descriptor_t _internal_function_table_descriptor_data={
	(const sll_internal_function_descriptor_t*)(&_internal_function_table),
	6
};



__WINDOW_EXTERNAL const sll_internal_function_table_descriptor_t* SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR=&_internal_function_table_descriptor_data;

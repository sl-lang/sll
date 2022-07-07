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
		SLL_CHAR("-D-DDDQ|Q")
	},
	{
		SLL_CHAR("window:window_destroy"),
		window_api_window_destroy,
		SLL_CHAR("Q|")
	},
	{
		SLL_CHAR("window:window_poll_events"),
		window_api_window_poll_events,
		SLL_CHAR("b|a")
	},
	{
		SLL_CHAR("window:window_set_cursor"),
		window_api_window_set_cursor,
		SLL_CHAR("QD|")
	},
	{
		SLL_CHAR("window:window_set_geometry"),
		window_api_window_set_geometry,
		SLL_CHAR("Q-D-DDDQ|")
	},
	{
		SLL_CHAR("window:window_set_size_constraints"),
		window_api_window_set_size_constraints,
		SLL_CHAR("QDDDD|")
	},
	{
		SLL_CHAR("window:window_set_state"),
		window_api_window_set_state,
		SLL_CHAR("Qc|")
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
	10
};



__WINDOW_EXTERNAL const sll_internal_function_table_descriptor_t* SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR=&_internal_function_table_descriptor_data;

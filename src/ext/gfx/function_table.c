#include <gfx/color.h>
#include <gfx/context.h>
#include <gfx/vulkan.h>
#include <sll.h>



static const sll_internal_function_descriptor_t _internal_function_table[]={
	{
		SLL_CHAR("gfx:color_set_clear_color"),
		gfx_api_color_set_clear_color,
		SLL_CHAR("Qffff|")
	},
	{
		SLL_CHAR("gfx:context_create"),
		gfx_api_context_create,
		SLL_CHAR("QQb|Q")
	},
	{
		SLL_CHAR("gfx:context_delete"),
		gfx_api_context_delete,
		SLL_CHAR("Q|")
	},
	{
		SLL_CHAR("gfx:context_render"),
		gfx_api_context_render,
		SLL_CHAR("Q|")
	},
	{
		SLL_CHAR("gfx:context_resize"),
		gfx_api_context_resize,
		SLL_CHAR("Q|")
	},
	{
		SLL_CHAR("gfx:vulkan_get_extensions"),
		gfx_api_vulkan_get_extensions,
		SLL_CHAR("|a")
	},
	{
		SLL_CHAR("gfx:vulkan_get_version"),
		gfx_api_vulkan_get_version,
		SLL_CHAR("|a")
	}
};



static const sll_internal_function_table_descriptor_t _internal_function_table_descriptor_data={
	(const sll_internal_function_descriptor_t*)(&_internal_function_table),
	7
};



__GFX_EXTERNAL const sll_internal_function_table_descriptor_t* SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR=&_internal_function_table_descriptor_data;

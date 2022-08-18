#include <gfx/context.h>
#include <gfx/vulkan.h>
#include <sll.h>



static const sll_internal_function_descriptor_t _internal_function_table[]={
	{
		SLL_CHAR("gfx:context_create"),
		gfx_api_context_create,
		SLL_CHAR("Q|Q")
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
	3
};



__GFX_EXTERNAL const sll_internal_function_table_descriptor_t* SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR=&_internal_function_table_descriptor_data;

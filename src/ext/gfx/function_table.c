#include <gfx/buffer.h>
#include <gfx/color.h>
#include <gfx/context.h>
#include <gfx/pipeline.h>
#include <gfx/sampler.h>
#include <gfx/shader.h>
#include <gfx/texture.h>
#include <gfx/vulkan.h>
#include <sll.h>



static const sll_internal_function_descriptor_t _internal_function_table[]={
	{
		SLL_CHAR("gfx:buffer_create"),
		gfx_api_buffer_create,
		SLL_CHAR("QBB|Q")
	},
	{
		SLL_CHAR("gfx:buffer_delete"),
		gfx_api_buffer_delete,
		SLL_CHAR("QQ|")
	},
	{
		SLL_CHAR("gfx:buffer_draw"),
		gfx_api_buffer_draw,
		SLL_CHAR("QQ|")
	},
	{
		SLL_CHAR("gfx:buffer_hint_update_frequency"),
		gfx_api_buffer_hint_update_frequency,
		SLL_CHAR("QQB|")
	},
	{
		SLL_CHAR("gfx:buffer_sync"),
		gfx_api_buffer_sync,
		SLL_CHAR("QQ#a|")
	},
	{
		SLL_CHAR("gfx:buffer_use"),
		gfx_api_buffer_use,
		SLL_CHAR("QQ|")
	},
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
		SLL_CHAR("gfx:pipeline_create"),
		gfx_api_pipeline_create,
		SLL_CHAR("QB#aD#a#aBbB#a|Q")
	},
	{
		SLL_CHAR("gfx:pipeline_delete"),
		gfx_api_pipeline_delete,
		SLL_CHAR("QQ|")
	},
	{
		SLL_CHAR("gfx:pipeline_use"),
		gfx_api_pipeline_use,
		SLL_CHAR("QQ|")
	},
	{
		SLL_CHAR("gfx:sampler_create"),
		gfx_api_sampler_create,
		SLL_CHAR("QBBBBB|Q")
	},
	{
		SLL_CHAR("gfx:sampler_delete"),
		gfx_api_sampler_delete,
		SLL_CHAR("QQ|")
	},
	{
		SLL_CHAR("gfx:shader_create"),
		gfx_api_shader_create,
		SLL_CHAR("Q#sB#s|Q")
	},
	{
		SLL_CHAR("gfx:shader_delete"),
		gfx_api_shader_delete,
		SLL_CHAR("QQ|")
	},
	{
		SLL_CHAR("gfx:texture_create"),
		gfx_api_texture_create,
		SLL_CHAR("Q#aBQ|Q")
	},
	{
		SLL_CHAR("gfx:texture_delete"),
		gfx_api_texture_delete,
		SLL_CHAR("QQ|")
	},
	{
		SLL_CHAR("gfx:texture_sync"),
		gfx_api_texture_sync,
		SLL_CHAR("QQ|")
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
	23
};



__GFX_EXTERNAL const sll_internal_function_table_descriptor_t* SLL_ABI_INTERNAL_FUNCTION_TABLE_DESCRIPTOR=&_internal_function_table_descriptor_data;

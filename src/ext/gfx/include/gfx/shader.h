#ifndef __GFX_SHADER_H__
#define __GFX_SHADER_H__ 1
#include <gfx/common.h>
#include <gfx/context.h>
#include <sll.h>
#include <vulkan/vulkan.h>



#define GFX_SHADER_TYPE_VERTEX 0
#define GFX_SHADER_TYPE_TESSELLETION_CONTROL 1
#define GFX_SHADER_TYPE_TESSELLETION_EVALUATION 2
#define GFX_SHADER_TYPE_GEOMETRY 3
#define GFX_SHADER_TYPE_FRAGMENT 4

#define GFX_SHADER_MAX_NAME_LENGTH 255



typedef uint8_t gfx_shader_type_t;



typedef struct _GFX_SHADER_DATA{
	gfx_shader_type_t type;
	VkShaderModule handle;
	sll_char_t entry_point[GFX_SHADER_MAX_NAME_LENGTH+1];
	VkPipelineShaderStageCreateInfo pipeline_shader_creation_info;
} gfx_shader_data_t;



typedef sll_size_t gfx_shader_t;



void _delete_shader(const gfx_context_data_t* ctx,gfx_shader_data_t* shader_data);



__GFX_API_CALL gfx_shader_t gfx_api_shader_create(gfx_context_t ctx_id,const sll_string_t* bytecode,gfx_shader_type_t type,const sll_string_t* entry_point);



__GFX_API_CALL void gfx_api_shader_delete(gfx_context_t ctx_id,gfx_shader_t shader_id);



#endif

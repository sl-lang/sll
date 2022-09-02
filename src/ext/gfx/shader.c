#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/shader.h>
#include <gfx/util.h>
#include <gfx/vulkan.h>
#include <sll.h>
#include <vulkan/vulkan.h>



void _delete_shader(const gfx_context_data_t* ctx,gfx_shader_data_t* shader){
	ctx->function_table.vkDestroyShaderModule(ctx->device.logical,shader->handle,NULL);
	sll_deallocate(shader);
}



__GFX_API_CALL gfx_shader_t gfx_api_shader_create(gfx_context_t ctx_id,const sll_string_t* bytecode,gfx_shader_stage_t type,const sll_string_t* entry_point){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return 0;
	}
	type^=type&(type-1);
	VkShaderModuleCreateInfo moduleCreateInfo={
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		NULL,
		0,
		bytecode->length,
		(const uint32_t*)(bytecode->data)
	};
	gfx_shader_data_t* shader=sll_allocate(sizeof(gfx_shader_data_t));
	VULKAN_CALL(ctx->function_table.vkCreateShaderModule(ctx->device.logical,&moduleCreateInfo,NULL,&(shader->handle)));
	shader->type=type;
	sll_copy_data(entry_point->data,(entry_point->length+1<GFX_SHADER_MAX_NAME_LENGTH?entry_point->length+1:GFX_SHADER_MAX_NAME_LENGTH),shader->entry_point);
	shader->entry_point[GFX_SHADER_MAX_NAME_LENGTH]=0;
	shader->pipeline_shader_creation_info.sType=VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader->pipeline_shader_creation_info.pNext=NULL;
	shader->pipeline_shader_creation_info.flags=0;
	shader->pipeline_shader_creation_info.stage=0;
	shader->pipeline_shader_creation_info.module=shader->handle;
	shader->pipeline_shader_creation_info.stage=_encode_shader_stages(type);
	shader->pipeline_shader_creation_info.pName=(const char*)(shader->entry_point);
	shader->pipeline_shader_creation_info.pSpecializationInfo=NULL;
	gfx_shader_t out;
	SLL_HANDLE_CONTAINER_ALLOC(&(ctx->child_objects.shaders),&out);
	*(ctx->child_objects.shaders.data+out)=shader;
	return out;
}



__GFX_API_CALL void gfx_api_shader_delete(gfx_context_t ctx_id,gfx_shader_t shader_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_shader_data_t* shader=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.shaders),shader_id);
	if (shader){
		SLL_HANDLE_CONTAINER_DEALLOC(&(ctx->child_objects.shaders),shader_id);
		_delete_shader(ctx,shader);
	}
}

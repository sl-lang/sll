#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/sampler.h>
#include <gfx/vulkan.h>
#include <sll.h>
#include <vulkan/vulkan.h>



void _delete_sampler(const gfx_context_data_t* ctx,gfx_sampler_data_t* sampler){
	ctx->function_table.vkDestroySampler(ctx->device.logical,sampler->handle,NULL);
	sll_deallocate(sampler);
}



__GFX_API_CALL gfx_sampler_t gfx_api_sampler_create(gfx_context_t ctx_id,gfx_samler_address_mode_t address_mode_x,gfx_samler_address_mode_t address_mode_y,gfx_samler_address_mode_t address_mode_z){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return 0;
	}
	gfx_sampler_data_t* sampler=sll_allocate(sizeof(gfx_sampler_data_t));
	VkSamplerCreateInfo sampler_creation_info={
		VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		NULL,
		0,
		VK_FILTER_LINEAR,
		VK_FILTER_LINEAR,
		VK_SAMPLER_MIPMAP_MODE_NEAREST,
		VK_SAMPLER_ADDRESS_MODE_REPEAT,
		VK_SAMPLER_ADDRESS_MODE_REPEAT,
		VK_SAMPLER_ADDRESS_MODE_REPEAT,
		0.0f,
		VK_FALSE,
		0.0f,
		VK_FALSE,
		VK_COMPARE_OP_ALWAYS,
		0.0f,
		0.0f,
		VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		VK_FALSE
	};
	VULKAN_CALL(ctx->function_table.vkCreateSampler(ctx->device.logical,&sampler_creation_info,NULL,&(sampler->handle)));
	gfx_sampler_t out;
	SLL_HANDLE_CONTAINER_ALLOC(&(ctx->samplers),&out);
	*(ctx->samplers.data+out)=sampler;
	return out;
}



__GFX_API_CALL void gfx_api_sampler_delete(gfx_context_t ctx_id,gfx_sampler_t sampler_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_sampler_data_t* sampler=SLL_HANDLE_CONTAINER_GET(&(ctx->samplers),sampler_id);
	if (sampler){
		SLL_HANDLE_CONTAINER_DEALLOC(&(ctx->samplers),sampler_id);
		_delete_sampler(ctx,sampler);
	}
}

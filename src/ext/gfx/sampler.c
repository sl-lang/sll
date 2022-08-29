#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/sampler.h>
#include <gfx/vulkan.h>
#include <sll.h>
#include <vulkan/vulkan.h>



static VkFilter _encode_filter(gfx_sampler_filter_t filter){
	if (filter==GFX_SAMPLER_FILTER_TYPE_LINEAR){
		return VK_FILTER_LINEAR;
	}
	if (filter==GFX_SAMPLER_FILTER_TYPE_CUBIC){
		return VK_FILTER_CUBIC_EXT;
	}
	return VK_FILTER_NEAREST;
}



static VkSamplerAddressMode _encode_address_mode(gfx_sampler_address_mode_t address_mode){
	if (address_mode==(GFX_ADDRESS_MODE_TYPE_REPEAT|GFX_ADDRESS_MODE_FLAG_MIRROR)){
		return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
	}
	if (address_mode==GFX_ADDRESS_MODE_TYPE_CLAMP){
		return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	}
	if (address_mode==(GFX_ADDRESS_MODE_TYPE_CLAMP|GFX_ADDRESS_MODE_FLAG_MIRROR)){
		return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
	}
	return VK_SAMPLER_ADDRESS_MODE_REPEAT;
}



void _delete_sampler(const gfx_context_data_t* ctx,gfx_sampler_data_t* sampler){
	ctx->function_table.vkDestroySampler(ctx->device.logical,sampler->handle,NULL);
	sll_deallocate(sampler);
}



__GFX_API_CALL gfx_sampler_t gfx_api_sampler_create(gfx_context_t ctx_id,gfx_sampler_filter_t upscale_filter,gfx_sampler_filter_t downscale_filter,gfx_sampler_address_mode_t address_mode_x,gfx_sampler_address_mode_t address_mode_y,gfx_sampler_address_mode_t address_mode_z){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return 0;
	}
	gfx_sampler_data_t* sampler=sll_allocate(sizeof(gfx_sampler_data_t));
	VkSamplerCreateInfo sampler_creation_info={
		VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		NULL,
		0,
		_encode_filter(upscale_filter),
		_encode_filter(downscale_filter),
		VK_SAMPLER_MIPMAP_MODE_NEAREST,
		_encode_address_mode(address_mode_x),
		_encode_address_mode(address_mode_y),
		_encode_address_mode(address_mode_z),
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

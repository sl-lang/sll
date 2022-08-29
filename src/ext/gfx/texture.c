#include <gfx/buffer.h>
#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/memory.h>
#include <gfx/texture.h>
#include <gfx/util.h>
#include <gfx/vulkan.h>
#include <sll.h>
#include <stdint.h>
#include <vulkan/vulkan.h>



void _delete_texture(const gfx_context_data_t* ctx,gfx_texture_data_t* texture_data){
	_deallocate_device_memory(ctx,texture_data->memory);
	ctx->function_table.vkDestroyImageView(ctx->device.logical,texture_data->view,NULL);
	ctx->function_table.vkDestroyImage(ctx->device.logical,texture_data->handle,NULL);
	sll_deallocate(texture_data);
}



__GFX_API_CALL gfx_texture_t gfx_api_texture_create(gfx_context_t ctx_id,const sll_array_t* size,gfx_data_format_t format,gfx_buffer_t buffer_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return 0;
	}
	gfx_texture_data_t* texture=sll_allocate(sizeof(gfx_texture_data_t));
	texture->data_buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->buffers),buffer_id);
	texture->format=format;
	texture->width=(uint32_t)(size->data[0]->data.int_);
	texture->height=(uint32_t)((size->length>1?size->data[1]->data.int_:0));
	texture->depth=(uint32_t)((size->length>2?size->data[2]->data.int_:0));
	VkImageType image_type=VK_IMAGE_TYPE_1D;
	VkImageViewType image_view_type=VK_IMAGE_VIEW_TYPE_1D;
	if (size->length==2){
		image_type=VK_IMAGE_TYPE_2D;
		image_view_type=VK_IMAGE_VIEW_TYPE_2D;
	}
	else if (size->length==3){
		image_type=VK_IMAGE_TYPE_3D;
		image_view_type=VK_IMAGE_VIEW_TYPE_3D;
	}
	VkImageCreateInfo image_creation_info={
		VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		NULL,
		0,
		image_type,
		VK_FORMAT_R8G8B8A8_SRGB,
		{
			texture->width,
			(texture->height?texture->height:1),
			(texture->depth?texture->depth:1)
		},
		1,
		1,
		VK_SAMPLE_COUNT_1_BIT,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT|VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		NULL,
		VK_IMAGE_LAYOUT_UNDEFINED
	};
	VULKAN_CALL(ctx->function_table.vkCreateImage(ctx->device.logical,&image_creation_info,NULL,&(texture->handle)));
	VkMemoryRequirements memory_requirements;
	ctx->function_table.vkGetImageMemoryRequirements(ctx->device.logical,texture->handle,&memory_requirements);
	texture->memory=_allocate_device_memory(ctx,memory_requirements.size,_get_memory_type(ctx,memory_requirements.memoryTypeBits,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
	ctx->function_table.vkBindImageMemory(ctx->device.logical,texture->handle,texture->memory,0);
	VkImageViewCreateInfo image_view_creation_info={
		VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		NULL,
		0,
		texture->handle,
		image_view_type,
		VK_FORMAT_R8G8B8A8_SRGB,
		{
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY
		},
		{
			VK_IMAGE_ASPECT_COLOR_BIT,
			0,
			1,
			0,
			1
		}
	};
	VULKAN_CALL(ctx->function_table.vkCreateImageView(ctx->device.logical,&image_view_creation_info,NULL,&(texture->view)));
	gfx_texture_t out;
	SLL_HANDLE_CONTAINER_ALLOC(&(ctx->textures),&out);
	*(ctx->textures.data+out)=texture;
	return out;
}



__GFX_API_CALL void gfx_api_texture_delete(gfx_context_t ctx_id,gfx_texture_t texture_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_texture_data_t* texture=SLL_HANDLE_CONTAINER_GET(&(ctx->textures),texture_id);
	if (texture){
		SLL_HANDLE_CONTAINER_DEALLOC(&(ctx->textures),texture_id);
		_delete_texture(ctx,texture);
	}
}



__GFX_API_CALL void gfx_api_texture_sync(gfx_context_t ctx_id,gfx_texture_t texture_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_texture_data_t* texture=SLL_HANDLE_CONTAINER_GET(&(ctx->textures),texture_id);
	if (!texture){
		return;
	}
}

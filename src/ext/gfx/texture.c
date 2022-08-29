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



void _delete_texture(const gfx_context_data_t* ctx,gfx_texture_data_t* texture){
	_deallocate_device_memory(ctx,texture->memory);
	ctx->function_table.vkDestroyImageView(ctx->device.logical,texture->view,NULL);
	ctx->function_table.vkDestroyImage(ctx->device.logical,texture->handle,NULL);
	sll_deallocate(texture);
}



__GFX_API_CALL gfx_texture_t gfx_api_texture_create(gfx_context_t ctx_id,const sll_array_t* size,gfx_data_format_t format,gfx_buffer_t buffer_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return 0;
	}
	gfx_texture_data_t* texture=sll_allocate(sizeof(gfx_texture_data_t));
	texture->data_buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->buffers),buffer_id);
	texture->format=_encode_data_format(format);
	texture->size.width=(uint32_t)(size->data[0]->data.int_);
	texture->size.height=(uint32_t)((size->length>1?size->data[1]->data.int_:1));
	texture->size.depth=(uint32_t)((size->length>2?size->data[2]->data.int_:1));
	texture->layout=VK_IMAGE_LAYOUT_UNDEFINED;
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
		texture->format,
		texture->size,
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
		texture->format,
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
	VkImageMemoryBarrier image_memory_barrier={
		VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		NULL,
		0,
		VK_ACCESS_TRANSFER_WRITE_BIT,
		texture->layout,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		VK_QUEUE_FAMILY_IGNORED,
		VK_QUEUE_FAMILY_IGNORED,
		texture->handle,
		{
			VK_IMAGE_ASPECT_COLOR_BIT,
			0,
			1,
			0,
			1
		}
	};
	ctx->function_table.vkCmdPipelineBarrier(ctx->buffer_transfer.command_buffer,VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,VK_PIPELINE_STAGE_TRANSFER_BIT,0,0,NULL,0,NULL,1,&image_memory_barrier);
	VkBufferImageCopy buffer_copy_region={
		0,
		0,
		0,
		{
			VK_IMAGE_ASPECT_COLOR_BIT,
			0,
			0,
			1
		},
		{
			0,
			0,
			0,
		},
		texture->size
	};
	ctx->function_table.vkCmdCopyBufferToImage(ctx->buffer_transfer.command_buffer,texture->data_buffer->device.buffer,texture->handle,VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,1,&buffer_copy_region);
	image_memory_barrier.srcAccessMask=VK_ACCESS_TRANSFER_WRITE_BIT;
	image_memory_barrier.dstAccessMask=VK_ACCESS_SHADER_READ_BIT;
	image_memory_barrier.oldLayout=VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	image_memory_barrier.newLayout=VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	ctx->function_table.vkCmdPipelineBarrier(ctx->buffer_transfer.command_buffer,VK_PIPELINE_STAGE_TRANSFER_BIT,VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,0,0,NULL,0,NULL,1,&image_memory_barrier);
	texture->layout=VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
}

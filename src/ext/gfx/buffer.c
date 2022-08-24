#include <gfx/buffer.h>
#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/vulkan.h>
#include <sll.h>
#include <vulkan/vulkan.h>



static void _delete_vulkan_buffers(const gfx_context_data_t* ctx,gfx_buffer_data_t* buffer_data){
	if (buffer_data->flags&GFX_BUFFER_FLAG_HAS_DEVICE_BUFFER){
		ctx->function_table.vkDestroyBuffer(ctx->device.logical,buffer_data->device.buffer,NULL);
		ctx->function_table.vkFreeMemory(ctx->device.logical,buffer_data->device.memory,NULL);
	}
	if (buffer_data->flags&GFX_BUFFER_FLAG_HAS_HOST_BUFFER){
		ctx->function_table.vkDestroyBuffer(ctx->device.logical,buffer_data->host.buffer,NULL);
		ctx->function_table.vkFreeMemory(ctx->device.logical,buffer_data->host.memory,NULL);
	}
}



void _delete_buffer(const gfx_context_data_t* ctx,gfx_buffer_data_t* buffer_data){
	_delete_vulkan_buffers(ctx,buffer_data);
	sll_deallocate(buffer_data);
}



__GFX_API_CALL gfx_buffer_t gfx_api_buffer_create(gfx_context_t ctx_id,gfx_buffer_type_t type){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return 0;
	}
	gfx_buffer_data_t* buffer=sll_allocate(sizeof(gfx_buffer_data_t));
	buffer->type=type;
	buffer->update_frequency_hint=GFX_BUFFER_UPDATE_FREQUENCY_HINT_LOW;
	buffer->flags=0;
	buffer->usage=VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	if (type&GFX_BUFFER_TYPE_VERTEX){
		buffer->usage|=VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	}
	if (type&GFX_BUFFER_TYPE_INDEX){
		buffer->usage|=VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	}
	if (type&GFX_BUFFER_TYPE_UNIFORM){
		buffer->usage|=VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	}
	buffer->last_size=0;
	gfx_buffer_t out;
	SLL_HANDLE_CONTAINER_ALLOC(&(ctx->buffers),&out);
	*(ctx->buffers.data+out)=buffer;
	return out;
}



__GFX_API_CALL void gfx_api_buffer_delete(gfx_context_t ctx_id,gfx_buffer_t buffer_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_buffer_data_t* buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->buffers),buffer_id);
	if (buffer){
		SLL_HANDLE_CONTAINER_DEALLOC(&(ctx->buffers),buffer_id);
		_delete_buffer(ctx,buffer);
	}
}



__GFX_API_CALL void gfx_api_buffer_hint_update_frequency(gfx_context_t ctx_id,gfx_buffer_t buffer_id,gfx_buffer_update_frequency_hint_t hint){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_buffer_data_t* buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->buffers),buffer_id);
	if (!buffer||buffer->update_frequency_hint==hint){
		return;
	}
	buffer->update_frequency_hint=hint;
}



__GFX_API_CALL void gfx_api_buffer_sync(gfx_context_t ctx_id,gfx_buffer_t buffer_id,const sll_array_t* data){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_buffer_data_t* buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->buffers),buffer_id);
	if (!buffer){
		return;
	}
	if (buffer->last_size!=data->length){
		_delete_vulkan_buffers(ctx,buffer);
	}
}

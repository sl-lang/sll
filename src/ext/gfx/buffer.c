#include <gfx/buffer.h>
#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/util.h>
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
	buffer_data->flags=0;
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



__GFX_API_CALL void gfx_api_buffer_draw(gfx_context_t ctx_id,gfx_buffer_t buffer_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_buffer_data_t* buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->buffers),buffer_id);
	if (!buffer){
		return;
	}
	SLL_WARN("Unimplemented!");
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
		buffer->last_size=data->length;
	}
	if (!(buffer->flags&GFX_BUFFER_FLAG_HAS_DEVICE_BUFFER)){
		VkBufferCreateInfo buffer_creation_info={
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			NULL,
			0,
			data->length*sizeof(float),
			buffer->usage,
			0,
			0,
			NULL
		};
		VULKAN_CALL(ctx->function_table.vkCreateBuffer(ctx->device.logical,&buffer_creation_info,NULL,&(buffer->device.buffer)));
		VkMemoryRequirements memory_requirements;
		ctx->function_table.vkGetBufferMemoryRequirements(ctx->device.logical,buffer->device.buffer,&memory_requirements);
		VkMemoryAllocateInfo memory_allocation_info={
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			NULL,
			memory_requirements.size,
			_get_memory_type(ctx,memory_requirements.memoryTypeBits,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		};
		VULKAN_CALL(ctx->function_table.vkAllocateMemory(ctx->device.logical,&memory_allocation_info,NULL,&(buffer->device.memory)));
		VULKAN_CALL(ctx->function_table.vkBindBufferMemory(ctx->device.logical,buffer->device.buffer,buffer->device.memory,0));
	}
	if (!(buffer->flags&GFX_BUFFER_FLAG_HAS_HOST_BUFFER)){
		VkBufferCreateInfo buffer_creation_info={
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			NULL,
			0,
			data->length*sizeof(float),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			0,
			0,
			NULL
		};
		VULKAN_CALL(ctx->function_table.vkCreateBuffer(ctx->device.logical,&buffer_creation_info,NULL,&(buffer->host.buffer)));
		VkMemoryRequirements memory_requirements;
		ctx->function_table.vkGetBufferMemoryRequirements(ctx->device.logical,buffer->host.buffer,&memory_requirements);
		VkMemoryAllocateInfo memory_allocation_info={
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			NULL,
			memory_requirements.size,
			_get_memory_type(ctx,memory_requirements.memoryTypeBits,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
		};
		VULKAN_CALL(ctx->function_table.vkAllocateMemory(ctx->device.logical,&memory_allocation_info,NULL,&(buffer->host.memory)));
		VULKAN_CALL(ctx->function_table.vkBindBufferMemory(ctx->device.logical,buffer->host.buffer,buffer->host.memory,0));
	}
	buffer->flags=GFX_BUFFER_FLAG_HAS_DEVICE_BUFFER|GFX_BUFFER_FLAG_HAS_HOST_BUFFER;
	float* host_buffer_data;
	VULKAN_CALL(ctx->function_table.vkMapMemory(ctx->device.logical,buffer->host.memory,0,data->length*sizeof(float),0,(void**)(&host_buffer_data)));
	for (sll_array_length_t i=0;i<data->length;i++){
		sll_object_t elem=sll_operator_cast(data->data[i],sll_static_int[SLL_OBJECT_TYPE_FLOAT]);
		*host_buffer_data=(float)(elem->data.float_);
		SLL_RELEASE(elem);
		host_buffer_data++;
	}
	VkMappedMemoryRange mapped_memory_range={
		VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
		NULL,
		buffer->host.memory,
		0,
		data->length*sizeof(float)
	};
	VULKAN_CALL(ctx->function_table.vkFlushMappedMemoryRanges(ctx->device.logical,1,&mapped_memory_range));
	ctx->function_table.vkUnmapMemory(ctx->device.logical,buffer->host.memory);
	VkBufferCopy buffer_copy={
		0,
		0,
		data->length*sizeof(float)
	};
	ctx->function_table.vkCmdCopyBuffer(ctx->buffer_transfer.command_buffer,buffer->host.buffer,buffer->device.buffer,1,&buffer_copy);
	ctx->buffer_transfer.has_data=1;
}



__GFX_API_CALL void gfx_api_buffer_use(gfx_context_t ctx_id,gfx_buffer_t buffer_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_buffer_data_t* buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->buffers),buffer_id);
	if (!buffer){
		return;
	}
	SLL_WARN("Unimplemented!");
}

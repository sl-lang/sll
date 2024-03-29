#include <gfx/buffer.h>
#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/memory.h>
#include <gfx/util.h>
#include <gfx/vulkan.h>
#include <sll.h>
#include <vulkan/vulkan.h>



static void _delete_host_buffer(const gfx_context_data_t* ctx,gfx_buffer_data_t* buffer){
	if (buffer->host_buffer_data){
		ctx->function_table.vkUnmapMemory(ctx->device.logical,buffer->host.memory);
		buffer->host_buffer_data=NULL;
	}
	if (buffer->flags&GFX_BUFFER_FLAG_HAS_HOST_BUFFER){
		ctx->function_table.vkDestroyBuffer(ctx->device.logical,buffer->host.buffer,NULL);
		_deallocate_device_memory(ctx,buffer->host.memory);
	}
	buffer->flags&=GFX_BUFFER_FLAG_HAS_DEVICE_BUFFER;
}



static void _delete_both_buffers(const gfx_context_data_t* ctx,gfx_buffer_data_t* buffer){
	_delete_host_buffer(ctx,buffer);
	if (buffer->flags&GFX_BUFFER_FLAG_HAS_DEVICE_BUFFER){
		ctx->function_table.vkDestroyBuffer(ctx->device.logical,buffer->device.buffer,NULL);
		_deallocate_device_memory(ctx,buffer->device.memory);
	}
	buffer->flags=0;
}



void _delete_buffer(const gfx_context_data_t* ctx,gfx_buffer_data_t* buffer){
	_delete_both_buffers(ctx,buffer);
	sll_deallocate(buffer);
}



__GFX_API_CALL gfx_buffer_t gfx_api_buffer_create(gfx_context_t ctx_id,gfx_buffer_type_t type,gfx_buffer_data_type_t data_type){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return 0;
	}
	gfx_buffer_data_t* buffer=sll_allocate(sizeof(gfx_buffer_data_t));
	buffer->type=type;
	buffer->data_type=data_type;
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
	if (type&GFX_BUFFER_TYPE_TEXTURE){
		buffer->usage|=VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	}
	if (data_type==GFX_BUFFER_DATA_TYPE_UINT16){
		buffer->elem_size=2;
	}
	else if (data_type==GFX_BUFFER_DATA_TYPE_UINT32||data_type==GFX_BUFFER_DATA_TYPE_FLOAT32){
		buffer->elem_size=4;
	}
	else{
		buffer->elem_size=8;
	}
	buffer->length=0;
	buffer->size=0;
	buffer->host_buffer_data=NULL;
	gfx_buffer_t out;
	SLL_HANDLE_CONTAINER_ALLOC(&(ctx->child_objects.buffers),&out);
	*(ctx->child_objects.buffers.data+out)=buffer;
	return out;
}



__GFX_API_CALL void gfx_api_buffer_delete(gfx_context_t ctx_id,gfx_buffer_t buffer_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_buffer_data_t* buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.buffers),buffer_id);
	if (buffer){
		SLL_HANDLE_CONTAINER_DEALLOC(&(ctx->child_objects.buffers),buffer_id);
		_delete_buffer(ctx,buffer);
	}
}



__GFX_API_CALL void gfx_api_buffer_draw(gfx_context_t ctx_id,gfx_buffer_t buffer_id,uint32_t count){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_buffer_data_t* buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.buffers),buffer_id);
	if (!buffer||!(buffer->type&GFX_BUFFER_TYPE_INDEX)||!(buffer->flags&GFX_BUFFER_FLAG_HAS_DEVICE_BUFFER)){
		return;
	}
	if (!count||count>buffer->length){
		count=(uint32_t)(buffer->length);
	}
	ctx->function_table.vkCmdDrawIndexed(ctx->frame.command_buffer,count,1,0,0,1);
}



__GFX_API_CALL void gfx_api_buffer_hint_update_frequency(gfx_context_t ctx_id,gfx_buffer_t buffer_id,gfx_buffer_update_frequency_hint_t hint){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_buffer_data_t* buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.buffers),buffer_id);
	if (!buffer||buffer->update_frequency_hint==hint){
		return;
	}
	buffer->update_frequency_hint=hint;
	if (hint==GFX_BUFFER_UPDATE_FREQUENCY_HINT_NEVER){
		_delete_host_buffer(ctx,buffer);
	}
}



__GFX_API_CALL void gfx_api_buffer_sync(gfx_context_t ctx_id,gfx_buffer_t buffer_id,const sll_array_t* data){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_buffer_data_t* buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.buffers),buffer_id);
	if (!buffer){
		return;
	}
	if (buffer->length!=data->length){
		_delete_both_buffers(ctx,buffer);
		buffer->length=data->length;
		buffer->size=(data->length*buffer->elem_size+ctx->device.limits.nonCoherentAtomSize-1)&((~ctx->device.limits.nonCoherentAtomSize)+1);
	}
	if (!(buffer->flags&GFX_BUFFER_FLAG_HAS_DEVICE_BUFFER)){
		VkBufferCreateInfo buffer_creation_info={
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			NULL,
			0,
			buffer->size,
			buffer->usage,
			0,
			0,
			NULL
		};
		VULKAN_CALL(ctx->function_table.vkCreateBuffer(ctx->device.logical,&buffer_creation_info,NULL,&(buffer->device.buffer)));
		VkMemoryRequirements memory_requirements;
		ctx->function_table.vkGetBufferMemoryRequirements(ctx->device.logical,buffer->device.buffer,&memory_requirements);
		buffer->device.memory=_allocate_device_memory(ctx,memory_requirements.size,_get_memory_type(ctx,memory_requirements.memoryTypeBits,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
		VULKAN_CALL(ctx->function_table.vkBindBufferMemory(ctx->device.logical,buffer->device.buffer,buffer->device.memory,0));
	}
	if (!(buffer->flags&GFX_BUFFER_FLAG_HAS_HOST_BUFFER)){
		VkBufferCreateInfo buffer_creation_info={
			VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			NULL,
			0,
			buffer->size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			0,
			0,
			NULL
		};
		VULKAN_CALL(ctx->function_table.vkCreateBuffer(ctx->device.logical,&buffer_creation_info,NULL,&(buffer->host.buffer)));
		VkMemoryRequirements memory_requirements;
		ctx->function_table.vkGetBufferMemoryRequirements(ctx->device.logical,buffer->host.buffer,&memory_requirements);
		buffer->host.memory=_allocate_device_memory(ctx,memory_requirements.size,_get_memory_type(ctx,memory_requirements.memoryTypeBits,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT));
		VULKAN_CALL(ctx->function_table.vkBindBufferMemory(ctx->device.logical,buffer->host.buffer,buffer->host.memory,0));
	}
	buffer->flags=GFX_BUFFER_FLAG_HAS_DEVICE_BUFFER|GFX_BUFFER_FLAG_HAS_HOST_BUFFER;
	if (!buffer->host_buffer_data){
		VULKAN_CALL(ctx->function_table.vkMapMemory(ctx->device.logical,buffer->host.memory,0,buffer->size,0,&(buffer->host_buffer_data)));
	}
	if (buffer->data_type==GFX_BUFFER_DATA_TYPE_UINT8){
		uint8_t* ptr=buffer->host_buffer_data;
		for (sll_array_length_t i=0;i<data->length;i++){
			*(ptr+i)=data->data[i]->data.int_&0xff;
		}
	}
	else if (buffer->data_type==GFX_BUFFER_DATA_TYPE_UINT16){
		uint16_t* ptr=buffer->host_buffer_data;
		for (sll_array_length_t i=0;i<data->length;i++){
			*(ptr+i)=data->data[i]->data.int_&0xffff;
		}
	}
	else if (buffer->data_type==GFX_BUFFER_DATA_TYPE_FLOAT16){
		SLL_WARN("Unimplemented!");
	}
	else if (buffer->data_type==GFX_BUFFER_DATA_TYPE_UINT32){
		uint32_t* ptr=buffer->host_buffer_data;
		for (sll_array_length_t i=0;i<data->length;i++){
			*(ptr+i)=data->data[i]->data.int_&0xffffffff;
		}
	}
	else if (buffer->data_type==GFX_BUFFER_DATA_TYPE_FLOAT32){
		float* ptr=buffer->host_buffer_data;
		for (sll_array_length_t i=0;i<data->length;i++){
			*(ptr+i)=(float)(data->data[i]->data.float_);
		}
	}
	else{
		uint64_t* ptr=buffer->host_buffer_data;
		for (sll_array_length_t i=0;i<data->length;i++){
			*(ptr+i)=data->data[i]->data.int_;
		}
	}
	VkMappedMemoryRange mapped_memory_range={
		VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
		NULL,
		buffer->host.memory,
		0,
		buffer->size
	};
	VULKAN_CALL(ctx->function_table.vkFlushMappedMemoryRanges(ctx->device.logical,1,&mapped_memory_range));
	if (buffer->update_frequency_hint!=GFX_BUFFER_UPDATE_FREQUENCY_HINT_HIGH){
		ctx->function_table.vkUnmapMemory(ctx->device.logical,buffer->host.memory);
		buffer->host_buffer_data=NULL;
	}
	VkBufferCopy buffer_copy={
		0,
		0,
		buffer->size
	};
	ctx->function_table.vkCmdCopyBuffer(ctx->command_buffers.transfer,buffer->host.buffer,buffer->device.buffer,1,&buffer_copy);
	ctx->command_buffers.flags|=GFX_FLAG_HAS_TRANSFER_COMMAND_BUFFER_DATA;
	if (buffer->update_frequency_hint==GFX_BUFFER_UPDATE_FREQUENCY_HINT_NEVER){
		_flush_command_buffers(ctx);
		_delete_host_buffer(ctx,buffer);
	}
}



__GFX_API_CALL void gfx_api_buffer_use(gfx_context_t ctx_id,gfx_buffer_t buffer_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_buffer_data_t* buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.buffers),buffer_id);
	if (!buffer||!(buffer->flags&GFX_BUFFER_FLAG_HAS_DEVICE_BUFFER)){
		return;
	}
	if (buffer->type&GFX_BUFFER_TYPE_VERTEX){
		VkDeviceSize offset=0;
		ctx->function_table.vkCmdBindVertexBuffers(ctx->frame.command_buffer,0,1,&(buffer->device.buffer),&offset);
	}
	else if (buffer->type&GFX_BUFFER_TYPE_INDEX){
		ctx->function_table.vkCmdBindIndexBuffer(ctx->frame.command_buffer,buffer->device.buffer,0,(buffer->data_type==GFX_BUFFER_DATA_TYPE_UINT16?VK_INDEX_TYPE_UINT16:VK_INDEX_TYPE_UINT32));
	}
}

#include <gfx/context.h>
#include <gfx/vulkan.h>
#include <stdint.h>
#include <vulkan/vulkan.h>



VkDeviceMemory _allocate_device_memory(const gfx_context_data_t* ctx,sll_size_t size,uint32_t type){
	VkMemoryAllocateInfo memory_allocation_info={
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		NULL,
		size,
		type
	};
	VkDeviceMemory out;
	VULKAN_CALL(ctx->function_table.vkAllocateMemory(ctx->device.logical,&memory_allocation_info,NULL,&out));
	return out;
}



void _deallocate_device_memory(const gfx_context_data_t* ctx,VkDeviceMemory handle){
	ctx->function_table.vkFreeMemory(ctx->device.logical,handle,NULL);
}

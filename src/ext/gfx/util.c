#include <gfx/context.h>
#include <sll.h>
#include <vulkan/vulkan.h>



uint32_t _get_memory_type(const gfx_context_data_t* ctx,uint32_t mask,VkMemoryPropertyFlagBits properties){
	for (uint32_t i=0;i<ctx->device.memory_properties.memoryTypeCount;i++){
		if ((mask&(1<<i))&&((ctx->device.memory_properties.memoryTypes+i)->propertyFlags&properties)==properties){
			return i;
		}
	}
	SLL_WARN("No memory type found!");
	return 0;
}


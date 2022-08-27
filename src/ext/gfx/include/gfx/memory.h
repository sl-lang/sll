#ifndef __GFX_MEMORY_H__
#define __GFX_MEMORY_H__ 1
#include <gfx/context.h>
#include <stdint.h>
#include <vulkan/vulkan.h>



VkDeviceMemory _allocate_device_memory(const gfx_context_data_t* ctx,sll_size_t size,uint32_t type);



void _deallocate_device_memory(const gfx_context_data_t* ctx,VkDeviceMemory handle);



#endif

#ifndef __GFX_UTIL_H__
#define __GFX_UTIL_H__ 1
#include <gfx/context.h>
#include <vulkan/vulkan.h>



uint32_t _get_memory_type(const gfx_context_data_t* ctx,uint32_t mask,VkMemoryPropertyFlagBits properties);



#endif

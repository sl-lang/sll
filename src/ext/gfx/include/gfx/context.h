#ifndef __GFX_CONTEXT_H__
#define __GFX_CONTEXT_H__ 1
#include <gfx/common.h>
#include <sll.h>
#include <vulkan/vulkan.h>



typedef struct _GFX_CONTEXT_DATA{
	VkInstance instance;
	VkSurfaceKHR surface;
} gfx_context_data_t;



typedef void* gfx_context_t;



__GFX_API_CALL gfx_context_t gfx_api_context_create(void* handle,void* extra_data);



__GFX_API_CALL void gfx_api_context_delete(gfx_context_t ctx);



#endif

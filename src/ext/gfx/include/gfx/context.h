#ifndef __GFX_CONTEXT_H__
#define __GFX_CONTEXT_H__ 1
#include <gfx/common.h>
#include <sll.h>
#include <vulkan/vulkan.h>



typedef struct _GFX_CONTEXT_DATA{
	VkInstance instance;
	VkSurfaceKHR surface;
} gfx_context_data_t;



typedef sll_size_t gfx_context_t;



extern sll_handle_container_t gfx_context_data;



void _delete_context(gfx_context_data_t* ctx);



__GFX_API_CALL gfx_context_t gfx_api_context_create(void* handle,void* extra_data);



__GFX_API_CALL void gfx_api_context_delete(gfx_context_t ctx_id);



#endif

#ifndef __GFX_BUFFER_H__
#define __GFX_BUFFER_H__ 1
#include <gfx/common.h>
#include <gfx/context.h>
#include <sll.h>
#include <stdint.h>
#include <vulkan/vulkan.h>



typedef uint8_t gfx_buffer_type_t;



typedef struct _GFX_BUFFER_DATA{
	VkBuffer handle;
	gfx_buffer_type_t type;
} gfx_buffer_data_t;



typedef sll_size_t gfx_buffer_t;



void _delete_buffer(const gfx_context_data_t* ctx,gfx_buffer_data_t* buffer_data);



__GFX_API_CALL gfx_buffer_t gfx_api_buffer_create(gfx_context_t ctx_id,gfx_buffer_type_t type);



__GFX_API_CALL void gfx_api_buffer_delete(gfx_context_t ctx_id,gfx_buffer_t buffer_id);



#endif

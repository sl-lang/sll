#ifndef __GFX_BUFFER_H__
#define __GFX_BUFFER_H__ 1
#include <gfx/common.h>
#include <gfx/context.h>
#include <sll.h>
#include <stdint.h>
#include <vulkan/vulkan.h>



#define GFX_BUFFER_TYPE_VERTEX 1
#define GFX_BUFFER_TYPE_INDEX 2
#define GFX_BUFFER_TYPE_UNIFORM 4

#define GFX_BUFFER_UPDATE_FREQUENCY_HINT_NEVER 0
#define GFX_BUFFER_UPDATE_FREQUENCY_HINT_LOW 1
#define GFX_BUFFER_UPDATE_FREQUENCY_HINT_MEDIUM 2
#define GFX_BUFFER_UPDATE_FREQUENCY_HINT_HIGH 3



typedef uint8_t gfx_buffer_type_t;



typedef struct _GFX_BUFFER_DATA{
	VkBuffer handle;
	gfx_buffer_type_t type;
} gfx_buffer_data_t;



typedef sll_size_t gfx_buffer_t;



typedef uint8_t gfx_buffer_update_frequency_hint_t;



void _delete_buffer(const gfx_context_data_t* ctx,gfx_buffer_data_t* buffer_data);



__GFX_API_CALL gfx_buffer_t gfx_api_buffer_create(gfx_context_t ctx_id,gfx_buffer_type_t type);



__GFX_API_CALL void gfx_api_buffer_delete(gfx_context_t ctx_id,gfx_buffer_t buffer_id);



__GFX_API_CALL void gfx_api_buffer_hint_update_frequency(gfx_context_t ctx_id,gfx_buffer_t buffer_id,gfx_buffer_update_frequency_hint_t hint);



__GFX_API_CALL void gfx_api_buffer_sync(gfx_context_t ctx_id,gfx_buffer_t buffer_id);



#endif

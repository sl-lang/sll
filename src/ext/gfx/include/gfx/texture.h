#ifndef __GFX_TEXTURE_H__
#define __GFX_TEXTURE_H__ 1
#include <gfx/buffer.h>
#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/util.h>
#include <sll.h>
#include <vulkan/vulkan.h>



typedef struct _GFX_TEXTURE_DATA{
	const gfx_buffer_data_t* data_buffer;
	VkFormat format;
	VkExtent3D size;
	VkImageLayout layout;
	VkImage handle;
	VkImageView view;
	VkDeviceMemory memory;
} gfx_texture_data_t;



typedef sll_size_t gfx_texture_t;



void _delete_texture(const gfx_context_data_t* ctx,gfx_texture_data_t* texture);



__GFX_API_CALL gfx_texture_t gfx_api_texture_create(gfx_context_t ctx_id,const sll_array_t* size,gfx_data_format_t format,gfx_buffer_t buffer_id);



__GFX_API_CALL void gfx_api_texture_delete(gfx_context_t ctx_id,gfx_texture_t texture_id);



__GFX_API_CALL void gfx_api_texture_sync(gfx_context_t ctx_id,gfx_texture_t texture_id);



#endif

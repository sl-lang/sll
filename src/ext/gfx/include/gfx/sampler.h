#ifndef __GFX_SAMPLER_H__
#define __GFX_SAMPLER_H__ 1
#include <gfx/common.h>
#include <gfx/context.h>
#include <sll.h>
#include <vulkan/vulkan.h>



#define GFX_ADDRESS_MODE_TYPE_REPEAT 0
#define GFX_ADDRESS_MODE_TYPE_CLAMP 1
#define GFX_ADDRESS_MODE_TYPE_MASK 1

#define GFX_ADDRESS_MODE_FLAG_MIRROR 2



typedef uint8_t gfx_samler_address_mode_t;



typedef struct _GFX_SAMPLER_DATA{
	VkSampler handle;
} gfx_sampler_data_t;



typedef sll_size_t gfx_sampler_t;



void _delete_sampler(const gfx_context_data_t* ctx,gfx_sampler_data_t* sampler);



__GFX_API_CALL gfx_sampler_t gfx_api_sampler_create(gfx_context_t ctx_id,gfx_samler_address_mode_t address_mode_x,gfx_samler_address_mode_t address_mode_y,gfx_samler_address_mode_t address_mode_z);



__GFX_API_CALL void gfx_api_sampler_delete(gfx_context_t ctx_id,gfx_sampler_t sampler_id);



#endif

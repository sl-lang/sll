#ifndef __GFX_PIPELINE_H__
#define __GFX_PIPELINE_H__ 1
#include <gfx/common.h>
#include <gfx/context.h>
#include <sll.h>
#include <vulkan/vulkan.h>



typedef struct _GFX_PIPELINE_DATA{
	VkPipeline handle;
} gfx_pipeline_data_t;



typedef sll_size_t gfx_pipeline_t;



void _delete_pipeline(const gfx_context_data_t* ctx,gfx_pipeline_data_t* pipeline_data);



__GFX_API_CALL gfx_pipeline_t gfx_api_pipeline_create(gfx_context_t ctx_id);



__GFX_API_CALL void gfx_api_pipeline_delete(gfx_context_t ctx_id,gfx_pipeline_t pipeline_id);



__GFX_API_CALL void gfx_api_pipeline_use(gfx_context_t ctx_id,gfx_pipeline_t pipeline_id);



#endif

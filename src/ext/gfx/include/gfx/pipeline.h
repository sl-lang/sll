#ifndef __GFX_PIPELINE_H__
#define __GFX_PIPELINE_H__ 1
#include <gfx/common.h>
#include <gfx/context.h>
#include <sll.h>
#include <vulkan/vulkan.h>



#define GFX_TOPOLOGY_POINT_LIST 0
#define GFX_TOPOLOGY_LINE_LIST 1
#define GFX_TOPOLOGY_LINE_STRIP 2
#define GFX_TOPOLOGY_TRIANGLE_LIST 3
#define GFX_TOPOLOGY_TRIANGLE_STRIP 4
#define GFX_TOPOLOGY_TRIANGLE_FAN 5
#define GFX_TOPOLOGY_LINE_LIST_WITH_ADJACENCY 6
#define GFX_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY 7
#define GFX_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY 8
#define GFX_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY 9
#define GFX_TOPOLOGY_PATCH_LIST 10

#define GFX_POLYGON_MODE_POINT 0
#define GFX_POLYGON_MODE_LINE 1
#define GFX_POLYGON_MODE_FILL 2

#define GFX_FRONT_FACE_CLOCKWISE 0
#define GFX_FRONT_FACE_COUNTER_CLOCKWISE 1

#define GFX_CULL_MODE_FRONT 1
#define GFX_CULL_MODE_BACK 2



typedef uint8_t gfx_pipeline_topology_t;



typedef uint8_t gfx_pipeline_polygone_mode_t;



typedef sll_bool_t gfx_pipeline_front_face_t;



typedef uint8_t gfx_pipeline_cull_mode_t;



typedef struct _GFX_PIPELINE_DATA{
	VkPipeline handle;
	VkPipelineLayout layout;
	VkDescriptorSet descriptor_set;
	VkDescriptorSetLayout descriptor_set_layout;
	VkDescriptorPool descriptor_pool;
} gfx_pipeline_data_t;



typedef sll_size_t gfx_pipeline_t;



void _delete_pipeline(const gfx_context_data_t* ctx,gfx_pipeline_data_t* pipeline);



__GFX_API_CALL gfx_pipeline_t gfx_api_pipeline_create(gfx_context_t ctx_id,gfx_pipeline_topology_t topology,const sll_array_t* input_attributes,uint32_t stride,const sll_array_t* uniform_buffers,gfx_pipeline_polygone_mode_t polygon_mode,gfx_pipeline_front_face_t front_face,gfx_pipeline_cull_mode_t cull_mode,const sll_array_t* shaders);



__GFX_API_CALL void gfx_api_pipeline_delete(gfx_context_t ctx_id,gfx_pipeline_t pipeline_id);



__GFX_API_CALL void gfx_api_pipeline_use(gfx_context_t ctx_id,gfx_pipeline_t pipeline_id);



#endif

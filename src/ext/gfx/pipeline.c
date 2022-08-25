#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/pipeline.h>
#include <gfx/vulkan.h>
#include <sll.h>
#include <vulkan/vulkan.h>



void _delete_pipeline(const gfx_context_data_t* ctx,gfx_pipeline_data_t* pipeline_data){
	ctx->function_table.vkDestroyPipeline(ctx->device.logical,pipeline_data->handle,NULL);
	sll_deallocate(pipeline_data);
}



__GFX_API_CALL gfx_pipeline_t gfx_api_pipeline_create(gfx_context_t ctx_id,gfx_pipeline_topology_t topology,const sll_array_t* input_attributes,uint32_t stride,gfx_pipeline_polygone_mode_t polygon_mode,gfx_pipeline_front_face_t front_face,gfx_pipeline_cull_mode_t cull_mode,const sll_array_t* shaders){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return 0;
	}
	gfx_pipeline_data_t* pipeline=sll_allocate(sizeof(gfx_pipeline_data_t));
	gfx_pipeline_t out;
	SLL_HANDLE_CONTAINER_ALLOC(&(ctx->pipelines),&out);
	*(ctx->pipelines.data+out)=pipeline;
	return out;
}



__GFX_API_CALL void gfx_api_pipeline_delete(gfx_context_t ctx_id,gfx_pipeline_t pipeline_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_pipeline_data_t* pipeline=SLL_HANDLE_CONTAINER_GET(&(ctx->pipelines),pipeline_id);
	if (pipeline){
		SLL_HANDLE_CONTAINER_DEALLOC(&(ctx->pipelines),pipeline_id);
		_delete_pipeline(ctx,pipeline);
	}
}



__GFX_API_CALL void gfx_api_pipeline_use(gfx_context_t ctx_id,gfx_pipeline_t pipeline_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_pipeline_data_t* pipeline=SLL_HANDLE_CONTAINER_GET(&(ctx->pipelines),pipeline_id);
	if (!pipeline){
		return;
	}
	SLL_WARN("Unimplemented!");
}

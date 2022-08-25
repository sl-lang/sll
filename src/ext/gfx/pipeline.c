#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/pipeline.h>
#include <gfx/shader.h>
#include <gfx/vulkan.h>
#include <sll.h>
#include <stdint.h>
#include <vulkan/vulkan.h>



void _delete_pipeline(const gfx_context_data_t* ctx,gfx_pipeline_data_t* pipeline_data){
	ctx->function_table.vkDestroyPipeline(ctx->device.logical,pipeline_data->handle,NULL);
	ctx->function_table.vkDestroyPipelineLayout(ctx->device.logical,pipeline_data->layout,NULL);
	sll_deallocate(pipeline_data);
}



__GFX_API_CALL gfx_pipeline_t gfx_api_pipeline_create(gfx_context_t ctx_id,gfx_pipeline_topology_t topology,const sll_array_t* input_attributes,uint32_t stride,gfx_pipeline_polygone_mode_t polygon_mode,gfx_pipeline_front_face_t front_face,gfx_pipeline_cull_mode_t cull_mode,const sll_array_t* shaders){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return 0;
	}
	gfx_pipeline_data_t* pipeline=sll_allocate(sizeof(gfx_pipeline_data_t));
	VkPipelineLayoutCreateInfo pipeline_layout_creation_info={
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		NULL,
		0,
		0,
		NULL,
		0,
		NULL
	};
	VULKAN_CALL(ctx->function_table.vkCreatePipelineLayout(ctx->device.logical,&pipeline_layout_creation_info,NULL,&(pipeline->layout)));
	VkPipelineShaderStageCreateInfo* shader_stages=sll_allocate_stack(shaders->length*sizeof(VkPipelineShaderStageCreateInfo));
	for (sll_array_length_t i=0;i<shaders->length;i++){
		sll_object_t elem=shaders->data[i];
		if (elem->type!=SLL_OBJECT_TYPE_INT){
			continue;
		}
		gfx_shader_data_t* shader=SLL_HANDLE_CONTAINER_GET(&(ctx->shaders),(gfx_shader_t)(elem->data.int_));
		if (!shader){
			SLL_WARN("Should never happen!");
			continue;
		}
		*(shader_stages+i)=shader->pipeline_shader_creation_info;
	}
	VkVertexInputBindingDescription vertex_input_binding_descriptor={
		0,
		stride,
		VK_VERTEX_INPUT_RATE_VERTEX
	};
	VkVertexInputAttributeDescription* vertex_input_attributes=sll_allocate_stack(input_attributes->length*sizeof(VkVertexInputAttributeDescription));
	for (sll_array_length_t i=0;i<input_attributes->length;i++){
		sll_object_t input_attribute=input_attributes->data[i];
		if (input_attribute->type!=SLL_OBJECT_TYPE_ARRAY||input_attribute->data.array.length!=4){
			SLL_WARN("Should never happen!");
			continue;
		}
		(vertex_input_attributes+i)->binding=(uint32_t)(input_attribute->data.array.data[0]->data.int_);
		(vertex_input_attributes+i)->location=(uint32_t)(input_attribute->data.array.data[1]->data.int_);
		(vertex_input_attributes+i)->offset=(uint32_t)(input_attribute->data.array.data[2]->data.int_);
		if (input_attribute->data.array.data[3]->data.int_==GFX_INPUT_TYPE_R){
			(vertex_input_attributes+i)->format=VK_FORMAT_R32_SFLOAT;
		}
		else if (input_attribute->data.array.data[3]->data.int_==GFX_INPUT_TYPE_RG){
			(vertex_input_attributes+i)->format=VK_FORMAT_R32G32_SFLOAT;
		}
		else if (input_attribute->data.array.data[3]->data.int_==GFX_INPUT_TYPE_RGB){
			(vertex_input_attributes+i)->format=VK_FORMAT_R32G32B32_SFLOAT;
		}
		else{
			(vertex_input_attributes+i)->format=VK_FORMAT_R32G32B32A32_SFLOAT;
		}
	}
	VkPipelineVertexInputStateCreateInfo vertex_input_state={
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		NULL,
		0,
		1,
		&vertex_input_binding_descriptor,
		2,
		vertex_input_attributes
	};
	VkPrimitiveTopology input_assembler_topology=VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	if (topology==GFX_TOPOLOGY_LINE_LIST){
		input_assembler_topology=VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	}
	else if (topology==GFX_TOPOLOGY_LINE_STRIP){
		input_assembler_topology=VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
	}
	else if (topology==GFX_TOPOLOGY_TRIANGLE_LIST){
		input_assembler_topology=VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	}
	else if (topology==GFX_TOPOLOGY_TRIANGLE_STRIP){
		input_assembler_topology=VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	}
	else if (topology==GFX_TOPOLOGY_TRIANGLE_FAN){
		input_assembler_topology=VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
	}
	else if (topology==GFX_TOPOLOGY_LINE_LIST_WITH_ADJACENCY){
		input_assembler_topology=VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;
	}
	else if (topology==GFX_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY){
		input_assembler_topology=VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;
	}
	else if (topology==GFX_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY){
		input_assembler_topology=VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;
	}
	else if (topology==GFX_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY){
		input_assembler_topology=VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;
	}
	else if (topology==GFX_TOPOLOGY_PATCH_LIST){
		input_assembler_topology=VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
	}
	VkPipelineInputAssemblyStateCreateInfo input_assembly_state={
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		NULL,
		0,
		input_assembler_topology,
		VK_FALSE
	};
	VkPipelineViewportStateCreateInfo viewport_state={
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		NULL,
		0,
		1,
		NULL,
		1,
		NULL
	};
	VkPolygonMode rasterization_polygon_mode=VK_POLYGON_MODE_POINT;
	if (polygon_mode==GFX_POLYGON_MODE_LINE){
		rasterization_polygon_mode=VK_POLYGON_MODE_LINE;
	}
	else if (polygon_mode==GFX_POLYGON_MODE_FILL){
		rasterization_polygon_mode=VK_POLYGON_MODE_FILL;
	}
	VkPipelineRasterizationStateCreateInfo rasterization_state={
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		NULL,
		0,
		VK_FALSE,
		VK_FALSE,
		rasterization_polygon_mode,
		((cull_mode&GFX_CULL_MODE_FRONT)?VK_CULL_MODE_FRONT_BIT:0)|((cull_mode&GFX_CULL_MODE_BACK)?VK_CULL_MODE_BACK_BIT:0),
		(front_face==GFX_FRONT_FACE_CLOCKWISE?VK_FRONT_FACE_CLOCKWISE:VK_FRONT_FACE_COUNTER_CLOCKWISE),
		VK_FALSE,
		0,
		0,
		0,
		1.0f
	};
	VkPipelineMultisampleStateCreateInfo multisample_state={
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		NULL,
		0,
		VK_SAMPLE_COUNT_1_BIT,
		VK_FALSE,
		0,
		NULL,
		VK_FALSE,
		VK_FALSE
	};
	VkPipelineDepthStencilStateCreateInfo depth_stencil_state={
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
		NULL,
		0,
		VK_TRUE,
		VK_TRUE,
		VK_COMPARE_OP_LESS_OR_EQUAL,
		VK_FALSE,
		VK_FALSE,
		{
			VK_STENCIL_OP_KEEP,
			VK_STENCIL_OP_KEEP,
			VK_STENCIL_OP_KEEP,
			VK_COMPARE_OP_ALWAYS,
			0,
			0,
			0
		},
		{
			VK_STENCIL_OP_KEEP,
			VK_STENCIL_OP_KEEP,
			VK_STENCIL_OP_KEEP,
			VK_COMPARE_OP_ALWAYS,
			0,
			0,
			0
		},
		0,
		0
	};
	VkPipelineColorBlendAttachmentState color_blend_attachment_state={
		VK_FALSE,
		VK_BLEND_FACTOR_ZERO,
		VK_BLEND_FACTOR_ZERO,
		VK_BLEND_OP_ADD,
		VK_BLEND_FACTOR_ZERO,
		VK_BLEND_FACTOR_ZERO,
		VK_BLEND_OP_ADD,
		0x0f
	};
	VkPipelineColorBlendStateCreateInfo color_blend_state={
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		NULL,
		0,
		VK_FALSE,
		VK_LOGIC_OP_OR,
		1,
		&color_blend_attachment_state,
		{
			0,
			0,
			0,
			0
		}
	};
	VkDynamicState dynamic_states[2]={
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamic_state={
		VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		NULL,
		0,
		2,
		dynamic_states
	};
	VkGraphicsPipelineCreateInfo pipeline_create_info={
		VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		NULL,
		0,
		shaders->length,
		shader_stages,
		&vertex_input_state,
		&input_assembly_state,
		NULL,
		&viewport_state,
		&rasterization_state,
		&multisample_state,
		&depth_stencil_state,
		&color_blend_state,
		&dynamic_state,
		pipeline->layout,
		ctx->pipeline.render_pass,
		0,
		NULL,
		0
	};
	VULKAN_CALL(ctx->function_table.vkCreateGraphicsPipelines(ctx->device.logical,ctx->pipeline.cache,1,&pipeline_create_info,NULL,&(pipeline->handle)));
	sll_deallocate(vertex_input_attributes);
	sll_deallocate(shader_stages);
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
	ctx->function_table.vkCmdBindPipeline(ctx->frame.command_buffer,VK_PIPELINE_BIND_POINT_GRAPHICS,pipeline->handle);
}

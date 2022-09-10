#include <gfx/buffer.h>
#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/pipeline.h>
#include <gfx/sampler.h>
#include <gfx/shader.h>
#include <gfx/texture.h>
#include <gfx/util.h>
#include <gfx/vulkan.h>
#include <sll.h>
#include <stdint.h>
#include <vulkan/vulkan.h>



void _delete_pipeline(const gfx_context_data_t* ctx,gfx_pipeline_data_t* pipeline){
	ctx->function_table.vkDestroyPipeline(ctx->device.logical,pipeline->handle,NULL);
	ctx->function_table.vkDestroyPipelineLayout(ctx->device.logical,pipeline->layout,NULL);
	ctx->function_table.vkFreeDescriptorSets(ctx->device.logical,pipeline->descriptor_pool,1,&(pipeline->descriptor_set));
	ctx->function_table.vkDestroyDescriptorSetLayout(ctx->device.logical,pipeline->descriptor_set_layout,NULL);
	ctx->function_table.vkDestroyDescriptorPool(ctx->device.logical,pipeline->descriptor_pool,NULL);
	sll_deallocate(pipeline);
}



__GFX_API_CALL gfx_pipeline_t gfx_api_pipeline_create(gfx_context_t ctx_id,gfx_pipeline_topology_t topology,const sll_array_t* input_attributes,uint32_t stride,const sll_array_t* uniform_buffers,const sll_array_t* samplers,gfx_pipeline_polygone_mode_t polygon_mode,gfx_pipeline_front_face_t front_face,gfx_pipeline_cull_mode_t cull_mode,const sll_array_t* shaders,uint32_t push_constant_size,gfx_shader_stage_t push_constant_stage){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return 0;
	}
	gfx_pipeline_data_t* pipeline=sll_allocate(sizeof(gfx_pipeline_data_t));
	pipeline->push_constant_stage=_encode_shader_stages(push_constant_stage);
	VkPushConstantRange push_constant_range={
		pipeline->push_constant_stage,
		0,
		(push_constant_size+3)&0xfffffffc
	};
	VkDescriptorSetLayoutBinding* layout_bindings=sll_allocate_stack((uniform_buffers->length+samplers->length)*sizeof(VkDescriptorSetLayoutBinding));
	uint32_t uniform_buffer_descriptor_count=0;
	uint32_t sampler_descriptor_count=0;
	for (sll_array_length_t i=0;i<uniform_buffers->length+samplers->length;i++){
		sll_object_t elem=(i>=uniform_buffers->length?samplers->data[i-uniform_buffers->length]:uniform_buffers->data[i]);
		uint32_t descriptor_count=(elem->data.array.length<4&&elem->data.array.data[2]->data.int_?elem->data.array.data[2]->data.int_:1);
		if (i<uniform_buffers->length){
			uniform_buffer_descriptor_count+=descriptor_count;
		}
		else{
			sampler_descriptor_count+=descriptor_count;
		}
		(layout_bindings+i)->binding=(uint32_t)(elem->data.array.data[0]->data.int_);
		(layout_bindings+i)->descriptorType=(i>=uniform_buffers->length?VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		(layout_bindings+i)->descriptorCount=descriptor_count;
		(layout_bindings+i)->stageFlags=_encode_shader_stages((gfx_shader_stage_t)(elem->data.array.data[1]->data.int_));
		(layout_bindings+i)->pImmutableSamplers=NULL;
	}
	VkDescriptorSetLayoutCreateInfo descriptor_layout={
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		NULL,
		0,
		uniform_buffers->length+samplers->length,
		layout_bindings
	};
	VULKAN_CALL(ctx->function_table.vkCreateDescriptorSetLayout(ctx->device.logical,&descriptor_layout,NULL,&(pipeline->descriptor_set_layout)));
	sll_deallocate(layout_bindings);
	VkPipelineLayoutCreateInfo pipeline_layout_creation_info={
		VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		NULL,
		0,
		1,
		&(pipeline->descriptor_set_layout),
		1,
		&push_constant_range
	};
	VULKAN_CALL(ctx->function_table.vkCreatePipelineLayout(ctx->device.logical,&pipeline_layout_creation_info,NULL,&(pipeline->layout)));
	VkPipelineShaderStageCreateInfo* shader_stages=sll_allocate_stack(shaders->length*sizeof(VkPipelineShaderStageCreateInfo));
	for (sll_array_length_t i=0;i<shaders->length;i++){
		sll_object_t elem=shaders->data[i];
		if (elem->type!=SLL_OBJECT_TYPE_INT){
			continue;
		}
		gfx_shader_data_t* shader=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.shaders),(gfx_shader_t)(elem->data.int_));
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
		(vertex_input_attributes+i)->format=_encode_data_format((gfx_data_format_t)(input_attribute->data.array.data[3]->data.int_));
	}
	VkPipelineVertexInputStateCreateInfo vertex_input_state={
		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		NULL,
		0,
		1,
		&vertex_input_binding_descriptor,
		input_attributes->length,
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
	VkDescriptorPoolSize descriptor_pool_sizes[2];
	uint32_t descriptor_pool_size_count=0;
	if (uniform_buffers->length){
		(descriptor_pool_sizes+descriptor_pool_size_count)->type=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		(descriptor_pool_sizes+descriptor_pool_size_count)->descriptorCount=uniform_buffer_descriptor_count;
		descriptor_pool_size_count++;
	}
	if (samplers->length){
		(descriptor_pool_sizes+descriptor_pool_size_count)->type=VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		(descriptor_pool_sizes+descriptor_pool_size_count)->descriptorCount=sampler_descriptor_count;
		descriptor_pool_size_count++;
	}
	VkDescriptorPoolCreateInfo descriptor_pool_creation_info={
		VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		NULL,
		VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
		1,
		descriptor_pool_size_count,
		descriptor_pool_sizes
	};
	VULKAN_CALL(ctx->function_table.vkCreateDescriptorPool(ctx->device.logical,&descriptor_pool_creation_info,NULL,&(pipeline->descriptor_pool)));
	VkDescriptorSetAllocateInfo allocInfo={
		VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		NULL,
		pipeline->descriptor_pool,
		1,
		&(pipeline->descriptor_set_layout)
	};
	VULKAN_CALL(ctx->function_table.vkAllocateDescriptorSets(ctx->device.logical,&allocInfo,&(pipeline->descriptor_set)));
	VkDescriptorBufferInfo* buffer_descriptors=sll_allocate_stack(uniform_buffers->length*sizeof(VkDescriptorBufferInfo));
	VkDescriptorImageInfo* image_descriptors=sll_allocate_stack(samplers->length*sizeof(VkDescriptorImageInfo));
	VkWriteDescriptorSet* write_descriptor_sets=sll_allocate_stack((uniform_buffers->length+samplers->length)*sizeof(VkWriteDescriptorSet));
	VkWriteDescriptorSet* write_descriptor_set=write_descriptor_sets;
	sll_array_length_t count=0;
	for (sll_array_length_t i=0;i<uniform_buffers->length;i++){
		sll_object_t elem=uniform_buffers->data[i];
		if (elem->data.array.length!=4){
			continue;
		}
		const gfx_buffer_data_t* buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.buffers),(gfx_buffer_t)(elem->data.array.data[2]->data.int_));
		if (!buffer){
			SLL_WARN("Should never happen!");
			continue;
		}
		(buffer_descriptors+i)->buffer=buffer->device.buffer;
		(buffer_descriptors+i)->offset=0;
		(buffer_descriptors+i)->range=buffer->size;
		write_descriptor_set->sType=VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_descriptor_set->pNext=NULL;
		write_descriptor_set->dstSet=pipeline->descriptor_set;
		write_descriptor_set->dstBinding=(uint32_t)(elem->data.array.data[0]->data.int_);
		write_descriptor_set->dstArrayElement=0;
		write_descriptor_set->descriptorCount=1;
		write_descriptor_set->descriptorType=VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		write_descriptor_set->pBufferInfo=buffer_descriptors+i;
		write_descriptor_set++;
		count++;
	}
	for (sll_array_length_t i=0;i<samplers->length;i++){
		sll_object_t elem=samplers->data[i];
		if (elem->data.array.length!=4){
			continue;
		}
		const gfx_texture_data_t* texture=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.textures),(gfx_texture_t)(elem->data.array.data[2]->data.int_));
		const gfx_sampler_data_t* sampler=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.samplers),(gfx_sampler_t)(elem->data.array.data[3]->data.int_));
		if (!texture||!sampler){
			SLL_WARN("Should never happen!");
			continue;
		}
		(image_descriptors+i)->sampler=sampler->handle;
		(image_descriptors+i)->imageView=texture->view;
		(image_descriptors+i)->imageLayout=texture->layout;
		write_descriptor_set->sType=VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write_descriptor_set->pNext=NULL;
		write_descriptor_set->dstSet=pipeline->descriptor_set;
		write_descriptor_set->dstBinding=(uint32_t)(elem->data.array.data[0]->data.int_);
		write_descriptor_set->dstArrayElement=0;
		write_descriptor_set->descriptorCount=1;
		write_descriptor_set->descriptorType=VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write_descriptor_set->pImageInfo=image_descriptors+i;
		write_descriptor_set++;
		count++;
	}
	ctx->function_table.vkUpdateDescriptorSets(ctx->device.logical,count,write_descriptor_sets,0,NULL);
	sll_deallocate(write_descriptor_sets);
	sll_deallocate(image_descriptors);
	sll_deallocate(buffer_descriptors);
	gfx_pipeline_t out;
	SLL_HANDLE_CONTAINER_ALLOC(&(ctx->child_objects.pipelines),&out);
	*(ctx->child_objects.pipelines.data+out)=pipeline;
	return out;
}



__GFX_API_CALL void gfx_api_pipeline_delete(gfx_context_t ctx_id,gfx_pipeline_t pipeline_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_pipeline_data_t* pipeline=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.pipelines),pipeline_id);
	if (pipeline){
		SLL_HANDLE_CONTAINER_DEALLOC(&(ctx->child_objects.pipelines),pipeline_id);
		_delete_pipeline(ctx,pipeline);
	}
}



__GFX_API_CALL void gfx_api_pipeline_update_descriptor(gfx_context_t ctx_id,gfx_pipeline_t pipeline_id,gfx_descriptor_type_t type,uint32_t binding,gfx_shader_stage_t stage,const sll_array_t* data,uint32_t index,sll_bool_t immediate){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_pipeline_data_t* pipeline=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.pipelines),pipeline_id);
	if (!pipeline){
		return;
	}
	VkWriteDescriptorSet descriptor={
		VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		NULL,
		pipeline->descriptor_set,
		binding,
		index,
		1
	};
	void* ptr=NULL;
	if (type==GFX_DESCRIPTOR_TYPE_UNIFORM_BUFFER){
		SLL_WARN("Unimplemented!");
	}
	else{
		VkDescriptorImageInfo* extra_data=sll_allocate(sizeof(VkDescriptorImageInfo));
		descriptor.descriptorType=VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptor.pImageInfo=extra_data;
		const gfx_texture_data_t* texture=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.textures),(gfx_texture_t)(data->data[0]->data.int_));
		const gfx_sampler_data_t* sampler=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.samplers),(gfx_sampler_t)(data->data[1]->data.int_));
		if (!texture||!sampler){
			SLL_WARN("Should never happen!");
			return;
		}
		extra_data->sampler=sampler->handle;
		extra_data->imageView=texture->view;
		extra_data->imageLayout=texture->layout;
		ptr=extra_data;
	}
	if (immediate){
		ctx->function_table.vkUpdateDescriptorSets(ctx->device.logical,1,&descriptor,0,NULL);
		sll_deallocate(ptr);
		return;
	}
	ctx->write_descriptors.count++;
	ctx->write_descriptors.data=sll_reallocate(ctx->write_descriptors.data,ctx->write_descriptors.count*sizeof(VkWriteDescriptorSet));
	*(ctx->write_descriptors.data+ctx->write_descriptors.count-1)=descriptor;
	ctx->write_descriptors.pointers=sll_reallocate(ctx->write_descriptors.pointers,ctx->write_descriptors.count*sizeof(void*));
	*(ctx->write_descriptors.pointers+ctx->write_descriptors.count-1)=ptr;
}



__GFX_API_CALL void gfx_api_pipeline_update_push_constant(gfx_context_t ctx_id,gfx_pipeline_t pipeline_id,const sll_string_t* data){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_pipeline_data_t* pipeline=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.pipelines),pipeline_id);
	if (!pipeline){
		return;
	}
	ctx->function_table.vkCmdPushConstants(ctx->frame.command_buffer,pipeline->layout,pipeline->push_constant_stage,0,(data->length+3)&0xfffffffc,data->data);
}



__GFX_API_CALL void gfx_api_pipeline_use(gfx_context_t ctx_id,gfx_pipeline_t pipeline_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_pipeline_data_t* pipeline=SLL_HANDLE_CONTAINER_GET(&(ctx->child_objects.pipelines),pipeline_id);
	if (!pipeline){
		return;
	}
	ctx->function_table.vkCmdBindDescriptorSets(ctx->frame.command_buffer,VK_PIPELINE_BIND_POINT_GRAPHICS,pipeline->layout,0,1,&(pipeline->descriptor_set),0,NULL);
	ctx->function_table.vkCmdBindPipeline(ctx->frame.command_buffer,VK_PIPELINE_BIND_POINT_GRAPHICS,pipeline->handle);
}

#include <gfx/buffer.h>
#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/pipeline.h>
#include <gfx/shader.h>
#include <gfx/util.h>
#include <gfx/vulkan.h>
#include <gfx/vulkan_functions.h>
#include <sll.h>
#include <vulkan/vulkan.h>



static const float _default_queue_priority=0.0f;



sll_handle_container_t gfx_context_data;



static VKAPI_ATTR VkBool32 _debug_messenger_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,VkDebugUtilsMessageTypeFlagsEXT type,const VkDebugUtilsMessengerCallbackDataEXT* data,gfx_context_data_t* ctx){
	if (severity>VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT){
		SLL_WARN("%s",data->pMessage);
	}
	return VK_FALSE;
}



static void _release_swapchain(gfx_context_data_t* ctx){
	ctx->function_table.vkDeviceWaitIdle(ctx->device.logical);
	for (uint32_t i=0;i<ctx->swapchain.image_count;i++){
		ctx->function_table.vkDestroyImageView(ctx->device.logical,ctx->swapchain.image_views[i],NULL);
		ctx->function_table.vkDestroyFence(ctx->device.logical,ctx->sync.fences[i],NULL);
		ctx->function_table.vkDestroyFramebuffer(ctx->device.logical,ctx->swapchain.frame_buffers[i],NULL);
	}
	ctx->function_table.vkDestroyImage(ctx->device.logical,ctx->depth_stensil.image,NULL);
	ctx->function_table.vkDestroyImageView(ctx->device.logical,ctx->depth_stensil.image_view,NULL);
	ctx->function_table.vkFreeMemory(ctx->device.logical,ctx->depth_stensil.memory,NULL);
}



static void _create_swapchain(gfx_context_data_t* ctx){
	VkSurfaceCapabilitiesKHR surface_caps;
	VULKAN_CALL(ctx->function_table.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ctx->device.physical,ctx->surface.handle,&surface_caps));
	ctx->surface.width=surface_caps.currentExtent.width;
	ctx->surface.height=surface_caps.currentExtent.height;
	VkSwapchainCreateInfoKHR swapchain_creation_info={
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		NULL,
		0,
		ctx->surface.handle,
		surface_caps.minImageCount+1,
		ctx->surface.color_format,
		ctx->surface.color_space,
		{
			ctx->surface.width,
			ctx->surface.height
		},
		1,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT|(surface_caps.supportedUsageFlags&(VK_IMAGE_USAGE_TRANSFER_SRC_BIT|VK_IMAGE_USAGE_TRANSFER_DST_BIT)),
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		NULL,
		((surface_caps.supportedTransforms&VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)?VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR:surface_caps.supportedTransforms),
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		VK_PRESENT_MODE_FIFO_KHR,
		VK_TRUE,
		ctx->swapchain.handle
	};
	VkSwapchainKHR swapchain;
	VULKAN_CALL(ctx->function_table.vkCreateSwapchainKHR(ctx->device.logical,&swapchain_creation_info,NULL,&swapchain));
	if (ctx->swapchain.handle!=VK_NULL_HANDLE){
		_release_swapchain(ctx);
		ctx->function_table.vkDestroySwapchainKHR(ctx->device.logical,ctx->swapchain.handle,NULL);
	}
	ctx->swapchain.handle=swapchain;
	VULKAN_CALL(ctx->function_table.vkGetSwapchainImagesKHR(ctx->device.logical,ctx->swapchain.handle,&(ctx->swapchain.image_count),NULL));
	ctx->swapchain.images=sll_reallocate(ctx->swapchain.images,ctx->swapchain.image_count*sizeof(VkImage));
	VULKAN_CALL(ctx->function_table.vkGetSwapchainImagesKHR(ctx->device.logical,ctx->swapchain.handle,&(ctx->swapchain.image_count),ctx->swapchain.images));
	ctx->command.buffers=sll_reallocate(ctx->command.buffers,ctx->swapchain.image_count*sizeof(VkCommandBuffer));
	VkCommandBufferAllocateInfo command_buffer_allocation_info={
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		NULL,
		ctx->command.pool,
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		ctx->swapchain.image_count
	};
	VULKAN_CALL(ctx->function_table.vkAllocateCommandBuffers(ctx->device.logical,&command_buffer_allocation_info,ctx->command.buffers));
	VkImageCreateInfo depth_stensil_image_creation_info={
		VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		NULL,
		0,
		VK_IMAGE_TYPE_2D,
		VK_FORMAT_D32_SFLOAT,
		{
			ctx->surface.width,
			ctx->surface.height,
			1
		},
		1,
		1,
		VK_SAMPLE_COUNT_1_BIT,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		0,
		1,
		NULL,
		0
	};
	VULKAN_CALL(ctx->function_table.vkCreateImage(ctx->device.logical,&depth_stensil_image_creation_info,NULL,&(ctx->depth_stensil.image)));
	VkMemoryRequirements mem_requirements;
	ctx->function_table.vkGetImageMemoryRequirements(ctx->device.logical,ctx->depth_stensil.image,&mem_requirements);
	VkMemoryAllocateInfo mem_alloc_info={
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		NULL,
		mem_requirements.size,
		_get_memory_type(ctx,mem_requirements.memoryTypeBits,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	};
	VULKAN_CALL(ctx->function_table.vkAllocateMemory(ctx->device.logical,&mem_alloc_info,NULL,&(ctx->depth_stensil.memory)));
	VULKAN_CALL(ctx->function_table.vkBindImageMemory(ctx->device.logical,ctx->depth_stensil.image,ctx->depth_stensil.memory,0));
	VkImageViewCreateInfo deptch_stensil_image_view_creation_info={
		VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		NULL,
		0,
		ctx->depth_stensil.image,
		VK_IMAGE_VIEW_TYPE_2D,
		VK_FORMAT_D32_SFLOAT,
		.subresourceRange={
			VK_IMAGE_ASPECT_DEPTH_BIT,
			0,
			1,
			0,
			1
		}
	};
	VULKAN_CALL(ctx->function_table.vkCreateImageView(ctx->device.logical,&deptch_stensil_image_view_creation_info,NULL,&(ctx->depth_stensil.image_view)));
	ctx->swapchain.image_views=sll_reallocate(ctx->swapchain.image_views,ctx->swapchain.image_count*sizeof(VkImageView));
	VkImageViewCreateInfo image_view_creation_info={
		VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		NULL,
		0,
		VK_NULL_HANDLE,
		VK_IMAGE_VIEW_TYPE_2D,
		ctx->surface.color_format,
		{
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY,
			VK_COMPONENT_SWIZZLE_IDENTITY
		},
		{
			VK_IMAGE_ASPECT_COLOR_BIT,
			0,
			1,
			0,
			1
		}
	};
	ctx->sync.fences=sll_reallocate(ctx->sync.fences,ctx->swapchain.image_count*sizeof(VkFence));
	VkFenceCreateInfo fence_creation_info={
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		NULL,
		VK_FENCE_CREATE_SIGNALED_BIT
	};
	ctx->swapchain.frame_buffers=sll_reallocate(ctx->swapchain.frame_buffers,ctx->swapchain.image_count*sizeof(VkFramebuffer));
	VkImageView frame_buffer_attachments[2]={
		VK_NULL_HANDLE,
		ctx->depth_stensil.image_view
	};
	VkFramebufferCreateInfo frame_buffer_creation_info={
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		NULL,
		0,
		ctx->pipeline.render_pass,
		2,
		frame_buffer_attachments,
		ctx->surface.width,
		ctx->surface.height,
		1
	};
	for (uint32_t i=0;i<ctx->swapchain.image_count;i++){
		image_view_creation_info.image=ctx->swapchain.images[i];
		VULKAN_CALL(ctx->function_table.vkCreateImageView(ctx->device.logical,&image_view_creation_info,NULL,ctx->swapchain.image_views+i));
		VULKAN_CALL(ctx->function_table.vkCreateFence(ctx->device.logical,&fence_creation_info,NULL,ctx->sync.fences+i));
		frame_buffer_attachments[0]=ctx->swapchain.image_views[i];
		VULKAN_CALL(ctx->function_table.vkCreateFramebuffer(ctx->device.logical,&frame_buffer_creation_info,NULL,ctx->swapchain.frame_buffers+i));
	}
}



static void _begin_frame(gfx_context_data_t* ctx){
	if (ctx->buffer_transfer.has_data){
		VkCommandBufferBeginInfo command_buffer_begin_info={
			VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			NULL,
			0,
			NULL
		};
		VULKAN_CALL(ctx->function_table.vkBeginCommandBuffer(ctx->buffer_transfer.command_buffer,&command_buffer_begin_info));
		ctx->buffer_transfer.has_data=0;
	}
	VkResult err=ctx->function_table.vkAcquireNextImageKHR(ctx->device.logical,ctx->swapchain.handle,UINT64_MAX,ctx->sync.present_semaphore,NULL,&(ctx->frame.image_index));
	if (err!=VK_SUBOPTIMAL_KHR){
		VULKAN_CALL(err);
	}
	ctx->frame.command_buffer=ctx->command.buffers[ctx->frame.image_index];
	VULKAN_CALL(ctx->function_table.vkWaitForFences(ctx->device.logical,1,ctx->sync.fences+ctx->frame.image_index,VK_TRUE,UINT64_MAX));
	VULKAN_CALL(ctx->function_table.vkResetFences(ctx->device.logical,1,ctx->sync.fences+ctx->frame.image_index));
	VkCommandBufferBeginInfo begin_info={
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		NULL,
		VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
		NULL
	};
	VULKAN_CALL(ctx->function_table.vkBeginCommandBuffer(ctx->frame.command_buffer,&begin_info));
	VkClearValue clear_values[2]={
		{
			.color={
				.float32={
					ctx->surface.clear_color[0],
					ctx->surface.clear_color[1],
					ctx->surface.clear_color[2],
					ctx->surface.clear_color[3]
				}
			}
		},
		{
			.depthStencil={
				1.0f,
				0
			}
		}
	};
	VkRenderPassBeginInfo render_pass_info={
		VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		NULL,
		ctx->pipeline.render_pass,
		ctx->swapchain.frame_buffers[ctx->frame.image_index],
		{
			{
				0,
				0
			},
			{
				ctx->surface.width,
				ctx->surface.height
			}
		},
		2,
		clear_values
	};
	ctx->function_table.vkCmdBeginRenderPass(ctx->frame.command_buffer,&render_pass_info,VK_SUBPASS_CONTENTS_INLINE);
}



static void _end_frame(gfx_context_data_t* ctx){
	if (ctx->buffer_transfer.has_data){
		VULKAN_CALL(ctx->function_table.vkEndCommandBuffer(ctx->buffer_transfer.command_buffer));
		VkSubmitInfo submit_info={
			VK_STRUCTURE_TYPE_SUBMIT_INFO,
			NULL,
			0,
			NULL,
			NULL,
			1,
			&(ctx->buffer_transfer.command_buffer),
			0,
			NULL
		};
		VULKAN_CALL(ctx->function_table.vkQueueSubmit(ctx->buffer_transfer.queue,1,&submit_info,ctx->buffer_transfer.fence));
		VULKAN_CALL(ctx->function_table.vkWaitForFences(ctx->device.logical,1,&(ctx->buffer_transfer.fence),VK_TRUE,UINT64_MAX));
		VULKAN_CALL(ctx->function_table.vkResetFences(ctx->device.logical,1,&(ctx->buffer_transfer.fence)));
	}
	ctx->function_table.vkCmdEndRenderPass(ctx->frame.command_buffer);
	VULKAN_CALL(ctx->function_table.vkEndCommandBuffer(ctx->frame.command_buffer));
	VkPipelineStageFlags wait_flags=VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submit_info={
		VK_STRUCTURE_TYPE_SUBMIT_INFO,
		NULL,
		1,
		&(ctx->sync.present_semaphore),
		&wait_flags,
		1,
		&(ctx->frame.command_buffer),
		1,
		&(ctx->sync.render_semaphore)
	};
	VULKAN_CALL(ctx->function_table.vkQueueSubmit(ctx->command.queue,1,&submit_info,ctx->sync.fences[ctx->frame.image_index]));
	VkPresentInfoKHR present_info={
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		NULL,
		1,
		&(ctx->sync.render_semaphore),
		1,
		&(ctx->swapchain.handle),
		&(ctx->frame.image_index),
		NULL
	};
	VkResult err=ctx->function_table.vkQueuePresentKHR(ctx->command.queue,&present_info);
	if (err==VK_SUBOPTIMAL_KHR){
		_create_swapchain(ctx);
	}
	else{
		VULKAN_CALL(err);
	}
	VULKAN_CALL(ctx->function_table.vkQueueWaitIdle(ctx->command.queue));
}



void _delete_context(gfx_context_data_t* ctx){
	_end_frame(ctx);
	SLL_HANDLE_CONTAINER_ITER_CLEAR(&(ctx->buffers),gfx_buffer_data_t,buffer,{
		_delete_buffer(ctx,buffer);
	});
	SLL_HANDLE_CONTAINER_ITER_CLEAR(&(ctx->pipelines),gfx_pipeline_data_t,pipeline,{
		_delete_pipeline(ctx,pipeline);
	});
	SLL_HANDLE_CONTAINER_ITER_CLEAR(&(ctx->shaders),gfx_shader_data_t,shader,{
		_delete_shader(ctx,shader);
	});
	ctx->function_table.vkDestroyFence(ctx->device.logical,ctx->buffer_transfer.fence,NULL);
	ctx->function_table.vkFreeCommandBuffers(ctx->device.logical,ctx->buffer_transfer.command_pool,1,&(ctx->buffer_transfer.command_buffer));
	ctx->function_table.vkDestroyCommandPool(ctx->device.logical,ctx->buffer_transfer.command_pool,NULL);
	_release_swapchain(ctx);
	sll_deallocate(ctx->swapchain.image_views);
	sll_deallocate(ctx->sync.fences);
	sll_deallocate(ctx->swapchain.frame_buffers);
	ctx->function_table.vkDestroySemaphore(ctx->device.logical,ctx->sync.present_semaphore,NULL);
	ctx->function_table.vkDestroySemaphore(ctx->device.logical,ctx->sync.render_semaphore,NULL);
	ctx->function_table.vkDestroyPipelineCache(ctx->device.logical,ctx->pipeline.cache,NULL);
	ctx->function_table.vkDestroyRenderPass(ctx->device.logical,ctx->pipeline.render_pass,NULL);
	ctx->function_table.vkDestroyCommandPool(ctx->device.logical,ctx->command.pool,NULL);
	ctx->function_table.vkDestroyDevice(ctx->device.logical,NULL);
	ctx->function_table.vkDestroySurfaceKHR(ctx->instance.handle,ctx->surface.handle,NULL);
	if (ctx->instance.debug_messenger!=VK_NULL_HANDLE){
		ctx->function_table.vkDestroyDebugUtilsMessengerEXT(ctx->instance.handle,ctx->instance.debug_messenger,NULL);
	}
	// ctx->function_table.vkDestroyInstansce(ctx->instance.handle,NULL);
	sll_deallocate(ctx);
}



__GFX_API_CALL gfx_context_t gfx_api_context_create(void* handle,void* extra_data,sll_bool_t debug){
	if (!vulkan_validation_layer_name){
		debug=0;
	}
	VkApplicationInfo app_info={
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		NULL,
		"Sll GFX v"SLL_VERSION_STRING,
		SLL_VERSION,
		"Sll GFX v"SLL_VERSION_STRING,
		SLL_VERSION,
		VK_API_VERSION_1_0
	};
	const char* enabled_extensions[3]={
		VK_KHR_SURFACE_EXTENSION_NAME,
		GFX_VULKAN_REQUIRED_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
	};
	VkInstanceCreateInfo instance_creation_info={
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		NULL,
		0,
		&app_info,
		debug,
		&vulkan_validation_layer_name,
		2+debug,
		enabled_extensions
	};
	gfx_context_data_t* ctx=sll_zero_allocate(sizeof(gfx_context_data_t));
	VULKAN_CALL(vkCreateInstance(&instance_creation_info,NULL,&(ctx->instance.handle)));
	sll_error_raise_bool(!_load_vulkan_function_table(ctx->instance.handle,&(ctx->function_table)));
	if (debug){
		VkDebugUtilsMessengerCreateInfoEXT debug_messenger_cretion_info={
			VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
			NULL,
			0,
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT|VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT|VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT|VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT|VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
			(PFN_vkDebugUtilsMessengerCallbackEXT)_debug_messenger_callback,
			ctx
		};
		VULKAN_CALL(ctx->function_table.vkCreateDebugUtilsMessengerEXT(ctx->instance.handle,&debug_messenger_cretion_info,NULL,&(ctx->instance.debug_messenger)));
	}
#ifdef __SLL_BUILD_DARWIN
	VkMacOSSurfaceCreateInfoMVK surface_creation_info={
		VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK,
		NULL,
		0,
		handle
	};
#elif defined(__SLL_BUILD_LINUX)
	VkXcbSurfaceCreateInfoKHR surface_creation_info={
		VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
		NULL,
		0,
		extra_data,
		(xcb_window_t)(sll_size_t)handle
	};
#else
	VkWin32SurfaceCreateInfoKHR surface_creation_info={
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
		NULL,
		0,
		extra_data,
		handle
	};
#endif
	VULKAN_CALL(ctx->function_table.create_system_surface(ctx->instance.handle,&surface_creation_info,NULL,&(ctx->surface.handle)));
	uint32_t count;
	VULKAN_CALL(ctx->function_table.vkEnumeratePhysicalDevices(ctx->instance.handle,&count,NULL));
	sll_error_raise_bool(!count||!"No GPU found");
	VkPhysicalDevice* physical_device_data=sll_allocate_stack(count*sizeof(VkPhysicalDevice));
	VULKAN_CALL(ctx->function_table.vkEnumeratePhysicalDevices(ctx->instance.handle,&count,physical_device_data));
	ctx->device.physical=physical_device_data[GFX_DEFAULT_GPU_INDEX];
	sll_deallocate(physical_device_data);
	ctx->function_table.vkGetPhysicalDeviceMemoryProperties(ctx->device.physical,&(ctx->device.memory_properties));
	ctx->function_table.vkGetPhysicalDeviceQueueFamilyProperties(ctx->device.physical,&count,NULL);
	VkQueueFamilyProperties* queue_properties=sll_allocate_stack(count*sizeof(VkQueueFamilyProperties));
	ctx->function_table.vkGetPhysicalDeviceQueueFamilyProperties(ctx->device.physical,&count,queue_properties);
	uint32_t graphic_queue_index=0;
	uint32_t transfer_queue_index=0;
	for (uint32_t i=0;i<count;i++){
		if ((queue_properties+i)->queueFlags&VK_QUEUE_GRAPHICS_BIT){
			VkBool32 present_support;
			VULKAN_CALL(ctx->function_table.vkGetPhysicalDeviceSurfaceSupportKHR(ctx->device.physical,i,ctx->surface.handle,&present_support));
			if (present_support){
				graphic_queue_index=i;
			}
		}
		if ((queue_properties+i)->queueFlags&VK_QUEUE_TRANSFER_BIT){
			transfer_queue_index=i;
		}
	}
	sll_deallocate(queue_properties);
	VULKAN_CALL(ctx->function_table.vkGetPhysicalDeviceSurfaceFormatsKHR(ctx->device.physical,ctx->surface.handle,&count,NULL));
	VkSurfaceFormatKHR* surface_formats=sll_allocate_stack(count*sizeof(VkSurfaceFormatKHR));
	VULKAN_CALL(ctx->function_table.vkGetPhysicalDeviceSurfaceFormatsKHR(ctx->device.physical,ctx->surface.handle,&count,surface_formats));
	ctx->surface.color_format=surface_formats->format;
	ctx->surface.color_space=surface_formats->colorSpace;
	for (uint32_t i=0;i<count;i++){
		if ((surface_formats+i)->format==VK_FORMAT_B8G8R8A8_UNORM){
			ctx->surface.color_format=VK_FORMAT_B8G8R8A8_UNORM;
			ctx->surface.color_space=(surface_formats+i)->colorSpace;
			break;
		}
	}
	sll_deallocate(surface_formats);
	if (ctx->surface.color_format==VK_FORMAT_UNDEFINED){
		ctx->surface.color_format=VK_FORMAT_B8G8R8A8_UNORM;
	}
	VkDeviceQueueCreateInfo device_queue_info[2]={
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			NULL,
			0,
			graphic_queue_index,
			1,
			&_default_queue_priority
		},
		{
			VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			NULL,
			0,
			transfer_queue_index,
			1,
			&_default_queue_priority
		}
	};
	const char* device_extensions[1]={
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	VkPhysicalDeviceFeatures device_features;
	sll_zero_memory(&device_features,sizeof(VkPhysicalDeviceFeatures));
	VkDeviceCreateInfo device_creation_info={
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		NULL,
		0,
		1+(graphic_queue_index!=transfer_queue_index),
		device_queue_info,
		0,
		NULL,
		1,
		device_extensions,
		&device_features
	};
	VULKAN_CALL(ctx->function_table.vkCreateDevice(ctx->device.physical,&device_creation_info,NULL,&(ctx->device.logical)));
	VkCommandPoolCreateInfo command_pool_creation_info={
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		NULL,
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		graphic_queue_index
	};
	VULKAN_CALL(ctx->function_table.vkCreateCommandPool(ctx->device.logical,&command_pool_creation_info,NULL,&(ctx->command.pool)));
	ctx->function_table.vkGetDeviceQueue(ctx->device.logical,graphic_queue_index,0,&(ctx->command.queue));
	VkAttachmentDescription render_pass_attachments[2]={
		{
			0,
			ctx->surface.color_format,
			VK_SAMPLE_COUNT_1_BIT,
			VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_STORE,
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		},
		{
			0,
			VK_FORMAT_D32_SFLOAT,
			VK_SAMPLE_COUNT_1_BIT,
			VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_STORE,
			VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
		}
	};
	VkAttachmentReference render_pass_subpass_color_ref={
		0,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};
	VkAttachmentReference render_pass_subpass_depth_ref={
		1,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	};
	VkSubpassDescription render_pass_subpass={
		0,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		0,
		NULL,
		1,
		&render_pass_subpass_color_ref,
		NULL,
		&render_pass_subpass_depth_ref,
		0,
		NULL
	};
	VkSubpassDependency render_pass_dependencies[2]={
		{
			VK_SUBPASS_EXTERNAL,
			0,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_ACCESS_COLOR_ATTACHMENT_READ_BIT|VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			VK_DEPENDENCY_BY_REGION_BIT
		},
		{
			0,
			VK_SUBPASS_EXTERNAL,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			VK_ACCESS_COLOR_ATTACHMENT_READ_BIT|VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_DEPENDENCY_BY_REGION_BIT
		}
	};
	VkRenderPassCreateInfo render_pass_creation_info={
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		NULL,
		0,
		2,
		render_pass_attachments,
		1,
		&render_pass_subpass,
		2,
		render_pass_dependencies
	};
	VULKAN_CALL(ctx->function_table.vkCreateRenderPass(ctx->device.logical,&render_pass_creation_info,NULL,&(ctx->pipeline.render_pass)));
	VkPipelineCacheCreateInfo pipeline_cache_creation_info={
		VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
		NULL,
		0,
		0,
		NULL
	};
	VULKAN_CALL(ctx->function_table.vkCreatePipelineCache(ctx->device.logical,&pipeline_cache_creation_info,NULL,&(ctx->pipeline.cache)));
	VkSemaphoreCreateInfo semaphore_creation_info={
		VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		NULL,
		0
	};
	VULKAN_CALL(ctx->function_table.vkCreateSemaphore(ctx->device.logical,&semaphore_creation_info,NULL,&(ctx->sync.present_semaphore)));
	VULKAN_CALL(ctx->function_table.vkCreateSemaphore(ctx->device.logical,&semaphore_creation_info,NULL,&(ctx->sync.render_semaphore)));
	_create_swapchain(ctx);
	command_pool_creation_info.queueFamilyIndex=transfer_queue_index;
	VULKAN_CALL(ctx->function_table.vkCreateCommandPool(ctx->device.logical,&command_pool_creation_info,NULL,&(ctx->buffer_transfer.command_pool)));
	ctx->function_table.vkGetDeviceQueue(ctx->device.logical,transfer_queue_index,0,&(ctx->buffer_transfer.queue));
	VkCommandBufferAllocateInfo command_buffer_allocation_info={
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		NULL,
		ctx->buffer_transfer.command_pool,
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		1
	};
	VULKAN_CALL(ctx->function_table.vkAllocateCommandBuffers(ctx->device.logical,&command_buffer_allocation_info,&(ctx->buffer_transfer.command_buffer)));
	VkFenceCreateInfo fence_creation_info={
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		0,
		0
	};
	VULKAN_CALL(ctx->function_table.vkCreateFence(ctx->device.logical,&fence_creation_info,NULL,&(ctx->buffer_transfer.fence)));
	ctx->buffer_transfer.has_data=1;
	_begin_frame(ctx);
	SLL_HANDLE_CONTAINER_INIT(&(ctx->buffers));
	SLL_HANDLE_CONTAINER_INIT(&(ctx->pipelines));
	SLL_HANDLE_CONTAINER_INIT(&(ctx->shaders));
	gfx_context_t out;
	SLL_HANDLE_CONTAINER_ALLOC(&gfx_context_data,&out);
	*(gfx_context_data.data+out)=ctx;
	return out;
}



__GFX_API_CALL void gfx_api_context_delete(gfx_context_t ctx_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (ctx){
		SLL_HANDLE_CONTAINER_DEALLOC(&gfx_context_data,ctx_id);
		_delete_context(ctx);
	}
}



__GFX_API_CALL void gfx_api_context_render(gfx_context_t ctx_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	_end_frame(ctx);
	_begin_frame(ctx);
}




__GFX_API_CALL void gfx_api_context_resize(gfx_context_t ctx_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	_end_frame(ctx);
	_create_swapchain(ctx);
	_begin_frame(ctx);
}

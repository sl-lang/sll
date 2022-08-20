#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/vulkan.h>
#include <gfx/vulkan_functions.h>
#include <sll.h>
#include <vulkan/vulkan.h>



#ifdef DEBUG_BUILD
#define EANBLED_EXTENSION_COUNT 3
#else
#define EANBLED_EXTENSION_COUNT 2
#endif



static const float _default_queue_priority=0.0f;



sll_handle_container_t gfx_context_data;



#ifdef DEBUG_BUILD
static VKAPI_ATTR VkBool32 _debug_messenger_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,VkDebugUtilsMessageTypeFlagsEXT type,const VkDebugUtilsMessengerCallbackDataEXT* data,gfx_context_data_t* ctx){
	SLL_WARN("%s",data->pMessage);
	return VK_FALSE;
}
#endif



static uint32_t _get_mem_type(const gfx_context_data_t* ctx,uint32_t mask,VkMemoryPropertyFlagBits properties){
	for (uint32_t i=0;i<ctx->memory_properties.memoryTypeCount;i++){
		if ((mask&1)&&((ctx->memory_properties.memoryTypes+i)->propertyFlags&properties)==properties){
			return i;
		}
		mask>>=1;
	}
	SLL_WARN("No memory type found!");
	return 0;
}



static void _create_swapchain(gfx_context_data_t* ctx){
	VkSurfaceCapabilitiesKHR surface_caps;
	VULKAN_CALL(ctx->function_table.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ctx->physical_device,ctx->surface,&surface_caps));
	VkSwapchainCreateInfoKHR swapchain_creation_info={
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		NULL,
		0,
		ctx->surface,
		surface_caps.minImageCount+1,
		ctx->color_format,
		ctx->color_space,
		surface_caps.currentExtent,
		1,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT|(surface_caps.supportedUsageFlags&(VK_IMAGE_USAGE_TRANSFER_SRC_BIT|VK_IMAGE_USAGE_TRANSFER_DST_BIT)),
		VK_SHARING_MODE_EXCLUSIVE,
		0,
		NULL,
		((surface_caps.supportedTransforms&VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)?VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR:surface_caps.supportedTransforms),
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		VK_PRESENT_MODE_FIFO_KHR,
		VK_TRUE,
		ctx->swapchain
	};
	VkSwapchainKHR old_swapchain=ctx->swapchain;
	VULKAN_CALL(ctx->function_table.vkCreateSwapchainKHR(ctx->logical_device,&swapchain_creation_info,NULL,&(ctx->swapchain)));
	if (old_swapchain!=VK_NULL_HANDLE){
		SLL_WARN("Unimplemented!");
	}
	VULKAN_CALL(ctx->function_table.vkGetSwapchainImagesKHR(ctx->logical_device,ctx->swapchain,&(ctx->swapchain_image_count),NULL));
	ctx->swapchain_images=sll_reallocate(ctx->swapchain_images,ctx->swapchain_image_count*sizeof(VkImage));
	VULKAN_CALL(ctx->function_table.vkGetSwapchainImagesKHR(ctx->logical_device,ctx->swapchain,&(ctx->swapchain_image_count),ctx->swapchain_images));
	ctx->command_buffers=sll_reallocate(ctx->command_buffers,ctx->swapchain_image_count*sizeof(VkCommandBuffer));
	VkCommandBufferAllocateInfo command_buffer_allocation_info={
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		NULL,
		ctx->command_pool,
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		ctx->swapchain_image_count
	};
	VULKAN_CALL(ctx->function_table.vkAllocateCommandBuffers(ctx->logical_device,&command_buffer_allocation_info,ctx->command_buffers));
	ctx->swapchain_image_views=sll_reallocate(ctx->swapchain_image_views,ctx->swapchain_image_count*sizeof(VkImageView));
	VkImageViewCreateInfo image_view_creation_info={
		VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		NULL,
		0,
		VK_NULL_HANDLE,
		VK_IMAGE_VIEW_TYPE_2D,
		ctx->color_format,
		{
			VK_COMPONENT_SWIZZLE_R,
			VK_COMPONENT_SWIZZLE_G,
			VK_COMPONENT_SWIZZLE_B,
			VK_COMPONENT_SWIZZLE_A
		},
		{
			VK_IMAGE_ASPECT_COLOR_BIT,
			0,
			0,
			0,
			1
		}
	};
	ctx->fences=sll_reallocate(ctx->fences,ctx->swapchain_image_count*sizeof(VkFence));
	VkFenceCreateInfo fence_creation_info={
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		NULL,
		VK_FENCE_CREATE_SIGNALED_BIT
	};
	ctx->frame_buffers=sll_reallocate(ctx->frame_buffers,ctx->swapchain_image_count*sizeof(VkFramebuffer));
	VkImageView frame_buffer_attachments[2]={
		VK_NULL_HANDLE,
		ctx->depth_stensil_image_view
	};
	VkFramebufferCreateInfo frame_buffer_creation_info={
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
		NULL,
		0,
		ctx->render_pass,
		2,
		frame_buffer_attachments,
		surface_caps.currentExtent.width,
		surface_caps.currentExtent.height,
		1
	};
	for (uint32_t i=0;i<ctx->swapchain_image_count;i++){
		image_view_creation_info.image=ctx->swapchain_images[i];
		VULKAN_CALL(ctx->function_table.vkCreateImageView(ctx->logical_device,&image_view_creation_info,NULL,ctx->swapchain_image_views+i));
		VULKAN_CALL(ctx->function_table.vkCreateFence(ctx->logical_device,&fence_creation_info,NULL,ctx->fences+i));
		frame_buffer_attachments[0]=ctx->swapchain_image_views[i];
		VULKAN_CALL(ctx->function_table.vkCreateFramebuffer(ctx->logical_device,&frame_buffer_creation_info,NULL,ctx->frame_buffers+i));
	}
	VkImageCreateInfo depth_stensil_image_creation_info={
		VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		NULL,
		0,
		VK_IMAGE_TYPE_2D,
		VK_FORMAT_D32_SFLOAT,
		{
			surface_caps.currentExtent.width,
			surface_caps.currentExtent.height,
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
	VULKAN_CALL(ctx->function_table.vkCreateImage(ctx->logical_device,&depth_stensil_image_creation_info,NULL,&(ctx->depth_stensil_image)));
	VkMemoryRequirements mem_requirements;
	ctx->function_table.vkGetImageMemoryRequirements(ctx->logical_device,ctx->depth_stensil_image,&mem_requirements);
	VkMemoryAllocateInfo mem_alloc_info={
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		NULL,
		mem_requirements.size,
		_get_mem_type(ctx,mem_requirements.memoryTypeBits,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
	};
	VULKAN_CALL(ctx->function_table.vkAllocateMemory(ctx->logical_device,&mem_alloc_info,NULL,&(ctx->depth_stensil_memory)));
	VULKAN_CALL(ctx->function_table.vkBindImageMemory(ctx->logical_device,ctx->depth_stensil_image,ctx->depth_stensil_memory,0));
	VkImageViewCreateInfo deptch_stensil_image_view_creation_info={
		VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		NULL,
		0,
		ctx->depth_stensil_image,
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
	VULKAN_CALL(ctx->function_table.vkCreateImageView(ctx->logical_device,&deptch_stensil_image_view_creation_info,NULL,&(ctx->depth_stensil_image_view)));
}



void _delete_context(gfx_context_data_t* ctx){
	if (!ctx){
		return;
	}
	ctx->function_table.vkDestroySwapchainKHR(ctx->logical_device,ctx->swapchain,NULL);
	ctx->function_table.vkDestroyCommandPool(ctx->logical_device,ctx->command_pool,NULL);
	ctx->function_table.vkDestroyDevice(ctx->logical_device,NULL);
	ctx->function_table.vkDestroySurfaceKHR(ctx->instance,ctx->surface,NULL);
	ctx->function_table.vkDestroyInstance(ctx->instance,NULL);
	sll_deallocate(ctx);
}



__GFX_API_CALL gfx_context_t gfx_api_context_create(void* handle,void* extra_data){
	VkApplicationInfo app_info={
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		NULL,
		"Sll GFX v"SLL_VERSION_STRING,
		SLL_VERSION,
		"Sll GFX v"SLL_VERSION_STRING,
		SLL_VERSION,
		VK_API_VERSION_1_0
	};
	const char* enabled_extensions[EANBLED_EXTENSION_COUNT]={
		VK_KHR_SURFACE_EXTENSION_NAME,
		GFX_VULKAN_REQUIRED_EXTENSION_NAME,
#ifdef DEBUG_BUILD
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
	};
	VkInstanceCreateInfo instance_creation_info={
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		NULL,
		0,
		&app_info,
		!!validation_layer_name,
		&validation_layer_name,
		2+!!validation_layer_name,
		enabled_extensions
	};
	gfx_context_data_t* ctx=sll_zero_allocate(sizeof(gfx_context_data_t));
	VULKAN_CALL(vkCreateInstance(&instance_creation_info,NULL,&(ctx->instance)));
#ifdef DEBUG_BUILD
	VkDebugUtilsMessengerCreateInfoEXT debug_messenger_cretion_info={
		VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		NULL,
		0,
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT|VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT|VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT|VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT|VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		(PFN_vkDebugUtilsMessengerCallbackEXT)_debug_messenger_callback,
		ctx
	};
	PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT=(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(ctx->instance,"vkCreateDebugUtilsMessengerEXT");
	if (vkCreateDebugUtilsMessengerEXT){
		VULKAN_CALL(vkCreateDebugUtilsMessengerEXT(ctx->instance,&debug_messenger_cretion_info,NULL,&(ctx->debug_messenger)));
	}
#endif
	sll_error_raise_bool(!_load_vulkan_function_table(ctx->instance,&(ctx->function_table)));
#ifdef __SLL_BUILD_DARWIN
	VkMacOSSurfaceCreateInfoMVK surface_creation_info={
		VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK,
		NULL,
		0,
		handle
	};
	VULKAN_CALL(ctx->function_table.vkCreateMacOSSurfaceMVK(ctx->instance,&surface_creation_info,NULL,&(ctx->surface)));
#elif defined(__SLL_BUILD_LINUX)
	VkXcbSurfaceCreateInfoKHR surface_creation_info={
		VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
		NULL,
		0,
		extra_data,
		(xcb_window_t)(sll_size_t)handle
	};
	VULKAN_CALL(ctx->function_table.vkCreateXcbSurfaceKHR(ctx->instance,&surface_creation_info,NULL,&(ctx->surface)));
#else
	VkWin32SurfaceCreateInfoKHR surface_creation_info={
		VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
		NULL,
		0,
		extra_data,
		handle
	};
	VULKAN_CALL(ctx->function_table.vkCreateWin32SurfaceKHR(ctx->instance,&surface_creation_info,NULL,&(ctx->surface)));
#endif
	uint32_t count;
	VULKAN_CALL(ctx->function_table.vkEnumeratePhysicalDevices(ctx->instance,&count,NULL));
	sll_error_raise_bool(!count||!"No GPU found");
	VkPhysicalDevice* physical_device_data=sll_allocate_stack(count*sizeof(VkPhysicalDevice));
	VULKAN_CALL(ctx->function_table.vkEnumeratePhysicalDevices(ctx->instance,&count,physical_device_data));
	ctx->physical_device=*(physical_device_data+GFX_DEFAULT_GPU_INDEX);
	sll_deallocate(physical_device_data);
	ctx->function_table.vkGetPhysicalDeviceMemoryProperties(ctx->physical_device,&(ctx->memory_properties));
	ctx->function_table.vkGetPhysicalDeviceQueueFamilyProperties(ctx->physical_device,&count,NULL);
	VkQueueFamilyProperties* queue_properties=sll_allocate_stack(count*sizeof(VkQueueFamilyProperties));
	ctx->function_table.vkGetPhysicalDeviceQueueFamilyProperties(ctx->physical_device,&count,queue_properties);
	ctx->device_queue_index=0;
	for (uint32_t i=0;i<count;i++){
		if ((queue_properties+i)->queueFlags&VK_QUEUE_GRAPHICS_BIT){
			VkBool32 present_support;
			VULKAN_CALL(ctx->function_table.vkGetPhysicalDeviceSurfaceSupportKHR(ctx->physical_device,i,ctx->surface,&present_support));
			if (present_support){
				ctx->device_queue_index=i;
				break;
			}
		}
	}
	sll_deallocate(queue_properties);
	VULKAN_CALL(ctx->function_table.vkGetPhysicalDeviceSurfaceFormatsKHR(ctx->physical_device,ctx->surface,&count,NULL));
	VkSurfaceFormatKHR* surface_formats=sll_allocate_stack(count*sizeof(VkSurfaceFormatKHR));
	VULKAN_CALL(ctx->function_table.vkGetPhysicalDeviceSurfaceFormatsKHR(ctx->physical_device,ctx->surface,&count,surface_formats));
	ctx->color_format=surface_formats->format;
	ctx->color_space=surface_formats->colorSpace;
	for (uint32_t i=0;i<count;i++){
		if ((surface_formats+i)->format==VK_FORMAT_B8G8R8A8_UNORM){
			ctx->color_format=VK_FORMAT_B8G8R8A8_UNORM;
			ctx->color_space=(surface_formats+i)->colorSpace;
			break;
		}
	}
	sll_deallocate(surface_formats);
	if (ctx->color_format==VK_FORMAT_UNDEFINED){
		ctx->color_format=VK_FORMAT_B8G8R8A8_UNORM;
	}
	VkDeviceQueueCreateInfo device_queue_info={
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		NULL,
		0,
		ctx->device_queue_index,
		1,
		&_default_queue_priority
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
		1,
		&device_queue_info,
		0,
		NULL,
		1,
		device_extensions,
		&device_features
	};
	VULKAN_CALL(ctx->function_table.vkCreateDevice(ctx->physical_device,&device_creation_info,NULL,&(ctx->logical_device)));
	VkCommandPoolCreateInfo command_pool_creation_info={
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		NULL,
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		ctx->device_queue_index
	};
	VULKAN_CALL(ctx->function_table.vkCreateCommandPool(ctx->logical_device,&command_pool_creation_info,NULL,&(ctx->command_pool)));
	ctx->function_table.vkGetDeviceQueue(ctx->logical_device,ctx->device_queue_index,0,&(ctx->queue));
	VkAttachmentDescription render_pass_attachments[2]={
		{
			0,
			ctx->color_format,
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
	VULKAN_CALL(ctx->function_table.vkCreateRenderPass(ctx->logical_device,&render_pass_creation_info,NULL,&(ctx->render_pass)));
	VkPipelineCacheCreateInfo pipeline_cache_creation_info={
		VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
		NULL,
		0,
		0,
		NULL
	};
	VULKAN_CALL(ctx->function_table.vkCreatePipelineCache(ctx->logical_device,&pipeline_cache_creation_info,NULL,&(ctx->pipeline_cache)));
	_create_swapchain(ctx);
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

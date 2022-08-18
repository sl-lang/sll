#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/vulkan.h>
#include <gfx/vulkan_functions.h>
#include <sll.h>
#include <vulkan/vulkan.h>



static const float _default_queue_priority=0.0f;



sll_handle_container_t gfx_context_data;



void _delete_context(gfx_context_data_t* ctx){
	if (!ctx){
		return;
	}
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
	const char* enabled_extensions[2]={
		VK_KHR_SURFACE_EXTENSION_NAME,
		GFX_VULKAN_REQUIRED_EXTENSION_NAME
	};
	VkInstanceCreateInfo instance_creation_info={
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		NULL,
		0,
		&app_info,
		0,
		NULL,
		2,
		enabled_extensions
	};
	gfx_context_data_t* ctx=sll_allocate(sizeof(gfx_context_data_t));
	VULKAN_CALL(vkCreateInstance(&instance_creation_info,NULL,&(ctx->instance)));
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

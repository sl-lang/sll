#include <gfx/common.h>
#include <gfx/vulkan_functions.h>
#include <sll.h>
#include <vulkan/vulkan.h>



#define LOAD_FUNCTION(name) \
	if (!(name=(PFN_##name)vkGetInstanceProcAddr(NULL,#name))){ \
		SLL_WARN("Failed to load Vulkan function '%s'!",#name); \
		ret=0; \
	}
#define LOAD_FUNCTION_TABLE(name) \
	if (!(out->name=(PFN_##name)vkGetInstanceProcAddr(instance,#name))){ \
		SLL_WARN("Failed to load Vulkan function '%s'!",#name); \
		ret=0; \
	}



PFN_vkCreateInstance vkCreateInstance;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;



sll_bool_t _load_vulkan_functions(PFN_vkGetInstanceProcAddr get_proc_addr){
	if (!get_proc_addr){
		SLL_WARN("Failed to load Vulkan function 'vkGetInstanceProcAddr'!");
		return 0;
	}
	vkGetInstanceProcAddr=get_proc_addr;
	sll_bool_t ret=1;
	LOAD_FUNCTION(vkCreateInstance);
	LOAD_FUNCTION(vkEnumerateInstanceExtensionProperties);
	LOAD_FUNCTION(vkEnumerateInstanceLayerProperties);
	return ret;
}



sll_bool_t _load_vulkan_function_table(VkInstance instance,gfx_vulkan_function_table_t* out){
	sll_bool_t ret=1;
	LOAD_FUNCTION_TABLE(vkAcquireNextImageKHR);
	LOAD_FUNCTION_TABLE(vkAllocateCommandBuffers);
	LOAD_FUNCTION_TABLE(vkAllocateMemory);
	LOAD_FUNCTION_TABLE(vkBeginCommandBuffer);
	LOAD_FUNCTION_TABLE(vkBindImageMemory);
	LOAD_FUNCTION_TABLE(vkCmdBeginRenderPass);
	LOAD_FUNCTION_TABLE(vkCmdEndRenderPass);
	LOAD_FUNCTION_TABLE(vkCreateCommandPool);
	LOAD_FUNCTION_TABLE(vkCreateDevice);
	LOAD_FUNCTION_TABLE(vkCreateFence);
	LOAD_FUNCTION_TABLE(vkCreateFramebuffer);
	LOAD_FUNCTION_TABLE(vkCreateImage);
	LOAD_FUNCTION_TABLE(vkCreateImageView);
	LOAD_FUNCTION_TABLE(vkCreatePipelineCache);
	LOAD_FUNCTION_TABLE(vkCreateRenderPass);
	LOAD_FUNCTION_TABLE(vkCreateSemaphore);
	LOAD_FUNCTION_TABLE(vkCreateSwapchainKHR);
	LOAD_FUNCTION_TABLE(vkDestroyCommandPool);
	LOAD_FUNCTION_TABLE(vkDestroyDevice);
	LOAD_FUNCTION_TABLE(vkDestroyFence)
	LOAD_FUNCTION_TABLE(vkDestroyFramebuffer);
	LOAD_FUNCTION_TABLE(vkDestroyImage);
	LOAD_FUNCTION_TABLE(vkDestroyImageView);
	LOAD_FUNCTION_TABLE(vkDestroyInstance);
	LOAD_FUNCTION_TABLE(vkDestroyPipelineCache);
	LOAD_FUNCTION_TABLE(vkDestroyRenderPass);
	LOAD_FUNCTION_TABLE(vkDestroySemaphore);
	LOAD_FUNCTION_TABLE(vkDestroySurfaceKHR);
	LOAD_FUNCTION_TABLE(vkDestroySwapchainKHR);
	LOAD_FUNCTION_TABLE(vkDeviceWaitIdle);
	LOAD_FUNCTION_TABLE(vkEndCommandBuffer);
	LOAD_FUNCTION_TABLE(vkEnumeratePhysicalDevices);
	LOAD_FUNCTION_TABLE(vkFreeMemory);
	LOAD_FUNCTION_TABLE(vkGetDeviceQueue);
	LOAD_FUNCTION_TABLE(vkGetImageMemoryRequirements);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceMemoryProperties);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceQueueFamilyProperties);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceSurfaceFormatsKHR);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceSurfaceSupportKHR);
	LOAD_FUNCTION_TABLE(vkGetSwapchainImagesKHR);
	LOAD_FUNCTION_TABLE(vkQueuePresentKHR);
	LOAD_FUNCTION_TABLE(vkQueueSubmit);
	LOAD_FUNCTION_TABLE(vkQueueWaitIdle);
#ifdef __SLL_BUILD_DARWIN
	LOAD_FUNCTION_TABLE(vkCreateMacOSSurfaceMVK);
#elif defined(__SLL_BUILD_LINUX)
	LOAD_FUNCTION_TABLE(vkCreateXcbSurfaceKHR);
#else
	LOAD_FUNCTION_TABLE(vkCreateWin32SurfaceKHR);
#endif
	return ret;
}

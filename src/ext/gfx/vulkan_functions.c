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
#ifdef __SLL_BUILD_DARWIN
PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK;
#elif defined(__SLL_BUILD_LINUX)
PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
#else
PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
#endif
PFN_vkDestroyInstance vkDestroyInstance;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
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
	return ret;
}



sll_bool_t _load_vulkan_function_table(VkInstance instance,gfx_vulkan_function_table_t* out){
	sll_bool_t ret=1;
#ifdef __SLL_BUILD_DARWIN
	LOAD_FUNCTION_TABLE(vkCreateMacOSSurfaceMVK);
#elif defined(__SLL_BUILD_LINUX)
	LOAD_FUNCTION_TABLE(vkCreateXcbSurfaceKHR);
#else
	LOAD_FUNCTION_TABLE(vkCreateWin32SurfaceKHR);
#endif
	LOAD_FUNCTION_TABLE(vkDestroyInstance);
	LOAD_FUNCTION_TABLE(vkDestroySurfaceKHR);
	LOAD_FUNCTION_TABLE(vkEnumeratePhysicalDevices);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceQueueFamilyProperties);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceSurfaceFormatsKHR);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceSurfaceSupportKHR);
	return ret;
}

#ifndef __GFX_VULKAN_FUNCTIONS_H__
#define __GFX_VULKAN_FUNCTIONS_H__ 1
#include <sll.h>
#include <vulkan/vulkan.h>



typedef struct _GFX_VULKAN_FUNCTION_TABLE{
	PFN_vkCreateDevice vkCreateDevice;
#ifdef __SLL_BUILD_DARWIN
	PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK;
#elif defined(__SLL_BUILD_LINUX)
	PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
#else
	PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
#endif
	PFN_vkDestroyDevice vkDestroyDevice;
	PFN_vkDestroyInstance vkDestroyInstance;
	PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
	PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
} gfx_vulkan_function_table_t;



extern PFN_vkCreateInstance vkCreateInstance;
extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;



sll_bool_t _load_vulkan_functions(PFN_vkGetInstanceProcAddr get_proc_addr);



sll_bool_t _load_vulkan_function_table(VkInstance instance,gfx_vulkan_function_table_t* out);



#endif

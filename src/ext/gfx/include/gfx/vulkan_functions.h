#ifndef __GFX_VULKAN_FUNCTIONS_H__
#define __GFX_VULKAN_FUNCTIONS_H__ 1
#include <sll.h>
#include <vulkan/vulkan.h>



extern PFN_vkCreateInstance vkCreateInstance;
extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;



sll_bool_t _load_vulkan_functions(PFN_vkGetInstanceProcAddr get_proc_addr);



#endif

#include <gfx/common.h>
#include <sll.h>
#include <vulkan/vulkan.h>



sll_bool_t _init_vulkan(void){
#ifdef __SLL_BUILD_WINDOWS
	sll_library_handle_t library_handle=sll_platform_load_library(SLL_CHAR("vulkan-1.dll"),NULL);
#elif defined(__SLL_BUILD_LINUX)
	sll_library_handle_t library_handle=sll_platform_load_library(SLL_CHAR("libvulkan.so.1"),NULL);
#else
	sll_library_handle_t library_handle=sll_platform_load_library(SLL_CHAR("libvulkan.1.dylib"),NULL);
#endif
	if (!library_handle){
		return 0;
	}
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr=sll_platform_lookup_symbol(library_handle,SLL_CHAR("vkGetInstanceProcAddr"));
	if (!vkGetInstanceProcAddr){
		return 0;
	}
	PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties=(PFN_vkEnumerateInstanceExtensionProperties)vkGetInstanceProcAddr(NULL,"vkEnumerateInstanceExtensionProperties");
	if (!vkEnumerateInstanceExtensionProperties){
		return 0;
	}
	return 1;
}

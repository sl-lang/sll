#include <gfx/common.h>
#include <sll.h>
#include <vulkan/vulkan.h>



static sll_library_handle_t _vulkan_library_handle;
static PFN_vkGetInstanceProcAddr _vulkan_get_instance_proc_addr;



void _deinit_vulkan(void){
	sll_error_raise(sll_platform_unload_library(_vulkan_library_handle));
}



sll_bool_t _init_vulkan(void){
#ifdef __SLL_BUILD_WINDOWS
	_vulkan_library_handle=sll_platform_load_library(SLL_CHAR("vulkan-1.dll"),NULL);
#elif defined(__SLL_BUILD_LINUX)
	_vulkan_library_handle=sll_platform_load_library(SLL_CHAR("libvulkan.so.1"),NULL);
#else
	_vulkan_library_handle=sll_platform_load_library(SLL_CHAR("libvulkan.1.dylib"),NULL);
#endif
	if (!_vulkan_library_handle){
		return 0;
	}
	_vulkan_get_instance_proc_addr=sll_platform_lookup_symbol(_vulkan_library_handle,SLL_CHAR("vkGetInstanceProcAddr"));
	if (!_vulkan_get_instance_proc_addr){
		return 0;
	}
	PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties=(PFN_vkEnumerateInstanceExtensionProperties)_vulkan_get_instance_proc_addr(NULL,"vkEnumerateInstanceExtensionProperties");
	if (!vkEnumerateInstanceExtensionProperties){
		return 0;
	}
	return 1;
}

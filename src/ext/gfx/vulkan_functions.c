#include <gfx/common.h>
#include <sll.h>
#include <vulkan/vulkan.h>



#define LOAD_FUNCTION(name) \
	if (!(name=(PFN_##name)vkGetInstanceProcAddr(NULL,#name))){ \
		SLL_WARN("Failed to load Vulkan function '%s'!",#name); \
		out=0; \
	} \



PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;



sll_bool_t _load_vulkan_functions(PFN_vkGetInstanceProcAddr get_proc_addr){
	if (!get_proc_addr){
		return 0;
	}
	vkGetInstanceProcAddr=get_proc_addr;
	sll_bool_t out=1;
	LOAD_FUNCTION(vkEnumerateInstanceExtensionProperties);
	return out;
}

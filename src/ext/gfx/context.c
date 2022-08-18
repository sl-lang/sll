#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/vulkan_functions.h>
#include <sll.h>
#include <vulkan/vulkan.h>



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
		"VK_KHR_surface",
		GFX_VULKAN_REQUIRED_EXTENSION_NAME
	};
	VkInstanceCreateInfo creation_info={
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		NULL,
		0,
		&app_info,
		0,
		NULL,
		2,
		enabled_extensions
	};
	gfx_context_data_t context;
	vkCreateInstance(&creation_info,NULL,&(context.instance));
	return (gfx_context_t)-12345;
}

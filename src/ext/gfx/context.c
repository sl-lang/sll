#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/vulkan.h>
#include <gfx/vulkan_functions.h>
#include <sll.h>
#include <vulkan/vulkan.h>



sll_handle_container_t gfx_context_data;



void _delete_context(gfx_context_data_t* ctx){
	if (!ctx){
		return;
	}
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
		"VK_KHR_surface",
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
#ifdef __SLL_BUILD_WINDOWS
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

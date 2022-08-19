#ifndef __GFX_CONTEXT_H__
#define __GFX_CONTEXT_H__ 1
#include <gfx/common.h>
#include <gfx/vulkan_functions.h>
#include <sll.h>
#include <vulkan/vulkan.h>



#define GFX_DEFAULT_GPU_INDEX 0



typedef struct _GFX_CONTEXT_DATA{
	gfx_vulkan_function_table_t function_table;
	VkInstance instance;
	VkSurfaceKHR surface;
	VkPhysicalDevice physical_device;
	VkDevice logical_device;
	VkCommandPool command_pool;
	VkQueue queue;
	VkSwapchainKHR swapchain;
	uint32_t device_queue_index;
	VkFormat color_format;
	VkColorSpaceKHR color_space;
	uint32_t swapchain_image_count;
	VkImage* swapchain_images;
	VkImageView* swapchain_image_views;
	VkCommandBuffer* command_buffers;
	VkFence* fences;
	VkImage depth_stensil_image;
	VkImageView depth_stensil_image_view;
	VkDeviceMemory depth_stensil_memory;
} gfx_context_data_t;



typedef sll_size_t gfx_context_t;



extern sll_handle_container_t gfx_context_data;



void _delete_context(gfx_context_data_t* ctx);



__GFX_API_CALL gfx_context_t gfx_api_context_create(void* handle,void* extra_data);



__GFX_API_CALL void gfx_api_context_delete(gfx_context_t ctx_id);



#endif

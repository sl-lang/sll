#ifndef __GFX_CONTEXT_H__
#define __GFX_CONTEXT_H__ 1
#include <gfx/common.h>
#include <gfx/vulkan_functions.h>
#include <sll.h>
#include <stdint.h>
#include <vulkan/vulkan.h>



#define GFX_DEFAULT_GPU_INDEX 0



typedef struct _GFX_CONTEXT_BUFFER_TRANSFER_DATA{
	VkQueue queue;
	VkCommandPool command_pool;
	VkCommandBuffer command_buffer;
	VkFence fence;
	sll_bool_t has_data;
} gfx_context_buffer_transfer_data_t;



typedef struct _GFX_CONTEXT_COMMAND_DATA{
	VkCommandPool pool;
	VkCommandBuffer* buffers;
	VkQueue queue;
} gfx_context_command_data_t;



typedef struct _GFX_CONTEXT_DEPTH_STENSIL_DATA{
	VkImage image;
	VkImageView image_view;
	VkDeviceMemory memory;
} gfx_context_depth_stensil_data_t;



typedef struct _GFX_CONTEXT_DEVICE_DATA{
	VkPhysicalDevice physical;
	VkDevice logical;
	VkPhysicalDeviceMemoryProperties memory_properties;
} gfx_context_device_data_t;



typedef struct _GFX_CONTEXT_FRAME_DATA{
	uint32_t image_index;
	VkCommandBuffer command_buffer;
} gfx_context_frame_data_t;



typedef struct _GFX_CONTEXT_INSTANCE_DATA{
	VkInstance handle;
	VkDebugUtilsMessengerEXT debug_messenger;
} gfx_context_instance_data_t;



typedef struct _GFX_CONTEXT_PIPELINE_DATA{
	VkRenderPass render_pass;
	VkPipelineCache cache;
} gfx_context_pipeline_data_t;



typedef struct _GFX_CONTEXT_SURFACE_DATA{
	uint32_t width;
	uint32_t height;
	VkSurfaceKHR handle;
	VkFormat color_format;
	VkColorSpaceKHR color_space;
	float clear_color[4];
} gfx_context_surface_data_t;



typedef struct _GFX_CONTEXT_SWAPCHAIN_DATA{
	VkSwapchainKHR handle;
	uint32_t image_count;
	VkImage* images;
	VkImageView* image_views;
	VkFramebuffer* frame_buffers;
} gfx_context_swapchain_data_t;



typedef struct _GFX_CONTEXT_SYNC_DATA{
	VkFence* fences;
	VkSemaphore present_semaphore;
	VkSemaphore render_semaphore;
} gfx_context_sync_data_t;



typedef struct _GFX_CONTEXT_DATA{
	gfx_context_buffer_transfer_data_t buffer_transfer;
	gfx_context_command_data_t command;
	gfx_context_depth_stensil_data_t depth_stensil;
	gfx_context_device_data_t device;
	gfx_context_frame_data_t frame;
	gfx_context_instance_data_t instance;
	gfx_context_pipeline_data_t pipeline;
	gfx_context_surface_data_t surface;
	gfx_context_swapchain_data_t swapchain;
	gfx_context_sync_data_t sync;
	gfx_vulkan_function_table_t function_table;
	sll_handle_container_t buffers;
	sll_handle_container_t pipelines;
	sll_handle_container_t shaders;
} gfx_context_data_t;



typedef sll_size_t gfx_context_t;



extern sll_handle_container_t gfx_context_data;



void _delete_context(gfx_context_data_t* ctx);



__GFX_API_CALL gfx_context_t gfx_api_context_create(void* handle,void* extra_data,sll_bool_t debug);



__GFX_API_CALL void gfx_api_context_delete(gfx_context_t ctx_id);



__GFX_API_CALL void gfx_api_context_render(gfx_context_t ctx_id);



__GFX_API_CALL void gfx_api_context_resize(gfx_context_t ctx_id);



#endif

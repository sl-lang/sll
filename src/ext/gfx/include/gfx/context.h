#ifndef __GFX_CONTEXT_H__
#define __GFX_CONTEXT_H__ 1
#include <gfx/common.h>
#include <gfx/vulkan_functions.h>
#include <sll.h>
#include <stdint.h>
#include <vulkan/vulkan.h>



#define GFX_DEFAULT_GPU_INDEX 0

#define GFX_FLAG_HAS_TRANSFER_COMMAND_BUFFER_DATA 1
#define GFX_FLAG_HAS_GRAPHICS_COMMAND_BUFFER_DATA 2



typedef uint8_t gfx_context_command_buffer_data_flags_t;



typedef struct _GFX_CONTEXT_INSTANCE_DATA{
	VkInstance handle;
	VkDebugUtilsMessengerEXT debug_messenger;
} gfx_context_instance_data_t;



typedef struct _GFX_CONTEXT_DEVICE_DATA{
	VkPhysicalDevice physical;
	VkDevice logical;
	VkPhysicalDeviceMemoryProperties memory_properties;
	VkPhysicalDeviceLimits limits;
} gfx_context_device_data_t;



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
	sll_bool_t rebuild;
	uint32_t image_count;
	VkImage* images;
	VkImageView* image_views;
	VkFramebuffer* frame_buffers;
	VkCommandBuffer* command_buffers;
} gfx_context_swapchain_data_t;



typedef struct _GFX_CONTEXT_QUEUE_DATA{
	VkCommandPool graphics_command_pool;
	VkQueue graphics_queue;
	uint32_t graphics_queue_index;
	VkCommandPool transfer_command_pool;
	VkQueue transfer_queue;
	uint32_t transfer_queue_index;
} gfx_context_queue_data_t;



typedef struct _GFX_CONTEXT_COMMAND_BUFFERS_DATA{
	VkCommandBuffer transfer;
	VkCommandBuffer graphics;
	VkFence fence;
	uint64_t index;
	gfx_context_command_buffer_data_flags_t flags;
} gfx_context_command_buffers_data_t;



typedef struct _GFX_CONTEXT_PIPELINE_DATA{
	VkRenderPass render_pass;
	VkPipelineCache cache;
} gfx_context_pipeline_data_t;



typedef struct _GFX_CONTEXT_SYNC_DATA{
	VkFence* fences;
	VkSemaphore present_semaphore;
	VkSemaphore render_semaphore;
} gfx_context_sync_data_t;



typedef struct _GFX_CONTEXT_DEPTH_STENSIL_DATA{
	VkImage image;
	VkImageView image_view;
	VkDeviceMemory memory;
} gfx_context_depth_stensil_data_t;



typedef struct _GFX_CONTEXT_FRAME_DATA{
	uint32_t image_index;
	VkCommandBuffer command_buffer;
	uint64_t index;
} gfx_context_frame_data_t;



typedef struct _GFX_CONTEXT_WRITE_DESCRIPTORS{
	uint32_t count;
	VkWriteDescriptorSet* data;
	void** pointers;
} gfx_context_write_descriptors_t;



typedef struct _GFX_CONTEXT_CHILD_OBJECTS{
	sll_handle_container_t buffers;
	sll_handle_container_t pipelines;
	sll_handle_container_t samplers;
	sll_handle_container_t shaders;
	sll_handle_container_t textures;
} gfx_context_child_objects_t;



typedef struct _GFX_CONTEXT_DATA{
	gfx_context_instance_data_t instance;
	gfx_context_device_data_t device;
	gfx_context_surface_data_t surface;
	gfx_context_swapchain_data_t swapchain;
	gfx_context_queue_data_t queue;
	gfx_context_command_buffers_data_t command_buffers;
	gfx_context_pipeline_data_t pipeline;
	gfx_context_sync_data_t sync;
	gfx_context_depth_stensil_data_t depth_stensil;
	gfx_context_frame_data_t frame;
	gfx_context_write_descriptors_t write_descriptors;
	gfx_vulkan_function_table_t function_table;
	gfx_context_child_objects_t child_objects;
} gfx_context_data_t;



typedef sll_size_t gfx_context_t;



extern sll_handle_container_t gfx_context_data;



void _delete_context(gfx_context_data_t* ctx);



void _flush_command_buffers(gfx_context_data_t* ctx);



__GFX_API_CALL gfx_context_t gfx_api_context_create(void* handle,void* extra_data,sll_bool_t debug);



__GFX_API_CALL void gfx_api_context_delete(gfx_context_t ctx_id);



__GFX_API_CALL void gfx_api_context_render(gfx_context_t ctx_id);



__GFX_API_CALL void gfx_api_context_resize(gfx_context_t ctx_id);



#endif

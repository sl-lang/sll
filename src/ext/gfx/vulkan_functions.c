#include <gfx/common.h>
#include <gfx/vulkan_functions.h>
#include <sll.h>
#include <vulkan/vulkan.h>



#define _LOAD_FUNCTION_STR(name) _LOAD_FUNCTION_STR_(name)
#define _LOAD_FUNCTION_STR_(name) #name
#define LOAD_FUNCTION(name) \
	if (!(name=(_GFX_FUNCTION_PROTOTYPE(name))vkGetInstanceProcAddr(NULL,_LOAD_FUNCTION_STR(name)))){ \
		SLL_WARN("Failed to load Vulkan function '%s'!",_LOAD_FUNCTION_STR(name)); \
		ret=0; \
	}
#define LOAD_FUNCTION_TABLE(name) LOAD_FUNCTION_TABLE_VAR(name,name)
#define LOAD_FUNCTION_TABLE_VAR(var,name) \
	if (!(out->var=(_GFX_FUNCTION_PROTOTYPE(name))vkGetInstanceProcAddr(instance,_LOAD_FUNCTION_STR(name)))){ \
		SLL_WARN("Failed to load Vulkan function '%s'!",_LOAD_FUNCTION_STR(name)); \
		ret=0; \
	}



PFN_vkCreateInstance vkCreateInstance;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;



sll_bool_t _load_vulkan_functions(PFN_vkGetInstanceProcAddr get_proc_addr){
	if (!get_proc_addr){
		SLL_WARN("Failed to load Vulkan function 'vkGetInstanceProcAddr'!");
		return 0;
	}
	vkGetInstanceProcAddr=get_proc_addr;
	sll_bool_t ret=1;
	LOAD_FUNCTION(vkCreateInstance);
	LOAD_FUNCTION(vkEnumerateInstanceExtensionProperties);
	LOAD_FUNCTION(vkEnumerateInstanceLayerProperties);
	return ret;
}



sll_bool_t _load_vulkan_function_table(VkInstance instance,gfx_vulkan_function_table_t* out){
	sll_bool_t ret=1;
	LOAD_FUNCTION_TABLE_VAR(create_system_surface,GFX_VULKAN_SYSTEM_SURFACE_CREATION_FUNCTION);
	out->vkCreateDebugUtilsMessengerEXT=(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,"vkCreateDebugUtilsMessengerEXT");
	out->vkDestroyDebugUtilsMessengerEXT=(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance,"vkDestroyDebugUtilsMessengerEXT");
	out->has_debug_utils=out->vkCreateDebugUtilsMessengerEXT&&out->vkDestroyDebugUtilsMessengerEXT;
	LOAD_FUNCTION_TABLE(vkAcquireNextImageKHR);
	LOAD_FUNCTION_TABLE(vkAllocateCommandBuffers);
	LOAD_FUNCTION_TABLE(vkAllocateDescriptorSets);
	LOAD_FUNCTION_TABLE(vkAllocateMemory);
	LOAD_FUNCTION_TABLE(vkBeginCommandBuffer);
	LOAD_FUNCTION_TABLE(vkBindBufferMemory);
	LOAD_FUNCTION_TABLE(vkBindImageMemory);
	LOAD_FUNCTION_TABLE(vkCmdBeginRenderPass);
	LOAD_FUNCTION_TABLE(vkCmdBindDescriptorSets);
	LOAD_FUNCTION_TABLE(vkCmdBindIndexBuffer);
	LOAD_FUNCTION_TABLE(vkCmdBindPipeline);
	LOAD_FUNCTION_TABLE(vkCmdBindVertexBuffers);
	LOAD_FUNCTION_TABLE(vkCmdCopyBuffer);
	LOAD_FUNCTION_TABLE(vkCmdCopyBufferToImage);
	LOAD_FUNCTION_TABLE(vkCmdDrawIndexed);
	LOAD_FUNCTION_TABLE(vkCmdEndRenderPass);
	LOAD_FUNCTION_TABLE(vkCmdPipelineBarrier);
	LOAD_FUNCTION_TABLE(vkCmdPushConstants);
	LOAD_FUNCTION_TABLE(vkCmdSetScissor);
	LOAD_FUNCTION_TABLE(vkCmdSetViewport);
	LOAD_FUNCTION_TABLE(vkCreateBuffer);
	LOAD_FUNCTION_TABLE(vkCreateCommandPool);
	LOAD_FUNCTION_TABLE(vkCreateDescriptorPool);
	LOAD_FUNCTION_TABLE(vkCreateDescriptorSetLayout);
	LOAD_FUNCTION_TABLE(vkCreateDevice);
	LOAD_FUNCTION_TABLE(vkCreateFence);
	LOAD_FUNCTION_TABLE(vkCreateFramebuffer);
	LOAD_FUNCTION_TABLE(vkCreateGraphicsPipelines);
	LOAD_FUNCTION_TABLE(vkCreateImage);
	LOAD_FUNCTION_TABLE(vkCreateImageView);
	LOAD_FUNCTION_TABLE(vkCreatePipelineCache);
	LOAD_FUNCTION_TABLE(vkCreatePipelineLayout);
	LOAD_FUNCTION_TABLE(vkCreateRenderPass);
	LOAD_FUNCTION_TABLE(vkCreateSampler);
	LOAD_FUNCTION_TABLE(vkCreateSemaphore);
	LOAD_FUNCTION_TABLE(vkCreateShaderModule);
	LOAD_FUNCTION_TABLE(vkCreateSwapchainKHR);
	LOAD_FUNCTION_TABLE(vkDestroyBuffer);
	LOAD_FUNCTION_TABLE(vkDestroyCommandPool);
	LOAD_FUNCTION_TABLE(vkDestroyDescriptorPool);
	LOAD_FUNCTION_TABLE(vkDestroyDescriptorSetLayout);
	LOAD_FUNCTION_TABLE(vkDestroyDevice);
	LOAD_FUNCTION_TABLE(vkDestroyFence)
	LOAD_FUNCTION_TABLE(vkDestroyFramebuffer);
	LOAD_FUNCTION_TABLE(vkDestroyImage);
	LOAD_FUNCTION_TABLE(vkDestroyImageView);
	LOAD_FUNCTION_TABLE(vkDestroyInstance);
	LOAD_FUNCTION_TABLE(vkDestroyPipeline);
	LOAD_FUNCTION_TABLE(vkDestroyPipelineCache);
	LOAD_FUNCTION_TABLE(vkDestroyPipelineLayout);
	LOAD_FUNCTION_TABLE(vkDestroyRenderPass);
	LOAD_FUNCTION_TABLE(vkDestroySampler);
	LOAD_FUNCTION_TABLE(vkDestroySemaphore);
	LOAD_FUNCTION_TABLE(vkDestroyShaderModule);
	LOAD_FUNCTION_TABLE(vkDestroySurfaceKHR);
	LOAD_FUNCTION_TABLE(vkDestroySwapchainKHR);
	LOAD_FUNCTION_TABLE(vkDeviceWaitIdle);
	LOAD_FUNCTION_TABLE(vkEndCommandBuffer);
	LOAD_FUNCTION_TABLE(vkEnumeratePhysicalDevices);
	LOAD_FUNCTION_TABLE(vkFlushMappedMemoryRanges);
	LOAD_FUNCTION_TABLE(vkFreeCommandBuffers);
	LOAD_FUNCTION_TABLE(vkFreeDescriptorSets);
	LOAD_FUNCTION_TABLE(vkFreeMemory);
	LOAD_FUNCTION_TABLE(vkGetBufferMemoryRequirements);
	LOAD_FUNCTION_TABLE(vkGetDeviceQueue);
	LOAD_FUNCTION_TABLE(vkGetImageMemoryRequirements);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceMemoryProperties);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceProperties);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceQueueFamilyProperties);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceSurfaceFormatsKHR);
	LOAD_FUNCTION_TABLE(vkGetPhysicalDeviceSurfaceSupportKHR);
	LOAD_FUNCTION_TABLE(vkGetSwapchainImagesKHR);
	LOAD_FUNCTION_TABLE(vkMapMemory);
	LOAD_FUNCTION_TABLE(vkQueuePresentKHR);
	LOAD_FUNCTION_TABLE(vkQueueSubmit);
	LOAD_FUNCTION_TABLE(vkQueueWaitIdle);
	LOAD_FUNCTION_TABLE(vkResetFences);
	LOAD_FUNCTION_TABLE(vkUnmapMemory);
	LOAD_FUNCTION_TABLE(vkUpdateDescriptorSets);
	LOAD_FUNCTION_TABLE(vkWaitForFences);
	return ret;
}

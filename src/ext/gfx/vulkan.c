#include <gfx/common.h>
#include <gfx/vulkan.h>
#include <gfx/vulkan_functions.h>
#include <sll.h>
#include <vulkan/vulkan.h>



static sll_library_handle_t _vulkan_library_handle;



const char* vulkan_validation_layer_name=NULL;



void _check_error_code(const char* str,VkResult err){
	const char* err_str;
	switch (err){
		case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
			err_str="VK_ERROR_COMPRESSION_EXHAUSTED_EXT";
			break;
		case VK_ERROR_DEVICE_LOST:
			err_str="VK_ERROR_DEVICE_LOST";
			break;
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			err_str="VK_ERROR_EXTENSION_NOT_PRESENT";
			break;
		case VK_ERROR_FEATURE_NOT_PRESENT:
			err_str="VK_ERROR_FEATURE_NOT_PRESENT";
			break;
		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			err_str="VK_ERROR_FORMAT_NOT_SUPPORTED";
			break;
		case VK_ERROR_FRAGMENTATION:
			err_str="VK_ERROR_FRAGMENTATION";
			break;
		case VK_ERROR_FRAGMENTED_POOL:
			err_str="VK_ERROR_FRAGMENTED_POOL";
			break;
		case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
			err_str="VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
			break;
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			err_str="VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
			break;
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			err_str="VK_ERROR_INCOMPATIBLE_DRIVER";
			break;
		case VK_ERROR_INITIALIZATION_FAILED:
			err_str="VK_ERROR_INITIALIZATION_FAILED";
			break;
		case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
			err_str="VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
			break;
		case VK_ERROR_INVALID_EXTERNAL_HANDLE:
			err_str="VK_ERROR_INVALID_EXTERNAL_HANDLE";
			break;
		case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
			err_str="VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
			break;
		case VK_ERROR_INVALID_SHADER_NV:
			err_str="VK_ERROR_INVALID_SHADER_NV";
			break;
		case VK_ERROR_LAYER_NOT_PRESENT:
			err_str="VK_ERROR_LAYER_NOT_PRESENT";
			break;
		case VK_ERROR_MEMORY_MAP_FAILED:
			err_str="VK_ERROR_MEMORY_MAP_FAILED";
			break;
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			err_str="VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
			break;
		case VK_ERROR_NOT_PERMITTED_KHR:
			err_str="VK_ERROR_NOT_PERMITTED_KHR";
			break;
		case VK_ERROR_OUT_OF_DATE_KHR:
			err_str="VK_ERROR_OUT_OF_DATE_KHR";
			break;
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			err_str="VK_ERROR_OUT_OF_DEVICE_MEMORY";
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			err_str="VK_ERROR_OUT_OF_HOST_MEMORY";
			break;
		case VK_ERROR_OUT_OF_POOL_MEMORY:
			err_str="VK_ERROR_OUT_OF_POOL_MEMORY";
			break;
		case VK_ERROR_SURFACE_LOST_KHR:
			err_str="VK_ERROR_SURFACE_LOST_KHR";
			break;
		case VK_ERROR_TOO_MANY_OBJECTS:
			err_str="VK_ERROR_TOO_MANY_OBJECTS";
			break;
		case VK_ERROR_UNKNOWN:
			err_str="VK_ERROR_UNKNOWN";
			break;
		case VK_ERROR_VALIDATION_FAILED_EXT:
			err_str="VK_ERROR_VALIDATION_FAILED_EXT";
			break;
		case VK_EVENT_RESET:
			err_str="VK_EVENT_RESET";
			break;
		case VK_EVENT_SET:
			err_str="VK_EVENT_SET";
			break;
		case VK_INCOMPLETE:
			err_str="VK_INCOMPLETE";
			break;
		case VK_NOT_READY:
			err_str="VK_NOT_READY";
			break;
		case VK_OPERATION_DEFERRED_KHR:
			err_str="VK_OPERATION_DEFERRED_KHR";
			break;
		case VK_OPERATION_NOT_DEFERRED_KHR:
			err_str="VK_OPERATION_NOT_DEFERRED_KHR";
			break;
		case VK_PIPELINE_COMPILE_REQUIRED:
			err_str="VK_PIPELINE_COMPILE_REQUIRED";
			break;
		case VK_SUBOPTIMAL_KHR:
			err_str="VK_SUBOPTIMAL_KHR";
			break;
		case VK_SUCCESS:
			err_str="VK_SUCCESS";
			break;
		case VK_THREAD_DONE_KHR:
			err_str="VK_THREAD_DONE_KHR";
			break;
		case VK_THREAD_IDLE_KHR:
			err_str="VK_THREAD_IDLE_KHR";
			break;
		case VK_TIMEOUT:
			err_str="VK_TIMEOUT";
			break;
		default:
			err_str="Unhandled VkResult";
			break;
	}
	SLL_WARN("%s: %s",str,err_str);
}



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
	if (!_vulkan_library_handle||!_load_vulkan_functions(sll_platform_lookup_symbol(_vulkan_library_handle,SLL_CHAR("vkGetInstanceProcAddr")))){
		return 0;
	}
	uint32_t count;
	VULKAN_CALL(vkEnumerateInstanceExtensionProperties(NULL,&count,NULL));
	VkExtensionProperties* extension_properties=sll_allocate_stack(count*sizeof(VkExtensionProperties));
	VULKAN_CALL(vkEnumerateInstanceExtensionProperties(NULL,&count,extension_properties));
	sll_bool_t has_vk_khr_surface=0;
	sll_bool_t has_system_surface=0;
	for (uint32_t i=0;i<count;i++){
		if (sll_string_compare_pointer(SLL_CHAR((extension_properties+i)->extensionName),SLL_CHAR(VK_KHR_SURFACE_EXTENSION_NAME))==SLL_COMPARE_RESULT_EQUAL){
			has_vk_khr_surface=1;
		}
		else if (sll_string_compare_pointer(SLL_CHAR((extension_properties+i)->extensionName),SLL_CHAR(GFX_VULKAN_REQUIRED_EXTENSION_NAME))==SLL_COMPARE_RESULT_EQUAL){
			has_system_surface=1;
		}
	}
	sll_deallocate(extension_properties);
	vulkan_validation_layer_name=NULL;
#ifdef DEBUG_BUILD
	VULKAN_CALL(vkEnumerateInstanceLayerProperties(&count,NULL));
	VkLayerProperties* layer_properties=sll_allocate_stack(count*sizeof(VkLayerProperties));
	VULKAN_CALL(vkEnumerateInstanceLayerProperties(&count,layer_properties));
	for (uint32_t i=0;i<count;i++){
		if (sll_string_compare_pointer(SLL_CHAR((layer_properties+i)->layerName),SLL_CHAR("VK_LAYER_KHRONOS_validation"))==SLL_COMPARE_RESULT_EQUAL){
			vulkan_validation_layer_name="VK_LAYER_KHRONOS_validation";
			break;
		}
	}
	sll_deallocate(layer_properties);
#endif
	return has_vk_khr_surface&&has_system_surface;
}



__GFX_API_CALL void gfx_api_vulkan_get_extensions(sll_array_t* out){
	uint32_t count;
	VULKAN_CALL(vkEnumerateInstanceExtensionProperties(NULL,&count,NULL));
	VkExtensionProperties* extension_properties=sll_allocate_stack(count*sizeof(VkExtensionProperties));
	VULKAN_CALL(vkEnumerateInstanceExtensionProperties(NULL,&count,extension_properties));
	sll_array_create(count,out);
	for (uint32_t i=0;i<count;i++){
		out->data[i]=sll_string_pointer_to_object(SLL_CHAR((extension_properties+i)->extensionName));
	}
	sll_deallocate(extension_properties);
}



__GFX_API_CALL void gfx_api_vulkan_get_version(sll_array_t* out){
	sll_new_object_array(SLL_CHAR("uuuu"),out,VK_API_VERSION_VARIANT(VK_HEADER_VERSION_COMPLETE),VK_API_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE),VK_API_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE),VK_API_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE));
}

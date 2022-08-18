#include <gfx/common.h>
#include <sll.h>
#include <vulkan/vulkan.h>



#define CHECK_VULKAN_ERROR(err) \
	do{ \
		VkResult __err=(err); \
		if (__err){ \
			SLL_WARN("%s: %u",_CHECK_VULKAN_ERROR_STR(err),err); \
		} \
	} while (0)
#define _CHECK_VULKAN_ERROR_STR(err) _CHECK_VULKAN_ERROR_STR_(err)
#define _CHECK_VULKAN_ERROR_STR_(err) #err



static sll_library_handle_t _vulkan_library_handle;
static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
static PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;



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
	vkGetInstanceProcAddr=sll_platform_lookup_symbol(_vulkan_library_handle,SLL_CHAR("vkGetInstanceProcAddr"));
	if (!vkGetInstanceProcAddr){
		return 0;
	}
	vkEnumerateInstanceExtensionProperties=(PFN_vkEnumerateInstanceExtensionProperties)vkGetInstanceProcAddr(NULL,"vkEnumerateInstanceExtensionProperties");
	if (!vkEnumerateInstanceExtensionProperties){
		return 0;
	}
	uint32_t count;
	CHECK_VULKAN_ERROR(vkEnumerateInstanceExtensionProperties(NULL,&count,NULL));
	VkExtensionProperties* extension_properties=sll_allocate_stack(count*sizeof(VkExtensionProperties));
	CHECK_VULKAN_ERROR(vkEnumerateInstanceExtensionProperties(NULL,&count,extension_properties));
	sll_bool_t has_vk_khr_surface=0;
	sll_bool_t has_system_surface=0;
	for (uint32_t i=0;i<count;i++){
		if (sll_string_compare_pointer(SLL_CHAR((extension_properties+i)->extensionName),SLL_CHAR("VK_KHR_surface"))==SLL_COMPARE_RESULT_EQUAL){
			has_vk_khr_surface=1;
		}
		else if (sll_string_compare_pointer(SLL_CHAR((extension_properties+i)->extensionName),SLL_CHAR(GFX_VULKAN_REQUIRED_EXTENSION_NAME))==SLL_COMPARE_RESULT_EQUAL){
			has_system_surface=1;
		}
	}
	sll_deallocate(extension_properties);
	return has_vk_khr_surface&&has_system_surface;
}



__GFX_API_CALL void gfx_api_vulkan_get_extensions(sll_array_t* out){
	uint32_t count;
	CHECK_VULKAN_ERROR(vkEnumerateInstanceExtensionProperties(NULL,&count,NULL));
	VkExtensionProperties* extension_properties=sll_allocate_stack(count*sizeof(VkExtensionProperties));
	CHECK_VULKAN_ERROR(vkEnumerateInstanceExtensionProperties(NULL,&count,extension_properties));
	sll_object_t object=sll_new_object(SLL_CHAR("{S}"),extension_properties,(sll_size_t)count,sizeof(VkExtensionProperties),SLL_OFFSETOF(VkExtensionProperties,extensionName));
	sll_deallocate(extension_properties);
	*out=object->data.array;
	sll_error_raise_bool(sll_destroy_object(object));
}



__GFX_API_CALL void gfx_api_vulkan_get_version(sll_array_t* out){
	sll_new_object_array(SLL_CHAR("uuuu"),out,VK_API_VERSION_VARIANT(VK_HEADER_VERSION_COMPLETE),VK_API_VERSION_MAJOR(VK_HEADER_VERSION_COMPLETE),VK_API_VERSION_MINOR(VK_HEADER_VERSION_COMPLETE),VK_API_VERSION_PATCH(VK_HEADER_VERSION_COMPLETE));
}

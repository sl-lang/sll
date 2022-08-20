#ifndef __GFX_VULKAN_H__
#define __GFX_VULKAN_H__ 1
#include <gfx/common.h>
#include <sll.h>
#include <vulkan/vulkan.h>



#define VULKAN_CALL(err) \
	do{ \
		VkResult __err=(err); \
		if (__err){ \
			_check_error_code(_VULKAN_CALL_STR(err),__err); \
		} \
	} while (0)
#define _VULKAN_CALL_STR(err) _VULKAN_CALL_STR_(err)
#define _VULKAN_CALL_STR_(err) #err



extern const char* vulkan_validation_layer_name;



void _check_error_code(const char* str,VkResult err);



void _deinit_vulkan(void);



sll_bool_t _init_vulkan(void);



__GFX_API_CALL void gfx_api_vulkan_get_extensions(sll_array_t* out);



__GFX_API_CALL void gfx_api_vulkan_get_version(sll_array_t* out);



#endif

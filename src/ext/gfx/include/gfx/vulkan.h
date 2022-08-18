#ifndef __GFX_VULKAN_H__
#define __GFX_VULKAN_H__ 1
#include <gfx/common.h>
#include <sll.h>



void _deinit_vulkan(void);



sll_bool_t _init_vulkan(void);



__GFX_API_CALL void gfx_api_vulkan_get_extensions(sll_array_t* out);



__GFX_API_CALL void gfx_api_vulkan_get_version(sll_array_t* out);



#endif

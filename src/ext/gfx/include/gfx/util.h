#ifndef __GFX_UTIL_H__
#define __GFX_UTIL_H__ 1
#include <gfx/context.h>
#include <stdint.h>
#include <vulkan/vulkan.h>



#define GFX_DATA_FORMAT_SIZE_8BIT 0
#define GFX_DATA_FORMAT_SIZE_16BIT 1
#define GFX_DATA_FORMAT_SIZE_32BIT 2
#define GFX_DATA_FORMAT_SIZE_64BIT 3
#define GFX_DATA_FORMAT_SIZE_MASK 3

#define GFX_DATA_FORMAT_COUNT_1 0
#define GFX_DATA_FORMAT_COUNT_2 4
#define GFX_DATA_FORMAT_COUNT_3 8
#define GFX_DATA_FORMAT_COUNT_4 12
#define GFX_DATA_FORMAT_COUNT_MASK 12

#define GFX_DATA_FORMAT_TYPE_UINT 0
#define GFX_DATA_FORMAT_TYPE_SINT 16
#define GFX_DATA_FORMAT_TYPE_FLOAT 32
#define GFX_DATA_FORMAT_TYPE_COLOR_UNORM 48
#define GFX_DATA_FORMAT_TYPE_COLOR_SNORM 64
#define GFX_DATA_FORMAT_TYPE_COLOR_SRGB 80
#define GFX_DATA_FORMAT_TYPE_MASK 112

#define GFX_DATA_FORMAT_FLAG_BGR 128

#define GFX_SHADER_STAGE_VERTEX 1
#define GFX_SHADER_STAGE_TESSELLATION_CONTROL 2
#define GFX_SHADER_STAGE_TESSELLATION_EVALUATION 4
#define GFX_SHADER_STAGE_GEOMETRY 8
#define GFX_SHADER_STAGE_FRAGMENT 16



typedef uint8_t gfx_data_format_t;



typedef uint8_t gfx_shader_stage_t;



uint32_t _get_memory_type(const gfx_context_data_t* ctx,uint32_t mask,VkMemoryPropertyFlagBits properties);



VkFormat _encode_data_format(gfx_data_format_t data_format);



VkShaderStageFlags _encode_shader_stages(gfx_shader_stage_t stage);



#endif

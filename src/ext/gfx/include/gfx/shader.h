#ifndef __GFX_SHADER_H__
#define __GFX_SHADER_H__ 1
#include <gfx/common.h>
#include <sll.h>
#include <stdint.h>



#define GFX_SHADER_STAGE_VERTEX 0
#define GFX_SHADER_STAGE_TESSCONTROL 1
#define GFX_SHADER_STAGE_TESSEVALUATION 2
#define GFX_SHADER_STAGE_GEOMETRY 3
#define GFX_SHADER_STAGE_FRAGMENT 4



typedef uint8_t gfx_shader_stage_t;



__GFX_API_CALL void gfx_api_shader_compile(const sll_string_t* code,gfx_shader_stage_t stage,sll_string_t* out);



#endif

#ifndef __GFX_SHADER_H__
#define __GFX_SHADER_H__ 1
#include <gfx/_shader_type.h>
#include <gfx/common.h>
#include <gfx/context.h>
#include <sll.h>



typedef sll_size_t gfx_shader_t;



void _delete_shader(const gfx_context_data_t* ctx,gfx_shader_data_t* shader_data);



__GFX_API_CALL gfx_shader_t gfx_api_shader_create(gfx_context_t ctx_id,const sll_string_t* bytecode);



__GFX_API_CALL void gfx_api_shader_delete(gfx_context_t ctx_id,gfx_shader_t shader_id);



#endif

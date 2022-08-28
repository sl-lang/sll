#include <gfx/buffer.h>
#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/texture.h>
#include <gfx/util.h>
#include <gfx/vulkan.h>
#include <sll.h>
#include <stdint.h>
#include <vulkan/vulkan.h>



void _delete_texture(const gfx_context_data_t* ctx,gfx_texture_data_t* texture_data){
	sll_deallocate(texture_data);
}



__GFX_API_CALL gfx_texture_t gfx_api_texture_create(gfx_context_t ctx_id,uint32_t width,uint32_t height,uint32_t depth,gfx_data_format_t format,gfx_buffer_t buffer_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return 0;
	}
	gfx_texture_data_t* texture=sll_allocate(sizeof(gfx_texture_data_t));
	texture->data_buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->buffers),buffer_id);
	texture->format=format;
	texture->width=width;
	texture->height=height;
	texture->depth=depth;
	gfx_texture_t out;
	SLL_HANDLE_CONTAINER_ALLOC(&(ctx->textures),&out);
	*(ctx->textures.data+out)=texture;
	return out;
}



__GFX_API_CALL void gfx_api_texture_delete(gfx_context_t ctx_id,gfx_texture_t texture_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_texture_data_t* texture=SLL_HANDLE_CONTAINER_GET(&(ctx->textures),texture_id);
	if (texture){
		SLL_HANDLE_CONTAINER_DEALLOC(&(ctx->textures),texture_id);
		_delete_texture(ctx,texture);
	}
}



__GFX_API_CALL void gfx_api_texture_sync(gfx_context_t ctx_id,gfx_texture_t texture_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_texture_data_t* texture=SLL_HANDLE_CONTAINER_GET(&(ctx->textures),texture_id);
	if (!texture){
		return;
	}
}

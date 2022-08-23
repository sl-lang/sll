#include <gfx/buffer.h>
#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/vulkan.h>
#include <sll.h>
#include <vulkan/vulkan.h>



void _delete_buffer(const gfx_context_data_t* ctx,gfx_buffer_data_t* buffer_data){
	sll_deallocate(buffer_data);
}



__GFX_API_CALL gfx_buffer_t gfx_api_buffer_create(gfx_context_t ctx_id,gfx_buffer_type_t type){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return 0;
	}
	gfx_buffer_data_t* buffer=sll_allocate(sizeof(gfx_buffer_data_t));
	buffer->type=type;
	gfx_buffer_t out;
	SLL_HANDLE_CONTAINER_ALLOC(&(ctx->buffers),&out);
	*(ctx->buffers.data+out)=buffer;
	return out;
}



__GFX_API_CALL void gfx_api_buffer_delete(gfx_context_t ctx_id,gfx_buffer_t buffer_id){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	gfx_buffer_data_t* buffer=SLL_HANDLE_CONTAINER_GET(&(ctx->buffers),buffer_id);
	if (buffer){
		SLL_HANDLE_CONTAINER_DEALLOC(&(ctx->buffers),buffer_id);
		_delete_buffer(ctx,buffer);
	}
}

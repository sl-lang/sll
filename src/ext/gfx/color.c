#include <gfx/common.h>
#include <gfx/context.h>
#include <sll.h>



__GFX_API_CALL void gfx_api_color_set_clear_color(gfx_context_t ctx_id,sll_float_t red,sll_float_t green,sll_float_t blue,sll_float_t alpha){
	gfx_context_data_t* ctx=SLL_HANDLE_CONTAINER_GET(&gfx_context_data,ctx_id);
	if (!ctx){
		return;
	}
	ctx->clear_color[0]=(float)red;
	ctx->clear_color[1]=(float)green;
	ctx->clear_color[2]=(float)blue;
	ctx->clear_color[3]=(float)alpha;
}

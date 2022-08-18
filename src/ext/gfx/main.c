#include <gfx/common.h>
#include <gfx/context.h>
#include <gfx/vulkan.h>
#include <sll.h>



__GFX_EXTERNAL sll_bool_t SLL_ABI_INIT(sll_version_t version){
	if (version!=SLL_VERSION||!_init_vulkan()){
		return 0;
	}
	SLL_HANDLE_CONTAINER_INIT(&gfx_context_data);
	return 1;
}



__GFX_EXTERNAL void SLL_ABI_DEINIT(void){
	SLL_HANDLE_CONTAINER_ITER_CLEAR(&gfx_context_data,gfx_context_data_t,ctx,{
		_delete_context(ctx);
	});
	_deinit_vulkan();
}

#include <gfx/common.h>
#include <gfx/vulkan.h>
#include <sll.h>



__GFX_EXTERNAL sll_bool_t SLL_ABI_INIT(sll_version_t version){
	return (version==SLL_VERSION&&_init_vulkan());
}



__GFX_EXTERNAL void SLL_ABI_DEINIT(void){
	_deinit_vulkan();
}

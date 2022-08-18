#include <gfx/common.h>
#include <gfx/vulkan.h>
#include <sll.h>



__GFX_EXTERNAL sll_bool_t SLL_ABI_INIT(sll_version_t version){
	if (version!=SLL_VERSION){
		return 0;
	}
	return _init_vulkan();
}



__GFX_EXTERNAL void SLL_ABI_DEINIT(void){
}

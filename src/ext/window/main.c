#include <window/common.h>
#include <sll.h>



__WINDOW_EXTERNAL sll_bool_t SLL_ABI_INIT(sll_version_t version){
	if (version!=SLL_VERSION){
		return 0;
	}
	_init_platform();
	return 1;
}



__WINDOW_EXTERNAL void SLL_ABI_DEINIT(void){
	_deinit_platform();
}

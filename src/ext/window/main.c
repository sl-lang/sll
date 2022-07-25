#include <window/common.h>
#include <window/keyboard.h>
#include <sll.h>



__WINDOW_EXTERNAL sll_bool_t SLL_ABI_INIT(sll_version_t version){
	if (version!=SLL_VERSION){
		return 0;
	}
	_init_platform();
	sll_zero_memory(_scancode_to_keycode,512*sizeof(window_key_t));
	sll_zero_memory(_keycode_to_scancode,WINDOW_MAX_KEY+1);
	_setup_key_mapping();
	return 1;
}



__WINDOW_EXTERNAL void SLL_ABI_DEINIT(void){
	_deinit_platform();
}

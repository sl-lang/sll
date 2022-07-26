#include <window/common.h>
#include <window/keyboard.h>
#include <sll.h>



__WINDOW_EXTERNAL sll_bool_t SLL_ABI_INIT(sll_version_t version){
	if (version!=SLL_VERSION||!_init_platform()){
		return 0;
	}
	sll_zero_memory(_scancode_to_keycode,512*sizeof(window_key_t));
	sll_zero_memory(_keycode_to_scancode,(WINDOW_MAX_KEY+1)*sizeof(window_scancode_t));
	_setup_key_mapping();
	for (window_scancode_t i=0;i<512;i++){
		if (_scancode_to_keycode[i]){
			_keycode_to_scancode[_scancode_to_keycode[i]]=i;
		}
	}
	return 1;
}



__WINDOW_EXTERNAL void SLL_ABI_DEINIT(void){
	_deinit_platform();
}

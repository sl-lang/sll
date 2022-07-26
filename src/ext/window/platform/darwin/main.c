#include <stddef.h>
#include <window/common.h>



void _deinit_platform(void){
}



sll_bool_t _init_platform(void){
	return 1;
}



__WINDOW_API_CALL void* window_api_get_context(void){
	return NULL;
}

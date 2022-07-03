#include <sll.h>
#include <stdint.h>
#include <window/common.h>



__WINDOW_API_CALL void* window_api_window_create(int32_t x,int32_t y,uint32_t w,uint32_t h,void* parent){
	return (void*)0;
}



__WINDOW_API_CALL void window_api_window_destroy(void* id){
}



__WINDOW_API_CALL void window_api_window_set_title(void* id,const sll_string_t* name){
}



__WINDOW_API_CALL void window_api_window_set_visibility(void* id,sll_bool_t show){
}

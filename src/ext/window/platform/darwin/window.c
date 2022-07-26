#include <sll.h>
#include <stdint.h>
#include <window/common.h>
#include <window/window.h>



__WINDOW_API_CALL window_handle_t window_api_window_create(int32_t x,int32_t y,uint32_t w,uint32_t h,window_handle_t parent){
	return (window_handle_t)0;
}



__WINDOW_API_CALL void window_api_window_destroy(window_handle_t id){
}



__WINDOW_API_CALL void window_api_window_set_geometry(window_handle_t id,int32_t x,int32_t y,uint32_t w,uint32_t h){
}



__WINDOW_API_CALL void window_api_window_set_icon(window_handle_t id,uint32_t width,const sll_string_t* data){
}



__WINDOW_API_CALL void window_api_window_set_size_constraints(window_handle_t id,uint32_t min_w,uint32_t min_h,uint32_t max_w,uint32_t max_h){
}



__WINDOW_API_CALL void window_api_window_set_state(window_handle_t id,window_state_t state){
}



__WINDOW_API_CALL void window_api_window_set_title(window_handle_t id,const sll_string_t* name){
}



__WINDOW_API_CALL void window_api_window_set_visibility(window_handle_t id,sll_bool_t show){
}

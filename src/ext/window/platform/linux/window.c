#include <sll.h>
#include <stdint.h>
#include <window/common.h>



__WINDOW_API_CALL void* window_api_window_create(int32_t x,int32_t y,uint32_t w,uint32_t h,void* parent){
	xcb_window_t id=xcb_generate_id(_xcb_conn);
	return (void*)id;
}



__WINDOW_API_CALL void window_api_window_update_visibility(void* id,sll_bool_t show){
	return;
}

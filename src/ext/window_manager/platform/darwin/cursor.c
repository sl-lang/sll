#include <sll.h>
#include <stdint.h>
#include <window_manager/common.h>
#include <window_manager/cursor.h>
#include <window_manager/window.h>



__WINDOW_MANAGER_API_CALL void window_api_cursor_apply_to_window(window_handle_t id,window_cursor_t cursor){
}



__WINDOW_MANAGER_API_CALL void window_api_cursor_get_position(sll_array_t* out){
	SLL_INIT_ARRAY(out);
}




__WINDOW_MANAGER_API_CALL void window_api_cursor_set_position(int32_t x,int32_t y){
}

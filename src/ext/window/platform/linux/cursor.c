#include <sll.h>
#include <stdint.h>
#include <stdlib.h>
#include <window/common.h>
#include <window/cursor.h>
#include <window/platform/linux.h>
#include <window/window.h>
#include <xcb/xcb.h>



__WINDOW_API_CALL void window_api_cursor_apply_to_window(window_handle_t id,window_cursor_t cursor){
	if (cursor>WINDOW_MAX_CURSOR){
		cursor=WINDOW_CURSOR_DEFAULT;
	}
	xcb_change_window_attributes(_xcb_conn,(int)(intptr_t)id,XCB_CW_CURSOR,_xcb_cursors+cursor);
	xcb_flush(_xcb_conn);
}



__WINDOW_API_CALL void window_api_cursor_get_position(sll_array_t* out){
	xcb_query_pointer_reply_t* data=xcb_query_pointer_reply(_xcb_conn,xcb_query_pointer(_xcb_conn,_xcb_screen->root),NULL);
	sll_new_object_array(SLL_CHAR("uu"),out,data->root_x,data->root_y);
	free(data);
}



__WINDOW_API_CALL void window_api_cursor_set_position(int32_t x,int32_t y){
	xcb_warp_pointer(_xcb_conn,XCB_NONE,_xcb_screen->root,0,0,0,0,x,y);
	xcb_flush(_xcb_conn);
}

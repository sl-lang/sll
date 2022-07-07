#include <sll.h>
#include <stdint.h>
#include <stdlib.h>
#include <window/common.h>



__WINDOW_API_CALL void window_api_cursor_get_position(sll_array_t* out){
	xcb_query_pointer_reply_t* data=xcb_query_pointer_reply(_xcb_conn,xcb_query_pointer(_xcb_conn,_xcb_screen->root),NULL);
	sll_new_object_array(SLL_CHAR("uu"),out,data->root_x,data->root_y);
	free(data);
}



__WINDOW_API_CALL void window_api_cursor_set_position(int32_t x,int32_t y){
	xcb_warp_pointer(_xcb_conn,XCB_NONE,_xcb_screen->root,0,0,0,0,x,y);
	xcb_flush(_xcb_conn);
}

#include <sll.h>
#include <stdint.h>
#include <window/common.h>



__WINDOW_API_CALL void* window_api_window_create(int32_t x,int32_t y,uint32_t w,uint32_t h,void* parent){
	xcb_window_t id=xcb_generate_id(_xcb_conn);
	xcb_create_window(_xcb_conn,XCB_COPY_FROM_PARENT,id,(parent==(void*)(0xffffffffffffffffull)?_xcb_screen->root:(int)(intptr_t)parent),x,y,w,h,10,XCB_WINDOW_CLASS_INPUT_OUTPUT,_xcb_screen->root_visual,0,NULL);
	xcb_flush(_xcb_conn);
	return (void*)(intptr_t)id;
}



__WINDOW_API_CALL void window_api_window_set_visibility(void* id,sll_bool_t show){
	(show?xcb_map_window:xcb_unmap_window)(_xcb_conn,(int)(intptr_t)id);
	xcb_flush(_xcb_conn);
}

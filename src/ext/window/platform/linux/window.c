#include <sll.h>
#include <stdint.h>
#include <stdlib.h>
#include <window/common.h>



__WINDOW_API_CALL void* window_api_window_create(int32_t x,int32_t y,uint32_t w,uint32_t h,void* parent){
	xcb_window_t id=xcb_generate_id(_xcb_conn);
	const uint32_t data[1]={XCB_EVENT_MASK_KEY_PRESS|XCB_EVENT_MASK_KEY_RELEASE|XCB_EVENT_MASK_BUTTON_PRESS|XCB_EVENT_MASK_BUTTON_RELEASE|XCB_EVENT_MASK_ENTER_WINDOW|XCB_EVENT_MASK_LEAVE_WINDOW|XCB_EVENT_MASK_POINTER_MOTION|XCB_EVENT_MASK_EXPOSURE|XCB_EVENT_MASK_VISIBILITY_CHANGE|XCB_EVENT_MASK_STRUCTURE_NOTIFY|XCB_EVENT_MASK_FOCUS_CHANGE|XCB_EVENT_MASK_PROPERTY_CHANGE};
	xcb_create_window(_xcb_conn,XCB_COPY_FROM_PARENT,id,(parent==(void*)(0xffffffffffffffffull)?_xcb_screen->root:(int)(intptr_t)parent),x,y,w,h,10,XCB_WINDOW_CLASS_INPUT_OUTPUT,_xcb_screen->root_visual,XCB_CW_EVENT_MASK,data);
	xcb_flush(_xcb_conn);
	return (void*)(intptr_t)id;
}



__WINDOW_API_CALL void window_api_window_destroy(void* id){
	xcb_destroy_window(_xcb_conn,(int)(intptr_t)id);
	xcb_flush(_xcb_conn);
}



__WINDOW_API_CALL void window_api_window_poll_events(void){
	while (1){
		xcb_generic_event_t* event=xcb_poll_for_event(_xcb_conn);
		if (!event){
			return;
		}
		free(event);
	}
}



__WINDOW_API_CALL void window_api_window_set_title(void* id,const sll_string_t* name){
	xcb_change_property(_xcb_conn,XCB_PROP_MODE_REPLACE,(int)(intptr_t)id,XCB_ATOM_WM_NAME,XCB_ATOM_STRING,8,name->length,name->data);
	xcb_flush(_xcb_conn);
}



__WINDOW_API_CALL void window_api_window_set_visibility(void* id,sll_bool_t show){
	(show?xcb_map_window:xcb_unmap_window)(_xcb_conn,(int)(intptr_t)id);
	xcb_flush(_xcb_conn);
}

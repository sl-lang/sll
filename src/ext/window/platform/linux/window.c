#include <sll.h>
#include <stdint.h>
#include <window/common.h>
#include <window/cursor.h>
#include <window/platform/linux.h>
#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>



__WINDOW_API_CALL window_handle_t window_api_window_create(int32_t x,int32_t y,uint32_t w,uint32_t h,window_handle_t parent){
	xcb_window_t id=xcb_generate_id(_xcb_conn);
	const uint32_t data[1]={XCB_EVENT_MASK_KEY_PRESS|XCB_EVENT_MASK_KEY_RELEASE|XCB_EVENT_MASK_BUTTON_PRESS|XCB_EVENT_MASK_BUTTON_RELEASE|XCB_EVENT_MASK_ENTER_WINDOW|XCB_EVENT_MASK_LEAVE_WINDOW|XCB_EVENT_MASK_POINTER_MOTION|XCB_EVENT_MASK_EXPOSURE|XCB_EVENT_MASK_STRUCTURE_NOTIFY|XCB_EVENT_MASK_FOCUS_CHANGE|XCB_EVENT_MASK_PROPERTY_CHANGE};
	xcb_create_window(_xcb_conn,XCB_COPY_FROM_PARENT,id,(parent==(window_handle_t)(0xffffffffffffffffull)?_xcb_screen->root:(int)(intptr_t)parent),x,y,w,h,10,XCB_WINDOW_CLASS_INPUT_OUTPUT,_xcb_screen->root_visual,XCB_CW_EVENT_MASK,data);
	const xcb_atom_t atom_data[2]={
		_xcb_wm_delete_window,
		_xcb_net_wm_ping
	};
	xcb_change_property(_xcb_conn,XCB_PROP_MODE_REPLACE,id,_xcb_wm_protocols,XCB_ATOM_ATOM,sizeof(xcb_atom_t)*8,2,atom_data);
	xcb_flush(_xcb_conn);
	sll_map_container_set(&_window_to_parent,(void*)(intptr_t)id,(void*)(intptr_t)(_xcb_screen->root));
	return (window_handle_t)(intptr_t)id;
}



__WINDOW_API_CALL void window_api_window_destroy(window_handle_t id){
	sll_map_container_delete(&_window_to_parent,id,NULL);
	xcb_destroy_window(_xcb_conn,(int)(intptr_t)id);
	xcb_flush(_xcb_conn);
}



__WINDOW_API_CALL void window_api_window_set_geometry(window_handle_t id,int32_t x,int32_t y,uint32_t w,uint32_t h){
	const uint32_t data[4]={x,y,w,h};
	xcb_configure_window(_xcb_conn,(int)(intptr_t)id,XCB_CONFIG_WINDOW_X|XCB_CONFIG_WINDOW_Y|XCB_CONFIG_WINDOW_WIDTH|XCB_CONFIG_WINDOW_HEIGHT,data);
	xcb_flush(_xcb_conn);
}



__WINDOW_API_CALL void window_api_window_set_icon(window_handle_t id,uint32_t width,const sll_string_t* data){
	if (!width){
		xcb_delete_property(_xcb_conn,(int)(intptr_t)id,_xcb_net_wm_icon);
	}
	else{
		uint32_t height=data->length/4/width;
		uint32_t count=2+width*height;
		uint32_t* buffer=sll_allocate_stack(count*sizeof(uint32_t));
		*buffer=width;
		*(buffer+1)=height;
		uint32_t i=0;
		for (uint32_t j=2;j<count;j++){
			*(buffer+j)=FORMAT_COLOR_BITS(data->data[i],data->data[i+1],data->data[i+2],data->data[i+3]);
			i+=4;
		}
		xcb_change_property(_xcb_conn,XCB_PROP_MODE_REPLACE,(int)(intptr_t)id,_xcb_net_wm_icon,XCB_ATOM_CARDINAL,sizeof(uint32_t)*8,count,buffer);
		sll_deallocate(buffer);
	}
	xcb_flush(_xcb_conn);
}



__WINDOW_API_CALL void window_api_window_set_size_constraints(window_handle_t id,uint32_t min_w,uint32_t min_h,uint32_t max_w,uint32_t max_h){
	xcb_size_hints_t hints;
	sll_zero_memory(&hints,sizeof(xcb_size_hints_t));
	xcb_icccm_size_hints_set_min_size(&hints,min_w,min_h);
	xcb_icccm_size_hints_set_max_size(&hints,max_w,max_h);
	xcb_icccm_set_wm_normal_hints(_xcb_conn,(int)(intptr_t)id,&hints);
	xcb_flush(_xcb_conn);
}



__WINDOW_API_CALL void window_api_window_set_state(window_handle_t id,window_state_t state){
	xcb_client_message_event_t event={
		XCB_CLIENT_MESSAGE,
		32,
		0,
		(int)(intptr_t)id,
		_xcb_net_wm_state,
		{
			.data32={
				(state==WINDOW_STATE_FULLSCREEN),
				_xcb_net_wm_state_fullscreen,
				0,
				0,
				0
			}
		}
	};
	xcb_send_event(_xcb_conn,0,_xcb_screen->root,XCB_EVENT_MASK_STRUCTURE_NOTIFY|XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,(const char*)(&event));
	if (state==WINDOW_STATE_MINIMIZED){
		event.type=_xcb_wm_change_state;
		event.data.data32[0]=XCB_ICCCM_WM_STATE_ICONIC;
	}
	else{
		event.type=_xcb_net_wm_state;
		event.data.data32[0]=(state==WINDOW_STATE_MAXIMIZED);
		event.data.data32[1]=_xcb_net_wm_state_maximized_vert;
		event.data.data32[2]=_xcb_net_wm_state_maximized_horz;
	}
	xcb_send_event(_xcb_conn,0,_xcb_screen->root,XCB_EVENT_MASK_STRUCTURE_NOTIFY|XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,(const char*)(&event));
	xcb_flush(_xcb_conn);
}



__WINDOW_API_CALL void window_api_window_set_title(window_handle_t id,const sll_string_t* name){
	xcb_change_property(_xcb_conn,XCB_PROP_MODE_REPLACE,(int)(intptr_t)id,XCB_ATOM_WM_NAME,XCB_ATOM_STRING,8,name->length,name->data);
	xcb_change_property(_xcb_conn,XCB_PROP_MODE_REPLACE,(int)(intptr_t)id,XCB_ATOM_WM_ICON_NAME,XCB_ATOM_STRING,8,name->length,name->data);
	xcb_flush(_xcb_conn);
}



__WINDOW_API_CALL void window_api_window_set_visibility(window_handle_t id,sll_bool_t show){
	(show?xcb_map_window:xcb_unmap_window)(_xcb_conn,(int)(intptr_t)id);
	xcb_flush(_xcb_conn);
}

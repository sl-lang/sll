#include <sll.h>
#include <stdint.h>
#include <stdlib.h>
#include <window/common.h>
#include <window/keyboard.h>
#include <window/window.h>
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
	xcb_change_property(_xcb_conn,XCB_PROP_MODE_REPLACE,id,_xcb_wm_protocols,4,sizeof(xcb_atom_t)*8,2,atom_data);
	xcb_flush(_xcb_conn);
	return (window_handle_t)(intptr_t)id;
}



__WINDOW_API_CALL void window_api_window_destroy(window_handle_t id){
	xcb_destroy_window(_xcb_conn,(int)(intptr_t)id);
	xcb_flush(_xcb_conn);
}



__WINDOW_API_CALL void window_api_window_poll_events(sll_bool_t blocking,sll_array_t* out){
	SLL_INIT_ARRAY(out);
	const xcb_generic_event_t* event=(blocking?xcb_wait_for_event:xcb_poll_for_event)(_xcb_conn);
	while (event){
		sll_object_t* arg=NULL;
		uint8_t type=event->response_type&0x7f;
		switch (type){
			case XCB_KEY_PRESS:
			case XCB_KEY_RELEASE:
				{
					const xcb_key_press_event_t* key_event=(const xcb_key_press_event_t*)event;
					arg=sll_new_object(SLL_CHAR("uuuu"),WINDOW_EVENT_KEY,key_event->event,_scancode_to_keycode[key_event->detail],(type==XCB_KEY_PRESS));
					break;
				}
			case XCB_BUTTON_PRESS:
			case XCB_BUTTON_RELEASE:
				{
					const xcb_button_press_event_t* button_event=(const xcb_button_press_event_t*)event;
					if (button_event->detail>3&&button_event->detail<8){
						arg=sll_new_object(SLL_CHAR("uuu"),WINDOW_EVENT_SCROLL,button_event->event,button_event->detail-4);
					}
					else{
						arg=sll_new_object(SLL_CHAR("uuuu"),WINDOW_EVENT_BUTTON,button_event->event,button_event->detail-(button_event->detail>7?4:0),(type==XCB_BUTTON_PRESS));
					}
					break;
				}
			case XCB_MOTION_NOTIFY:
				{
					const xcb_motion_notify_event_t* motion_event=(const xcb_motion_notify_event_t*)event;
					arg=sll_new_object(SLL_CHAR("uuuu"),WINDOW_EVENT_BUTTON,motion_event->event,motion_event->event_x,motion_event->event_y);
					break;
				}
			case XCB_ENTER_NOTIFY:
			case XCB_LEAVE_NOTIFY:
				{
					const xcb_enter_notify_event_t* enter_event=(const xcb_enter_notify_event_t*)event;
					arg=sll_new_object(SLL_CHAR("uuu"),WINDOW_EVENT_MOUSE_ENTER,enter_event->event,(type==XCB_ENTER_NOTIFY));
					break;
				}
			case XCB_FOCUS_IN:
			case XCB_FOCUS_OUT:
				{
					const xcb_focus_in_event_t* focus_event=(const xcb_focus_in_event_t*)event;
					arg=sll_new_object(SLL_CHAR("uuu"),WINDOW_EVENT_FOCUS,focus_event->event,(type==XCB_FOCUS_IN));
					break;
				}
			case XCB_EXPOSE:
				{
					const xcb_configure_notify_event_t* configure_event=(const xcb_configure_notify_event_t*)event;
					arg=sll_new_object(SLL_CHAR("uuuuuu"),WINDOW_EVENT_REDRAW,configure_event->event,configure_event->x,configure_event->y,configure_event->width,configure_event->height);
					break;
				}
			case XCB_CONFIGURE_NOTIFY:
				{
					const xcb_configure_notify_event_t* configure_event=(const xcb_configure_notify_event_t*)event;
					xcb_translate_coordinates_reply_t* pos=xcb_translate_coordinates_reply(_xcb_conn,xcb_translate_coordinates(_xcb_conn,configure_event->event,_xcb_screen->root,configure_event->x,configure_event->y),NULL);
					arg=sll_new_object(SLL_CHAR("uuuuuu"),WINDOW_EVENT_GEOMETRY,configure_event->event,pos->dst_x,pos->dst_y,configure_event->width,configure_event->height);
					free(pos);
					break;
				}
			case XCB_PROPERTY_NOTIFY:
				{
					const xcb_property_notify_event_t* property_event=(const xcb_property_notify_event_t*)event;
					if (property_event->atom==_xcb_net_wm_state){
						xcb_get_property_reply_t* data=xcb_get_property_reply(_xcb_conn,xcb_get_property(_xcb_conn,0,property_event->window,_xcb_net_wm_state,XCB_ATOM_ATOM,0,32),NULL);
						const xcb_atom_t* wm_state=xcb_get_property_value(data);
						unsigned int state=WINDOW_STATE_NORMAL;
						if (*wm_state==_xcb_net_wm_state_hidden){
							state=WINDOW_STATE_MINIMIZED;
						}
						else if (*wm_state==_xcb_net_wm_state_fullscreen){
							state=WINDOW_STATE_FULLSCREEN;
						}
						else if (*wm_state==_xcb_net_wm_state_maximized_vert||*wm_state==_xcb_net_wm_state_maximized_horz){
							state=WINDOW_STATE_MAXIMIZED;
						}
						arg=sll_new_object(SLL_CHAR("uuu"),WINDOW_EVENT_STATE,property_event->window,state);
						free(data);
					}
					break;
				}
			case XCB_CLIENT_MESSAGE:
				{
					const xcb_client_message_event_t* client_event=(const xcb_client_message_event_t*)event;
					if (client_event->type==_xcb_wm_protocols){
						if (client_event->data.data32[0]==_xcb_wm_delete_window){
							arg=sll_new_object(SLL_CHAR("uu"),WINDOW_EVENT_CLOSE,client_event->window);
						}
						else if (client_event->data.data32[0]==_xcb_net_wm_ping){
							xcb_client_message_event_t reply=*client_event;
							reply.window=_xcb_screen->root;
							xcb_send_event(_xcb_conn,0,_xcb_screen->root,XCB_EVENT_MASK_STRUCTURE_NOTIFY|XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT,(const char*)(&reply));
						}
					}
					break;
				}
		}
		free((void*)event);
		if (arg){
			sll_array_push(NULL,arg,out);
			SLL_RELEASE(arg);
		}
		event=xcb_poll_for_event(_xcb_conn);
	}
}



__WINDOW_API_CALL void window_api_window_set_geometry(window_handle_t id,int32_t x,int32_t y,uint32_t w,uint32_t h){
	const uint32_t data[4]={x,y,w,h};
	xcb_configure_window(_xcb_conn,(int)(intptr_t)id,XCB_CONFIG_WINDOW_X|XCB_CONFIG_WINDOW_Y|XCB_CONFIG_WINDOW_WIDTH|XCB_CONFIG_WINDOW_HEIGHT,data);
	xcb_flush(_xcb_conn);
}



__WINDOW_API_CALL void window_api_window_set_state(window_handle_t id,sll_char_t state){
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

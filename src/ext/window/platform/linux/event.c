#include <sll.h>
#include <stdint.h>
#include <stdlib.h>
#include <window/common.h>
#include <window/event.h>
#include <window/keyboard.h>
#include <window/platform/linux.h>
#include <window/window.h>
#include <xcb/xcb.h>



__WINDOW_API_CALL void window_api_event_poll(sll_bool_t blocking,sll_array_t* out){
	SLL_INIT_ARRAY(out);
	const xcb_generic_event_t* event=(blocking?xcb_wait_for_event:xcb_poll_for_event)(_xcb_conn);
	while (event){
		sll_object_t* arg=NULL;
		uint8_t type=event->response_type&0x7f;
		sll_bool_t is_send_event=event->response_type>>7;
		switch (type){
			case 0:
				{
					const xcb_generic_error_t* error=(const xcb_generic_error_t*)event;
					arg=sll_new_object(SLL_CHAR("u[uuuu]"),WINDOW_EVENT_ERROR,error->error_code,error->resource_id,error->minor_code,error->major_code);
					break;
				}
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
					const xcb_expose_event_t* expose_event=(const xcb_expose_event_t*)event;
					arg=sll_new_object(SLL_CHAR("uuuuuu"),WINDOW_EVENT_REDRAW,expose_event->window,expose_event->x,expose_event->y,expose_event->width,expose_event->height);
					break;
				}
			case XCB_CONFIGURE_NOTIFY:
				{
					const xcb_configure_notify_event_t* configure_event=(const xcb_configure_notify_event_t*)event;
					int16_t x=configure_event->x;
					int16_t y=configure_event->y;
					if (!is_send_event){
						xcb_window_t parent=(xcb_window_t)(intptr_t)sll_map_container_get(&_window_to_parent,(void*)(intptr_t)(configure_event->window),(void*)(intptr_t)(_xcb_screen->root));
						if (parent!=_xcb_screen->root){
							xcb_translate_coordinates_reply_t* pos=xcb_translate_coordinates_reply(_xcb_conn,xcb_translate_coordinates(_xcb_conn,parent,_xcb_screen->root,x,y),NULL);
							x=pos->dst_x;
							y=pos->dst_y;
							free(pos);
						}
					}
					arg=sll_new_object(SLL_CHAR("uuuuuu"),WINDOW_EVENT_GEOMETRY,configure_event->window,x,y,configure_event->width,configure_event->height);
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
			case XCB_REPARENT_NOTIFY:
				{
					const xcb_reparent_notify_event_t* reparent_event=(const xcb_reparent_notify_event_t*)event;
					sll_map_container_set(&_window_to_parent,(void*)(intptr_t)(reparent_event->window),(void*)(intptr_t)(reparent_event->parent));
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

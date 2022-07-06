#include <xcb/xcb.h>
#include <stdlib.h>



#define GET_ATOM(name,var) \
	do{ \
		xcb_intern_atom_reply_t* data=xcb_intern_atom_reply(_xcb_conn,xcb_intern_atom(_xcb_conn,1,sizeof(name)-1,name),NULL); \
		var=data->atom; \
		free(data); \
	} while (0)



xcb_connection_t* _xcb_conn;
xcb_screen_t* _xcb_screen;
xcb_atom_t _xcb_wm_protocols;
xcb_atom_t _xcb_wm_delete_window;
xcb_atom_t _xcb_net_wm_ping;
xcb_atom_t _xcb_net_wm_state;
xcb_atom_t _xcb_net_wm_state_hidden;
xcb_atom_t _xcb_net_wm_state_maximized_vert;
xcb_atom_t _xcb_net_wm_state_maximized_horz;



void _deinit_platform(void){
	xcb_disconnect(_xcb_conn);
	_xcb_conn=NULL;
	_xcb_screen=NULL;
}



void _init_platform(void){
	_xcb_conn=xcb_connect(NULL,NULL);
	_xcb_screen=xcb_setup_roots_iterator(xcb_get_setup(_xcb_conn)).data;
	GET_ATOM("WM_PROTOCOLS",_xcb_wm_protocols);
	GET_ATOM("WM_DELETE_WINDOW",_xcb_wm_delete_window);
	GET_ATOM("_NET_WM_PING",_xcb_net_wm_ping);
	GET_ATOM("_NET_WM_STATE",_xcb_net_wm_state);
	GET_ATOM("_NET_WM_STATE_HIDDEN",_xcb_net_wm_state_hidden);
	GET_ATOM("_NET_WM_STATE_MAXIMIZED_VERT",_xcb_net_wm_state_maximized_vert);
	GET_ATOM("_NET_WM_STATE_MAXIMIZED_HORZ",_xcb_net_wm_state_maximized_horz);
}

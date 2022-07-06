#include <xcb/xcb.h>
#include <stdlib.h>



xcb_connection_t* _xcb_conn;
xcb_screen_t* _xcb_screen;
xcb_atom_t _xcb_wm_protocols;
xcb_atom_t _xcb_wm_delete_window;



void _deinit_platform(void){
	xcb_disconnect(_xcb_conn);
	_xcb_conn=NULL;
	_xcb_screen=NULL;
}



void _init_platform(void){
	_xcb_conn=xcb_connect(NULL,NULL);
	_xcb_screen=xcb_setup_roots_iterator(xcb_get_setup(_xcb_conn)).data;
	xcb_intern_atom_reply_t* data=xcb_intern_atom_reply(_xcb_conn,xcb_intern_atom(_xcb_conn,1,12,"WM_PROTOCOLS"),NULL);
	_xcb_wm_protocols=data->atom;
	free(data);
	data=xcb_intern_atom_reply(_xcb_conn,xcb_intern_atom(_xcb_conn,1,16,"WM_DELETE_WINDOW"),NULL);
	_xcb_wm_delete_window=data->atom;
	free(data);
}

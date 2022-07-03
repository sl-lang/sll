#include <xcb/xcb.h>



xcb_connection_t* _xcb_conn;
xcb_screen_t* _xcb_screen;



void _deinit_platform(void){
	xcb_disconnect(_xcb_conn);
	_xcb_conn=NULL;
	_xcb_screen=NULL;
}



void _init_platform(void){
	_xcb_conn=xcb_connect(NULL,NULL);
	_xcb_screen=xcb_setup_roots_iterator(xcb_get_setup(_xcb_conn)).data;
}

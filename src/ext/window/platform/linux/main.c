#include <stdlib.h>
#include <window/cursor.h>
#include <xcb/xcb.h>
#include <xcb/xcb_cursor.h>
#include <xcb/xcb_image.h>



#define GET_ATOM(name,var) \
	do{ \
		xcb_intern_atom_reply_t* data=xcb_intern_atom_reply(_xcb_conn,xcb_intern_atom(_xcb_conn,1,sizeof(name)-1,name),NULL); \
		var=data->atom; \
		free(data); \
	} while (0)

#define LOAD_CURSOR(id,...) \
	do{ \
		const char* __names[]={__VA_ARGS__,"X_cursor"}; \
		for (unsigned int __i=0;__i<sizeof(__names)/sizeof(const char*);__i++){ \
			_xcb_cursors[id]=xcb_cursor_load_cursor(_xcb_cursor_ctx,__names[__i]); \
			if (_xcb_cursors[id]!=XCB_NONE){ \
				break; \
			} \
		} \
	} while (0)



static const uint8_t _blank_cursor_bits[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};



static xcb_cursor_context_t* _xcb_cursor_ctx;



xcb_connection_t* _xcb_conn;
xcb_screen_t* _xcb_screen;
xcb_atom_t _xcb_wm_protocols;
xcb_atom_t _xcb_wm_delete_window;
xcb_atom_t _xcb_net_wm_ping;
xcb_atom_t _xcb_net_wm_state;
xcb_atom_t _xcb_net_wm_state_hidden;
xcb_atom_t _xcb_net_wm_state_maximized_vert;
xcb_atom_t _xcb_net_wm_state_maximized_horz;
xcb_atom_t _xcb_net_wm_state_fullscreen;
xcb_atom_t _xcb_wm_change_state;
xcb_cursor_t _xcb_cursors[WINDOW_MAX_CURSOR+1];



void _deinit_platform(void){
	for (window_cursor_t i=0;i<WINDOW_MAX_CURSOR;i++){
		xcb_free_cursor(_xcb_conn,_xcb_cursors[i]);
	}
	xcb_cursor_context_free(_xcb_cursor_ctx);
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
	GET_ATOM("_NET_WM_STATE_FULLSCREEN",_xcb_net_wm_state_fullscreen);
	GET_ATOM("WM_CHANGE_STATE",_xcb_wm_change_state);
	xcb_cursor_context_new(_xcb_conn,_xcb_screen,&_xcb_cursor_ctx);
	_xcb_cursors[WINDOW_CURSOR_HIDDEN]=xcb_generate_id(_xcb_conn);
	xcb_pixmap_t empty_bitmap=xcb_create_pixmap_from_bitmap_data(_xcb_conn,_xcb_screen->root,(uint8_t*)_blank_cursor_bits,16,16,1,0,0,0);
	xcb_create_cursor(_xcb_conn,_xcb_cursors[WINDOW_CURSOR_HIDDEN],empty_bitmap,empty_bitmap,0,0,0,0xffff,0xffff,0xffff,8,8);
	xcb_free_pixmap(_xcb_conn,empty_bitmap);
	LOAD_CURSOR(WINDOW_CURSOR_DEFAULT,"left_ptr","default","top_left_arrow","left_arrow");
	LOAD_CURSOR(WINDOW_CURSOR_CROSS,"cross");
	LOAD_CURSOR(WINDOW_CURSOR_I_BEAM,"ibeam","text","xterm");
	LOAD_CURSOR(WINDOW_CURSOR_WAIT,"wait","watch","0426c94ea35c87780ff01dc239897213");
	LOAD_CURSOR(WINDOW_CURSOR_BUSY,"left_ptr_watch","half-busy","progress","00000000000000020006000e7e9ffc3f","08e8e1c95fe2fc01f976f1e063a24ccd");
	LOAD_CURSOR(WINDOW_CURSOR_FORBIDDEN,"forbidden","not-allowed","crossed_circle","circle","03b6e0fcb3499374a867c041f52298f0");
	LOAD_CURSOR(WINDOW_CURSOR_POINTER,"pointing_hand","pointer","hand1","e29285e634086352946a0e7090d73106");
	LOAD_CURSOR(WINDOW_CURSOR_RESIZE_VERTICAL,"size_ver","ns-resize","v_double_arrow","00008160000006810000408080010102");
	LOAD_CURSOR(WINDOW_CURSOR_RESIZE_HORIZONTAL,"size_hor","ew-resize","h_double_arrow","028006030e0e7ebffc7f7070c0600140");
	LOAD_CURSOR(WINDOW_CURSOR_RESIZE_DIAGONAL_BT,"size_bdiag","top_right_corner","nesw-resize","50585d75b494802d0151028115016902","fcf1c3c7cd4491d801f1e1c78f100000");
	LOAD_CURSOR(WINDOW_CURSOR_RESIZE_DIAGONAL_TB,"size_fdiag","bottom_right_corner","nwse-resize","38c5dff7c7b8962045400281044508d2","c7088f0f3e6c8088236ef8e1e3e70000");
	LOAD_CURSOR(WINDOW_CURSOR_RESIZE_ALL,"fleur","size_all");
	LOAD_CURSOR(WINDOW_CURSOR_HELP,"whats_this","help","question_arrow","5c6cd98b3f3ebcb1f9c7f1c204630408","d9ce0ab605698f320427677b458ad60b");
}

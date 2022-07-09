#ifndef __WINDOW_PLATFORM_LINUX_H__
#define __WINDOW_PLATFORM_LINUX_H__ 1
#include <window/cursor.h>
#include <xcb/xcb.h>



extern xcb_connection_t* _xcb_conn;
extern xcb_screen_t* _xcb_screen;
extern xcb_atom_t _xcb_wm_protocols;
extern xcb_atom_t _xcb_wm_delete_window;
extern xcb_atom_t _xcb_net_wm_ping;
extern xcb_atom_t _xcb_net_wm_state;
extern xcb_atom_t _xcb_net_wm_state_hidden;
extern xcb_atom_t _xcb_net_wm_state_maximized_vert;
extern xcb_atom_t _xcb_net_wm_state_maximized_horz;
extern xcb_atom_t _xcb_net_wm_state_fullscreen;
extern xcb_atom_t _xcb_wm_change_state;
extern xcb_cursor_t _xcb_cursors[WINDOW_MAX_CURSOR+1];



#endif

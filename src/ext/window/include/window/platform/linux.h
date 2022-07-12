#ifndef __WINDOW_PLATFORM_LINUX_H__
#define __WINDOW_PLATFORM_LINUX_H__ 1
#include <window/cursor.h>
#include <xcb/xcb.h>



#define FORMAT_COLOR_BITS(r,g,b,a) ((((uint32_t)(r))<<16)|(((uint32_t)(g))<<8)|((uint32_t)(b))|(((uint32_t)(a))<<24))



extern xcb_connection_t* _xcb_conn;
extern xcb_screen_t* _xcb_screen;
extern xcb_atom_t _xcb_net_wm_icon;
extern xcb_atom_t _xcb_net_wm_ping;
extern xcb_atom_t _xcb_net_wm_state;
extern xcb_atom_t _xcb_net_wm_state_fullscreen;
extern xcb_atom_t _xcb_net_wm_state_hidden;
extern xcb_atom_t _xcb_net_wm_state_maximized_horz;
extern xcb_atom_t _xcb_net_wm_state_maximized_vert;
extern xcb_atom_t _xcb_wm_change_state;
extern xcb_atom_t _xcb_wm_delete_window;
extern xcb_atom_t _xcb_wm_protocols;
extern xcb_cursor_t _xcb_cursors[WINDOW_MAX_CURSOR+1];
extern sll_map_container_t _window_to_parent;



#endif

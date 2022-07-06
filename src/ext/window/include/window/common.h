#ifndef __WINDOW_COMMON_H__
#define __WINDOW_COMMON_H__ 1
#ifdef __SLL_BUILD_LINUX
#include <xcb/xcb.h>
#endif



#ifdef __SLL_BUILD_WINDOWS
#define __WINDOW_EXTERNAL __declspec(dllexport)
#define __WINDOW_API_CALL
#else
#define __WINDOW_EXTERNAL __attribute__((visibility("default")))
#define __WINDOW_API_CALL __attribute__((ms_abi))
#endif



#ifdef __SLL_BUILD_WINDOWS
extern void* _winapi_hinstance;
#elif defined(__SLL_BUILD_LINUX)
extern xcb_connection_t* _xcb_conn;
extern xcb_screen_t* _xcb_screen;
extern xcb_atom_t _xcb_wm_protocols;
extern xcb_atom_t _xcb_wm_delete_window;
extern xcb_atom_t _xcb_net_wm_ping;
#endif



void _deinit_platform(void);



void _init_platform(void);



#endif

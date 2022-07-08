#ifndef __WINDOW_WINDOW_H__
#define __WINDOW_WINDOW_H__ 1
#include <sll.h>
#include <stdint.h>
#include <window/common.h>



#define WINDOW_EVENT_ERROR 0
#define WINDOW_EVENT_KEY 1
#define WINDOW_EVENT_BUTTON 2
#define WINDOW_EVENT_MOUSE 3
#define WINDOW_EVENT_MOUSE_ENTER 4
#define WINDOW_EVENT_FOCUS 5
#define WINDOW_EVENT_GEOMETRY 6
#define WINDOW_EVENT_SCROLL 7
#define WINDOW_EVENT_REDRAW 8
#define WINDOW_EVENT_CLOSE 9
#define WINDOW_EVENT_STATE 10

#define WINDOW_SCROLL_DOWN 0
#define WINDOW_SCROLL_UP 1
#define WINDOW_SCROLL_RIGHT 2
#define WINDOW_SCROLL_LEFT 3

#define WINDOW_STATE_NORMAL 0
#define WINDOW_STATE_MINIMIZED 1
#define WINDOW_STATE_MAXIMIZED 2
#define WINDOW_STATE_FULLSCREEN 3



typedef void* window_handle_t;



#ifdef __SLL_BUILD_WINDOWS
unsigned __int64 _window_wnd_proc(void* id,unsigned int msg,unsigned __int64 w_param,__int64 l_param);
#endif



__WINDOW_API_CALL window_handle_t window_api_window_create(int32_t x,int32_t y,uint32_t w,uint32_t h,window_handle_t parent);



__WINDOW_API_CALL void window_api_window_destroy(window_handle_t id);



__WINDOW_API_CALL void window_api_window_poll_events(sll_bool_t blocking,sll_array_t* out);



__WINDOW_API_CALL void window_api_window_set_geometry(window_handle_t id,int32_t x,int32_t y,uint32_t w,uint32_t h);



__WINDOW_API_CALL void window_api_window_set_size_constraints(window_handle_t id,uint32_t min_w,uint32_t min_h,uint32_t max_w,uint32_t max_h);



__WINDOW_API_CALL void window_api_window_set_state(window_handle_t id,sll_char_t state);



__WINDOW_API_CALL void window_api_window_set_title(window_handle_t id,const sll_string_t* name);



__WINDOW_API_CALL void window_api_window_set_visibility(window_handle_t id,sll_bool_t show);



#endif

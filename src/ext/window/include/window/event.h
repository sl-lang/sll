#ifndef __WINDOW_EVENT_H__
#define __WINDOW_EVENT_H__ 1
#include <sll.h>
#include <window/common.h>



#define WINDOW_EVENT_ERROR 0
#define WINDOW_EVENT_KEY 1
#define WINDOW_EVENT_BUTTON 2
#define WINDOW_EVENT_MOUSE 3
#define WINDOW_EVENT_MOUSE_LEAVE 4
#define WINDOW_EVENT_FOCUS 5
#define WINDOW_EVENT_GEOMETRY 6
#define WINDOW_EVENT_SCROLL 7
#define WINDOW_EVENT_REDRAW 8
#define WINDOW_EVENT_CLOSE 9
#define WINDOW_EVENT_STATE 10
#define WINDOW_EVENT_POSITION 11
#define WINDOW_EVENT_SIZE 12



__WINDOW_API_CALL void window_api_event_poll(sll_bool_t blocking,sll_array_t* out);



#endif

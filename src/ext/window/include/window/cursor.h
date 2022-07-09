#ifndef __WINDOW_CURSOR_H__
#define __WINDOW_CURSOR_H__ 1
#include <sll.h>
#include <stdint.h>
#include <window/common.h>
#include <window/window.h>



#define WINDOW_CURSOR_HIDDEN 0
#define WINDOW_CURSOR_DEFAULT 1
#define WINDOW_CURSOR_CROSS 2
#define WINDOW_CURSOR_I_BEAM 3
#define WINDOW_CURSOR_WAIT 4
#define WINDOW_CURSOR_BUSY 5
#define WINDOW_CURSOR_FORBIDDEN 6
#define WINDOW_CURSOR_POINTER 7
#define WINDOW_CURSOR_RESIZE_VERTICAL 8
#define WINDOW_CURSOR_RESIZE_HORIZONTAL 9
#define WINDOW_CURSOR_RESIZE_DIAGONAL_BT 10
#define WINDOW_CURSOR_RESIZE_DIAGONAL_TB 11
#define WINDOW_CURSOR_RESIZE_ALL 12
#define WINDOW_CURSOR_HELP 13

#define WINDOW_MAX_CURSOR WINDOW_CURSOR_HELP



typedef uint32_t window_cursor_t;



__WINDOW_API_CALL void window_api_cursor_apply_to_window(window_handle_t id,window_cursor_t cursor);



__WINDOW_API_CALL void window_api_cursor_get_position(sll_array_t* out);



__WINDOW_API_CALL void window_api_cursor_set_position(int32_t x,int32_t y);



#endif

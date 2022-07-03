#ifndef __WINDOW_WINDOW_H__
#define __WINDOW_WINDOW_H__ 1
#include <sll.h>
#include <stdint.h>
#include <window/common.h>
#include <window/window.h>



__WINDOW_API_CALL void* window_api_window_create(int32_t x,int32_t y,uint32_t w,uint32_t h,void* parent);



__WINDOW_API_CALL void window_api_window_update_visibility(void* id,sll_bool_t show);



#endif

#ifndef __WINDOW_WINDOW_H__
#define __WINDOW_WINDOW_H__ 1
#include <sll.h>
#include <stdint.h>
#include <window/common.h>
#include <window/window.h>



#ifdef __SLL_BUILD_WINDOWS
unsigned __int64 _window_wnd_proc(void* id,unsigned int msg,unsigned __int64 w_param,__int64 l_param);
#endif



__WINDOW_API_CALL void* window_api_window_create(int32_t x,int32_t y,uint32_t w,uint32_t h,void* parent);



__WINDOW_API_CALL void window_api_window_destroy(void* id);



__WINDOW_API_CALL void window_api_window_set_title(void* id,const sll_string_t* name);



__WINDOW_API_CALL void window_api_window_set_visibility(void* id,sll_bool_t show);



#endif

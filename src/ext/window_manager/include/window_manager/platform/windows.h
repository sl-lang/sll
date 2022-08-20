#ifndef __WINDOW_MANAGER_PLATFORM_WINDOWS_H__
#define __WINDOW_MANAGER_PLATFORM_WINDOWS_H__ 1
#include <sll.h>
#include <stdint.h>



extern void* _winapi_hinstance;
extern sll_map_container_t _window_size_constraints;


typedef struct _WINDOW_SIZE_CONSTRAINTS{
	uint32_t min_w;
	uint32_t min_h;
	uint32_t max_w;
	uint32_t max_h;
} window_size_constraints_t;



unsigned __int64 _window_wnd_proc(void* id,unsigned int msg,unsigned __int64 w_param,__int64 l_param);



#endif

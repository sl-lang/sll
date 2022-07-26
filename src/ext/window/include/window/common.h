#ifndef __WINDOW_COMMON_H__
#define __WINDOW_COMMON_H__ 1
#include <sll.h>



#ifdef __SLL_BUILD_WINDOWS
#define __WINDOW_EXTERNAL __declspec(dllexport)
#define __WINDOW_API_CALL
#else
#define __WINDOW_EXTERNAL __attribute__((visibility("default")))
#define __WINDOW_API_CALL __attribute__((ms_abi))
#endif



void _deinit_platform(void);



sll_bool_t _init_platform(void);



__WINDOW_API_CALL void* window_api_get_context(void);



#endif

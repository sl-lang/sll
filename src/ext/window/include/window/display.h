#ifndef __WINDOW_DISPLAY_H__
#define __WINDOW_DISPLAY_H__ 1
#include <window/common.h>
#include <sll.h>



#define WINDOW_DISPLAY_FLAG_ACTIVE 1
#define WINDOW_DISPLAY_FLAG_MAIN_DISPLAY 2



__WINDOW_API_CALL void window_api_display_enumerate(sll_array_t* out);



#endif

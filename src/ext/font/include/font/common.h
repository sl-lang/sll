#ifndef __FONT_COMMON_H__
#define __FONT_COMMON_H__ 1
#include <sll.h>



#ifdef __SLL_BUILD_WINDOWS
#define __FONT_EXTERNAL __declspec(dllexport)
#define __FONT_API_CALL
#else
#define __FONT_EXTERNAL __attribute__((visibility("default")))
#define __FONT_API_CALL __attribute__((ms_abi))
#endif



#endif

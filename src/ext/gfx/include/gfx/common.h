#ifndef __GFX_COMMON_H__
#define __GFX_COMMON_H__ 1
#include <sll.h>



#ifdef __SLL_BUILD_WINDOWS
#define __GFX_EXTERNAL __declspec(dllexport)
#define __GFX_API_CALL
#else
#define __GFX_EXTERNAL __attribute__((visibility("default")))
#define __GFX_API_CALL __attribute__((ms_abi))
#endif



#endif

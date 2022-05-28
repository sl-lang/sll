#ifndef __CLIB_COMMON_H__
#define __CLIB_COMMON_H__ 1



#ifdef __SLL_BUILD_WINDOWS
#define __CLIB_EXTERNAL __declspec(dllexport)
#define __CLIB_API_CALL
#else
#define __CLIB_EXTERNAL __attribute__((visibility("default")))
#define __CLIB_API_CALL __attribute__((ms_abi))
#endif



#endif

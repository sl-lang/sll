#ifndef __DS4_COMMON_H__
#define __DS4_COMMON_H__ 1



#ifdef __SLL_BUILD_WINDOWS
#define __DS4_EXTERNAL __declspec(dllexport)
#define __DS4_API_CALL
#else
#define __DS4_EXTERNAL __attribute__((visibility("default")))
#define __DS4_API_CALL __attribute__((ms_abi))
#endif



#endif

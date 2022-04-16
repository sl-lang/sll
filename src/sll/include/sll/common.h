#ifndef __SLL_COMMON_H__
#define __SLL_COMMON_H__ 1



#ifdef __SLL_BUILD_WINDOWS
#ifdef __SLL_COMPILATION__
#define __SLL_EXTERNAL __declspec(dllexport)
#else
#define __SLL_EXTERNAL __declspec(dllimport)
#endif
#define __SLL_CHECK_OUTPUT _Check_return_
#define __SLL_API_CALL
#else
#ifdef __SLL_COMPILATION__
#define __SLL_EXTERNAL __attribute__((visibility("default")))
#else
#define __SLL_EXTERNAL
#endif
#define __SLL_CHECK_OUTPUT __attribute__((warn_unused_result))
#define __SLL_API_CALL __attribute__((ms_abi))
#endif



#ifndef NULL
#define NULL ((void*)0)
#endif



#endif

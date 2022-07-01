#ifndef __SLL_COMMON_H__
#define __SLL_COMMON_H__ 1
#ifdef __SLL_BUILD_WINDOWS
#include <sal.h>
#endif



#ifdef __SLL_BUILD_WINDOWS
#ifdef __SLL_COMPILATION__
#define __SLL_EXTERNAL __declspec(dllexport)
#else
#define __SLL_EXTERNAL __declspec(dllimport)
#endif
#define __SLL_CHECK_OUTPUT _Must_inspect_result_
#define __SLL_API_CALL
#define __SLL_TYPEOF(x) decltype((x))
#else
#ifdef __SLL_COMPILATION__
#define __SLL_EXTERNAL __attribute__((visibility("default")))
#else
#define __SLL_EXTERNAL
#endif
#define __SLL_CHECK_OUTPUT __attribute__((warn_unused_result))
#define __SLL_API_CALL __attribute__((ms_abi))
#define __SLL_TYPEOF(x) __typeof__((x))
#endif



#ifdef __SLL_COMPILATION__
#ifndef NULL
#define NULL ((void*)0)
#endif
#endif



#endif

#ifndef __SLL_EXT_COMMON_H__
#define __SLL_EXT_COMMON_H__ 1
#include <stddef.h>



#ifdef _MSC_VER
#ifdef __SLL_EXT_COMPILATION__
#define __SLL_EXT_EXTERNAL __declspec(dllexport)
#else
#define __SLL_EXT_EXTERNAL __declspec(dllimport)
#endif
#else
#ifdef __SLL_EXT_COMPILATION__
#define __SLL_EXT_EXTERNAL __attribute__((visibility("default")))
#else
#define __SLL_EXT_EXTERNAL
#endif
#endif



#endif

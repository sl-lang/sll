#ifndef __SLL_COMMON_H__
#define __SLL_COMMON_H__ 1



#ifdef __SLL_BUILD_WINDOWS
#ifdef __SLL_STATIC__
#define __SLL_EXTERNAL
#else
#ifdef __SLL_COMPILATION__
#define __SLL_EXTERNAL __declspec(dllexport)
#else
#define __SLL_EXTERNAL __declspec(dllimport)
#endif
#endif
#define __SLL_CHECK_OUTPUT _Check_return_
#define __SLL_NO_RETURN __declspec(noreturn)
#else
#ifdef __SLL_STATIC__
#define __SLL_EXTERNAL
#else
#ifdef __SLL_COMPILATION__
#define __SLL_EXTERNAL __attribute__((visibility("default")))
#else
#define __SLL_EXTERNAL
#endif
#endif
#define __SLL_CHECK_OUTPUT __attribute__((warn_unused_result))
#define __SLL_NO_RETURN __attribute__((noreturn))
#endif



#ifdef DEBUG_BUILD
#define __SLL_ADD_DEBUG_DATA(o,t) sll_add_debug_data((o),(sll_char_t*)(__FILE__),__LINE__,(sll_char_t*)(__func__),(t))
#else
#define __SLL_ADD_DEBUG_DATA(o,t) o
#endif
#define __SLL_DEBUG_TYPE_CREATE 0
#define __SLL_DEBUG_TYPE_ACQUIRE 1
#define __SLL_DEBUG_TYPE_RELEASE 2



#ifndef NULL
#define NULL ((void*)0)
#endif



#define SLL_MAX_SHORT_IDENTIFIER_LENGTH 15



#endif

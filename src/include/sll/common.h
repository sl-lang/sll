#ifndef __SLL_COMMON_H__
#define __SLL_COMMON_H__ 1



#ifdef _MSC_VER
#ifdef __SLL_STATIC__
#define __SLL_FUNC
#else
#ifdef __SLL_COMPILATION__
#define __SLL_FUNC __declspec(dllexport)
#else
#define __SLL_FUNC __declspec(dllimport)
#endif
#endif
#define __SLL_CHECK_OUTPUT _Check_return_
#else
#ifdef __SLL_STATIC__
#define __SLL_FUNC
#else
#ifdef __SLL_COMPILATION__
#define __SLL_FUNC __attribute__((visibility("default")))
#else
#define __SLL_FUNC
#endif
#endif
#define __SLL_CHECK_OUTPUT __attribute__((warn_unused_result))
#endif



#ifdef DEBUG_BUILD
#define __SLL_ADD_DEBUG_DATA(o,t) sll__add_debug_data((o),__FILE__,__LINE__,__func__,(t))
#else
#define __SLL_ADD_DEBUG_DATA(o,t) o
#endif
#define __SLL_DEBUG_TYPE_CREATE 0
#define __SLL_DEBUG_TYPE_ACQUIRE 1
#define __SLL_DEBUG_TYPE_RELEASE 2



#endif

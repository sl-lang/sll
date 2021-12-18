#ifndef __SLL_VERSION_H__
#define __SLL_VERSION_H__ 1
#include <sll/types.h>



#define __VERSION_STR_(x) #x
#define __VERSION_STR(x) __VERSION_STR_(x)



#define SLL_VERSION_MAJOR 0
#define SLL_VERSION_MINOR 6
#define SLL_VERSION_PATCH 33
#ifdef __SLL_STATIC__
#define SLL_VERSION_STANDALONE 1
#endif
#ifdef __SHA__
#define SLL_VERSION_SHA __SHA__
#define SLL_VERSION_FULL_SHA __FULL_SHA__
#define SLL_VERSION_HAS_SHA 1
#else
#define SLL_VERSION_SHA NULL
#define SLL_VERSION_FULL_SHA NULL
#endif
#define SLL_VERSION_BUILD_TIME __TIME__
#define SLL_VERSION_BUILD_DATE __DATE__

#define SLL_VERSION_STRING __VERSION_STR(SLL_VERSION_MAJOR)"."__VERSION_STR(SLL_VERSION_MINOR)"."__VERSION_STR(SLL_VERSION_PATCH)
#define SLL_VERSION ((sll_version_t)((SLL_VERSION_MAJOR<<24)|(SLL_VERSION_MINOR<<16)|SLL_VERSION_PATCH))

#define SLL_GET_MAJOR(v) ((v)>>24)
#define SLL_GET_MINOR(v) (((v)>>16)&0xff)
#define SLL_GET_PATCH(v) ((v)&0xffff)



#endif

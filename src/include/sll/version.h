#ifndef __SLL_VERSION_H__
#define __SLL_VERSION_H__ 1



#define SLL_VERSION_MAJOR 0
#define SLL_VERSION_MINOR 6
#define SLL_VERSION_PATCH 12
#ifdef __SLL_STATIC__
#define SLL_VERSION_TYPE "-standalone"
#else
#define SLL_VERSION_TYPE ""
#endif
#ifdef __SHA__
#define SLL_VERSION_SHA __SHA__
#define SLL_VERSION_FULL_SHA __FULL_SHA__
#endif
#define SLL_VERSION_BUILD_TIME __TIME__
#define SLL_VERSION_BUILD_DATE __DATE__



#define SLL_VERSION ((sll_version_t)((SLL_VERSION_MAJOR<<12)|(SLL_VERSION_MINOR<<8)|SLL_VERSION_PATCH))



#endif

#ifndef __LLL_VERSION_H__
#define __LLL_VERSION_H__ 1



#define LLL_VERSION_MAJOR 0
#define LLL_VERSION_MINOR 6
#define LLL_VERSION_PATCH 5
#define LLL_VERSION_BUILD_TIME __TIME__
#define LLL_VERSION_BUILD_DATE __DATE__
#define LLL_VERSION ((lll_version_t)((LLL_VERSION_MAJOR<<12)|(LLL_VERSION_MINOR<<8)|LLL_VERSION_PATCH))



#endif

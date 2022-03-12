#ifndef __SLL_PLATFORM__DISPATCHER_H__
#define __SLL_PLATFORM__DISPATCHER_H__ 1
#ifdef __SLL_BUILD_WINDOWS
#include <../platform/windows/dispatcher.h>
#else
#include <../platform/linux/dispatcher.h>
#endif
#endif

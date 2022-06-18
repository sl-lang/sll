#ifndef __SLL__INTERNAL_ERROR_H__
#define __SLL__INTERNAL_ERROR_H__ 1
#include <sll/error.h>
#include <sll/platform/util.h>
#ifdef __SLL_BUILD_FUZZER
#include <sll/log.h>
#endif



#ifdef __SLL_BUILD_FUZZER
#define ERROR_PTR(e) \
	do{ \
		sll_error_t __err=(e); \
		if (__err!=SLL_NO_ERROR){ \
			SLL_LOG(">>> [%s:%u(%s)]: %.16llx\n",__FILE__,__LINE__,__func__,__err); \
		} \
		if (err){ \
			*err=__err; \
		} \
	} while (0)
#else
#define ERROR_PTR(e) \
	do{ \
		if (err){ \
			*err=(e); \
		} \
	} while (0)
#endif

#define ERROR_PTR_RESET ERROR_PTR(SLL_NO_ERROR)
#define ERROR_PTR_SYSTEM ERROR_PTR(sll_platform_get_error())
#define DATA_IF_ERROR_PTR(x,v) (err&&*err!=SLL_NO_ERROR?(v):(x))
#define ZERO_IF_ERROR_PTR(x) DATA_IF_ERROR_PTR((x),0)

#define ERROR_STRING_POINTER_TABLE_SIZE 32



#endif

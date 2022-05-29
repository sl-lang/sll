#ifndef __SLL__INTERNAL_ERROR_H__
#define __SLL__INTERNAL_ERROR_H__ 1
#include <sll/error.h>
#include <sll/platform/util.h>



#define ERROR_PTR(e) \
	do{ \
		if (err){ \
			*err=(e); \
		} \
	} while (0)
#define ERROR_PTR_RESET ERROR_PTR(SLL_NO_ERROR)
#define ERROR_PTR_SYSTEM ERROR_PTR(sll_platform_get_error())
#define DATA_IF_ERROR_PTR(x,v) (err&&*err!=SLL_NO_ERROR?(v):(x))
#define ZERO_IF_ERROR_PTR(x) DATA_IF_ERROR_PTR((x),0)

#define ERROR_STRING_POINTER_TABLE_SIZE 32



#endif

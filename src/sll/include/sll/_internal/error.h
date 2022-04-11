#ifndef __SLL__INTERNAL_ERROR_H__
#define __SLL__INTERNAL_ERROR_H__ 1
#ifdef __SLL_BUILD_WINDOWS
#include <../platform/windows/error.h>
#else
#include <errno.h>
#endif
#include <sll/error.h>



#define RESET_ERROR_PTR \
	do{ \
		if (err){ \
			*err=SLL_NO_ERROR; \
		} \
	} while (0)
#define WINAPI_ERROR_PTR \
	do{ \
		if (err){ \
			*err=WINAPI_ERROR; \
		} \
	} while (0)
#define LIBC_ERROR_PTR \
	do{ \
		if (err){ \
			*err=LIBC_ERROR; \
		} \
	} while (0)
#define ERROR_PTR(e) \
	do{ \
		if (err){ \
			*err=(e); \
		} \
	} while (0)
#define SET_ERROR_IF_LIBC_FAIL_PTR(x) \
	do{ \
		if ((x)==-1){ \
			LIBC_ERROR_PTR; \
		} \
	} while (0)
#define DATA_IF_ERROR_PTR(x,v) (err&&*err!=SLL_NO_ERROR?(v):(x))
#define ZERO_IF_ERROR_PTR(x) DATA_IF_ERROR_PTR((x),0)
#define WINAPI_ERROR _platform_get_win_error()
#define LIBC_ERROR (errno|SLL_ERROR_FLAG_LIBC)



#endif

#ifndef __SLL_INTERNAL_ERROR_H__
#define __SLL_INTERNAL_ERROR_H__ 1
#ifndef __SLL_BUILD_WINDOWS
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
			*err=GetLastError()|SLL_ERROR_FLAG_WINAPI; \
		} \
	} while (0)
#define LIBC_ERROR_PTR \
	do{ \
		if (err){ \
			*err=errno|SLL_ERROR_FLAG_LIBC; \
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
#define WINAPI_ERROR (GetLastError()|SLL_ERROR_FLAG_WINAPI)
#define LIBC_ERROR (errno|SLL_ERROR_FLAG_LIBC)



#endif

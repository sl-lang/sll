#ifndef __SLL_INTERNAL_PLATFORM_H__
#define __SLL_INTERNAL_PLATFORM_H__ 1
#ifndef __SLL_BUILD_WINDOWS
#include <errno.h>
#endif
#include <sll/internal/common.h>
#include <sll/_size_types.h>
#include <sll/error.h>
#include <sll/types.h>



#define CSR_REGISTER_FLAGS 0x8040

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



typedef struct __EXECUTE_WRAPPER_DATA{
	sll_internal_thread_function_t fn;
	void* arg;
	void* lck;
} execute_wrapper_data_t;



typedef struct __FILE_LIST_DATA{
	sll_string_t* dt;
	sll_array_length_t l;
} file_list_data_t;



#ifdef __SLL_BUILD_WINDOWS
extern void* _win_dll_handle;
#endif



__SLL_NO_RETURN void _force_exit_platform(void);



void _deinit_platform(void);



void _init_platform(void);



#endif

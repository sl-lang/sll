#ifndef __SLL__INTERNAL_PLATFORM_H__
#define __SLL__INTERNAL_PLATFORM_H__ 1
#include <sll/_internal/common.h>
#include <sll/platform/thread.h>
#include <sll/string.h>
#include <sll/types.h>



#define CSR_REGISTER_FLAGS 0x8040



typedef struct _EXECUTE_WRAPPER_DATA{
	sll_internal_thread_function_t fn;
	void* arg;
	void* lck;
} execute_wrapper_data_t;



typedef struct _FILE_LIST_DATA{
	sll_string_t* dt;
	sll_array_length_t l;
} file_list_data_t;



#ifdef __SLL_BUILD_WINDOWS
extern void* _win_dll_handle;
#endif



__SLL_NO_RETURN void _force_exit_platform(void);



void _deinit_platform(void);



void _deinit_platform_time(void);



void _init_platform(void);



void _init_platform_env(void);



void _init_platform_time(void);



#endif

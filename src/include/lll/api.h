#ifndef __LLL_FUNC_H__
#define __LLL_FUNC_H__ 1
#include <lll/common.h>
#include <lll/types.h>



#define __API_FUNC(nm) __LLL_FUNC void nm(lll_runtime_object_t* o,lll_arg_count_t ac,lll_runtime_object_t* a)



#define LLL_API_FILE_APPEND 0
#define LLL_API_FILE_READ 1
#define LLL_API_FILE_WRITE 2

#define LLL_API_INVALID_FILE_HANDLE 65535
#define LLL_API_MAX_FILE_PATH_LENGTH 4096
#define LLL_API_MAX_OPEN_FILES 8192



__API_FUNC(lll_api_get_time);



__API_FUNC(lll_api_open_file);



__API_FUNC(lll_api_close_file);



__API_FUNC(lll_api_write_file);



#endif

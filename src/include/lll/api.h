#ifndef __LLL_FUNC_H__
#define __LLL_FUNC_H__ 1
#include <lll/common.h>
#include <lll/types.h>



#define __API_FUNC(nm) INTERNAL_FUNCTION(#nm,lll_api_##nm);__API_FUNC_DECL(nm)
#define __API_FUNC_DECL(nm) __LLL_FUNC void lll_api_##nm(lll_runtime_object_t* o,lll_arg_count_t ac,lll_runtime_object_t* a)



#define LLL_API_INVALID_FILE_HANDLE 65535
#define LLL_API_MAX_FILE_PATH_LENGTH 4096
#define LLL_API_MAX_OPEN_FILES 8192



__API_FUNC_DECL(file_close);



__API_FUNC_DECL(file_open);



__API_FUNC_DECL(file_write);



__API_FUNC_DECL(path_absolute);



__API_FUNC_DECL(path_exists);



__API_FUNC_DECL(path_is_dir);



__API_FUNC_DECL(path_relative);



__API_FUNC_DECL(path_size);



__API_FUNC_DECL(sys_arg_get);



__API_FUNC_DECL(sys_arg_get_count);



__API_FUNC_DECL(sys_get_platform);



__API_FUNC_DECL(time_current);



__API_FUNC_DECL(time_current_nanos);



#endif

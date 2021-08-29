#ifndef __SLL_FUNC_H__
#define __SLL_FUNC_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define __API_FUNC_DECL(nm) __SLL_FUNC sll_runtime_object_t* sll_api_##nm(const sll_runtime_object_t*const* a,sll_arg_count_t ac)



#define SLL_API_MAX_FILE_PATH_LENGTH 4096



/*TYPE api
FUNC file_close
DESC Closes a file previously opened by $file_open
ARG H Previously allocated file handle
RET 0 Failure
RET 1 Success*/
__API_FUNC_DECL(file_close);



/*TYPE api
FUNC file_open
DESC Opens a file with the specified mode
ARG S The path to the file to open
ARG S? The optional file mode
RET 0 Failure
RET H Success*/
__API_FUNC_DECL(file_open);



/*TYPE api
FUNC file_write
DESC Writes to a file prevoiusly opened by $file_open
ARG H Previously opened file handle
ARG O? The object to write to file
RET 0 Failure
RET I Number of bytes written*/
__API_FUNC_DECL(file_write);



__API_FUNC_DECL(path_absolute);



__API_FUNC_DECL(path_exists);



__API_FUNC_DECL(path_is_dir);



__API_FUNC_DECL(path_relative);



__API_FUNC_DECL(path_size);



__API_FUNC_DECL(string_convert);



__API_FUNC_DECL(string_length);



__API_FUNC_DECL(sys_arg_get);



__API_FUNC_DECL(sys_arg_get_count);



__API_FUNC_DECL(sys_get_platform);



__API_FUNC_DECL(time_current);



__API_FUNC_DECL(time_current_nanos);



__API_FUNC_DECL(time_sleep);



__API_FUNC_DECL(time_sleep_nanos);



__API_FUNC_DECL(util_ref_count);



__SLL_FUNC void sll_object_to_string(const sll_runtime_object_t*const* a,sll_array_length_t al,sll_string_t* o);



__SLL_FUNC sll_string_length_t sll_object_to_string_length(const sll_runtime_object_t*const* a,sll_array_length_t al);



__SLL_FUNC void sll_set_argument_count(sll_integer_t ac);



__SLL_FUNC void sll_set_argument(sll_integer_t i,const char* a);



#endif

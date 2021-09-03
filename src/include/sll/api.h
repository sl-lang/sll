#ifndef __SLL_FUNC_H__
#define __SLL_FUNC_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define __API_FUNC_DECL(nm) __SLL_FUNC sll_runtime_object_t* sll_api_##nm##_raw(const sll_runtime_object_t*const* al,const sll_arg_count_t all);__SLL_FUNC sll_runtime_object_t* sll_api_##nm(__SLL_API_ARGS_sll_api_##nm)



#define SLL_API_MAX_FILE_PATH_LENGTH 4096



/*~~DOCS~~
TYPE api
FUNC sll_api_file_close
DESC Closes a file previously opened by :sll_api_file_open: and invalidates its handle
ARG H Previously allocated file handle
RET 0 Failure: Invalid file handle
RET 1 Success*/
__API_FUNC_DECL(file_close);



/*~~DOCS~~
TYPE api
FUNC sll_api_file_open
DESC Opens a file with the specified mode
ARG S The path to the file to open
ARG S? The optional file mode
RET 0 Failure
RET H Success*/
__API_FUNC_DECL(file_open);



/*~~DOCS~~
TYPE api var_arg
FUNC sll_api_file_write
DESC Writes to a file prevoiusly opened by :sll_api_file_open:
ARG HI Previously opened file handle
ARG O The object to write to file
RET 0 Failure
RET I Number of bytes written*/
__API_FUNC_DECL(file_write);



/*~~DOCS~~
TYPE api
FUNC sll_api_json_parse
DESC Converts a [JSON][https://www.json.org/json-en.html] string into SLL types
ARG S The string to parse
RET 0 Failure
RET O The parsed object*/
__API_FUNC_DECL(json_parse);



/*~~DOCS~~
TYPE api
FUNC sll_api_json_stringify
DESC Docs!
ARG O Docs!
RET 0 Failure
RET S Success*/
__API_FUNC_DECL(json_stringify);



/*~~DOCS~~
TYPE api
FUNC sll_api_json_type
DESC Docs!
ARG I Docs!
RET 0 Failure
RET H Success*/
__API_FUNC_DECL(json_type);



/*~~DOCS~~
TYPE api
FUNC sll_api_path_absolute
DESC Docs!
ARG S Docs!
RET Z Failure
RET S Success*/
__API_FUNC_DECL(path_absolute);



/*~~DOCS~~
TYPE api
FUNC sll_api_path_exists
DESC Docs!
ARG S Docs!
RET 0 Failure
RET I Success*/
__API_FUNC_DECL(path_exists);



/*~~DOCS~~
TYPE api
FUNC sll_api_path_is_dir
DESC Docs!
ARG S Docs!
RET 0 Failure
RET I Success*/
__API_FUNC_DECL(path_is_dir);



/*~~DOCS~~
TYPE api
FUNC sll_api_path_relative
DESC Docs!
ARG S Docs!
RET Z Failure
RET S Success*/
__API_FUNC_DECL(path_relative);



/*~~DOCS~~
TYPE api
FUNC sll_api_path_size
DESC Docs!
ARG S Docs!
RET 0 Failure
RET I Success*/
__API_FUNC_DECL(path_size);



/*~~DOCS~~
TYPE api var_arg
FUNC sll_api_string_convert
DESC Docs!
ARG O Docs!
RET Z Failure
RET S Success*/
__API_FUNC_DECL(string_convert);



/*~~DOCS~~
TYPE api var_arg
FUNC sll_api_string_length
DESC Docs!
ARG O Docs!
RET 0 Failure
RET I Success*/
__API_FUNC_DECL(string_length);



/*~~DOCS~~
TYPE api
FUNC sll_api_sys_arg_get
DESC Docs!
ARG I Docs!
RET Z Failure
RET S Success*/
__API_FUNC_DECL(sys_arg_get);



/*~~DOCS~~
TYPE api
FUNC sll_api_sys_arg_get_count
DESC Docs!
RET I Docs!*/
__API_FUNC_DECL(sys_arg_get_count);



/*~~DOCS~~
TYPE api
FUNC sll_api_sys_get_platform
DESC Docs!
RET S Docs!*/
__API_FUNC_DECL(sys_get_platform);



/*~~DOCS~~
TYPE api
FUNC sll_api_time_current
DESC Docs!
RET F Docs!*/
__API_FUNC_DECL(time_current);



/*~~DOCS~~
TYPE api
FUNC sll_api_time_current_nanos
DESC Docs!
RET I Docs!*/
__API_FUNC_DECL(time_current_nanos);



/*~~DOCS~~
TYPE api
FUNC sll_api_time_sleep
DESC Docs!
ARG IF Docs!
RET 0 Failure
RET F Success*/
__API_FUNC_DECL(time_sleep);



/*~~DOCS~~
TYPE api
FUNC sll_api_time_sleep_nanos
DESC Docs!
ARG IF Docs!
RET 0 Failure
RET I Success*/
__API_FUNC_DECL(time_sleep_nanos);



/*~~DOCS~~
TYPE api
FUNC sll_api_url_execute_request
DESC Docs!
ARG S Docs!
ARG S Docs!
ARG S Docs!
ARG M Docs!
ARG S Docs!
RET 0 Failure
RET A Success*/
__API_FUNC_DECL(url_execute_request);



/*~~DOCS~~
TYPE api
FUNC sll_api_util_ref_count
DESC Docs!
ARG O Docs!
RET 0 Failure
RET I Success*/
__API_FUNC_DECL(util_ref_count);



__SLL_FUNC void sll_free_json_object(sll_json_object_t* json);



__SLL_FUNC sll_json_object_t* sll_json_get_by_key(sll_json_object_t* json,const sll_string_t* k);



__SLL_FUNC __SLL_RETURN sll_json_parse(sll_json_parser_state_t* p,sll_json_object_t* o);



__SLL_FUNC void sll_object_to_string(const sll_runtime_object_t*const* a,sll_array_length_t al,sll_string_t* o);



__SLL_FUNC sll_string_length_t sll_object_to_string_length(const sll_runtime_object_t*const* a,sll_array_length_t al,sll_bool_t q);



__SLL_FUNC void sll_set_argument_count(sll_integer_t ac);



__SLL_FUNC void sll_set_argument(sll_integer_t i,const char* a);



#endif

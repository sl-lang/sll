#ifndef __SLL_API_PATH_H__
#define __SLL_API_PATH_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



#ifdef _MSC_VER
#define SLL_API_FILE_PATH_SEPARATOR '\\'
#define SLL_API_MAX_FILE_PATH_LENGTH 260
#else
#define SLL_API_FILE_PATH_SEPARATOR '/'
#define SLL_API_MAX_FILE_PATH_LENGTH 4096
#endif



__SLL_EXTERNAL sll_string_length_t sll_path_absolute(const sll_char_t* s,sll_char_t* o,sll_string_length_t ol);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_path_absolute
DESC Docs!
ARG S Docs!
RET Z Failure
RET S Success*/
__API_FUNC_DECL(path_absolute);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_path_exists
DESC Docs!
ARG S Docs!
RET 0 Failure
RET I Success*/
__API_FUNC_DECL(path_exists);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_path_get_cwd
DESC Docs!
RET S Docs!*/
__API_FUNC_DECL(path_get_cwd);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_path_is_dir
DESC Docs!
ARG S Docs!
RET 0 Failure
RET I Success*/
__API_FUNC_DECL(path_is_dir);



/*~~DOCS~~
TYPE api optimizable var_arg
FUNC sll_api_path_join
DESC Docs!
ARG S? Docs!
RET Z Failure
RET S Success*/
__API_FUNC_DECL(path_join);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_path_list_dir
DESC Docs!
ARG S Docs!
RET E Failure
RET A Success*/
__API_FUNC_DECL(path_list_dir);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_path_recursive_list_dir
DESC Docs!
ARG S Docs!
RET E Failure
RET A Success*/
__API_FUNC_DECL(path_recursive_list_dir);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_path_relative
DESC Docs!
ARG S Docs!
RET Z Failure
RET S Success*/
__API_FUNC_DECL(path_relative);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_path_set_cwd
ARG S Docs!
RET 0 Failure
RET I Docs!*/
__API_FUNC_DECL(path_set_cwd);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_path_size
DESC Docs!
ARG S Docs!
RET 0 Failure
RET I Success*/
__API_FUNC_DECL(path_size);



#endif

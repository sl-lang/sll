#ifndef __SLL_API_PATH_H__
#define __SLL_API_PATH_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



#define SLL_API_MAX_FILE_PATH_LENGTH 4096



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



#endif

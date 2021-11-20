#ifndef __SLL_API_SYS_H__
#define __SLL_API_SYS_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_set_argument(sll_integer_t i,const sll_char_t* a);



__SLL_EXTERNAL void sll_set_argument_count(sll_integer_t ac);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_sys_arg_get
DESC Docs!
ARG I Docs!
RET Z Failure
RET O Success*/
__API_FUNC_DECL(sys_arg_get);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_sys_arg_get_count
DESC Docs!
RET I Docs!*/
__API_FUNC_DECL(sys_arg_get_count);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_sys_get_executable
DESC Docs!
RET O Docs!*/
__API_FUNC_DECL(sys_get_executable);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_sys_get_platform
DESC Docs!
RET O Docs!*/
__API_FUNC_DECL(sys_get_platform);



#endif

#ifndef __SLL_API_SYS_H__
#define __SLL_API_SYS_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



/*~~DOCS~~
TYPE api
FUNC sll_api_sys_arg_get
DESC Docs!
ARG I Docs!
RET Z Failure
RET O Success*/
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
RET O Docs!*/
__API_FUNC_DECL(sys_get_platform);



__SLL_FUNC void sll_set_argument(sll_integer_t i,const char* a);



__SLL_FUNC void sll_set_argument_count(sll_integer_t ac);



#endif

#ifndef __SLL_API_STRING_H__
#define __SLL_API_STRING_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



/*~~DOCS~~
TYPE api compilation_call optimizable var_arg
FUNC sll_api_string_convert
DESC Docs!
ARG O Docs!
RET Z Failure
RET S Success*/
__API_FUNC_DECL(string_convert);



/*~~DOCS~~
TYPE api compilation_call optimizable
FUNC sll_api_string_replace
DESC Docs!
ARG S Docs!
ARG CS? Docs!
ARG CS? Docs!
RET Z Failure
RET S Success*/
__API_FUNC_DECL(string_replace);



/*~~DOCS~~
TYPE api compilation_call optimizable
FUNC sll_api_string_to_lower_case
DESC Docs!
ARG S Docs!
RET Z Failure
RET S Success*/
__API_FUNC_DECL(string_to_lower_case);



/*~~DOCS~~
TYPE api compilation_call optimizable
FUNC sll_api_string_to_title_case
DESC Docs!
ARG S Docs!
RET Z Failure
RET S Success*/
__API_FUNC_DECL(string_to_title_case);



/*~~DOCS~~
TYPE api compilation_call optimizable
FUNC sll_api_string_to_upper_case
DESC Docs!
ARG S Docs!
RET Z Failure
RET S Success*/
__API_FUNC_DECL(string_to_upper_case);



#endif

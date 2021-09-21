#ifndef __SLL_API_STRING_H__
#define __SLL_API_STRING_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



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



__SLL_FUNC void sll_object_to_string(const sll_runtime_object_t*const* a,sll_array_length_t al,sll_string_t* o);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_length_t sll_object_to_string_length(const sll_runtime_object_t*const* a,sll_array_length_t al,sll_bool_t q);



#endif

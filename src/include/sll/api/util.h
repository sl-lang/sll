#ifndef __SLL_API_UTIL_H__
#define __SLL_API_UTIL_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_util_instruction_count
DESC Docs!
RET I Success*/
__API_FUNC_DECL(util_instruction_count);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_util_ref_count
DESC Docs!
ARG O Docs!
RET 0 Failure
RET I Success*/
__API_FUNC_DECL(util_ref_count);



#endif

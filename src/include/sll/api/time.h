#ifndef __SLL_API_TIME_H__
#define __SLL_API_TIME_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_time_current
DESC Docs!
RET F Docs!*/
__API_FUNC_DECL(time_current);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_time_current_nanos
DESC Docs!
RET I Docs!*/
__API_FUNC_DECL(time_current_nanos);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_time_sleep
DESC Docs!
ARG IF Docs!
RET f Failure
RET F Success*/
__API_FUNC_DECL(time_sleep);



/*~~DOCS~~
TYPE api optimizable
FUNC sll_api_time_sleep_nanos
DESC Docs!
ARG IF Docs!
RET 0 Failure
RET I Success*/
__API_FUNC_DECL(time_sleep_nanos);



#endif

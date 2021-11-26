#ifndef __SLL_API_TIME_H__
#define __SLL_API_TIME_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



/**
 * \flags api func optimizable
 * \name sll_api_time_current
 * \desc Docs!
 * \ret F
 */
__API_FUNC_DECL(time_current);



/**
 * \flags api func optimizable
 * \name sll_api_time_current_nanos
 * \desc Docs!
 * \ret I
 */
__API_FUNC_DECL(time_current_nanos);



/**
 * \flags api func optimizable
 * \name sll_api_time_sleep
 * \desc Docs!
 * \arg IF
 * \ret f -> Failure
 * \ret F -> Success
 */
__API_FUNC_DECL(time_sleep);



/**
 * \flags api func optimizable
 * \name sll_api_time_sleep_nanos
 * \desc Docs!
 * \arg IF
 * \ret 0 -> Failure
 * \ret I -> Success
 */
__API_FUNC_DECL(time_sleep_nanos);



#endif

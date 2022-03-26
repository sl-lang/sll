#ifndef __SLL_API_TIME_H__
#define __SLL_API_TIME_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Time API
 * \group time-api
 * \desc Docs!
 */



/**
 * \flags api func
 * \name sll_api_time_current
 * \group time-api
 * \desc Docs!
 * \ret F
 */
__API_FUNC_DECL(time_current);



/**
 * \flags api func
 * \name sll_api_time_current_ns
 * \group time-api
 * \desc Docs!
 * \ret I
 */
__API_FUNC_DECL(time_current_ns);



/**
 * \flags api func
 * \name sll_api_time_sleep
 * \group time-api
 * \desc Docs!
 * \arg IF
 * \ret F
 */
__API_FUNC_DECL(time_sleep);



/**
 * \flags api func
 * \name sll_api_time_sleep_ns
 * \group time-api
 * \desc Docs!
 * \arg IF
 * \ret I
 */
__API_FUNC_DECL(time_sleep_ns);



#endif

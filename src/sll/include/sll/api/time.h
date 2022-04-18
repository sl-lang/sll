#ifndef __SLL_API_TIME_H__
#define __SLL_API_TIME_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Time API
 * \group time-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_time_current
 * \group time-api
 * \desc Docs!
 * \api |f
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_time_current(void);



/**
 * \flags func
 * \name sll_api_time_current_ns
 * \group time-api
 * \desc Docs!
 * \api |Q
 * \ret sll_time_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_time_t sll_api_time_current_ns(void);



/**
 * \flags func
 * \name sll_api_time_sleep
 * \group time-api
 * \desc Docs!
 * \api x|f
 * \arg const sll_int_float_t* time
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_time_sleep(const sll_int_float_t* time);



/**
 * \flags func
 * \name sll_api_time_sleep_ns
 * \group time-api
 * \desc Docs!
 * \api x|Q
 * \arg const sll_int_float_t* time
 * \ret sll_time_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_time_t sll_api_time_sleep_ns(const sll_int_float_t* time);



#endif

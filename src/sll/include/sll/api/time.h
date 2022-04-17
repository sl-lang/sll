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
 * \api |i
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_time_current_ns(void);



/**
 * \flags func
 * \name sll_api_time_sleep
 * \group time-api
 * \desc Docs!
 * \api x|f
 * \arg sll_int_float_t* a
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_time_sleep(sll_int_float_t* a);



/**
 * \flags func
 * \name sll_api_time_sleep_ns
 * \group time-api
 * \desc Docs!
 * \api x|i
 * \arg sll_int_float_t* a
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_time_sleep_ns(sll_int_float_t* a);



#endif

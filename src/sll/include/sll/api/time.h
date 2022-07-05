#ifndef __SLL_API_TIME_H__
#define __SLL_API_TIME_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/parse_args.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Time API
 * \group time-api
 * \desc Docs!
 */



/**
 * \flags type var
 * \name sll_time_t
 * \group time-api
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_time_t;



/**
 * \flags check_output func
 * \name sll_api_time_current
 * \group time-api
 * \desc Docs!
 * \api |Q
 * \ret sll_time_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_time_t sll_api_time_current(void);



/**
 * \flags check_output func
 * \name sll_api_time_sleep
 * \group time-api
 * \desc Docs!
 * \api x|Q
 * \arg const sll_number_t* time
 * \ret sll_time_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_time_t sll_api_time_sleep(const sll_number_t* time);



#endif

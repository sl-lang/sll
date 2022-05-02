#ifndef __SLL_PLATFORM_TIME_H__
#define __SLL_PLATFORM_TIME_H__ 1
#include <sll/_size_types.h>
#include <sll/api/date.h>
#include <sll/api/time.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags subgroup
 * \name Time
 * \group platform
 * \subgroup platform-time
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_TIME
 * \group platform
 * \subgroup platform-time
 * \desc Docs!
 * \type sll_time_t
 */
#define SLL_MAX_TIME __SLL_U64_MAX



/**
 * \flags var
 * \name sll_platform_time_zone
 * \group platform
 * \subgroup platform-time
 * \desc Docs!
 * \type const sll_time_zone_t*
 */
__SLL_EXTERNAL extern const sll_time_zone_t* sll_platform_time_zone;



/**
 * \flags check_output func
 * \name sll_platform_get_current_time
 * \group platform
 * \subgroup platform-time
 * \desc Docs!
 * \ret sll_time_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void);



/**
 * \flags func
 * \name sll_platform_sleep
 * \group platform
 * \subgroup platform-time
 * \desc Docs!
 * \arg sll_time_t tm
 */
__SLL_EXTERNAL void sll_platform_sleep(sll_time_t tm);



#endif

#ifndef __SLL_PLATFORM_UTIL_H__
#define __SLL_PLATFORM_UTIL_H__ 1
#include <sll/_size_types.h>
#include <sll/api/date.h>
#include <sll/api/time.h>
#include <sll/common.h>
#include <sll/environment.h>
#include <sll/error.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Platform
 * \group platform
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_TIME
 * \group platform
 * \desc Docs!
 * \type sll_time_t
 */
#define SLL_MAX_TIME __SLL_U64_MAX



/**
 * \flags var
 * \name sll_platform_cpu_count
 * \group platform
 * \desc Docs!
 * \type const sll_cpu_t*
 */
__SLL_EXTERNAL extern const sll_cpu_t* sll_platform_cpu_count;



/**
 * \flags var
 * \name sll_environment
 * \group platform
 * \desc Docs!
 * \type const sll_environment_t*
 */
__SLL_EXTERNAL extern const sll_environment_t* sll_environment;



/**
 * \flags var
 * \name sll_platform_string
 * \group platform
 * \desc Docs!
 * \type const sll_string_t*
 */
__SLL_EXTERNAL extern const sll_string_t* sll_platform_string;



/**
 * \flags var
 * \name sll_platform_time_zone
 * \group platform
 * \desc Docs!
 * \type const sll_time_zone_t*
 */
__SLL_EXTERNAL extern const sll_time_zone_t* sll_platform_time_zone;



/**
 * \flags check_output func
 * \name sll_platform_get_current_time
 * \group platform
 * \desc Docs!
 * \ret sll_time_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void);



/**
 * \flags check_output func
 * \name sll_platform_get_error
 * \group platform
 * \desc Docs!
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_get_error(void);



/**
 * \flags func
 * \name sll_platform_random
 * \group platform
 * \desc Docs!
 * \arg void* bf
 * \arg sll_size_t l
 */
__SLL_EXTERNAL void sll_platform_random(void* bf,sll_size_t l);



/**
 * \flags func
 * \name sll_platform_remove_environment_variable
 * \group platform
 * \desc Docs!
 * \arg const sll_char_t* k
 */
__SLL_EXTERNAL void sll_platform_remove_environment_variable(const sll_char_t* k);



/**
 * \flags func
 * \name sll_platform_set_environment_variable
 * \group platform
 * \desc Docs!
 * \arg const sll_char_t* k
 * \arg const sll_char_t* v
 */
__SLL_EXTERNAL void sll_platform_set_environment_variable(const sll_char_t* k,const sll_char_t* v);



/**
 * \flags func
 * \name sll_platform_sleep
 * \group platform
 * \desc Docs!
 * \arg sll_time_t tm
 */
__SLL_EXTERNAL void sll_platform_sleep(sll_time_t tm);



#endif

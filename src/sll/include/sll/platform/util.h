#ifndef __SLL_PLATFORM_UTIL_H__
#define __SLL_PLATFORM_UTIL_H__ 1
#include <sll/common.h>
#include <sll/environment.h>
#include <sll/error.h>
#include <sll/platform/thread.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Platform
 * \group platform
 * \desc Docs!
 */



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
 * \name sll_platform_string
 * \group platform
 * \desc Docs!
 * \type const sll_string_t*
 */
__SLL_EXTERNAL extern const sll_string_t* sll_platform_string;



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



#endif

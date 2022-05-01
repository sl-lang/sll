#ifndef __SLL_PLATFORM_LOCK_H__
#define __SLL_PLATFORM_LOCK_H__ 1
#include <sll/common.h>
#include <sll/error.h>
#include <sll/types.h>
/**
 * \flags subgroup
 * \name Locks
 * \group platform
 * \subgroup platform-lock
 * \desc Docs!
 */



/**
 * \flags type var
 * \name sll_lock_handle_t
 * \group platform
 * \subgroup platform-lock
 * \desc Docs!
 * \type void*
 */
typedef void* sll_lock_handle_t;



/**
 * \flags check_output func
 * \name sll_platform_lock_acquire
 * \group platform
 * \subgroup platform-lock
 * \desc Docs!
 * \arg sll_lock_handle_t l
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_lock_acquire(sll_lock_handle_t l);



/**
 * \flags check_output func
 * \name sll_platform_lock_create
 * \group platform
 * \subgroup platform-lock
 * \desc Docs!
 * \ret sll_lock_handle_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_lock_handle_t sll_platform_lock_create(void);



/**
 * \flags check_output func
 * \name sll_platform_lock_delete
 * \group platform
 * \subgroup platform-lock
 * \desc Docs!
 * \arg sll_lock_handle_t l
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_lock_delete(sll_lock_handle_t l);



/**
 * \flags check_output func
 * \name sll_platform_lock_release
 * \group platform
 * \subgroup platform-lock
 * \desc Docs!
 * \arg sll_lock_handle_t l
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_lock_release(sll_lock_handle_t l);



#endif

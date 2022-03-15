#ifndef __SLL_LOCK_H__
#define __SLL_LOCK_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Locks
 * \group lock
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_lock_create
 * \group lock
 * \desc Docs!
 * \ret sll_lock_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_lock_index_t sll_lock_create(void);



/**
 * \flags check_output func
 * \name sll_lock_delete
 * \group lock
 * \desc Docs!
 * \arg sll_lock_index_t l
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_lock_delete(sll_lock_index_t l);



/**
 * \flags check_output func
 * \name sll_lock_release
 * \group lock
 * \desc Docs!
 * \arg sll_lock_index_t l
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_lock_release(sll_lock_index_t l);



#endif

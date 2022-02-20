#ifndef __SLL_SCHEDULER_H__
#define __SLL_SCHEDULER_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Scheduler
 * \group scheduler
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_UNKNOWN_THREAD_INDEX
 * \group scheduler
 * \desc Docs!
 * \type sll_thread_index_t
 */
#define SLL_UNKNOWN_THREAD_INDEX 0xffffffff



/**
 * \flags var
 * \name sll_current_thread_index
 * \group scheduler
 * \desc Docs!
 * \type sll_thread_index_t
 */
__SLL_EXTERNAL extern sll_thread_index_t sll_current_thread_index;



/**
 * \flags check_output func
 * \name sll_create_lock
 * \group scheduler
 * \desc Docs!
 * \ret sll_lock_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_lock_index_t sll_create_lock(void);



/**
 * \flags check_output func
 * \name sll_create_thread
 * \group scheduler
 * \desc Docs!
 * \arg sll_integer_t fn
 * \arg sll_object_t*const* al
 * \arg sll_arg_count_t all
 * \ret sll_thread_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_thread_index_t sll_create_thread(sll_integer_t fn,sll_object_t*const* al,sll_arg_count_t all);



/**
 * \flags check_output func
 * \name sll_delete_lock
 * \group scheduler
 * \desc Docs!
 * \arg sll_lock_index_t l
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_delete_lock(sll_lock_index_t l);



/**
 * \flags check_output func
 * \name sll_delete_thread
 * \group scheduler
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_delete_thread(sll_thread_index_t t);



/**
 * \flags check_output func
 * \name sll_release_lock
 * \group scheduler
 * \desc Docs!
 * \arg sll_lock_index_t l
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_release_lock(sll_lock_index_t l);



/**
 * \flags func
 * \name sll_start_thread
 * \group scheduler
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret sll_bool_t
 */
__SLL_EXTERNAL sll_bool_t sll_start_thread(sll_thread_index_t t);



#endif

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
 * \name sll_barrier_create
 * \group scheduler
 * \desc Docs!
 * \ret sll_barrier_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_barrier_index_t sll_barrier_create(void);



/**
 * \flags check_output func
 * \name sll_barrier_delete
 * \group scheduler
 * \desc Docs!
 * \arg sll_barrier_index_t b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_barrier_delete(sll_barrier_index_t b);



/**
 * \flags check_output func
 * \name sll_barrier_increase
 * \group scheduler
 * \desc Docs!
 * \arg sll_barrier_index_t b
 * \ret sll_barrier_counter_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_barrier_counter_t sll_barrier_increase(sll_barrier_index_t b);



/**
 * \flags check_output func
 * \name sll_barrier_reset
 * \group scheduler
 * \desc Docs!
 * \arg sll_barrier_index_t b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_barrier_reset(sll_barrier_index_t b);



/**
 * \flags check_output func
 * \name sll_lock_create
 * \group scheduler
 * \desc Docs!
 * \ret sll_lock_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_lock_index_t sll_lock_create(void);



/**
 * \flags check_output func
 * \name sll_lock_delete
 * \group scheduler
 * \desc Docs!
 * \arg sll_lock_index_t l
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_lock_delete(sll_lock_index_t l);



/**
 * \flags check_output func
 * \name sll_lock_release
 * \group scheduler
 * \desc Docs!
 * \arg sll_lock_index_t l
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_lock_release(sll_lock_index_t l);



/**
 * \flags check_output func
 * \name sll_semaphore_create
 * \group scheduler
 * \desc Docs!
 * \arg sll_semaphore_counter_t c
 * \ret sll_semaphore_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_semaphore_index_t sll_semaphore_create(sll_semaphore_counter_t c);



/**
 * \flags check_output func
 * \name sll_semaphore_delete
 * \group scheduler
 * \desc Docs!
 * \arg sll_semaphore_index_t s
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_semaphore_delete(sll_semaphore_index_t s);



/**
 * \flags check_output func
 * \name sll_semaphore_release
 * \group scheduler
 * \desc Docs!
 * \arg sll_semaphore_index_t l
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_semaphore_release(sll_semaphore_index_t l);



/**
 * \flags check_output func
 * \name sll_thread_create
 * \group scheduler
 * \desc Docs!
 * \arg sll_integer_t fn
 * \arg sll_object_t*const* al
 * \arg sll_arg_count_t all
 * \ret sll_thread_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_thread_index_t sll_thread_create(sll_integer_t fn,sll_object_t*const* al,sll_arg_count_t all);



/**
 * \flags check_output func
 * \name sll_thread_delete
 * \group scheduler
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_delete(sll_thread_index_t t);



/**
 * \flags check_output func
 * \name sll_thread_get_call_stack
 * \group scheduler
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret const sll_call_stack_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_call_stack_t* sll_thread_get_call_stack(sll_thread_index_t t);



/**
 * \flags check_output func
 * \name sll_thread_get_instruction_index
 * \group scheduler
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret sll_instruction_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_instruction_index_t sll_thread_get_instruction_index(sll_thread_index_t t);



/**
 * \flags check_output func
 * \name sll_thread_restart
 * \group scheduler
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_restart(sll_thread_index_t t);



/**
 * \flags check_output func
 * \name sll_thread_start
 * \group scheduler
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_start(sll_thread_index_t t);



/**
 * \flags check_output func
 * \name sll_thread_suspend
 * \group scheduler
 * \desc Docs!
 * \arg sll_thread_index_t t
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_thread_suspend(sll_thread_index_t t);



#endif

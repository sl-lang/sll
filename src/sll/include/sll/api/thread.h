#ifndef __SLL_API_THREAD_H__
#define __SLL_API_THREAD_H__ 1
#include <sll/barrier.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/lock.h>
#include <sll/semaphore.h>
#include <sll/thread.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Threading API
 * \group thread-api
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_api_thread_create
 * \group thread-api
 * \desc Docs!
 * \api i#a|~D
 * \arg sll_integer_t function
 * \arg const sll_array_t* args
 * \arg sll_thread_index_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_thread_create(sll_integer_t function,const sll_array_t* args,sll_thread_index_t* out);



/**
 * \flags check_output func
 * \name sll_api_thread_create_barrier
 * \group thread-api
 * \desc Docs!
 * \api |~D
 * \arg sll_barrier_index_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_thread_create_barrier(sll_barrier_index_t* out);



/**
 * \flags check_output func
 * \name sll_api_thread_create_lock
 * \group thread-api
 * \desc Docs!
 * \api |~D
 * \arg sll_lock_index_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_thread_create_lock(sll_lock_index_t* out);



/**
 * \flags check_output func
 * \name sll_api_thread_create_semaphore
 * \group thread-api
 * \desc Docs!
 * \api D|~D
 * \arg sll_semaphore_counter_t count
 * \arg sll_lock_index_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_thread_create_semaphore(sll_semaphore_counter_t count,sll_lock_index_t* out);



/**
 * \flags check_output func
 * \name sll_api_thread_delete
 * \group thread-api
 * \desc Docs!
 * \api D|b
 * \arg sll_thread_index_t thread_index
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete(sll_thread_index_t thread_index);



/**
 * \flags check_output func
 * \name sll_api_thread_delete_barrier
 * \group thread-api
 * \desc Docs!
 * \api D|b
 * \arg sll_barrier_index_t barrier_index
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_barrier(sll_barrier_index_t barrier_index);



/**
 * \flags check_output func
 * \name sll_api_thread_delete_lock
 * \group thread-api
 * \desc Docs!
 * \api D|b
 * \arg sll_lock_index_t lock_index
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_lock(sll_lock_index_t lock_index);



/**
 * \flags check_output func
 * \name sll_api_thread_delete_semaphore
 * \group thread-api
 * \desc Docs!
 * \api D|b
 * \arg sll_semaphore_index_t sempahore_index
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_semaphore(sll_semaphore_index_t sempahore_index);



/**
 * \flags func
 * \name sll_api_thread_get_internal_data
 * \group thread-api
 * \desc Docs!
 * \api D|a
 * \arg sll_thread_index_t thread_index
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_thread_get_internal_data(sll_thread_index_t thread_index,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_thread_increase_barrier
 * \group thread-api
 * \desc Docs!
 * \api D|D
 * \arg sll_barrier_index_t barrier_index
 * \ret sll_barrier_counter_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_barrier_counter_t sll_api_thread_increase_barrier(sll_barrier_index_t barrier_index);



/**
 * \flags check_output func
 * \name sll_api_thread_release_lock
 * \group thread-api
 * \desc Docs!
 * \api D|b
 * \arg sll_lock_index_t lock_index
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_release_lock(sll_lock_index_t lock_index);



/**
 * \flags check_output func
 * \name sll_api_thread_release_semaphore
 * \group thread-api
 * \desc Docs!
 * \api D|b
 * \arg sll_semaphore_index_t sempahore_index
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_release_semaphore(sll_semaphore_index_t sempahore_index);



/**
 * \flags check_output func
 * \name sll_api_thread_reset_barrier
 * \group thread-api
 * \desc Docs!
 * \api D|D
 * \arg sll_barrier_index_t barrier_index
 * \ret sll_barrier_counter_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_barrier_counter_t sll_api_thread_reset_barrier(sll_barrier_index_t barrier_index);



/**
 * \flags check_output func
 * \name sll_api_thread_restart
 * \group thread-api
 * \desc Docs!
 * \api D|b
 * \arg sll_thread_index_t thread_index
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_restart(sll_thread_index_t thread_index);



/**
 * \flags check_output func
 * \name sll_api_thread_suspend
 * \group thread-api
 * \desc Docs!
 * \api D|b
 * \arg sll_thread_index_t thread_index
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_suspend(sll_thread_index_t thread_index);



#endif

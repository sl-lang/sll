#ifndef __SLL_API_THREAD_H__
#define __SLL_API_THREAD_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Threading API
 * \group thread-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_thread_create
 * \group thread-api
 * \desc Docs!
 * \api ia|i
 * \arg sll_integer_t fn
 * \arg sll_array_t* args
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create(sll_integer_t fn,sll_array_t* args);



/**
 * \flags func
 * \name sll_api_thread_create_barrier
 * \group thread-api
 * \desc Docs!
 * \api |i
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create_barrier(void);



/**
 * \flags func
 * \name sll_api_thread_create_lock
 * \group thread-api
 * \desc Docs!
 * \api |i
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create_lock(void);



/**
 * \flags func
 * \name sll_api_thread_create_semaphore
 * \group thread-api
 * \desc Docs!
 * \api i|i
 * \arg sll_semaphore_counter_t cnt
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_create_semaphore(sll_semaphore_counter_t cnt);



/**
 * \flags func
 * \name sll_api_thread_delete
 * \group thread-api
 * \desc Docs!
 * \api i|b
 * \arg sll_thread_index_t tid
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete(sll_thread_index_t tid);



/**
 * \flags func
 * \name sll_api_thread_delete_barrier
 * \group thread-api
 * \desc Docs!
 * \api i|b
 * \arg sll_barrier_index_t bid
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_barrier(sll_barrier_index_t bid);



/**
 * \flags func
 * \name sll_api_thread_delete_lock
 * \group thread-api
 * \desc Docs!
 * \api i|b
 * \arg sll_lock_index_t lid
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_lock(sll_lock_index_t lid);



/**
 * \flags func
 * \name sll_api_thread_delete_semaphore
 * \group thread-api
 * \desc Docs!
 * \api i|b
 * \arg sll_semaphore_index_t sid
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_delete_semaphore(sll_semaphore_index_t sid);



/**
 * \flags func
 * \name sll_api_thread_get_internal_data
 * \group thread-api
 * \desc Docs!
 * \api i|a
 * \arg sll_thread_index_t tid
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_thread_get_internal_data(sll_thread_index_t tid,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_thread_increase_barrier
 * \group thread-api
 * \desc Docs!
 * \api i|i
 * \arg sll_barrier_index_t bid
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_increase_barrier(sll_barrier_index_t bid);



/**
 * \flags func
 * \name sll_api_thread_release_lock
 * \group thread-api
 * \desc Docs!
 * \api i|b
 * \arg sll_lock_index_t lid
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_release_lock(sll_lock_index_t lid);



/**
 * \flags func
 * \name sll_api_thread_release_semaphore
 * \group thread-api
 * \desc Docs!
 * \api i|b
 * \arg sll_semaphore_index_t sid
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_release_semaphore(sll_semaphore_index_t sid);



/**
 * \flags func
 * \name sll_api_thread_reset_barrier
 * \group thread-api
 * \desc Docs!
 * \api i|i
 * \arg sll_barrier_index_t bid
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_thread_reset_barrier(sll_barrier_index_t bid);



/**
 * \flags func
 * \name sll_api_thread_restart
 * \group thread-api
 * \desc Docs!
 * \api i|b
 * \arg sll_thread_index_t tid
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_restart(sll_thread_index_t tid);



/**
 * \flags func
 * \name sll_api_thread_suspend
 * \group thread-api
 * \desc Docs!
 * \api i|b
 * \arg sll_thread_index_t tid
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_thread_suspend(sll_thread_index_t tid);



#endif

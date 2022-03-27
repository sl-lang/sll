#ifndef __SLL_API_THREAD_H__
#define __SLL_API_THREAD_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Threading API
 * \group thread-api
 * \desc Docs!
 */



/**
 * \flags api func
 * \name sll_api_thread_create
 * \group thread-api
 * \desc Docs!
 * \arg I
 * \arg A
 * \ret I
 */
__API_FUNC_DECL(thread_create);



/**
 * \flags api func
 * \name sll_api_thread_create_barrier
 * \group thread-api
 * \desc Docs!
 * \ret I
 */
__API_FUNC_DECL(thread_create_barrier);



/**
 * \flags api func
 * \name sll_api_thread_create_lock
 * \group thread-api
 * \desc Docs!
 * \ret I
 */
__API_FUNC_DECL(thread_create_lock);



/**
 * \flags api func
 * \name sll_api_thread_create_semaphore
 * \group thread-api
 * \desc Docs!
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(thread_create_semaphore);



/**
 * \flags api func
 * \name sll_api_thread_delete
 * \group thread-api
 * \desc Docs!
 * \arg I
 * \ret B
 */
__API_FUNC_DECL(thread_delete);



/**
 * \flags api func
 * \name sll_api_thread_delete_barrier
 * \group thread-api
 * \desc Docs!
 * \arg I
 * \ret B
 */
__API_FUNC_DECL(thread_delete_barrier);



/**
 * \flags api func
 * \name sll_api_thread_delete_lock
 * \group thread-api
 * \desc Docs!
 * \arg I
 * \ret B
 */
__API_FUNC_DECL(thread_delete_lock);



/**
 * \flags api func
 * \name sll_api_thread_delete_semaphore
 * \group thread-api
 * \desc Docs!
 * \arg I
 * \ret B
 */
__API_FUNC_DECL(thread_delete_semaphore);



/**
 * \flags api func
 * \name sll_api_thread_get_internal_data
 * \group thread-api
 * \desc Docs!
 * \arg I
 * \ret A
 */
__API_FUNC_DECL(thread_get_internal_data);



/**
 * \flags api func
 * \name sll_api_thread_increase_barrier
 * \group thread-api
 * \desc Docs!
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(thread_increase_barrier);



/**
 * \flags api func
 * \name sll_api_thread_release_lock
 * \group thread-api
 * \desc Docs!
 * \arg I
 * \ret B
 */
__API_FUNC_DECL(thread_release_lock);



/**
 * \flags api func
 * \name sll_api_thread_release_semaphore
 * \group thread-api
 * \desc Docs!
 * \arg I
 * \ret B
 */
__API_FUNC_DECL(thread_release_semaphore);



/**
 * \flags api func
 * \name sll_api_thread_reset_barrier
 * \group thread-api
 * \desc Docs!
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(thread_reset_barrier);



/**
 * \flags api func
 * \name sll_api_thread_restart
 * \group thread-api
 * \desc Docs!
 * \arg I
 * \ret B
 */
__API_FUNC_DECL(thread_restart);



/**
 * \flags api func
 * \name sll_api_thread_suspend
 * \group thread-api
 * \desc Docs!
 * \arg I
 * \ret B
 */
__API_FUNC_DECL(thread_suspend);



#endif

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



#endif

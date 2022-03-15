#ifndef __SLL_SEMAPHORE_H__
#define __SLL_SEMAPHORE_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Semaphores
 * \group semaphore
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_semaphore_create
 * \group semaphore
 * \desc Docs!
 * \arg sll_semaphore_counter_t c
 * \ret sll_semaphore_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_semaphore_index_t sll_semaphore_create(sll_semaphore_counter_t c);



/**
 * \flags check_output func
 * \name sll_semaphore_delete
 * \group semaphore
 * \desc Docs!
 * \arg sll_semaphore_index_t s
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_semaphore_delete(sll_semaphore_index_t s);



/**
 * \flags check_output func
 * \name sll_semaphore_release
 * \group semaphore
 * \desc Docs!
 * \arg sll_semaphore_index_t l
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_semaphore_release(sll_semaphore_index_t l);



#endif

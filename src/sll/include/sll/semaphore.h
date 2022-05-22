#ifndef __SLL_SEMAPHORE_H__
#define __SLL_SEMAPHORE_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Semaphores
 * \group semaphore
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_SEMAPHORE_INDEX
 * \group semaphore
 * \desc Docs!
 * \type sll_semaphore_index_t
 */
#define SLL_MAX_SEMAPHORE_INDEX __SLL_U32_MAX



/**
 * \flags type var
 * \name sll_semaphore_counter_t
 * \group semaphore
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_semaphore_counter_t;



/**
 * \flags type var
 * \name sll_semaphore_index_t
 * \group semaphore
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_semaphore_index_t;



/**
 * \flags check_output func
 * \name sll_semaphore_create
 * \group semaphore
 * \desc Docs!
 * \arg sll_semaphore_counter_t count
 * \ret sll_semaphore_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_semaphore_index_t sll_semaphore_create(sll_semaphore_counter_t count);



/**
 * \flags check_output func
 * \name sll_semaphore_delete
 * \group semaphore
 * \desc Docs!
 * \arg sll_semaphore_index_t sempahore_index
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_semaphore_delete(sll_semaphore_index_t sempahore_index);



/**
 * \flags check_output func
 * \name sll_semaphore_release
 * \group semaphore
 * \desc Docs!
 * \arg sll_semaphore_index_t sempahore_index
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_semaphore_release(sll_semaphore_index_t sempahore_index);



#endif

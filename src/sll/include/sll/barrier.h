#ifndef __SLL_BARRIER_H__
#define __SLL_BARRIER_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Barriers
 * \group barrier
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_BARRIER_INDEX
 * \group barrier
 * \desc Docs!
 * \type sll_barrier_index_t
 */
#define SLL_MAX_BARRIER_INDEX __SLL_U32_MAX



/**
 * \flags type var
 * \name sll_barrier_counter_t
 * \group barrier
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_barrier_counter_t;



/**
 * \flags type var
 * \name sll_barrier_index_t
 * \group barrier
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_barrier_index_t;



/**
 * \flags check_output func
 * \name sll_barrier_create
 * \group barrier
 * \desc Docs!
 * \ret sll_barrier_index_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_barrier_index_t sll_barrier_create(void);



/**
 * \flags check_output func
 * \name sll_barrier_delete
 * \group barrier
 * \desc Docs!
 * \arg sll_barrier_index_t barrier_indexarrier_index
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_barrier_delete(sll_barrier_index_t barrier_index);



/**
 * \flags check_output func
 * \name sll_barrier_increase
 * \group barrier
 * \desc Docs!
 * \arg sll_barrier_index_t barrier_index
 * \ret sll_barrier_counter_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_barrier_counter_t sll_barrier_increase(sll_barrier_index_t barrier_index);



/**
 * \flags check_output func
 * \name sll_barrier_reset
 * \group barrier
 * \desc Docs!
 * \arg sll_barrier_index_t barrier_index
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_barrier_reset(sll_barrier_index_t barrier_index);



#endif

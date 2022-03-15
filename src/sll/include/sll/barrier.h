#ifndef __SLL_BARRIER_H__
#define __SLL_BARRIER_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Barriers
 * \group barrier
 * \desc Docs!
 */



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
 * \arg sll_barrier_index_t b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_barrier_delete(sll_barrier_index_t b);



/**
 * \flags check_output func
 * \name sll_barrier_increase
 * \group barrier
 * \desc Docs!
 * \arg sll_barrier_index_t b
 * \ret sll_barrier_counter_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_barrier_counter_t sll_barrier_increase(sll_barrier_index_t b);



/**
 * \flags check_output func
 * \name sll_barrier_reset
 * \group barrier
 * \desc Docs!
 * \arg sll_barrier_index_t b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_barrier_reset(sll_barrier_index_t b);



#endif

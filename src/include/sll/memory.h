#ifndef __SLL_MEMORY_H__
#define __SLL_MEMORY_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_MEMORY_MOVE_DIRECTION_FROM_STACK 0
#define SLL_MEMORY_MOVE_DIRECTION_TO_STACK 1



/**
 * \flags check_output func
 * \name sll_allocate
 * \desc Docs!
 * \arg sll_size_t sz
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate(sll_size_t sz);



/**
 * \flags check_output func
 * \name sll_allocate_stack
 * \desc Docs!
 * \arg sll_size_t sz
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate_stack(sll_size_t sz);



/**
 * \flags func
 * \name sll_deallocate
 * \desc Docs!
 * \arg void* p
 */
__SLL_EXTERNAL void sll_deallocate(void* p);



/**
 * \flags func
 * \name sll_memory_move
 * \desc Docs!
 * \arg void* p
 * \arg sll_bool_t d
 * \ret void*
 */
__SLL_EXTERNAL void* sll_memory_move(void* p,sll_bool_t d);



/**
 * \flags check_output func
 * \name sll_reallocate
 * \desc Docs!
 * \arg void* p
 * \arg sll_size_t sz
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_reallocate(void* p,sll_size_t sz);



/**
 * \flags check_output func
 * \name sll_zero_allocate
 * \desc Docs!
 * \arg sll_size_t sz
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate(sll_size_t sz);



/**
 * \flags check_output func
 * \name sll_zero_allocate_stack
 * \desc Docs!
 * \arg sll_size_t sz
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate_stack(sll_size_t sz);



#endif

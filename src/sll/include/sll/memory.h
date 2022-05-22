#ifndef __SLL_MEMORY_H__
#define __SLL_MEMORY_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Memory Management
 * \group memory
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MEMORY_MOVE_DIRECTION_FROM_STACK
 * \group memory
 * \desc Docs!
 * \type sll_bool_t
 */
#define SLL_MEMORY_MOVE_DIRECTION_FROM_STACK 0



/**
 * \flags macro var
 * \name SLL_MEMORY_MOVE_DIRECTION_TO_STACK
 * \group memory
 * \desc Docs!
 * \type sll_bool_t
 */
#define SLL_MEMORY_MOVE_DIRECTION_TO_STACK 1



/**
 * \flags check_output func
 * \name sll_allocate
 * \group memory
 * \desc Docs!
 * \arg sll_size_t size
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate(sll_size_t size);



/**
 * \flags check_output func
 * \name sll_allocate_raw
 * \group memory
 * \desc Docs!
 * \arg sll_size_t size
 * \arg sll_bool_t fail_on_error
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate_raw(sll_size_t size,sll_bool_t fail_on_error);



/**
 * \flags check_output func
 * \name sll_allocate_stack
 * \group memory
 * \desc Docs!
 * \arg sll_size_t size
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate_stack(sll_size_t size);



/**
 * \flags check_output func
 * \name sll_allocate_stack_raw
 * \group memory
 * \desc Docs!
 * \arg sll_size_t size
 * \arg sll_bool_t fail_on_error
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocate_stack_raw(sll_size_t size,sll_bool_t fail_on_error);



/**
 * \flags func
 * \name sll_deallocate
 * \group memory
 * \desc Docs!
 * \arg void* pointer
 */
__SLL_EXTERNAL void sll_deallocate(void* pointer);



/**
 * \flags func
 * \name sll_memory_move
 * \group memory
 * \desc Docs!
 * \arg void* pointer
 * \arg sll_bool_t direction
 * \ret void*
 */
__SLL_EXTERNAL void* sll_memory_move(void* pointer,sll_bool_t direction);



/**
 * \flags check_output func
 * \name sll_reallocate
 * \group memory
 * \desc Docs!
 * \arg void* pointer
 * \arg sll_size_t size
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_reallocate(void* pointer,sll_size_t size);



/**
 * \flags check_output func
 * \name sll_reallocate_raw
 * \group memory
 * \desc Docs!
 * \arg void* pointer
 * \arg sll_size_t size
 * \arg sll_bool_t fail_on_error
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_reallocate_raw(void* pointer,sll_size_t size,sll_bool_t fail_on_error);



/**
 * \flags check_output func
 * \name sll_zero_allocate
 * \group memory
 * \desc Docs!
 * \arg sll_size_t size
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate(sll_size_t size);



/**
 * \flags check_output func
 * \name sll_zero_allocate_raw
 * \group memory
 * \desc Docs!
 * \arg sll_size_t size
 * \arg sll_bool_t fail_on_error
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate_raw(sll_size_t size,sll_bool_t fail_on_error);



/**
 * \flags check_output func
 * \name sll_zero_allocate_stack
 * \group memory
 * \desc Docs!
 * \arg sll_size_t size
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate_stack(sll_size_t size);



/**
 * \flags check_output func
 * \name sll_zero_allocate_stack_raw
 * \group memory
 * \desc Docs!
 * \arg sll_size_t size
 * \arg sll_bool_t fail_on_error
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_zero_allocate_stack_raw(sll_size_t size,sll_bool_t fail_on_error);



#endif

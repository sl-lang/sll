#ifndef __SLL_ALLOCATOR_H__
#define __SLL_ALLOCATOR_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Allocator
 * \group allocator
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_allocator_collapse
 * \group allocator
 * \desc Docs!
 * \arg void** pointer
 * \arg sll_size_t size
 */
__SLL_EXTERNAL void sll_allocator_collapse(void** pointer,sll_size_t size);



/**
 * \flags check_output func
 * \name sll_allocator_from_memory
 * \group allocator
 * \desc Docs!
 * \arg void* pointer
 * \arg sll_size_t size
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocator_from_memory(void* pointer,sll_size_t size);



/**
 * \flags check_output func
 * \name sll_allocator_init
 * \group allocator
 * \desc Docs!
 * \arg sll_size_t size
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocator_init(sll_size_t size);



/**
 * \flags func
 * \name sll_allocator_move
 * \group allocator
 * \desc Docs!
 * \arg void** pointer
 * \arg sll_bool_t direction
 */
__SLL_EXTERNAL void sll_allocator_move(void** pointer,sll_bool_t direction);



/**
 * \flags func
 * \name sll_allocator_release
 * \group allocator
 * \desc Docs!
 * \arg void* pointer
 */
__SLL_EXTERNAL void sll_allocator_release(void* pointer);



/**
 * \flags func
 * \name sll_allocator_resize
 * \group allocator
 * \desc Docs!
 * \arg void** pointer
 * \arg sll_size_t size
 */
__SLL_EXTERNAL void sll_allocator_resize(void** pointer,sll_size_t size);



#endif

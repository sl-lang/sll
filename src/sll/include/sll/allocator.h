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
 * \flags check_output func
 * \name sll_allocator_from_memory
 * \group allocator
 * \desc Docs!
 * \arg void* ptr
 * \arg sll_size_t sz
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocator_from_memory(void* ptr,sll_size_t sz);



/**
 * \flags check_output func
 * \name sll_allocator_init
 * \group allocator
 * \desc Docs!
 * \arg sll_size_t sz
 * \ret void*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocator_init(sll_size_t sz);



/**
 * \flags func
 * \name sll_allocator_move
 * \group allocator
 * \desc Docs!
 * \arg void** a
 * \arg sll_bool_t d
 */
__SLL_EXTERNAL void sll_allocator_move(void** a,sll_bool_t d);



/**
 * \flags func
 * \name sll_allocator_release
 * \group allocator
 * \desc Docs!
 * \arg void* v
 */
__SLL_EXTERNAL void sll_allocator_release(void* a);



/**
 * \flags func
 * \name sll_allocator_resize
 * \group allocator
 * \desc Docs!
 * \arg void** a
 * \arg sll_size_t sz
 */
__SLL_EXTERNAL void sll_allocator_resize(void** a,sll_size_t sz);



#endif

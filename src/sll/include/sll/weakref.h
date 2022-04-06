#ifndef __SLL_WEAKREF_H__
#define __SLL_WEAKREF_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Weak References
 * \group weakref
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_weakref_clone
 * \group weakref
 * \desc Docs!
 * \arg sll_weak_ref_t wr
 * \ret sll_weak_ref_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_weak_ref_t sll_weakref_clone(sll_weak_ref_t wr);



/**
 * \flags check_output func
 * \name sll_weakref_create
 * \group weakref
 * \desc Docs!
 * \arg sll_object_t* o
 * \ret sll_weak_ref_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_weak_ref_t sll_weakref_create(sll_object_t* o);



/**
 * \flags check_output func
 * \name sll_weakref_delete
 * \group weakref
 * \desc Docs!
 * \arg sll_weak_ref_t wr
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_weakref_delete(sll_weak_ref_t wr);



/**
 * \flags check_output func
 * \name sll_weakref_get
 * \group weakref
 * \desc Docs!
 * \arg sll_weak_ref_t wr
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_weakref_get(sll_weak_ref_t wr);



/**
 * \flags func
 * \name sll_weakref_set_callback
 * \group weakref
 * \desc Docs!
 * \arg sll_weak_ref_t wr
 * \arg sll_weak_ref_destructor_t cb
 * \arg void* arg
 */
__SLL_EXTERNAL void sll_weakref_set_callback(sll_weak_ref_t wr,sll_weak_ref_destructor_t cb,void* arg);



#endif

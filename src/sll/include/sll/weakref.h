#ifndef __SLL_WEAKREF_H__
#define __SLL_WEAKREF_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Weak References
 * \group weakref
 * \desc Docs!
 */



/**
 * \flags type var
 * \name sll_weak_reference_t
 * \group weakref
 * \desc Docs!
 * \type void*
 */
typedef void* sll_weak_reference_t;



/**
 * \flags func type
 * \name sll_weak_ref_destructor_t
 * \group weakref
 * \desc Docs!
 * \arg sll_weak_reference_t weak_reference
 * \arg sll_object_t object
 * \arg void* arg
 */
typedef void (*sll_weak_ref_destructor_t)(sll_weak_reference_t weak_reference,sll_object_t object,void* arg);



/**
 * \flags check_output func
 * \name sll_weakref_clone
 * \group weakref
 * \desc Docs!
 * \arg sll_weak_reference_t weak_reference
 * \ret sll_weak_reference_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_weak_reference_t sll_weakref_clone(sll_weak_reference_t weak_reference);



/**
 * \flags check_output func
 * \name sll_weakref_create
 * \group weakref
 * \desc Docs!
 * \arg sll_object_t object
 * \ret sll_weak_reference_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_weak_reference_t sll_weakref_create(sll_object_t object);



/**
 * \flags check_output func
 * \name sll_weakref_delete
 * \group weakref
 * \desc Docs!
 * \arg sll_weak_reference_t weak_reference
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_weakref_delete(sll_weak_reference_t weak_reference);



/**
 * \flags check_output func
 * \name sll_weakref_get
 * \group weakref
 * \desc Docs!
 * \arg sll_weak_reference_t weak_reference
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_weakref_get(sll_weak_reference_t weak_reference);



/**
 * \flags func
 * \name sll_weakref_set_callback
 * \group weakref
 * \desc Docs!
 * \arg sll_weak_reference_t weak_reference
 * \arg sll_weak_ref_destructor_t destructor
 * \arg void* arg
 */
__SLL_EXTERNAL void sll_weakref_set_callback(sll_weak_reference_t weak_reference,sll_weak_ref_destructor_t destructor,void* arg);



#endif

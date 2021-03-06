#ifndef __SLL_GC_H__
#define __SLL_GC_H__ 1
#include <sll/_object_types.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/object.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Garbage Collection
 * \group gc
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_OBJECT_REFERENCE_COUNTER_MASK
 * \group gc
 * \desc Docs!
 * \type sll_reference_count_t
 */
#define SLL_OBJECT_REFERENCE_COUNTER_MASK 0xffffffffffffull



/**
 * \flags func macro
 * \name SLL_GET_OBJECT_REFERENCE_COUNTER
 * \group gc
 * \desc Docs!
 * \arg sll_object_t o
 * \ret sll_reference_count_t
 */
#define SLL_GET_OBJECT_REFERENCE_COUNTER(o) ((o)->reference_count&SLL_OBJECT_REFERENCE_COUNTER_MASK)



/**
 * \flags func macro
 * \name SLL_ACQUIRE
 * \group gc
 * \desc Docs!
 * \arg sll_object_t object
 */
#define SLL_ACQUIRE(object) ((object)->reference_count++)



/**
 * \flags func macro
 * \name SLL_RELEASE
 * \group gc
 * \desc Docs!
 * \arg sll_object_t object
 */
#ifdef DEBUG_BUILD
#define SLL_RELEASE(object) \
	do{ \
		sll_object_t __o=(object); \
		if (!SLL_GET_OBJECT_REFERENCE_COUNTER(__o)){ \
			sll__gc_error(__o); \
		} \
		__o->reference_count--; \
		if (!SLL_GET_OBJECT_REFERENCE_COUNTER(__o)){ \
			sll__release_object_internal(__o); \
		} \
	} while (0)
#else
#define SLL_RELEASE(object) \
	do{ \
		sll_object_t __o=(object); \
		__o->reference_count--; \
		if (!SLL_GET_OBJECT_REFERENCE_COUNTER(__o)){ \
			sll__release_object_internal(__o); \
		} \
	} while (0)
#endif



/**
 * \flags func
 * \name sll__gc_error
 * \group gc
 * \desc Docs!
 * \arg sll_object_t object
 */
__SLL_EXTERNAL void sll__gc_error(sll_object_t object);



/**
 * \flags func
 * \name sll__release_object_internal
 * \group gc
 * \desc Docs!
 * \arg sll_object_t object
 */
__SLL_EXTERNAL void sll__release_object_internal(sll_object_t object);



/**
 * \flags func
 * \name sll_acquire_object
 * \group gc
 * \desc Docs!
 * \arg sll_object_t object
 */
__SLL_EXTERNAL void sll_acquire_object(sll_object_t object);



/**
 * \flags check_output func
 * \name sll_create_object
 * \group gc
 * \desc Docs!
 * \arg sll_object_type_t type
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t sll_create_object(sll_object_type_t type);



/**
 * \flags check_output func
 * \name sll_destroy_object
 * \group gc
 * \desc Docs!
 * \arg sll_object_t object
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_destroy_object(sll_object_t object);



/**
 * \flags func
 * \name sll_gc_add_root
 * \group gc
 * \desc Docs!
 * \arg sll_object_t object
 * \arg sll_bool_t fast
 */
__SLL_EXTERNAL void sll_gc_add_root(sll_object_t object,sll_bool_t fast);



/**
 * \flags func
 * \name sll_gc_add_roots
 * \group gc
 * \desc Docs!
 * \arg const sll_object_t* pointer
 * \arg sll_size_t length
 */
__SLL_EXTERNAL void sll_gc_add_roots(const sll_object_t* pointer,sll_size_t length);



/**
 * \flags func
 * \name sll_gc_collect
 * \group gc
 * \desc Docs!
 * \arg void
 */
__SLL_EXTERNAL void sll_gc_collect(void);



/**
 * \flags func
 * \name sll_gc_remove_root
 * \group gc
 * \desc Docs!
 * \arg sll_object_t object
 */
__SLL_EXTERNAL void sll_gc_remove_root(sll_object_t object);



/**
 * \flags func
 * \name sll_gc_remove_roots
 * \group gc
 * \desc Docs!
 * \arg const sll_object_t* pointer
 */
__SLL_EXTERNAL void sll_gc_remove_roots(const sll_object_t* pointer);



/**
 * \flags func
 * \name sll_release_object
 * \group gc
 * \desc Docs!
 * \arg sll_object_t object
 */
__SLL_EXTERNAL void sll_release_object(sll_object_t object);



#endif

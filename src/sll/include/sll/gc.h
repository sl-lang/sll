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
 * \flags func macro
 * \name SLL_ACQUIRE
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* object
 */
#define SLL_ACQUIRE(object) ((object)->rc++)



/**
 * \flags func macro
 * \name SLL_RELEASE
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* object
 */
#ifdef DEBUG_BUILD
#define SLL_RELEASE(object) \
	do{ \
		sll_object_t* __o=(object); \
		if (!__o->rc){ \
			sll__gc_error(__o); \
		} \
		__o->rc--; \
		if (!__o->rc){ \
			sll__release_object_internal(__o); \
		} \
	} while (0)
#else
#define SLL_RELEASE(object) \
	do{ \
		sll_object_t* __o=(object); \
		__o->rc--; \
		if (!__o->rc){ \
			sll__release_object_internal(__o); \
		} \
	} while (0)
#endif



/**
 * \flags func
 * \name sll__gc_error
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* object
 */
__SLL_EXTERNAL void sll__gc_error(sll_object_t* object);



/**
 * \flags func
 * \name sll__release_object_internal
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* object
 */
__SLL_EXTERNAL void sll__release_object_internal(sll_object_t* object);



/**
 * \flags func
 * \name sll_acquire_object
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* object
 */
__SLL_EXTERNAL void sll_acquire_object(sll_object_t* object);



/**
 * \flags check_output func
 * \name sll_create_object
 * \group gc
 * \desc Docs!
 * \arg sll_object_type_t type
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_object(sll_object_type_t type);



/**
 * \flags check_output func
 * \name sll_destroy_object
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* object
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_destroy_object(sll_object_t* object);



/**
 * \flags func
 * \name sll_gc_add_root
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* object
 */
__SLL_EXTERNAL void sll_gc_add_root(sll_object_t* object);



/**
 * \flags func
 * \name sll_gc_add_roots
 * \group gc
 * \desc Docs!
 * \arg sll_object_t*const* pointer
 * \arg sll_size_t length
 */
__SLL_EXTERNAL void sll_gc_add_roots(sll_object_t*const* pointer,sll_size_t length);



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
 * \arg sll_object_t* object
 */
__SLL_EXTERNAL void sll_gc_remove_root(sll_object_t* object);



/**
 * \flags func
 * \name sll_gc_remove_roots
 * \group gc
 * \desc Docs!
 * \arg sll_object_t*const* pointer
 */
__SLL_EXTERNAL void sll_gc_remove_roots(sll_object_t*const* pointer);



/**
 * \flags func
 * \name sll_release_object
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* object
 */
__SLL_EXTERNAL void sll_release_object(sll_object_t* object);



#endif

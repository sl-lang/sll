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
 * \arg sll_object_t* o
 */
#define SLL_ACQUIRE(o) ((o)->rc++)



/**
 * \flags func macro
 * \name SLL_RELEASE
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* o
 */
#ifdef DEBUG_BUILD
#define SLL_RELEASE(o) \
	do{ \
		sll_object_t* __o=(o); \
		if (!__o->rc){ \
			sll__gc_error(__o); \
		} \
		__o->rc--; \
		if (!__o->rc){ \
			sll__release_object_internal(__o); \
		} \
	} while (0)
#else
#define SLL_RELEASE(o) \
	do{ \
		sll_object_t* __o=(o); \
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
 * \arg sll_object_t* o
 */
__SLL_EXTERNAL void sll__gc_error(sll_object_t* o);



/**
 * \flags func
 * \name sll__release_object_internal
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* o
 */
__SLL_EXTERNAL void sll__release_object_internal(sll_object_t* o);



/**
 * \flags func
 * \name sll_acquire_object
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* o
 */
__SLL_EXTERNAL void sll_acquire_object(sll_object_t* o);



/**
 * \flags check_output func
 * \name sll_create_object
 * \group gc
 * \desc Docs!
 * \arg sll_object_type_t t
 * \arg sll_object_t* out
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_object(sll_object_type_t t);



/**
 * \flags check_output func
 * \name sll_destroy_object
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* o
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_destroy_object(sll_object_t* o);



/**
 * \flags func
 * \name sll_release_object
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* o
 */
__SLL_EXTERNAL void sll_release_object(sll_object_t* o);



#endif

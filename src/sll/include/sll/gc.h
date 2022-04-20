#ifndef __SLL_GC_H__
#define __SLL_GC_H__ 1
#include <sll/common.h>
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
 * \arg sll_object_t* x
 */
#define SLL_ACQUIRE(x) ((x)->rc++)



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

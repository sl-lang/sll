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



#define SLL_ACQUIRE(x) SLL_ACQUIRE_NO_DEBUG(__SLL_ADD_DEBUG_DATA((x),__SLL_DEBUG_TYPE_ACQUIRE))
#define SLL_ACQUIRE_NO_DEBUG(x) ((x)->rc++)
#define SLL_CREATE() __SLL_ADD_DEBUG_DATA(sll_create_object(),__SLL_DEBUG_TYPE_CREATE)
#define SLL_GC_ZERO_DEBUG_DATA(r) \
	do{ \
		(r)->_dbg0=0xff; \
		(r)->_dbg1=0xffff; \
	} while (0)
#define SLL_GC_ZERO_DEBUG_DATA_STRUCT ._dbg0=0xff,._dbg1=0xffff
#define SLL_RELEASE(x) sll_release_object(__SLL_ADD_DEBUG_DATA((x),__SLL_DEBUG_TYPE_RELEASE))



/**
 * \flags check_output func
 * \name sll_add_debug_data
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* o
 * \arg const char* fp
 * \arg unsigned int ln
 * \arg const char* fn
 * \arg unsigned int t
 * \ret sll_object_t*
 */
__SLL_EXTERNAL sll_object_t* sll_add_debug_data(sll_object_t* o,const char* fp,unsigned int ln,const char* fn,unsigned int t);



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
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_create_object(void);



/**
 * \flags func
 * \name sll_release_object
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* o
 */
__SLL_EXTERNAL void sll_release_object(sll_object_t* o);



/**
 * \flags func
 * \name sll_remove_debug_data
 * \group gc
 * \desc Docs!
 * \arg sll_object_t* o
 */
__SLL_EXTERNAL void sll_remove_debug_data(sll_object_t* o);



/**
 * \flags check_output func
 * \name sll_verify_object_stack_cleanup
 * \group gc
 * \desc Docs!
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_verify_object_stack_cleanup(void);



#endif

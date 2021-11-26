#ifndef __SLL_API_MEMORY_H__
#define __SLL_API_MEMORY_H__ 1
#include <sll/common.h>
#include <sll/types.h>



/**
 * \flags check_output func
 * \name sll_memory_from_object
 * \desc Docs!
 * \arg sll_object_t* v
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_memory_from_object(sll_object_t* v);



/**
 * \flags check_output func
 * \name sll_memory_from_pointer
 * \desc Docs!
 * \arg void* p
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_memory_from_pointer(void* p);



/**
 * \flags check_output func
 * \name sll_memory_get_null_pointer
 * \desc Docs!
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_memory_get_null_pointer(void);



#endif

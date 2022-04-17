#ifndef __SLL_API_ARRAY_H__
#define __SLL_API_ARRAY_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Array API
 * \group array-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_array_count
 * \group array-api
 * \desc Docs!
 * \api ao|i
 * \arg sll_array_t* a
 * \arg sll_object_t* b
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_array_count(sll_array_t* a,sll_object_t* b);



/**
 * \flags func
 * \name sll_api_array_create
 * \group array-api
 * \desc Docs!
 * \api i|a
 * \arg sll_integer_t a
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_create(sll_integer_t a,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_array_extend
 * \group array-api
 * \desc Docs!
 * \api aa|v
 * \arg sll_array_t* a
 * \arg sll_array_t* b
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_extend(sll_array_t* a,sll_array_t* b);



/**
 * \flags func
 * \name sll_api_array_index
 * \group array-api
 * \desc Docs!
 * \api ao|i
 * \arg sll_array_t* a
 * \arg sll_object_t* b
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_array_index(sll_array_t* a,sll_object_t* b);



/**
 * \flags func
 * \name sll_api_array_join
 * \group array-api
 * \desc Docs!
 * \api a+o|a
 * \arg sll_array_t*const* a
 * \arg sll_arg_count_t ac
 * \arg sll_object_t* b
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_join(sll_array_t*const* a,sll_arg_count_t ac,sll_object_t* b,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_array_pop
 * \group array-api
 * \desc Docs!
 * \api a|o
 * \arg sll_array_t* a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_pop(sll_array_t* a);



/**
 * \flags func
 * \name sll_api_array_push
 * \group array-api
 * \desc Docs!
 * \api ao|v
 * \arg sll_array_t* a
 * \arg sll_object_t* b
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_push(sll_array_t* a,sll_object_t* b);



/**
 * \flags func
 * \name sll_api_array_remove
 * \group array-api
 * \desc Docs!
 * \api ao|v
 * \arg sll_array_t* a
 * \arg sll_object_t* b
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_remove(sll_array_t* a,sll_object_t* b);



/**
 * \flags func
 * \name sll_api_array_replace
 * \group array-api
 * \desc Docs!
 * \api aoo|a
 * \arg sll_array_t* a
 * \arg sll_object_t* b
 * \arg sll_object_t* c
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_replace(sll_array_t* a,sll_object_t* b,sll_object_t* c,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_array_reverse
 * \group array-api
 * \desc Docs!
 * \api a|a
 * \arg sll_array_t* a
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_reverse(sll_array_t* a,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_array_shift
 * \group array-api
 * \desc Docs!
 * \api a|o
 * \arg sll_array_t* a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_shift(sll_array_t* a);



/**
 * \flags func
 * \name sll_api_array_split
 * \group array-api
 * \desc Docs!
 * \api ao|a
 * \arg sll_array_t* a
 * \arg sll_object_t* b
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_split(sll_array_t* a,sll_object_t* b,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_array_unshift
 * \group array-api
 * \desc Docs!
 * \api ao|v
 * \arg sll_array_t* a
 * \arg sll_object_t* b
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_unshift(sll_array_t* a,sll_object_t* b);



#endif

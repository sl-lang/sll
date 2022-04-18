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
 * \flags check_output func
 * \name sll_api_array_count
 * \group array-api
 * \desc Docs!
 * \api #ao|D
 * \arg const sll_array_t* arr
 * \arg sll_object_t* elem
 * \ret sll_array_length_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_array_length_t sll_api_array_count(const sll_array_t* arr,sll_object_t* elem);



/**
 * \flags func
 * \name sll_api_array_create
 * \group array-api
 * \desc Docs!
 * \api D|a
 * \arg sll_array_length_t len
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_create(sll_array_length_t len,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_array_extend
 * \group array-api
 * \desc Docs!
 * \api a#a|D
 * \arg sll_array_t* arr
 * \arg const sll_array_t* new
 * \ret sll_array_length_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_array_length_t sll_api_array_extend(sll_array_t* arr,const sll_array_t* new);



/**
 * \flags check_output func
 * \name sll_api_array_index
 * \group array-api
 * \desc Docs!
 * \api #ao|i
 * \arg const sll_array_t* arr
 * \arg sll_object_t* elem
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_array_index(const sll_array_t* arr,sll_object_t* elem);



/**
 * \flags func
 * \name sll_api_array_join
 * \group array-api
 * \desc Docs!
 * \api #a+o|a
 * \arg const sll_array_t*const* arr
 * \arg sll_arg_count_t len
 * \arg sll_object_t* elem
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_join(const sll_array_t*const* arr,sll_arg_count_t len,sll_object_t* elem,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_array_pop
 * \group array-api
 * \desc Docs!
 * \api a|o
 * \arg sll_array_t* arr
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_pop(sll_array_t* arr);



/**
 * \flags func
 * \name sll_api_array_push
 * \group array-api
 * \desc Docs!
 * \api ao|D
 * \arg sll_array_t* arr
 * \arg sll_object_t* elem
 * \ret sll_array_length_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_array_length_t sll_api_array_push(sll_array_t* arr,sll_object_t* elem);



/**
 * \flags func
 * \name sll_api_array_remove
 * \group array-api
 * \desc Docs!
 * \api ao|D
 * \arg sll_array_t* arr
 * \arg sll_object_t* elem
 * \ret sll_array_length_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_array_length_t sll_api_array_remove(sll_array_t* arr,sll_object_t* elem);



/**
 * \flags func
 * \name sll_api_array_replace
 * \group array-api
 * \desc Docs!
 * \api #aoo|a
 * \arg const sll_array_t* arr
 * \arg sll_object_t* old
 * \arg sll_object_t* new
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_replace(const sll_array_t* arr,sll_object_t* old,sll_object_t* new,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_array_reverse
 * \group array-api
 * \desc Docs!
 * \api #a|a
 * \arg const sll_array_t* arr
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_reverse(const sll_array_t* arr,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_array_shift
 * \group array-api
 * \desc Docs!
 * \api a|o
 * \arg sll_array_t* arr
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_array_shift(sll_array_t* arr);



/**
 * \flags func
 * \name sll_api_array_split
 * \group array-api
 * \desc Docs!
 * \api #ao|a
 * \arg const sll_array_t* arr
 * \arg sll_object_t* key
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_array_split(const sll_array_t* arr,sll_object_t* key,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_array_unshift
 * \group array-api
 * \desc Docs!
 * \api ao|D
 * \arg sll_array_t* arr
 * \arg sll_object_t* elem
 * \ret sll_array_length_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_array_length_t sll_api_array_unshift(sll_array_t* arr,sll_object_t* elem);



#endif

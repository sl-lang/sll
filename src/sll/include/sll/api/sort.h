#ifndef __SLL_API_SORT_H__
#define __SLL_API_SORT_H__ 1
#include <sll/common.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Sort API
 * \group sort-api
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_api_sort_quicksort
 * \group sort-api
 * \desc Docs!
 * \api &abbi|o
 * \arg sll_object_t arr
 * \arg sll_bool_t reverse
 * \arg sll_bool_t inplace
 * \arg sll_integer_t key_fn
 * \ret sll_object_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t sll_api_sort_quicksort(sll_object_t arr,sll_bool_t reverse,sll_bool_t inplace,sll_integer_t key_fn);



/**
 * \flags func
 * \name sll_quicksort
 * \group sort-api
 * \desc Docs!
 * \arg sll_object_t* elements
 * \arg sll_array_length_t length
 * \arg sll_compare_result_t cmp
 * \arg sll_integer_t key_fn
 */
__SLL_EXTERNAL void sll_quicksort(sll_object_t* elements,sll_array_length_t length,sll_compare_result_t cmp,sll_integer_t key_fn);



#endif

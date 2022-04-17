#ifndef __SLL_API_SORT_H__
#define __SLL_API_SORT_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Sort API
 * \group sort-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_quicksort
 * \group sort-api
 * \desc Docs!
 * \arg sll_object_t** a
 * \arg sll_array_length_t l
 * \arg sll_compare_result_t cmp
 * \arg sll_integer_t fn
 */
__SLL_EXTERNAL void sll_quicksort(sll_object_t** a,sll_array_length_t l,sll_compare_result_t cmp,sll_integer_t fn);



/**
 * \flags func
 * \name sll_api_sort_quicksort
 * \group sort-api
 * \desc Docs!
 * \api abbi|o
 * \arg sll_array_t* a
 * \arg sll_bool_t b
 * \arg sll_bool_t c
 * \arg sll_integer_t d
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_sort_quicksort(sll_array_t* a,sll_bool_t b,sll_bool_t c,sll_integer_t d);



#endif

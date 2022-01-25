#ifndef __SLL_API_SORT_H__
#define __SLL_API_SORT_H__ 1
#include <sll/api.h>
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
 * \flags api func
 * \name sll_api_sort_quicksort
 * \group sort-api
 * \desc Docs!
 * \arg A
 * \arg B
 * \arg B
 * \arg I
 * \ret O
 */
__API_FUNC_DECL(sort_quicksort);



#endif

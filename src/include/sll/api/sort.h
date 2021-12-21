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
 * \arg const sll_object_t** a
 * \arg sll_array_length_t l
 * \arg sll_compare_result_t cmp
 */
__SLL_EXTERNAL void sll_quicksort(const sll_object_t** a,sll_array_length_t l,sll_compare_result_t cmp);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_sort_sort
 * \group sort-api
 * \desc Docs!
 * \arg A
 * \arg I?
 * \ret A
 */
__API_FUNC_DECL(sort_sort);



#endif

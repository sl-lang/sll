#ifndef __SLL_API_SORT_H__
#define __SLL_API_SORT_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC void sll_quicksort(const sll_runtime_object_t** a,sll_array_length_t l,sll_compare_result_t cmp);



/*~~DOCS~~
TYPE api compilation_call optimizable
FUNC sll_api_sort_sort
DESC Docs!
ARG A Docs!
ARG I? Docs!
RET a Docs!
RET I Success*/
__API_FUNC_DECL(sort_sort);



#endif

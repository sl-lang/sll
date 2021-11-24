#ifndef __SLL_BINARY_HEAP_H__
#define __SLL_BINARY_HEAP_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_INIT_BINARY_HEAP_STRUCT {NULL,0}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_binary_heap_add(sll_binary_heap_t* bh,sll_integer_t k);



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_binary_heap_contains(sll_binary_heap_t* bh,sll_integer_t k);



__SLL_EXTERNAL void sll_free_binary_heap(sll_binary_heap_t* bh);



#endif

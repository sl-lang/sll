#ifndef __SLL_MEMORY_H__
#define __SLL_MEMORY_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_allocate(sll_size_t sz);



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_allocate_stack(sll_size_t sz);



__SLL_FUNC void sll_deallocate(void* p);



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_reallocate(void* p,sll_size_t sz);



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_zero_allocate(sll_size_t sz,sll_size_t c);



#endif

#ifndef __SLL_MEMORY_H__
#define __SLL_MEMORY_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_MEMORY_MOVE_DIRECTION_FROM_STACK 0
#define SLL_MEMORY_MOVE_DIRECTION_TO_STACK 1



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_allocate(sll_size_t sz);



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_allocate_stack(sll_size_t sz);



__SLL_FUNC void sll_deallocate(void* p);



__SLL_FUNC void* sll_memory_move(void* p,sll_bool_t d);



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_reallocate(void* p,sll_size_t sz);



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_zero_allocate(sll_size_t sz);



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_zero_allocate_stack(sll_size_t sz);



#endif

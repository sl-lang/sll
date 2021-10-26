#ifndef __SLL_API_MEMORY_H__
#define __SLL_API_MEMORY_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_memory_from_object(sll_runtime_object_t* v);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_memory_from_pointer(void* p);



__SLL_FUNC __SLL_CHECK_OUTPUT sll_runtime_object_t* sll_memory_get_null_pointer(void);



#endif

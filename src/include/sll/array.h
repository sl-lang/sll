#ifndef __SLL_ARRAY_H__
#define __SLL_ARRAY_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC sll_array_t* sll_array_create(sll_array_length_t l);



__SLL_FUNC void sll_array_release(sll_array_t* s);



#endif

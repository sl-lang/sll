#ifndef __SLL_STRING_H__
#define __SLL_STRING_H__ 1
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC sll_string_t* sll_string_create(sll_string_length_t l);



__SLL_FUNC void sll_string_release(sll_string_t* s);



#endif

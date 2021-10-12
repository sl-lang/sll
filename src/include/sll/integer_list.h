#ifndef __SLL_INTEGER_LIST_H__
#define __SLL_INTEGER_LIST_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_ZERO_INTEGER_LIST_STRUCT {NULL,0}



__SLL_FUNC void sll_free_integer_list(sll_integer_list_t* il);



__SLL_FUNC sll_bool_t sll_integer_list_add(sll_integer_list_t* il,sll_integer_t k,void* v);



__SLL_FUNC sll_bool_t sll_integer_list_contains(sll_integer_list_t* il,sll_integer_t k);



__SLL_FUNC sll_bool_t sll_integer_list_get(sll_integer_list_t* il,sll_integer_t k,void** v);



__SLL_FUNC void sll_integer_list_init(sll_integer_list_t* o);



#endif

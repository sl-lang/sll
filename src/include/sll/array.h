#ifndef __SLL_ARRAY_H__
#define __SLL_ARRAY_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_ZERO_ARRAY(a) \
	do{ \
		(a)->l=0; \
		(a)->v=NULL; \
	} while (0)
#define SLL_ZERO_ARRAY_STRUCT {0,NULL}



__SLL_FUNC void sll_array_clone(const sll_array_t* a,sll_array_t* o);



__SLL_FUNC void sll_array_create(sll_array_length_t l,sll_array_t* o);



__SLL_FUNC void sll_array_join(const sll_array_t* a,const sll_array_t* b,sll_array_t* o);



__SLL_FUNC void sll_array_push(const sll_array_t* a,sll_runtime_object_t* v,sll_array_t* o);



__SLL_FUNC void sll_array_move(const sll_array_t* a,sll_integer_t v,sll_array_t* o);



#endif

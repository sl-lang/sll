#ifndef __SLL_INTEGER_HEAP_QUEUE_H__
#define __SLL_INTEGER_HEAP_QUEUE_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_ZERO_INTEGER_HEAP_QUEUE(q) \
	do{ \
		(q)->v=NULL; \
		(q)->l=0; \
	} while (0)
#define SLL_ZERO_INTEGER_HEAP_QUEUE_STRUCT {NULL,0}



__SLL_FUNC void sll_free_integer_heap_queue(sll_integer_heap_queue_t* il);



__SLL_FUNC sll_bool_t sll_integer_heap_queue_add(sll_integer_heap_queue_t* il,sll_integer_t k);



__SLL_FUNC sll_bool_t sll_integer_heap_queue_contains(sll_integer_heap_queue_t* il,sll_integer_t k);



__SLL_FUNC void sll_integer_heap_queue_init(sll_integer_heap_queue_t* o);



#endif

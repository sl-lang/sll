#ifndef __SLL_ARRAY_H__
#define __SLL_ARRAY_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_SAME_ARRAY(a,b) ((a)->v==(b)->v)
#define SLL_ZERO_ARRAY(a) \
	do{ \
		(a)->l=0; \
		(a)->v=NULL; \
	} while (0)
#define SLL_ZERO_ARRAY_STRUCT {0,NULL}



__SLL_FUNC void sll_array_create(sll_array_length_t l,sll_array_t* o);



#endif

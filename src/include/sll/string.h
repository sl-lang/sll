#ifndef __SLL_STRING_H__
#define __SLL_STRING_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_SAME_STRING(a,b) ((a)->v==(b)->v)
#define SLL_ZERO_STRING(s) \
	do{ \
		(s)->l=0; \
		(s)->c=0; \
		(s)->v=NULL; \
	} while (0)
#define SLL_ZERO_STRING_STRUCT {0,0,NULL}



__SLL_FUNC void sll_string_create(sll_string_length_t l,sll_string_t* o);



__SLL_FUNC void sll_string_clone(const sll_string_t* s,sll_string_t* d);



#endif

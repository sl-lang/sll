#ifndef __SLL_MAP_H__
#define __SLL_MAP_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_SAME_MAP(a,b) ((a)->v==(b)->v)
#define SLL_ZERO_MAP(m) \
	do{ \
		(m)->l=0; \
		(m)->v=NULL; \
	} while (0)
#define SLL_ZERO_MAP_STRUCT {0,NULL}



__SLL_FUNC void sll_map_create(sll_map_length_t l,sll_map_t* o);



#endif

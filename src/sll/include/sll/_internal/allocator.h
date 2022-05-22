#ifndef __SLL__INTERNAL_ALLOCATOR_H__
#define __SLL__INTERNAL_ALLOCATOR_H__ 1
#include <sll/_size_types.h>



#define ALLOCATOR_HEADER_GET_SIZE(h) ((h)->data)
#define ALLOCATOR_HEADER_INIT(h,sz) \
	do{ \
		(h)->data=(sz); \
	} while (0)



typedef struct _ALLOCATOR_HEADER{
	__SLL_U64 data;
} allocator_header_t;



#endif

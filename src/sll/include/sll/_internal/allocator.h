#ifndef __SLL__INTERNAL_ALLOCATOR_H__
#define __SLL__INTERNAL_ALLOCATOR_H__ 1
#include <sll/_size_types.h>



#define ALLOCATOR_HEADER_GET_SIZE(h) ((h)->dt)
#define ALLOCATOR_HEADER_INIT(h,sz) \
	do{ \
		(h)->dt=(sz); \
	} while (0)



typedef struct _ALLOCATOR_HEADER{
	__SLL_U64 dt;
} allocator_header_t;



#endif

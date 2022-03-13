#ifndef __SLL_INTERNAL_GC_H__
#define __SLL_INTERNAL_GC_H__ 1
#include <sll/string.h>
#include <sll/types.h>



#define GC_INIT_PAGE_COUNT 4
#define GC_GET_NEXT_OBJECT(o) ((sll_object_t*)((o)->dt.s.v))
#define GC_SET_NEXT_OBJECT(o,n) ((o)->dt.s.v=SLL_CHAR((n)))

#define GC_OBJECT_POOL_ALLOC_SIZE 65536



void _gc_release_data(void);




#endif

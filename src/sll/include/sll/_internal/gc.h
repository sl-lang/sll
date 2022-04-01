#ifndef __SLL__INTERNAL_GC_H__
#define __SLL__INTERNAL_GC_H__ 1
#include <sll/_internal/common.h>
#include <sll/types.h>



#define GC_PAGE_POOL_SIZE 256
#define GC_PAGE_INIT_POOL_SIZE 128
#define GC_PAGE_DYNAMIC_POOL_SIZE 32

#define GC_FLAG_HAS_WEAKREF 1

#define GC_RELEASE(o) \
	do{ \
		sll_object_t* __o=(o); \
		SLL_ASSERT(__o->rc); \
		__o->rc--; \
		if (!__o->rc){ \
			_gc_release_object(__o); \
		} \
	} while (0)



typedef struct __GC_PAGE_HEADER{
	struct __GC_PAGE_HEADER* p;
	struct __GC_PAGE_HEADER* n;
	sll_size_t cnt;
} gc_page_header_t;



void _gc_release_data(void);



void _gc_release_object(sll_object_t* o);



#endif

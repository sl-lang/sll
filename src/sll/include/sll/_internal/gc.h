#ifndef __SLL__INTERNAL_GC_H__
#define __SLL__INTERNAL_GC_H__ 1
#include <sll/_internal/common.h>
#include <sll/types.h>



#define GC_PAGE_POOL_SIZE 4
#define GC_PAGE_INIT_POOL_SIZE 4
#define GC_PAGE_DYNAMIC_POOL_SIZE 2

#define GC_FLAG_HAS_WEAKREF 1
#define GC_FLAG_IN_FAST_POOL 2

#define GC_MEMORY_PAGE_SIZE_SHIFT 15
#define GC_MEMORY_PAGE_SIZE (1ull<<GC_MEMORY_PAGE_SIZE_SHIFT)
#define GC_MEMORY_PAGE_HEADER(o) ((gc_page_header_t*)PTR(ADDR(o)&(0-GC_MEMORY_PAGE_SIZE)))

#define GC_OBJECT_POOL_SIZE 2048

#define GC_FAST_OBJECT_POOL_SIZE 64

#define GC_PAGE_HEADER_DEALLOCATION_THRESHOLD 128

#define GC_PAGE_HEADER_CAN_DELETE(pg) ((pg)->cnt==1)
#define GC_PAGE_HEADER_INCREASE(pg) \
	do{ \
		gc_page_header_t* __pg=(pg); \
		__pg->cnt+=2; \
		if (__pg->cnt>=GC_PAGE_HEADER_DEALLOCATION_THRESHOLD){ \
			__pg->cnt|=1; \
		} \
	} while (0)
#define GC_PAGE_HEADER_DECREASE(pg) \
	do{ \
		(pg)->cnt-=2; \
	} while (0)



typedef struct _GC_PAGE_HEADER{
	sll_size_t cnt;
	struct _GC_PAGE_HEADER* prev;
	struct _GC_PAGE_HEADER* next;
} gc_page_header_t;



typedef struct _GC_FAST_OBJECT_POOL{
	sll_object_t* data[GC_FAST_OBJECT_POOL_SIZE];
	__SLL_U8 read;
	__SLL_U8 write;
	__SLL_U8 space;
} gc_fast_object_pool_t;



void _gc_release_data(void);



#endif

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
#define GC_MEMORY_PAGE_POINTER(o) PTR(ADDR(o)&(0-GC_MEMORY_PAGE_SIZE))

#define GC_OBJECT_POOL_SIZE 512



typedef struct _GC_PAGE_HEADER{
	addr_t addr;
	sll_size_t cnt;
} gc_page_header_t;



void _gc_release_data(void);



#endif

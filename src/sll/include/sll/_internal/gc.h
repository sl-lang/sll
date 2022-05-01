#ifndef __SLL__INTERNAL_GC_H__
#define __SLL__INTERNAL_GC_H__ 1
#include <sll/_internal/common.h>
#include <sll/types.h>



#define GC_PAGE_POOL_SIZE 4
#define GC_PAGE_INIT_POOL_SIZE 4
#define GC_PAGE_DYNAMIC_POOL_SIZE 2

#define GC_FLAG_HAS_WEAKREF 1

#define GC_MEMORY_PAGE_SIZE 32768
#define GC_MEMORY_PAGE_HEADER_ADDR(o) ((gc_page_header_t*)PTR(ADDR(o)&(0-((sll_size_t)GC_MEMORY_PAGE_SIZE))))



typedef struct _GC_PAGE_HEADER{
	struct _GC_PAGE_HEADER* p;
	struct _GC_PAGE_HEADER* n;
	sll_size_t cnt;
} gc_page_header_t;



void _gc_release_data(void);



#endif

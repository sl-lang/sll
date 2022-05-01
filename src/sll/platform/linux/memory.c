#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/platform/memory.h>
#include <sll/platform/util.h>
#include <sll/types.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>



#ifndef __SLL_BUILD_DARWIN
static sll_bool_t _linux_huge_tlb=1;
#endif



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_size_t sz,sll_bool_t l,sll_error_t* err){
	ERROR_PTR_RESET;
#ifndef __SLL_BUILD_DARWIN
	if (_linux_huge_tlb&&l){
		SLL_ASSERT(SLL_ROUND_LARGE_PAGE(sz)==sz);
		void* o=mmap(NULL,sz,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_HUGETLB,-1,0);
		if (o!=MAP_FAILED){
			return o;
		}
		if (errno==EOPNOTSUPP){
			_linux_huge_tlb=0;
		}
	}
#endif
	SLL_ASSERT(SLL_ROUND_PAGE(sz)==sz);
	SLL_ASSERT(sysconf(_SC_PAGESIZE)==SLL_PAGE_SIZE);
	void* o=mmap(NULL,sz,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
	if (o!=MAP_FAILED){
		return o;
	}
	ERROR_PTR_SYSTEM;
	return NULL;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page_aligned(sll_size_t sz,sll_size_t align,sll_error_t* err){
	ERROR_PTR_RESET;
	SLL_ASSERT(!(align&(align-1)));
	if (align<=SLL_PAGE_SIZE){
		void* o=mmap(NULL,sz,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
		if (o!=MAP_FAILED){
			return o;
		}
		ERROR_PTR_SYSTEM;
		return NULL;
	}
	sll_size_t mem_sz=sz+align-SLL_PAGE_SIZE;
	void* o=mmap(NULL,mem_sz,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
	if (o==MAP_FAILED){
		ERROR_PTR_SYSTEM;
		return NULL;
	}
	sll_size_t off=(ADDR(o)&(align-1));
	if (off){
		off=align-off;
		munmap(o,off);
		o=PTR(ADDR(o)+off);
	}
	munmap(PTR(ADDR(o)+sz),align-SLL_PAGE_SIZE-off);
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_free_page(void* pg,sll_size_t sz){
	return (munmap(pg,sz)?sll_platform_get_error():SLL_NO_ERROR);
}

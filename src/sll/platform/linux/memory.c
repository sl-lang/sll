#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/platform/memory.h>
#include <sll/types.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>



#ifndef __SLL_BUILD_DARWIN
static sll_bool_t _linux_has_huge_tlb=1;
#endif



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_size_t sz,sll_bool_t l){
#ifndef __SLL_BUILD_DARWIN
	if (_linux_has_huge_tlb&&l){
		SLL_ASSERT(SLL_ROUND_LARGE_PAGE(sz)==sz);
		void* o=mmap(NULL,sz,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_HUGETLB,-1,0);
		if (o!=MAP_FAILED){
			return o;
		}
		if (errno==EOPNOTSUPP){
			_linux_has_huge_tlb=0;
		}
	}
#endif
	SLL_ASSERT(SLL_ROUND_PAGE(sz)==sz);
	SLL_ASSERT(sysconf(_SC_PAGESIZE)==SLL_PAGE_SIZE);
	void* o=mmap(NULL,sz,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
	return (o==MAP_FAILED?NULL:o);
}



__SLL_EXTERNAL void sll_platform_free_page(void* pg,sll_size_t sz){
	munmap(pg,sz);
}

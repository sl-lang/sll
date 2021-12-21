#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/platform.h>
#include <sll/types.h>
#include <sys/mman.h>
#include <unistd.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_allocate_page(sll_size_t sz,sll_bool_t l){
	if (l){
		SLL_ASSERT(SLL_ROUND_LARGE_PAGE(sz)==sz);
		void* o=mmap(NULL,sz,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS|MAP_HUGETLB,-1,0);
		if (o!=MAP_FAILED){
			return o;
		}
	}
	SLL_ASSERT(SLL_ROUND_PAGE(sz)==sz);
	SLL_ASSERT(sysconf(_SC_PAGESIZE)==SLL_PAGE_SIZE);
	void* o=mmap(NULL,sz,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
	return (o==MAP_FAILED?NULL:o);
}



__SLL_EXTERNAL void sll_platform_free_page(void* pg,sll_size_t sz){
	munmap(pg,sz);
}

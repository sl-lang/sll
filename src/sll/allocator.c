#include <sll/_internal/allocator.h>
#include <sll/_internal/common.h>
#include <sll/allocator.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/memory.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_allocator_collapse(void** a,sll_size_t sz){
	if (!*a){
		*a=sll_allocator_init(sz);
		return;
	}
	SLL_ASSERT(!(sz&7));
	sz>>=3;
	allocator_header_t* h=PTR(ADDR(*a)-sizeof(allocator_header_t));
	if (ALLOCATOR_HEADER_GET_SIZE(h)==sz){
		return;
	}
	ALLOCATOR_HEADER_INIT(h,sz);
	h=sll_reallocate(h,(sz<<3)+sizeof(allocator_header_t));
	*a=PTR(ADDR(h)+sizeof(allocator_header_t));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocator_from_memory(void* ptr,sll_size_t sz){
	void* o=sll_allocator_init(sz);
	sll_copy_data(ptr,sz,o);
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocator_init(sll_size_t sz){
	SLL_ASSERT(!(sz&7));
	allocator_header_t* o=sll_allocate(sz+sizeof(allocator_header_t));
	ALLOCATOR_HEADER_INIT(o,sz>>3);
	return PTR(ADDR(o)+sizeof(allocator_header_t));
}



__SLL_EXTERNAL void sll_allocator_move(void** a,sll_bool_t d){
	if (!*a){
		return;
	}
	*a=PTR(ADDR(sll_memory_move(PTR(ADDR(*a)-sizeof(allocator_header_t)),d))+sizeof(allocator_header_t));
}



__SLL_EXTERNAL void sll_allocator_release(void* a){
	if (!a){
		return;
	}
	sll_deallocate(PTR(ADDR(a)-sizeof(allocator_header_t)));
}



__SLL_EXTERNAL void sll_allocator_resize(void** a,sll_size_t sz){
	if (!*a){
		*a=sll_allocator_init(sz);
		return;
	}
	SLL_ASSERT(!(sz&7));
	sz>>=3;
	allocator_header_t* h=PTR(ADDR(*a)-sizeof(allocator_header_t));
	if (ALLOCATOR_HEADER_GET_SIZE(h)>=sz&&sz>=(ALLOCATOR_HEADER_GET_SIZE(h)>>1)){
		return;
	}
	sll_size_t mem_sz=(!sz?0:sz+(sz>>3)+(sz>>6));
	ALLOCATOR_HEADER_INIT(h,mem_sz);
	h=sll_reallocate(h,(mem_sz<<3)+sizeof(allocator_header_t));
	*a=PTR(ADDR(h)+sizeof(allocator_header_t));
}

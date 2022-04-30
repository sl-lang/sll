#include <sll/_internal/allocator.h>
#include <sll/_internal/common.h>
#include <sll/allocator.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/memory.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_allocator_collapse(void** ptr,sll_size_t size){
	if (!*ptr){
		*ptr=sll_allocator_init(size);
		return;
	}
	SLL_ASSERT(!(size&7));
	size>>=3;
	allocator_header_t* h=PTR(ADDR(*ptr)-sizeof(allocator_header_t));
	if (ALLOCATOR_HEADER_GET_SIZE(h)==size){
		return;
	}
	ALLOCATOR_HEADER_INIT(h,size);
	h=sll_reallocate(h,(size<<3)+sizeof(allocator_header_t));
	*ptr=PTR(ADDR(h)+sizeof(allocator_header_t));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocator_from_memory(void* ptr,sll_size_t size){
	void* o=sll_allocator_init(size);
	sll_copy_data(ptr,size,o);
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocator_init(sll_size_t size){
	SLL_ASSERT(!(size&7));
	allocator_header_t* o=sll_allocate(size+sizeof(allocator_header_t));
	SLL_ASSERT(!(ADDR(o)&7));
	ALLOCATOR_HEADER_INIT(o,size>>3);
	return PTR(ADDR(o)+sizeof(allocator_header_t));
}



__SLL_EXTERNAL void sll_allocator_move(void** ptr,sll_bool_t direction){
	if (!*ptr){
		return;
	}
	*ptr=PTR(ADDR(sll_memory_move(PTR(ADDR(*ptr)-sizeof(allocator_header_t)),direction))+sizeof(allocator_header_t));
}



__SLL_EXTERNAL void sll_allocator_release(void* ptr){
	if (!ptr){
		return;
	}
	sll_deallocate(PTR(ADDR(ptr)-sizeof(allocator_header_t)));
}



__SLL_EXTERNAL void sll_allocator_resize(void** ptr,sll_size_t size){
	if (!*ptr){
		*ptr=sll_allocator_init(size);
		return;
	}
	SLL_ASSERT(!(size&7));
	size>>=3;
	allocator_header_t* h=PTR(ADDR(*ptr)-sizeof(allocator_header_t));
	if (ALLOCATOR_HEADER_GET_SIZE(h)>=size&&size>=(ALLOCATOR_HEADER_GET_SIZE(h)>>1)){
		return;
	}
	sll_size_t mem_size=(!size?0:size+(size>>3)+(size>>6));
	ALLOCATOR_HEADER_INIT(h,mem_size);
	h=sll_reallocate(h,(mem_size<<3)+sizeof(allocator_header_t));
	*ptr=PTR(ADDR(h)+sizeof(allocator_header_t));
}

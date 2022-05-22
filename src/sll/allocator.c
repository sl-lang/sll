#include <sll/_internal/allocator.h>
#include <sll/_internal/common.h>
#include <sll/allocator.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/memory.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_allocator_collapse(void** pointer,sll_size_t size){
	if (!*pointer){
		*pointer=sll_allocator_init(size);
		return;
	}
	SLL_ASSERT(!(size&7));
	size>>=3;
	allocator_header_t* header=PTR(ADDR(*pointer)-sizeof(allocator_header_t));
	if (ALLOCATOR_HEADER_GET_SIZE(header)==size){
		return;
	}
	ALLOCATOR_HEADER_INIT(header,size);
	header=sll_reallocate(header,(size<<3)+sizeof(allocator_header_t));
	*pointer=PTR(ADDR(header)+sizeof(allocator_header_t));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocator_from_memory(void* pointer,sll_size_t size){
	void* out=sll_allocator_init(size);
	sll_copy_data(pointer,size,out);
	return out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocator_init(sll_size_t size){
	SLL_ASSERT(!(size&7));
	allocator_header_t* out=sll_allocate(size+sizeof(allocator_header_t));
	SLL_ASSERT(!(ADDR(out)&7));
	ALLOCATOR_HEADER_INIT(out,size>>3);
	return PTR(ADDR(out)+sizeof(allocator_header_t));
}



__SLL_EXTERNAL void sll_allocator_move(void** pointer,sll_bool_t direction){
	if (!*pointer){
		return;
	}
	*pointer=PTR(ADDR(sll_memory_move(PTR(ADDR(*pointer)-sizeof(allocator_header_t)),direction))+sizeof(allocator_header_t));
}



__SLL_EXTERNAL void sll_allocator_release(void* pointer){
	if (!pointer){
		return;
	}
	sll_deallocate(PTR(ADDR(pointer)-sizeof(allocator_header_t)));
}



__SLL_EXTERNAL void sll_allocator_resize(void** pointer,sll_size_t size){
	if (!*pointer){
		*pointer=sll_allocator_init(size);
		return;
	}
	SLL_ASSERT(!(size&7));
	size>>=3;
	allocator_header_t* header=PTR(ADDR(*pointer)-sizeof(allocator_header_t));
	if (ALLOCATOR_HEADER_GET_SIZE(header)>=size&&size>=(ALLOCATOR_HEADER_GET_SIZE(header)>>1)){
		return;
	}
	sll_size_t mem_size=(!size?0:size+(size>>3)+(size>>6));
	ALLOCATOR_HEADER_INIT(header,mem_size);
	header=sll_reallocate(header,(mem_size<<3)+sizeof(allocator_header_t));
	*pointer=PTR(ADDR(header)+sizeof(allocator_header_t));
}

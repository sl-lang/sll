#include <sll/_sll_internal.h>
#include <sll/allocator.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/memory.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocator_from_memory(void* ptr,sll_size_t sz){
	void* o=sll_allocator_init(sz);
	sll_copy_data(ptr,sz,o);
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_allocator_init(sll_size_t sz){
	return sll_allocate(sz);
}



__SLL_EXTERNAL void sll_allocator_release(void* a){
	sll_deallocate(a);
}



__SLL_EXTERNAL void sll_allocator_resize(void** a,sll_size_t sz){
	*a=sll_reallocate(*a,sz);
}

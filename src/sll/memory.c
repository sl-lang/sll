#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/types.h>
#include <stdlib.h>



static char _memory_zero_ptr[1];



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_allocate(sll_size_t sz){
	if (!sz){
		return _memory_zero_ptr;
	}
	return malloc(sz);
}



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_zero_allocate(sll_size_t sz,sll_size_t c){
	return calloc(sz,c);
}



__SLL_FUNC __SLL_CHECK_OUTPUT void* sll_rellocate(void* p,sll_size_t sz){
	if (!p||p==_memory_zero_ptr){
		return sll_allocate(sz);
	}
	if (!sz){
		sll_deallocate(p);
		return _memory_zero_ptr;
	}
	return realloc(p,sz);
}



__SLL_FUNC void sll_deallocate(void* p){
	if (!p||p==_memory_zero_ptr){
		return;
	}
	free(p);
}

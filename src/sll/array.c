#include <sll/_sll_internal.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/core.h>
#include <sll/types.h>



__SLL_FUNC sll_array_t* sll_array_create(sll_array_length_t l){
	sll_array_t* o=(sll_array_t*)sll_allocate_raw(sizeof(sll_array_t)+l*sizeof(sll_runtime_object_t));
	*((sll_array_length_t*)(&(o->l)))=l;
	return o;
}



__SLL_FUNC void sll_array_release(sll_array_t* s){
	SLL_ASSERT(s->rc);
	s->rc--;
	if (!s->rc){
		sll_deallocate_raw((sll_allocated_block_t*)s,sizeof(sll_array_t)+s->l*sizeof(sll_runtime_object_t));
	}
}

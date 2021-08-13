#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/core.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_FUNC sll_string_t* sll_string_create(sll_string_length_t l){
	sll_string_t* o=(sll_string_t*)sll_allocate_raw(sizeof(sll_string_t)+(l+1)*sizeof(sll_char_t));
	*((sll_string_length_t*)(&(o->l)))=l;
	o->c=0;
	o->v[l]=0;
	return o;
}



__SLL_FUNC void sll_string_release(sll_string_t* s){
	SLL_ASSERT(s->rc);
	s->rc--;
	if (!s->rc){
		sll_deallocate_raw((sll_allocated_block_t*)s,sizeof(sll_string_t)+(s->l+1)*sizeof(sll_char_t));
	}
}

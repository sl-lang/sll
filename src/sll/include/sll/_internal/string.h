#ifndef __SLL__INTERNAL_STRING_H__
#define __SLL__INTERNAL_STRING_H__ 1
#include <sll/_internal/attribute.h>
#include <sll/_internal/common.h>
#include <sll/allocator.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>



#define STRING_DATA_PTR(x) ASSUME_ALIGNED(x,4,0)

#define INIT_PADDING(v,l) (*((wide_data_t*)((v)+((l)&0xfffffffffffffff8ull)))=0)



static __SLL_FORCE_INLINE sll_bool_t STRING_EQUAL(const sll_string_t* a,const sll_string_t* b){
	if (a->l!=b->l||a->c!=b->c){
		return 0;
	}
	if (!a->l){
		return 1;
	}
	const wide_data_t* ap=(const wide_data_t*)(a->v);
	const wide_data_t* bp=(const wide_data_t*)(b->v);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	for (sll_string_length_t i=0;i<((a->l+7)>>3);i++){
		if (*(ap+i)!=*(bp+i)){
			return 0;
		}
	}
	return 1;
}



static __SLL_FORCE_INLINE void STRING_INIT_STACK(sll_string_t* o){
	o->l=0;
	o->c=0;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(0)*sizeof(sll_char_t));
	INIT_PADDING(o->v,0);
	sll_allocator_move((void**)(&(o->v)),SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
}



#endif

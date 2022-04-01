#ifndef __SLL__INTERNAL_STRING_H__
#define __SLL__INTERNAL_STRING_H__ 1
#include <sll/_internal/common.h>
#include <sll/types.h>



#define STRING_DATA_PTR(x) ASSUME_ALIGNED(x,4,0)



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



#endif

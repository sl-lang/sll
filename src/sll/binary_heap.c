#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_binary_heap_add(sll_binary_heap_t* bh,sll_integer_t v){
	if (!bh->l){
		bh->v=sll_allocate(sizeof(sll_integer_t));
		SLL_CHECK_NO_MEMORY(bh->v);
		bh->l=1;
		bh->v[0]=v;
		return 1;
	}
	if (bh->l==1){
		if (bh->v[0]==v){
			return 0;
		}
		bh->l=2;
		bh->v=sll_reallocate(bh->v,2*sizeof(sll_integer_t));
		SLL_CHECK_NO_MEMORY(bh->v);
		if (bh->v[0]<v){
			bh->v[1]=v;
		}
		else{
			bh->v[1]=bh->v[0];
			bh->v[0]=v;
		}
		return 1;
	}
	sll_array_length_t i=bh->l;
	bh->l++;
	bh->v=sll_reallocate(bh->v,bh->l*sizeof(sll_integer_t));
	SLL_CHECK_NO_MEMORY(bh->v);
	do{
		sll_array_length_t j=(i-1)>>1;
		if (v>=bh->v[j]){
			break;
		}
		bh->v[i]=bh->v[j];
		i=j;
	} while (i);
	if (i&&bh->v[i-1]==v){
		SLL_UNIMPLEMENTED();
		return 0;
	}
	bh->v[i]=v;
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_binary_heap_contains(sll_binary_heap_t* bh,sll_integer_t v){
	SLL_UNIMPLEMENTED();
	return 0;
}



__SLL_EXTERNAL void sll_free_binary_heap(sll_binary_heap_t* bh){
	sll_deallocate(bh->v);
	bh->v=NULL;
	bh->l=0;
}

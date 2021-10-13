#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/types.h>
#include <stdlib.h>



__SLL_FUNC void sll_free_integer_heap_queue(sll_integer_heap_queue_t* il){
	free(il->v);
	il->v=NULL;
	il->l=0;
}



__SLL_FUNC sll_bool_t sll_integer_heap_queue_add(sll_integer_heap_queue_t* il,sll_integer_t v){
	if (!il->l){
		il->v=malloc(sizeof(sll_integer_t));
		il->l=1;
		il->v[0]=v;
		return 1;
	}
	if (il->l==1){
		if (il->v[0]==v){
			return 0;
		}
		il->l=2;
		il->v=realloc(il->v,2*sizeof(sll_integer_t));
		if (il->v[0]<v){
			il->v[1]=v;
		}
		else{
			il->v[1]=il->v[0];
			il->v[0]=v;
		}
		return 1;
	}
	sll_array_length_t i=il->l;
	il->l++;
	il->v=realloc(il->v,il->l*sizeof(sll_integer_t));
	do{
		sll_array_length_t j=(i-1)>>1;
		if (v>=il->v[j]){
			break;
		}
		il->v[i]=il->v[j];
		i=j;
	} while (i);
	if (i&&il->v[i-1]==v){
		SLL_UNIMPLEMENTED();
		return 0;
	}
	il->v[i]=v;
	return 1;
}



__SLL_FUNC sll_bool_t sll_integer_heap_queue_contains(sll_integer_heap_queue_t* il,sll_integer_t v){
	SLL_UNIMPLEMENTED();
	return 0;
}



__SLL_FUNC void sll_integer_heap_queue_init(sll_integer_heap_queue_t* o){
	o->v=NULL;
	o->l=0;
}

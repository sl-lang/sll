#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/types.h>
#include <stdlib.h>



__SLL_FUNC void sll_free_integer_list(sll_integer_list_t* il){
	free(il->v);
	il->v=NULL;
	il->l=0;
}



__SLL_FUNC sll_bool_t sll_integer_list_add(sll_integer_list_t* il,sll_integer_t k,void* v){
	if (!il->l){
		il->v=malloc(sizeof(sll_integer_list_element_t));
		il->l=1;
		il->v->k=k;
		il->v->v=v;
		return 1;
	}
	for (sll_array_length_t i=0;i<il->l;i++){
		if ((il->v+i)->k==k){
			return 0;
		}
	}
	il->l++;
	il->v=realloc(il->v,il->l*sizeof(sll_integer_list_element_t));
	(il->v+il->l-1)->k=k;
	(il->v+il->l-1)->v=v;
	return 1;
}



__SLL_FUNC sll_bool_t sll_integer_list_contains(sll_integer_list_t* il,sll_integer_t v){
	SLL_UNIMPLEMENTED();
	return 0;
}



__SLL_FUNC sll_bool_t sll_integer_list_get(sll_integer_list_t* il,sll_integer_t k,void** v){
	for (sll_array_length_t i=0;i<il->l;i++){
		if ((il->v+i)->k==k){
			*v=(il->v+i)->v;
			return 1;
		}
	}
	return 0;
}



__SLL_FUNC void sll_integer_list_init(sll_integer_list_t* o){
	o->v=NULL;
	o->l=0;
}

#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/types.h>
#include <stdlib.h>



__SLL_FUNC void sll_free_integer_list(sll_integer_list_t* o){
	free(o->v);
	o->v=NULL;
	o->l=0;
}



__SLL_FUNC sll_bool_t sll_integer_list_add(sll_integer_list_t* o,sll_integer_t v){
	if (!o->v){
		o->v=malloc(sizeof(sll_integer_t));
		o->l=1;
		o->v[0]=v;
		return 1;
	}
	for (sll_array_length_t i=0;i<o->l;i++){
		if (o->v[i]==v){
			return 0;
		}
	}
	o->l++;
	o->v=realloc(o->v,o->l*sizeof(sll_integer_t));
	o->v[o->l-1]=v;
	return 1;
}



__SLL_FUNC sll_bool_t sll_integer_list_contains(sll_integer_list_t* o,sll_integer_t v){
	SLL_UNIMPLEMENTED();
	return 0;
}



__SLL_FUNC void sll_integer_list_init(sll_integer_list_t* o){
	o->v=NULL;
	o->l=0;
}

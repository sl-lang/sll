#include <sll/common.h>
#include <sll/types.h>
#include <stdlib.h>



__SLL_FUNC sll_array_t* sll_array_create(sll_array_length_t l){
	sll_array_t* o=malloc(sizeof(sll_array_t)+l*sizeof(sll_runtime_object_t));
	o->rc=1;
	*((sll_array_length_t*)(&(o->l)))=l;
	return o;
}

#include <sll/_sll_internal.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_FUNC sll_array_t* sll_array_create(sll_array_length_t l){
	sll_array_t* o=malloc(sizeof(sll_array_t)+l*sizeof(sll_runtime_object_t));
	*((sll_array_length_t*)(&(o->l)))=l;
	return o;
}

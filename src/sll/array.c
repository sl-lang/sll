#include <sll/common.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdlib.h>



__SLL_FUNC void sll_array_create(sll_array_length_t l,sll_array_t* o){
	o->l=l;
	o->v=calloc(sizeof(sll_runtime_object_t*),l);
}

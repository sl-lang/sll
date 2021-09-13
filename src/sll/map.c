#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/types.h>
#include <stdlib.h>



__SLL_FUNC void sll_map_create(sll_map_length_t l,sll_map_t* o){
	o->l=l;
	o->v=calloc(sizeof(sll_runtime_object_t*),l<<1);
}

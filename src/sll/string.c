#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_FUNC sll_string_t* sll_string_create(sll_string_length_t l){
	sll_string_t* o=malloc(sizeof(sll_string_t)+(l+1)*sizeof(sll_char_t));
	*((sll_string_length_t*)(&(o->l)))=l;
	o->c=0;
	o->v[l]=0;
	return o;
}

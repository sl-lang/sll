#include <sll/common.h>
#include <sll/types.h>
#include <stdlib.h>
#include <string.h>



__SLL_FUNC void sll_string_create(sll_string_length_t l,sll_string_t* o){
	o->l=l;
	o->c=0;
	o->v=malloc((l+1)*sizeof(sll_char_t));
	o->v[l]=0;
}



__SLL_FUNC void sll_string_clone(const sll_string_t* s,sll_string_t* d){
	d->l=s->l;
	d->c=s->c;
	d->v=malloc((s->l+1)*sizeof(sll_char_t));
	memcpy(d->v,s->v,s->l+1);
}

#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



__SLL_FUNC void sll_string_create(sll_string_length_t l,sll_string_t* o){
	o->l=l;
	o->c=0;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	o->v[l]=0;
}



__SLL_FUNC void sll_string_clone(const sll_string_t* s,sll_string_t* d){
	d->l=s->l;
	d->c=s->c;
	d->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	memcpy(d->v,s->v,s->l+1);
}



__SLL_FUNC void sll_string_hash(sll_string_t* s){
	sll_string_checksum_t c=0;
	for (sll_string_length_t i=0;i<s->l;i++){
		c^=s->v[i];
	}
	s->c=c;
}

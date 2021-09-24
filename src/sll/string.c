#include <sll/_sll_internal.h>
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
	SLL_STRING_FORMAT_PADDING(o->v,l);
}



__SLL_FUNC void sll_string_create_from_pointer(const sll_char_t* s,sll_string_t* o){
	sll_string_length_t l=sll_string_length(s);
	o->l=l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	memcpy(o->v,s,l);
	SLL_STRING_FORMAT_PADDING(o->v,l);
	sll_string_hash(o);
}



__SLL_FUNC void sll_string_clone(const sll_string_t* s,sll_string_t* d){
	d->l=s->l;
	d->c=s->c;
	d->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	memcpy(d->v,s->v,s->l+1);
	SLL_STRING_FORMAT_PADDING(d->v,d->l);
}



__SLL_FUNC void sll_string_hash(sll_string_t* s){
	uint64_t c=0;
	const uint64_t* p=(const uint64_t*)(s->v);
	for (sll_string_length_t i=0;i<((s->l+7)>>3);i++){
		c^=*(p+i);
	}
	s->c=((sll_string_checksum_t)c)^((sll_string_checksum_t)(c>>32));
}



__SLL_FUNC void sll_string_join(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	o->l=a->l+b->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	memcpy(o->v,a->v,a->l);
	memcpy(o->v+a->l,b->v,b->l);
	o->c=SLL_STRING_COMBINE_CHECKSUMS_FAST(a->c,a->l,b->c);
}



__SLL_FUNC sll_string_length_t sll_string_length(const sll_char_t* s){
	uint64_t o=(uint64_t)s;
	while (((uint64_t)s)&(sizeof(uint64_t)-1)){
		if (!*s){
			return (sll_string_length_t)((uint64_t)s-o);
		}
		s++;
	}
	const uint64_t* p=(const uint64_t*)s;
	while (1){
		uint64_t v=((*p)-72340172838076673ull)&9259542123273814144ull&(~(*p));
		if (v){
			return (sll_string_length_t)((uint64_t)p-o)+(FIND_FIRST_SET_BIT(v)>>3);
		}
		p++;
	}
}

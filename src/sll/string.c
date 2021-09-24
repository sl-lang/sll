#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



static const sll_char_t string_to_lower_case_map[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,215,248,249,250,251,252,253,254,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255};
static const sll_char_t string_to_upper_case_map[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255};



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
	for (sll_string_length_t i=0;i<((s->l+sizeof(uint64_t)-1)>>3);i++){
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
		if (!(*s)){
			return (sll_string_length_t)((uint64_t)s-o);
		}
		s++;
	}
	const uint64_t* p=(const uint64_t*)s;
	uint64_t v;
	do{
		v=((*p)-72340172838076673ull)&9259542123273814144ull&(~(*p));
		p++;
	} while (!v);
	return (sll_string_length_t)((uint64_t)p-o-sizeof(uint64_t)+(FIND_FIRST_SET_BIT(v)>>3));
}



__SLL_FUNC void sll_string_lower_case(const sll_string_t* a,sll_string_t* o){
	o->l=a->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(a->l)*sizeof(sll_char_t));
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	sll_string_checksum_t c=0;
	const uint32_t* p=(const uint32_t*)(o->v);
	sll_string_length_t i=0;
	for (sll_string_length_t j=0;j<((o->l+3)>>2);j++){
		o->v[i]=string_to_lower_case_map[a->v[i]];
		o->v[i+1]=string_to_lower_case_map[a->v[i+1]];
		o->v[i+2]=string_to_lower_case_map[a->v[i+2]];
		o->v[i+3]=string_to_lower_case_map[a->v[i+3]];
		c^=*(p+j);
		i+=4;
	}
	o->c=c;
}



__SLL_FUNC void sll_string_upper_case(const sll_string_t* a,sll_string_t* o){
	o->l=a->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(a->l)*sizeof(sll_char_t));
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	sll_string_checksum_t c=0;
	const uint32_t* p=(const uint32_t*)(o->v);
	sll_string_length_t i=0;
	for (sll_string_length_t j=0;j<((o->l+3)>>2);j++){
		o->v[i]=string_to_upper_case_map[a->v[i]];
		o->v[i+1]=string_to_upper_case_map[a->v[i+1]];
		o->v[i+2]=string_to_upper_case_map[a->v[i+2]];
		o->v[i+3]=string_to_upper_case_map[a->v[i+3]];
		c^=*(p+j);
		i+=4;
	}
	o->c=c;
}

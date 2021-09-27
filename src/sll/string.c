#include <sll/_sll_internal.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



static const sll_char_t string_to_lower_case_map[]={
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
	64,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
	112,113,114,115,116,117,118,119,120,121,122,91,92,93,94,95,
	96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,
	112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
	240,241,242,243,244,245,246,215,248,249,250,251,252,253,254,223,
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
};
static const uint64_t string_letter_map[]={
	0x0000000000000000,
	0x07fffffe07fffffe,
	0x0000000000000000,
	0x0000000000000000
};
static const sll_char_t string_to_upper_case_map[]={
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
	16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
	32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
	48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,
	64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,
	96,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,
	80,81,82,83,84,85,86,87,88,89,90,123,124,125,126,127,
	128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
	144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
	160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
	176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
	192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
	208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
	224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
	240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255
};



__SLL_FUNC void sll_string_and(const sll_string_t* s,sll_char_t v,sll_string_t* o){
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const uint64_t* a=(const uint64_t*)(s->v);
	uint64_t* b=(uint64_t*)(o->v);
	uint16_t v16=(((uint16_t)v)<<8)|v;
	uint32_t v32=(((uint32_t)v16)<<16)|v16;
	uint64_t v64=(((uint64_t)v32)<<32)|v32;
	uint64_t c=0;
	sll_string_length_t e=((o->l+7)>>3)-1;
	for (sll_string_length_t i=0;i<e;i++){
		*(b+i)=(*(a+i))&v64;
		c^=*(b+i);
	}
	*(b+e)=(*(a+e))&v64;
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	c^=*(b+e);
	o->c=((sll_string_checksum_t)c)^((sll_string_checksum_t)(c>>32));
}



__SLL_FUNC void sll_string_and_string(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (a->l<b->l){
		const sll_string_t* c=a;
		a=b;
		b=c;
	}
	o->l=b->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(b->l)*sizeof(sll_char_t));
	const uint64_t* ap=(const uint64_t*)(a->v);
	const uint64_t* bp=(const uint64_t*)(b->v);
	uint64_t* op=(uint64_t*)(o->v);
	sll_string_length_t i=0;
	uint64_t c=0;
	for (;i<((b->l+7)>>3);i++){
		*(op+i)=(*(ap+i))&(*(bp+i));
		c^=*(op+i);
	}
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	o->c=((sll_string_checksum_t)c)^((sll_string_checksum_t)(c>>32));
}



__SLL_FUNC void sll_string_clone(const sll_string_t* s,sll_string_t* d){
	d->l=s->l;
	d->c=s->c;
	d->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	memcpy(d->v,s->v,s->l+1);
	SLL_STRING_FORMAT_PADDING(d->v,d->l);
}



__SLL_FUNC void sll_string_combinations(const sll_string_t* a,const sll_string_t* b,sll_array_t* o){
	if (!a->l||!b->l){
		SLL_ZERO_ARRAY(o);
		return;
	}
	o->l=a->l*b->l;
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	sll_array_length_t i=0;
	for (sll_string_length_t j=0;j<a->l;j++){
		for (sll_string_length_t k=0;k<b->l;k++){
			sll_runtime_object_t* n=SLL_CREATE();
			n->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
			n->dt.s.l=2;
			n->dt.s.c=(a->v[j])|(((sll_string_checksum_t)(b->v[k]))<<8);
			n->dt.s.v=malloc(SLL_STRING_ALIGN_LENGTH(2)*sizeof(sll_char_t));
			SLL_STRING_FORMAT_PADDING(n->dt.s.v,2);
			n->dt.s.v[0]=a->v[j];
			n->dt.s.v[1]=b->v[k];
			o->v[i]=n;
			i++;
		}
	}
}



__SLL_FUNC sll_compare_result_t sll_string_compare(const sll_string_t* a,const sll_string_t* b){
	SLL_UNIMPLEMENTED();
	return SLL_COMPARE_RESULT_EQUAL;
}



__SLL_FUNC sll_compare_result_t sll_string_compare_array(const sll_string_t* s,const sll_array_t* a,sll_bool_t inv){
	SLL_UNIMPLEMENTED();
	return SLL_COMPARE_RESULT_EQUAL;
}



__SLL_FUNC sll_compare_result_t sll_string_compare_map(const sll_string_t* s,const sll_map_t* m,sll_bool_t inv){
	SLL_UNIMPLEMENTED();
	return SLL_COMPARE_RESULT_EQUAL;
}



__SLL_FUNC sll_string_length_t sll_string_count(const sll_string_t* a,const sll_string_t* b){
	if (a->l<b->l){
		return 0;
	}
	sll_string_length_t o=0;
	for (sll_string_length_t i=0;i<a->l-b->l+1;i++){
		if (!memcmp(a->v+i,b->v,b->l)){
			o++;
		}
	}
	return o;
}



__SLL_FUNC sll_string_length_t sll_string_count_char(const sll_string_t* s,sll_char_t c){
	sll_string_length_t o=0;
	for (sll_string_length_t i=0;i<s->l;i++){
		if (s->v[i]==c){
			o++;
		}
	}
	return o;
}



__SLL_FUNC void sll_string_create(sll_string_length_t l,sll_string_t* o){
	o->l=l;
	o->c=0;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	SLL_STRING_FORMAT_PADDING(o->v,l);
}



__SLL_FUNC void sll_string_duplicate(const sll_string_t* s,sll_integer_t n,sll_string_length_t e,sll_string_t* o){
	SLL_ASSERT(e<s->l);
	if (e){
		SLL_UNIMPLEMENTED();
	}
	sll_bool_t r=0;
	if (n<0){
		n=-n;
		r=1;
	}
	uint8_t st=n&7;
	n*=s->l;
	if (!n){
		SLL_ZERO_STRING(o);
		return;
	}
	SLL_ASSERT(n<SLL_MAX_STRING_LENGTH);
	o->l=(sll_string_length_t)n;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(n)*sizeof(sll_char_t));
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	if (r){
		sll_string_length_t i=s->l-1;
		for (sll_string_length_t j=0;j<i;i--,j++){
			o->v[i]=s->v[j];
			o->v[j]=s->v[i];
		}
		if (s->l&1){
			o->v[i]=s->v[i];
		}
	}
	else{
		memcpy(o->v,s->v,s->l);
	}
	sll_string_length_t i=s->l;
	while (i<n){
		sll_string_length_t j=i<<1;
		if (j>n){
			j=(sll_string_length_t)n;
		}
		memcpy(o->v+i,o->v,j-i);
		i=j;
	}
	o->c=0;
	switch (st){
		case 3:
			o->c^=(s->c>>16)|(s->c<<16);
		case 2:
			o->c^=(s->c>>8)|(s->c<<24);
		case 1:
			o->c^=s->c;
			break;
		case 4:
			o->c^=s->c;
		case 5:
			o->c^=(s->c>>8)|(s->c<<24);
		case 6:
			o->c^=(s->c>>16)|(s->c<<16);
		case 7:
			o->c^=(s->c>>24)|(s->c<<8);
			break;
	}
}



__SLL_FUNC sll_bool_t sll_string_equal(const sll_string_t* a,const sll_string_t* b){
	if (a->l!=b->l||a->c!=b->c){
		return 0;
	}
	const uint64_t* ap=(const uint64_t*)(a->v);
	const uint64_t* bp=(const uint64_t*)(b->v);
	for (sll_string_length_t i=0;i<((a->l+7)>>3);i++){
		if (*(ap+i)!=*(bp+i)){
			return 0;
		}
	}
	return 1;
}



__SLL_FUNC sll_bool_t sll_string_equal_array(const sll_string_t* s,const sll_array_t* a){
	if (s->l!=a->l){
		return 0;
	}
	for (sll_string_length_t i=0;i<s->l;i++){
		sll_runtime_object_t* e=a->v[i];
		if ((e->t==SLL_RUNTIME_OBJECT_TYPE_CHAR&&e->dt.c==s->v[i])||(e->t==SLL_RUNTIME_OBJECT_TYPE_INT&&e->dt.i==s->v[i])||(e->t==SLL_RUNTIME_OBJECT_TYPE_STRING&&e->dt.s.l==1&&e->dt.s.v[0]==s->v[i])){
			continue;
		}
		return 0;
	}
	return 1;
}



__SLL_FUNC sll_bool_t sll_string_equal_map(const sll_string_t* s,const sll_map_t* m){
	if (s->l!=m->l){
		return 0;
	}
	for (sll_map_length_t i=0;i<(m->l<<1);i+=2){
		sll_runtime_object_t* e=m->v[i];
		if (e->t!=SLL_RUNTIME_OBJECT_TYPE_INT||e->dt.i<0||e->dt.i>=s->l){
			return 0;
		}
		sll_char_t c=s->v[e->dt.i];
		e=m->v[i+1];
		if ((e->t==SLL_RUNTIME_OBJECT_TYPE_CHAR&&e->dt.c==c)||(e->t==SLL_RUNTIME_OBJECT_TYPE_INT&&e->dt.i==c)||(e->t==SLL_RUNTIME_OBJECT_TYPE_STRING&&e->dt.s.l==1&&e->dt.s.v[0]==c)){
			continue;
		}
		return 0;
	}
	return 1;
}



__SLL_FUNC void sll_string_from_data(sll_runtime_object_t** v,sll_string_length_t vl,sll_string_t* o){
	o->l=vl;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(vl)*sizeof(sll_char_t));
	for (sll_string_length_t i=0;i<vl;i++){
		sll_runtime_object_t* n=sll_operator_cast(*(v+i),sll_static_int[SLL_CONSTANT_TYPE_INT]);
		SLL_ASSERT(n->t==SLL_RUNTIME_OBJECT_TYPE_INT);
		o->v[i]=n->dt.i&SLL_MAX_CHAR;
		SLL_RELEASE(n);
	}
	SLL_STRING_FORMAT_PADDING(o->v,vl);
	sll_string_hash(o);
}



__SLL_FUNC void sll_string_from_pointer(const sll_char_t* s,sll_string_t* o){
	sll_string_length_t l=sll_string_length(s);
	o->l=l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	memcpy(o->v,s,l);
	SLL_STRING_FORMAT_PADDING(o->v,l);
	sll_string_hash(o);
}



__SLL_FUNC void sll_string_hash(sll_string_t* s){
	uint64_t c=0;
	const uint64_t* p=(const uint64_t*)(s->v);
	for (sll_string_length_t i=0;i<((s->l+sizeof(uint64_t)-1)>>3);i++){
		c^=*(p+i);
	}
	s->c=((sll_string_checksum_t)c)^((sll_string_checksum_t)(c>>32));
}



__SLL_FUNC void sll_string_inv(const sll_string_t* s,sll_string_t* o){
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const uint64_t* a=(const uint64_t*)(s->v);
	uint64_t* b=(uint64_t*)(o->v);
	for (sll_string_length_t i=0;i<((o->l+7)>>3);i++){
		*(b+i)=~(*(a+i));
	}
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	sll_string_hash(o);
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



__SLL_FUNC void sll_string_lower_case(const sll_string_t* s,sll_string_t* o){
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	sll_string_checksum_t c=0;
	const uint32_t* p=(const uint32_t*)(o->v);
	sll_string_length_t i=0;
	for (sll_string_length_t j=0;j<((o->l+3)>>2);j++){
		o->v[i]=string_to_lower_case_map[s->v[i]];
		o->v[i+1]=string_to_lower_case_map[s->v[i+1]];
		o->v[i+2]=string_to_lower_case_map[s->v[i+2]];
		o->v[i+3]=string_to_lower_case_map[s->v[i+3]];
		c^=*(p+j);
		i+=4;
	}
	o->c=c;
}



__SLL_FUNC void sll_string_or(const sll_string_t* s,sll_char_t v,sll_string_t* o){
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const uint64_t* a=(const uint64_t*)(s->v);
	uint64_t* b=(uint64_t*)(o->v);
	uint16_t v16=(((uint16_t)v)<<8)|v;
	uint32_t v32=(((uint32_t)v16)<<16)|v16;
	uint64_t v64=(((uint64_t)v32)<<32)|v32;
	uint64_t c=0;
	sll_string_length_t e=((o->l+7)>>3)-1;
	for (sll_string_length_t i=0;i<e;i++){
		*(b+i)=(*(a+i))|v64;
		c^=*(b+i);
	}
	*(b+e)=(*(a+e))|v64;
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	c^=*(b+e);
	o->c=((sll_string_checksum_t)c)^((sll_string_checksum_t)(c>>32));
}



__SLL_FUNC void sll_string_or_string(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (a->l<b->l){
		const sll_string_t* c=a;
		a=b;
		b=c;
	}
	o->l=a->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(a->l)*sizeof(sll_char_t));
	const uint64_t* ap=(const uint64_t*)(a->v);
	const uint64_t* bp=(const uint64_t*)(b->v);
	uint64_t* op=(uint64_t*)(o->v);
	sll_string_length_t i=0;
	for (;i<((b->l+7)>>3);i++){
		*(op+i)=(*(ap+i))|(*(bp+i));
	}
	if ((i<<3)<a->l){
		memcpy(o->v+(i<<3),a->v+(i<<3),a->l-(i<<3));
	}
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	sll_string_hash(o);
}



__SLL_FUNC void sll_string_remove(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (!a->l){
		SLL_ZERO_STRING(o);
		return;
	}
	o->l=a->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(a->l)*sizeof(sll_char_t));
	sll_string_length_t i=0;
	for (sll_string_length_t j=0;j<a->l-b->l+1;j++){
		if (memcmp(a->v+j,b->v,b->l)){
			o->v[i]=a->v[j];
			i++;
		}
		else{
			j+=b->l-1;
		}
	}
	memcpy(o->v+i,a->v+a->l-b->l+1,b->l-1);
	i+=b->l-1;
	o->l=i;
	if (!o->l){
		free(o->v);
		o->v=NULL;
		return;
	}
	o->v=realloc(o->v,SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	sll_string_hash(o);
}



__SLL_FUNC void sll_string_replace(const sll_string_t* s,const sll_string_t* k,const sll_string_t* v,sll_string_t* o){
	if (!v->l){
		sll_string_remove(s,k,o);
		return;
	}
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	sll_string_length_t i=0;
	if (v->l<=k->l){
		for (sll_string_length_t j=0;j<s->l-k->l+1;j++){
			if (memcmp(s->v+j,k->v,k->l)){
				o->v[i]=s->v[j];
				i++;
			}
			else{
				memcpy(o->v+i,v->v,v->l);
				i+=v->l;
				j+=k->l-1;
			}
		}
	}
	else{
		for (sll_string_length_t j=0;j<s->l-k->l+1;j++){
			if (memcmp(s->v+j,k->v,k->l)){
				o->v[i]=s->v[j];
				i++;
			}
			else{
				o->l+=v->l-k->l;
				o->v=realloc(o->v,SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
				memcpy(o->v+i,v->v,v->l);
				i+=v->l;
				j+=k->l-1;
			}
		}
	}
	memcpy(o->v+i,s->v+s->l-k->l+1,k->l-1);
	i+=k->l-1;
	if (i!=o->l){
		SLL_ASSERT(i<o->l);
		o->l=i;
		if (!o->l){
			free(o->v);
			o->v=NULL;
			return;
		}
		o->v=realloc(o->v,SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	}
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	sll_string_hash(o);
}



__SLL_FUNC void sll_string_title_case(const sll_string_t* s,sll_string_t* o){
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	sll_bool_t st=1;
	for (sll_string_length_t i=0;i<o->l;i++){
		sll_char_t c=s->v[i];
		if (string_letter_map[c>>6]&(1ull<<(c&63))){
			o->v[i]=(st?string_to_upper_case_map:string_to_lower_case_map)[c];
			st=0;
		}
		else{
			o->v[i]=c;
			st=1;
		}
	}
	sll_string_hash(o);
}



__SLL_FUNC void sll_string_to_array(const sll_string_t* s,sll_array_t* o){
	o->l=s->l;
	o->v=malloc(s->l*sizeof(sll_runtime_object_t*));
	for (sll_string_length_t i=0;i<s->l;i++){
		o->v[i]=SLL_ACQUIRE_STATIC_CHAR(s->v[i]);
	}
}



__SLL_FUNC void sll_string_to_map(const sll_string_t* s,sll_map_t* o){
	if (!s->l){
		SLL_ZERO_MAP(o);
		return;
	}
	o->l=s->l;
	sll_map_length_t e=s->l<<1;
	o->v=malloc(e*sizeof(sll_runtime_object_t*));
	sll_string_length_t i=0;
	for (sll_map_length_t j=0;j<e;j+=2){
		o->v[j]=SLL_FROM_INT(i);
		o->v[j+1]=SLL_FROM_CHAR(s->v[i]);
		i++;
	}
}



__SLL_FUNC void sll_string_upper_case(const sll_string_t* s,sll_string_t* o){
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	sll_string_checksum_t c=0;
	const uint32_t* p=(const uint32_t*)(o->v);
	sll_string_length_t i=0;
	for (sll_string_length_t j=0;j<((o->l+3)>>2);j++){
		o->v[i]=string_to_upper_case_map[s->v[i]];
		o->v[i+1]=string_to_upper_case_map[s->v[i+1]];
		o->v[i+2]=string_to_upper_case_map[s->v[i+2]];
		o->v[i+3]=string_to_upper_case_map[s->v[i+3]];
		c^=*(p+j);
		i+=4;
	}
	o->c=c;
}



__SLL_FUNC void sll_string_xor(const sll_string_t* s,sll_char_t v,sll_string_t* o){
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const uint64_t* a=(const uint64_t*)(s->v);
	uint64_t* b=(uint64_t*)(o->v);
	uint16_t v16=(((uint16_t)v)<<8)|v;
	uint32_t v32=(((uint32_t)v16)<<16)|v16;
	uint64_t v64=(((uint64_t)v32)<<32)|v32;
	uint64_t c=0;
	sll_string_length_t e=((o->l+7)>>3)-1;
	for (sll_string_length_t i=0;i<e;i++){
		*(b+i)=(*(a+i))^v64;
		c^=*(b+i);
	}
	*(b+e)=(*(a+e))^v64;
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	c^=*(b+e);
	o->c=((sll_string_checksum_t)c)^((sll_string_checksum_t)(c>>32));
}



__SLL_FUNC void sll_string_xor_string(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (a->l<b->l){
		const sll_string_t* c=a;
		a=b;
		b=c;
	}
	o->l=a->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(a->l)*sizeof(sll_char_t));
	const uint64_t* ap=(const uint64_t*)(a->v);
	const uint64_t* bp=(const uint64_t*)(b->v);
	uint64_t* op=(uint64_t*)(o->v);
	sll_string_length_t i=0;
	for (;i<((b->l+7)>>3);i++){
		*(op+i)=(*(ap+i))^(*(bp+i));
	}
	if ((i<<3)<a->l){
		memcpy(o->v+(i<<3),a->v+(i<<3),a->l-(i<<3));
	}
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	sll_string_hash(o);
}

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



#define INIT_PADDING(v,l) (*((uint64_t*)((v)+((l)&0xfffffffffffffff8)))=0)
#define SPLIT_CHAR_ALLOCATION_SIZE 16



__SLL_FUNC void sll_string_and(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (a->l<b->l){
		const sll_string_t* c=a;
		a=b;
		b=c;
	}
	o->l=b->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(b->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,b->l);
	const uint64_t* ap=(const uint64_t*)(a->v);
	const uint64_t* bp=(const uint64_t*)(b->v);
	uint64_t* op=(uint64_t*)(o->v);
	sll_string_length_t i=0;
	uint64_t c=0;
	for (;i<((b->l+7)>>3);i++){
		*(op+i)=(*(ap+i))&(*(bp+i));
		c^=*(op+i);
	}
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_FUNC void sll_string_and_char(const sll_string_t* s,sll_char_t v,sll_string_t* o){
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const uint64_t* a=(const uint64_t*)(s->v);
	uint64_t* b=(uint64_t*)(o->v);
	uint64_t v64=0x101010101010101ull*v;
	uint64_t c=0;
	sll_string_length_t l=s->l;
	while (l>7){
		*b=(*a)&v64;
		c^=*b;
		a++;
		b++;
		l-=8;
	}
	*b=(*a)&v64&((1ull<<(l<<3))-1);
	c^=*b;
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_FUNC void sll_string_clone(const sll_string_t* s,sll_string_t* d){
	d->l=s->l;
	d->c=s->c;
	d->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const uint64_t* a=(const uint64_t*)(s->v);
	uint64_t* b=(uint64_t*)(d->v);
	for (sll_string_length_t i=0;i<=(s->l>>3);i++){
		*(b+i)=*(a+i);
	}
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
			*((uint64_t*)(n->dt.s.v))=(uint64_t)(n->dt.s.c);
			o->v[i]=n;
			i++;
		}
	}
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare(const sll_string_t* a,const sll_string_t* b){
	SLL_UNIMPLEMENTED();
	return SLL_COMPARE_RESULT_EQUAL;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_array(const sll_string_t* s,const sll_array_t* a,sll_bool_t inv){
	SLL_UNIMPLEMENTED();
	return SLL_COMPARE_RESULT_EQUAL;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_map(const sll_string_t* s,const sll_map_t* m,sll_bool_t inv){
	SLL_UNIMPLEMENTED();
	return SLL_COMPARE_RESULT_EQUAL;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count(const sll_string_t* a,const sll_string_t* b){
	if (a->l<b->l){
		return 0;
	}
	if (!b->l){
		return a->l+1;
	}
	if (!a->l){
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



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_char(const sll_string_t* s,sll_char_t c){
	if (!s->l){
		return 0;
	}
	sll_string_length_t o=0;
	const uint64_t* p=(const uint64_t*)(s->v);
	uint64_t m=0x101010101010101ull*c;
	for (sll_string_length_t i=0;i<(s->l+7)>>3;i++){
		uint64_t v=(*(p+i))^m;
		o+=(sll_string_length_t)POPULATION_COUNT((v-0x101010101010101ull)&0x8080808080808080ull&(~v));
	}
	if (!c){
		return o-((s->l&7)?8-(s->l&7):0);
	}
	return o;
}



__SLL_FUNC void sll_string_create(sll_string_length_t l,sll_string_t* o){
	o->l=l;
	o->c=0;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,l);
}



__SLL_FUNC void sll_string_duplicate(const sll_string_t* s,sll_integer_t n,sll_string_length_t e,sll_string_t* o){
	SLL_ASSERT(e<s->l);
	sll_bool_t r=0;
	if (n<0){
		n=-n;
		r=1;
	}
	uint8_t st=n&7;
	n*=s->l;
	if (!n){
		if (!e||!s->l){
			SLL_ZERO_STRING(o);
			return;
		}
		o->l=e;
		o->v=malloc(SLL_STRING_ALIGN_LENGTH(e)*sizeof(sll_char_t));
		const uint64_t* a=(const uint64_t*)(s->v);
		uint64_t* b=(uint64_t*)(o->v);
		uint64_t c=0;
		while (e>7){
			*b=*a;
			c^=*b;
			a++;
			b++;
			e-=8;
		}
		*b=(*a)&((1ull<<(e<<3))-1);
		c^=*b;
		o->c=(sll_string_length_t)(c^(c>>32));
		return;
	}
	SLL_ASSERT(n<SLL_MAX_STRING_LENGTH);
	o->l=((sll_string_length_t)n)+e;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,o->l);
	if (r){
		sll_string_length_t i=s->l-1;
		for (sll_string_length_t j=0;j<i;j++){
			o->v[i]=s->v[j];
			o->v[j]=s->v[i];
			i--;
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
			o->c^=sll_rotate_bits(s->c,16);
		case 2:
			o->c^=sll_rotate_bits(s->c,24);
		case 1:
			o->c^=s->c;
			break;
		case 4:
			o->c^=s->c;
		case 5:
			o->c^=sll_rotate_bits(s->c,24);
		case 6:
			o->c^=sll_rotate_bits(s->c,16);
		case 7:
			o->c^=sll_rotate_bits(s->c,8);
			break;
	}
	if (e){
		memcpy(o->v+i,o->v,e);
		e+=i;
		while (i<e){
			o->c^=sll_rotate_bits(o->v[i],(i&3)<<3);
			i++;
		}
	}
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal(const sll_string_t* a,const sll_string_t* b){
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



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_array(const sll_string_t* s,const sll_array_t* a){
	if (s->l!=a->l){
		return 0;
	}
	for (sll_string_length_t i=0;i<s->l;i++){
		sll_runtime_object_t* e=a->v[i];
		if ((SLL_RUNTIME_OBJECT_GET_TYPE(e)==SLL_RUNTIME_OBJECT_TYPE_CHAR&&e->dt.c==s->v[i])||(SLL_RUNTIME_OBJECT_GET_TYPE(e)==SLL_RUNTIME_OBJECT_TYPE_INT&&e->dt.i==s->v[i])||(SLL_RUNTIME_OBJECT_GET_TYPE(e)==SLL_RUNTIME_OBJECT_TYPE_STRING&&e->dt.s.l==1&&e->dt.s.v[0]==s->v[i])){
			continue;
		}
		return 0;
	}
	return 1;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_map(const sll_string_t* s,const sll_map_t* m){
	if (s->l!=m->l){
		return 0;
	}
	for (sll_map_length_t i=0;i<(m->l<<1);i+=2){
		sll_runtime_object_t* e=m->v[i];
		if (SLL_RUNTIME_OBJECT_GET_TYPE(e)!=SLL_RUNTIME_OBJECT_TYPE_INT||e->dt.i<0||e->dt.i>=s->l){
			return 0;
		}
		sll_char_t c=s->v[e->dt.i];
		e=m->v[i+1];
		if ((SLL_RUNTIME_OBJECT_GET_TYPE(e)==SLL_RUNTIME_OBJECT_TYPE_CHAR&&e->dt.c==c)||(SLL_RUNTIME_OBJECT_GET_TYPE(e)==SLL_RUNTIME_OBJECT_TYPE_INT&&e->dt.i==c)||(SLL_RUNTIME_OBJECT_GET_TYPE(e)==SLL_RUNTIME_OBJECT_TYPE_STRING&&e->dt.s.l==1&&e->dt.s.v[0]==c)){
			continue;
		}
		return 0;
	}
	return 1;
}



__SLL_FUNC void sll_string_from_data(sll_runtime_object_t** v,sll_string_length_t vl,sll_string_t* o){
	o->l=vl;
	o->c=0;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(vl)*sizeof(sll_char_t));
	INIT_PADDING(o->v,vl);
	for (sll_string_length_t i=0;i<vl;i++){
		sll_runtime_object_t* n=sll_operator_cast(*(v+i),sll_static_int[SLL_CONSTANT_TYPE_CHAR]);
		SLL_ASSERT(SLL_RUNTIME_OBJECT_GET_TYPE(n)==SLL_RUNTIME_OBJECT_TYPE_CHAR);
		o->v[i]=n->dt.c;
		o->c^=sll_rotate_bits(n->dt.c,(i&3)<<3);
		SLL_RELEASE(n);
	}
}



__SLL_FUNC void sll_string_from_int(sll_integer_t v,sll_string_t* o){
	if (!v){
		o->l=1;
		o->v=malloc(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
		*((uint64_t*)(o->v))='0';
		return;
	}
	sll_bool_t n=0;
	if (v<0){
		v=-v;
		n=1;
	}
	sll_char_t bf[20];
	sll_string_length_t i=20;
	do{
		i--;
		bf[i]=v%10;
		v/=10;
	} while (v);
	o->l=n+(20-i);
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,o->l);
	if (n){
		o->v[0]='-';
	}
	memcpy(o->v+n,bf+i,20-i);
	sll_string_hash(o);
}



__SLL_FUNC void sll_string_from_pointer(const sll_char_t* s,sll_string_t* o){
	sll_string_length_t l=sll_string_length_unaligned(s);
	if (!l){
		SLL_ZERO_STRING(o);
		return;
	}
	o->l=l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	const uint64_t* a=(const uint64_t*)s;
	uint64_t* b=(uint64_t*)(o->v);
	uint64_t c=0;
	while (l>7){
		*b=*a;
		c^=*b;
		a++;
		b++;
		l-=8;
	}
	*b=(*a)&((1ull<<(l<<3))-1);
	c^=*b;
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_FUNC void sll_string_hash(sll_string_t* s){
	const uint64_t* p=(const uint64_t*)(s->v);
	uint64_t c=0;
	for (sll_string_length_t i=0;i<((s->l+7)>>3);i++){
		c^=*(p+i);
	}
	s->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_FUNC void sll_string_inv(const sll_string_t* s,sll_string_t* o){
	if (!s->l){
		SLL_ZERO_STRING(o);
		return;
	}
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const uint64_t* a=(const uint64_t*)(s->v);
	uint64_t* b=(uint64_t*)(o->v);
	uint64_t c=0;
	sll_string_length_t l=s->l;
	while (l>7){
		*b=~(*a);
		c^=*b;
		a++;
		b++;
		l-=8;
	}
	*b=(~(*a))&((1ull<<(l<<3))-1);
	c^=*b;
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_FUNC void sll_string_join(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	o->l=a->l+b->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,o->l);
	memcpy(o->v,a->v,a->l);
	memcpy(o->v+a->l,b->v,b->l);
	o->c=SLL_STRING_COMBINE_CHECKSUMS_FAST(a->c,a->l,b->c);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_length_t sll_string_length(const sll_char_t* s){
	SLL_ASSERT(!(((uint64_t)s)&7));
	const uint64_t* p=(const uint64_t*)s;
	while (1){
		uint64_t v=((*p)-0x101010101010101ull)&0x8080808080808080ull&(~(*p));
		if (v){
			return (sll_string_length_t)(((uint64_t)p)-((uint64_t)s)+(FIND_FIRST_SET_BIT(v)>>3));
		}
		p++;
	}
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_string_length_t sll_string_length_unaligned(const sll_char_t* s){
	uint64_t o=(uint64_t)s;
	while (((uint64_t)s)&7){
		if (!(*s)){
			return (sll_string_length_t)((uint64_t)s-o);
		}
		s++;
	}
	return sll_string_length(s)+(sll_string_length_t)((uint64_t)s-o);
}



__SLL_FUNC void sll_string_lower_case(const sll_string_t* s,sll_string_t* o){
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,o->l);
	const uint64_t* a=(const uint64_t*)(s->v);
	uint64_t* b=(uint64_t*)(o->v);
	uint64_t c=0;
	for (sll_string_length_t i=0;i<((o->l+7)>>3);i++){
		*(b+i)=(*(a+i))+((((0xdadadadadadadadaull-((*(a+i))&0x7f7f7f7f7f7f7f7full))&(~(*(a+i)))&(((*(a+i))&0x7f7f7f7f7f7f7f7full)+0x3f3f3f3f3f3f3f3full))&0x8080808080808080ull)>>2);
		c^=*(b+i);
	}
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_FUNC void sll_string_op(const sll_string_t* s,const sll_string_t* a,sll_binary_operator_t f,sll_string_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_string_op_array(const sll_string_t* s,const sll_array_t* a,sll_binary_operator_t f,sll_bool_t inv,sll_array_t* o){
	sll_array_length_t e=a->l;
	o->l=s->l;
	if (s->l<a->l){
		e=s->l;
		o->l=a->l;
	}
	o->v=malloc(o->l*sizeof(sll_runtime_object_t*));
	for (sll_array_length_t i=0;i<e;i++){
		o->v[i]=(inv?f(a->v[i],sll_static_char[s->v[i]]):f(sll_static_char[s->v[i]],a->v[i]));
	}
	if (s->l==a->l){
		return;
	}
	if (s->l>a->l){
		for (sll_array_length_t i=e;i<o->l;i++){
			o->v[i]=sll_static_char[s->v[i]];
		}
	}
	else{
		memcpy(o->v+e,a->v+e,(o->l-e)*sizeof(sll_runtime_object_t*));
	}
	for (sll_array_length_t i=e;i<o->l;i++){
		SLL_ACQUIRE(o->v[i]);
	}
}



__SLL_FUNC void sll_string_op_map(const sll_string_t* s,const sll_map_t* m,sll_binary_operator_t f,sll_bool_t inv,sll_map_t* o){
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_string_or(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
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
	uint64_t c=0;
	sll_string_length_t i=0;
	for (;i<((b->l+7)>>3);i++){
		*(op+i)=(*(ap+i))|(*(bp+i));
		c^=*(op+i);
	}
	for (;i<((a->l+7)>>3);i++){
		*(op+i)=*(ap+i);
		c^=*(op+i);
	}
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_FUNC void sll_string_or_char(const sll_string_t* s,sll_char_t v,sll_string_t* o){
	if (!s->l){
		SLL_ZERO_STRING(o);
		return;
	}
	if (!v){
		sll_string_clone(s,o);
		return;
	}
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const uint64_t* a=(const uint64_t*)(s->v);
	uint64_t* b=(uint64_t*)(o->v);
	uint64_t v64=0x101010101010101ull*v;
	uint64_t c=0;
	sll_string_length_t l=s->l;
	while (l>7){
		*b=(*a)|v64;
		c^=*b;
		a++;
		b++;
		l-=8;
	}
	*b=((*a)|v64)&((1ull<<(l<<3))-1);
	c^=*b;
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_char_t sll_string_parse_char(const sll_string_t* s){
	SLL_UNIMPLEMENTED();
	return 0;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_float_t sll_string_parse_float(const sll_string_t* s){
	SLL_UNIMPLEMENTED();
	return 0;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_integer_t sll_string_parse_int(const sll_string_t* s){
	SLL_UNIMPLEMENTED();
	return 0;
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
	if (k->l==1&&v->l==1){
		sll_string_replace_char(s,k->v[0],v->v[0],o);
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



__SLL_FUNC void sll_string_replace_char(const sll_string_t* s,sll_char_t k,sll_char_t v,sll_string_t* o){
	if (!s->l){
		SLL_ZERO_STRING(o);
		return;
	}
	if (k==v){
		sll_string_clone(s,o);
		return;
	}
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const uint64_t* a=(const uint64_t*)(s->v);
	uint64_t* b=(uint64_t*)(o->v);
	uint64_t k64=0x101010101010101ull*k;
	uint64_t v64=0x101010101010101ull*(k^v);
	uint64_t c=0;
	sll_string_length_t l=s->l;
	while (l>7){
		uint64_t e=(*a)^k64;
		*b=(*a)^(((((e-0x101010101010101ull)&0x8080808080808080ull&(~e))>>7)*255)&v64);
		c^=*b;
		a++;
		b++;
		l-=8;
	}
	uint64_t e=(*a)^k64;
	*b=(*a)^(((((e-0x101010101010101ull)&0x8080808080808080ull&(~e))>>7)*255)&((1ull<<(l<<3))-1)&v64);
	c^=*b;
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_FUNC void sll_string_select(const sll_string_t* s,sll_integer_t a,sll_integer_t b,sll_integer_t c,sll_string_t* o){
	a=(a<0?s->l:0)+(a%s->l);
	b=(b<0?s->l:0)+(b%s->l);
	if (!s->l||a==b||!c){
		SLL_ZERO_STRING(o);
		return;
	}
	if (a<b){
		SLL_ASSERT(c>0);
		if (c>s->l+a){
			o->l=1;
			o->c=s->v[a];
			o->v=malloc(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
			o->v[0]=s->v[a];
			return;
		}
		o->l=(sll_string_length_t)((b-a)/c);
		o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
		if (c==1){
			memcpy(o->v,s->v+a,o->l);
		}
		else{
			SLL_UNIMPLEMENTED();
		}
	}
	else{
		SLL_UNIMPLEMENTED();
	}
	sll_string_hash(o);
}



__SLL_FUNC void sll_string_shift(const sll_string_t* s,sll_integer_t v,sll_string_t* o){
	if (!s->l){
		SLL_ZERO_STRING(o);
		return;
	}
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const uint64_t* a=(const uint64_t*)(s->v);
	uint64_t* b=(uint64_t*)(o->v);
	if (!v){
		o->c=s->c;
		for (sll_string_length_t i=0;i<((s->l+8)>>3);i++){
			*(b+i)=*(a+i);
		}
		return;
	}
	if (v<-7||v>7){
		o->c=0;
		for (sll_string_length_t i=0;i<((s->l+8)>>3);i++){
			*(b+i)=0;
		}
		return;
	}
	INIT_PADDING(o->v,s->l);
	sll_string_length_t l=(s->l+7)>>3;
	uint64_t c=0;
	if (v<0){
		v=-v;
		uint64_t m=0x101010101010101ull*(256-(1<<v));
		do{
			l--;
			*(b+l)=((*(a+l))&m)>>v;
			c^=*(b+l);
		} while (l);
	}
	else{
		uint64_t m=0x101010101010101ull*((1<<(8-v))-1);
		do{
			l--;
			*(b+l)=((*(a+l))&m)<<v;
			c^=*(b+l);
		} while (l);
	}
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_FUNC void sll_string_split(const sll_string_t* s,const sll_string_t* p,sll_array_t* o){
	if (!s->l){
		o->l=1;
		o->v=malloc(sizeof(sll_runtime_object_t*));
		o->v[0]=SLL_ACQUIRE_STATIC(str_zero);
		return;
	}
	if (!p->l){
		o->l=s->l;
		o->v=malloc(s->l*sizeof(sll_runtime_object_t*));
		for (sll_string_length_t i=0;i<s->l;i++){
			o->v[i]=SLL_FROM_CHAR(s->v[i]);
		}
		return;
	}
	if (p->l==1){
		sll_string_split_char(s,p->v[0],o);
		return;
	}
	SLL_UNIMPLEMENTED();
}



__SLL_FUNC void sll_string_split_char(const sll_string_t* s,sll_char_t c,sll_array_t* o){
	if (!s->l){
		o->l=1;
		o->v=malloc(sizeof(sll_runtime_object_t*));
		o->v[0]=SLL_ACQUIRE_STATIC(str_zero);
		return;
	}
	o->l=SPLIT_CHAR_ALLOCATION_SIZE;
	o->v=malloc(SPLIT_CHAR_ALLOCATION_SIZE*sizeof(sll_runtime_object_t*));
	const uint64_t* p=(const uint64_t*)(s->v);
	uint64_t m=0x101010101010101ull*c;
	sll_array_length_t i=0;
	sll_string_length_t j=0;
	for (sll_string_length_t k=0;k<(s->l+7)>>3;k++){
		uint64_t v=(*(p+k))^m;
		v=(v-0x101010101010101ull)&0x8080808080808080ull&(~v);
		while (v){
			sll_runtime_object_t* n;
			sll_string_length_t l=(k<<3)|(FIND_FIRST_SET_BIT(v)>>3);
			if (j!=l){
				n=SLL_CREATE();
				n->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
				sll_string_t ns={
					l-j,
					0,
					malloc(SLL_STRING_ALIGN_LENGTH(l-j)*sizeof(sll_char_t))
				};
				INIT_PADDING(ns.v,ns.l);
				memcpy(ns.v,s->v+j,ns.l);
				n->dt.s=ns;
			}
			else{
				n=SLL_ACQUIRE_STATIC(str_zero);
			}
			if (i==o->l){
				o->l+=SPLIT_CHAR_ALLOCATION_SIZE;
				o->v=realloc(o->v,o->l*sizeof(sll_runtime_object_t*));
			}
			o->v[i]=n;
			i++;
			j=l+1;
			v&=v-1;
		}
	}
	sll_runtime_object_t* n;
	if (j!=s->l){
		n=SLL_CREATE();
		n->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
		sll_string_t ns={
			s->l-j,
			0,
			malloc(SLL_STRING_ALIGN_LENGTH(s->l-j)*sizeof(sll_char_t))
		};
		INIT_PADDING(ns.v,ns.l);
		memcpy(ns.v,s->v+j,ns.l);
		n->dt.s=ns;
	}
	else{
		n=SLL_ACQUIRE_STATIC(str_zero);
	}
	if (i==o->l){
		o->l+=SPLIT_CHAR_ALLOCATION_SIZE;
		o->v=realloc(o->v,o->l*sizeof(sll_runtime_object_t*));
	}
	o->v[i]=n;
	i++;
	if (i!=o->l){
		o->l=i;
		o->v=realloc(o->v,i*sizeof(sll_runtime_object_t*));
	}
}



__SLL_FUNC void sll_string_subtract_array(const sll_string_t* s,const sll_array_t* a,sll_array_t* o){
	if (s->l<a->l){
		if (!s->l){
			SLL_ZERO_ARRAY(o);
			return;
		}
		o->l=a->l;
		o->v=malloc(a->l*sizeof(sll_runtime_object_t*));
		sll_string_length_t i=0;
		for (;i<s->l;i++){
			o->v[i]=sll_operator_sub(sll_static_char[s->v[i]],a->v[i]);
		}
		while (i<a->l){
			o->v[i]=sll_operator_sub(sll_static_char[0],a->v[i]);
			i++;
		}
		return;
	}
	if (!a->l){
		SLL_ZERO_ARRAY(o);
		return;
	}
	o->l=s->l;
	o->v=malloc(s->l*sizeof(sll_runtime_object_t*));
	sll_array_length_t i=0;
	do{
		o->v[i]=sll_operator_sub(sll_static_char[s->v[i]],a->v[i]);
		i++;
	} while (i<a->l);
	while (i<s->l){
		o->v[i]=SLL_FROM_CHAR(s->v[i]);
		i++;
	}
}



__SLL_FUNC void sll_string_subtract_map(const sll_string_t* s,const sll_map_t* m,sll_map_t* o){
	o->l=s->l+m->l;
	o->v=malloc((o->l<<1)*sizeof(sll_runtime_object_t*));
	sll_map_length_t i=0;
	SLL_UNIMPLEMENTED();
	if ((i>>1)!=o->l){
		o->l=i>>1;
		o->v=realloc(o->v,i*sizeof(sll_runtime_object_t*));
	}
}



__SLL_FUNC void sll_string_title_case(const sll_string_t* s,sll_string_t* o){
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,s->l);
	const uint64_t* p=(const uint64_t*)(o->v);
	uint64_t c=0;
	sll_char_t off=64;
	for (sll_string_length_t i=0;i<o->l;i++){
		sll_char_t e=s->v[i];
		if ((e>>6)==1&&(0x07fffffe&(1u<<(e&31)))){
			o->v[i]=(e&31)|off;
			off=96;
		}
		else{
			o->v[i]=e;
			off=64;
		}
		if ((i&7)==7){
			c^=*p;
			p++;
		}
	}
	if ((o->l&7)<7){
		c^=*p;
	}
	o->c=(sll_string_length_t)(c^(c>>32));
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
	INIT_PADDING(o->v,o->l);
	const uint64_t* a=(const uint64_t*)(s->v);
	uint64_t* b=(uint64_t*)(o->v);
	uint64_t c=0;
	for (sll_string_length_t i=0;i<((o->l+7)>>3);i++){
		*(b+i)=(*(a+i))-((((0xfafafafafafafafaull-((*(a+i))&0x7f7f7f7f7f7f7f7full))&(~(*(a+i)))&(((*(a+i))&0x7f7f7f7f7f7f7f7full)+0x1f1f1f1f1f1f1f1full))&0x8080808080808080ull)>>2);
		c^=*(b+i);
	}
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_FUNC void sll_string_xor(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
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
	uint64_t c=0;
	sll_string_length_t i=0;
	for (;i<((b->l+7)>>3);i++){
		*(op+i)=(*(ap+i))^(*(bp+i));
		c^=*(op+i);
	}
	for (;i<((a->l+7)>>3);i++){
		*(op+i)=*(ap+i);
		c^=*(op+i);
	}
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_FUNC void sll_string_xor_char(const sll_string_t* s,sll_char_t v,sll_string_t* o){
	if (!s->l){
		SLL_ZERO_STRING(o);
		return;
	}
	if (!v){
		sll_string_clone(s,o);
		return;
	}
	o->l=s->l;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const uint64_t* a=(const uint64_t*)(s->v);
	uint64_t* b=(uint64_t*)(o->v);
	uint64_t v64=0x101010101010101ull*v;
	uint64_t c=0;
	sll_string_length_t l=s->l;
	while (l>7){
		*b=(*a)^v64;
		c^=*b;
		a++;
		b++;
		l-=8;
	}
	*b=((*a)^v64)&((1ull<<(l<<3))-1);
	c^=*b;
	o->c=(sll_string_length_t)(c^(c>>32));
}

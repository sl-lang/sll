#include <sll/_internal/intrinsics.h>
#include <sll/_internal/static_object.h>
#include <sll/_internal/string.h>
#include <sll/_internal/util.h>
#include <sll/allocator.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



#define ADD(a,b) ((a)+(b))
#define SUB(a,b) ((a)-(b))

#define EXECUTE_CASE_SWITCH(x1,x2,sgn) \
	if (!s){ \
		if (!o->length){ \
			return; \
		} \
		wide_data_t* p=(wide_data_t*)(o->data); \
		STRING_DATA_PTR(p); \
		wide_data_t c=0; \
		for (sll_string_length_t i=0;i<((o->length+7)>>3);i++){ \
			*(p+i)=sgn(*(p+i),(((x1-((*(p+i))&0x7f7f7f7f7f7f7f7full))&(~(*(p+i)))&(((*(p+i))&0x7f7f7f7f7f7f7f7full)+x2))&0x8080808080808080ull)>>2); \
			c^=*(p+i); \
		} \
		o->checksum=(sll_string_length_t)(c^(c>>32)); \
		return; \
	} \
	if (!s->length){ \
		SLL_INIT_STRING(o); \
		return; \
	} \
	o->length=s->length; \
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->length)*sizeof(sll_char_t)); \
	INIT_PADDING(o->data,o->length); \
	const wide_data_t* a=(const wide_data_t*)(s->data); \
	wide_data_t* b=(wide_data_t*)(o->data); \
	STRING_DATA_PTR(a); \
	STRING_DATA_PTR(b); \
	wide_data_t c=0; \
	for (sll_string_length_t i=0;i<((o->length+7)>>3);i++){ \
		*(b+i)=sgn(*(a+i),(((x1-((*(a+i))&0x7f7f7f7f7f7f7f7full))&(~(*(a+i)))&(((*(a+i))&0x7f7f7f7f7f7f7f7full)+x2))&0x8080808080808080ull)>>2); \
		c^=*(b+i); \
	} \
	o->checksum=(sll_string_length_t)(c^(c>>32));
#define EXECUTE_BINARY_OPERATOR_CHAR(operator) \
	if (!s->length){ \
		SLL_INIT_STRING(o); \
		return; \
	} \
	if (!v){ \
		sll_string_clone(s,o); \
		return; \
	} \
	o->length=s->length; \
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->length)*sizeof(sll_char_t)); \
	const wide_data_t* a=(const wide_data_t*)(s->data); \
	wide_data_t* b=(wide_data_t*)(o->data); \
	STRING_DATA_PTR(a); \
	STRING_DATA_PTR(b); \
	wide_data_t v64=0x101010101010101ull*v; \
	wide_data_t c=0; \
	sll_string_length_t l=s->length; \
	while (l>7){ \
		*b=(*a) operator v64; \
		c^=*b; \
		a++; \
		b++; \
		l-=8; \
	} \
	*b=((*a) operator v64)&((1ull<<(l<<3))-1); \
	c^=*b; \
	o->checksum=(sll_string_length_t)(c^(c>>32));



static const sll_char_t _string_whitespace[]={'\t','\n','\v','\x0c','\r',' '};
static const sll_string_length_t _string_whitespace_count=sizeof(_string_whitespace)/sizeof(const sll_char_t);



static sll_bool_t _compare_data(const sll_char_t* a,const sll_char_t* b,sll_string_length_t l,sll_bool_t align){
	const wide_data_t* ap64=(const wide_data_t*)a;
	const wide_data_t* bp64=(const wide_data_t*)b;
	if (align){
		STRING_DATA_PTR(ap64);
	}
	STRING_DATA_PTR(bp64);
	while (l>7){
		if (*ap64!=*bp64){
			return 0;
		}
		ap64++;
		bp64++;
		l-=8;
	}
	const sll_char_t* ap=(const sll_char_t*)ap64;
	const sll_char_t* bp=(const sll_char_t*)bp64;
	while (l){
		if (*ap!=*bp){
			return 0;
		}
		ap++;
		bp++;
		l--;
	}
	return 1;
}



__SLL_EXTERNAL void sll_free_string(sll_string_t* string){
	string->length=0;
	string->checksum=0;
	sll_allocator_release(string->data);
	string->data=NULL;
}



__SLL_EXTERNAL void sll_string_and(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (a->length<b->length){
		const sll_string_t* c=a;
		a=b;
		b=c;
	}
	o->length=b->length;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(b->length)*sizeof(sll_char_t));
	INIT_PADDING(o->data,b->length);
	const wide_data_t* ap=(const wide_data_t*)(a->data);
	const wide_data_t* bp=(const wide_data_t*)(b->data);
	wide_data_t* op=(wide_data_t*)(o->data);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	STRING_DATA_PTR(op);
	sll_string_length_t i=0;
	wide_data_t c=0;
	for (;i<((b->length+7)>>3);i++){
		*(op+i)=(*(ap+i))&(*(bp+i));
		c^=*(op+i);
	}
	o->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_and_char(const sll_string_t* s,sll_char_t v,sll_string_t* o){
	if (!s->length){
		SLL_INIT_STRING(o);
		return;
	}
	if (v==255){
		sll_string_clone(s,o);
		return;
	}
	o->length=s->length;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->length)*sizeof(sll_char_t));
	wide_data_t* b=(wide_data_t*)(o->data);
	STRING_DATA_PTR(b);
	if (!v){
		for (sll_string_length_t i=0;i<((s->length+7)>>3);i++){
			*b=0;
			b++;
		}
		return;
	}
	const wide_data_t* a=(const wide_data_t*)(s->data);
	STRING_DATA_PTR(a);
	wide_data_t v64=0x101010101010101ull*v;
	wide_data_t c=0;
	sll_string_length_t l=s->length;
	while (l>7){
		*b=(*a)&v64;
		c^=*b;
		a++;
		b++;
		l-=8;
	}
	*b=(*a)&v64&((1ull<<(l<<3))-1);
	c^=*b;
	o->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_calculate_checksum(sll_string_t* s){
	const wide_data_t* p=(const wide_data_t*)(s->data);
	STRING_DATA_PTR(p);
	sll_string_length_t l=((s->length+7)>>3);
	wide_data_t c=0;
#ifndef __SLL_BUILD_DARWIN
	if (l>3){
		__m256i c256=_mm256_setzero_si256();
		do{
			c256=_mm256_xor_si256(c256,_mm256_lddqu_si256((const __m256i*)p));
			l-=4;
			p+=4;
		} while (l>3);
		c256=_mm256_xor_si256(c256,_mm256_permute2f128_si256(c256,c256,1));
		c=_mm256_extract_epi64(_mm256_xor_si256(c256,_mm256_shuffle_epi32(c256,0x4e)),0);
	}
#endif
	while (l){
		c^=*p;
		l--;
		p++;
	}
	s->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_clone(const sll_string_t* s,sll_string_t* d){
	if (!s->length){
		SLL_INIT_STRING(d);
		return;
	}
	d->length=s->length;
	d->checksum=s->checksum;
	d->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->length)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)(s->data);
	wide_data_t* b=(wide_data_t*)(d->data);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	sll_string_length_t l=(s->length>>3)+1;
#ifdef __SLL_BUILD_DARWIN
	while (l){
		*b=*a;
		l--;
		a++;
		b++;
	}
#else
	while (l>3){
		_mm256_storeu_si256((__m256i*)b,_mm256_lddqu_si256((const __m256i*)a));
		l-=4;
		a+=4;
		b+=4;
	}
	if (l>1){
		_mm_storeu_si128((__m128i*)b,_mm_lddqu_si128((const __m128i*)a));
		l-=2;
		a+=2;
		b+=2;
	}
	if (l){
		*b=*a;
	}
#endif
}



__SLL_EXTERNAL void sll_string_combinations(const sll_string_t* a,const sll_string_t* b,sll_array_t* o){
	if (!a->length||!b->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=a->length*b->length;
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_string_length_t j=0;j<a->length;j++){
		for (sll_string_length_t k=0;k<b->length;k++){
			sll_char_t bf[2]={
				a->data[j],
				b->data[k]
			};
			o->data[i]=STRING_POINTER_LENGTH_TO_OBJECT(bf,2);
			i++;
		}
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_checksum_t sll_string_combine_checksums(sll_string_checksum_t a,sll_string_length_t l,sll_string_checksum_t b){
	return a^ROTATE_BITS(b,(l&3)<<3);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare(const sll_string_t* a,const sll_string_t* b){
	const wide_data_t* ap=(const wide_data_t*)(a->data);
	const wide_data_t* bp=(const wide_data_t*)(b->data);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	for (sll_string_length_t i=0;i<(((a->length<b->length?a->length:b->length)+7)>>3);i++){
		wide_data_t av=*(ap+i);
		wide_data_t bv=*(bp+i);
		if (av!=bv){
			return (SWAP_BYTES64(av)<SWAP_BYTES64(bv)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
	}
	return (a->length==b->length?SLL_COMPARE_RESULT_EQUAL:(a->length<b->length?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_array(const sll_string_t* s,const sll_array_t* a,sll_bool_t inv){
	for (sll_array_length_t i=0;i<(s->length<a->length?s->length:a->length);i++){
		sll_compare_result_t cmp=sll_operator_compare(sll_static_char[s->data[i]],a->data[i]);
		if (cmp!=SLL_COMPARE_RESULT_EQUAL){
			return (((cmp==SLL_COMPARE_RESULT_ABOVE)^inv)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
	}
	return (s->length==a->length?SLL_COMPARE_RESULT_EQUAL:(((s->length<a->length)^inv)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_map(const sll_string_t* s,const sll_map_t* m,sll_bool_t inv){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_pointer(const sll_char_t* a,const sll_char_t* b){
	if ((ADDR(a)&7)&&(ADDR(b)&7)){
		if ((ADDR(a)&7)<(ADDR(b)&7)){
			const sll_char_t* t=a;
			a=b;
			b=t;
		}
		sll_string_length_t i=8-(ADDR(a)&7);
		do{
			if (*a!=*b){
				return (*a<*b?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
			}
			if (!(*a)){
				return SLL_COMPARE_RESULT_EQUAL;
			}
			a++;
			b++;
			i--;
		} while (i);
	}
	const wide_data_t* ap=(const wide_data_t*)a;
	const wide_data_t* bp=(const wide_data_t*)b;
	while (1){
		wide_data_t av=*ap;
		wide_data_t bv=*bp;
		wide_data_t al=(av-0x101010101010101ull)&0x8080808080808080ull&(~av);
		wide_data_t bl=(bv-0x101010101010101ull)&0x8080808080808080ull&(~bv);
		if (al||bl){
			al=(al?FIND_FIRST_SET_BIT(al):64);
			bl=(bl?FIND_FIRST_SET_BIT(bl):64);
			wide_data_t m=(1ull<<(al<bl?al:bl))-1;
			av&=m;
			bv&=m;
		}
		else{
			al=64;
			bl=64;
		}
		if (av!=bv){
			return (SWAP_BYTES64(av)<SWAP_BYTES64(bv)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
		if (al!=bl){
			return (al<bl?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
		if (al!=64){
			return SLL_COMPARE_RESULT_EQUAL;
		}
		ap++;
		bp++;
	}
}



__SLL_EXTERNAL void sll_string_concat(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (!a->length){
		if (!b->length){
			SLL_INIT_STRING(o);
		}
		else{
			sll_string_clone(b,o);
		}
		return;
	}
	if (!b->length){
		sll_string_clone(a,o);
		return;
	}
	o->length=a->length+b->length;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->length)*sizeof(sll_char_t));
	INIT_PADDING(o->data,o->length);
	const wide_data_t* ap=(const wide_data_t*)(a->data);
	wide_data_t* op=(wide_data_t*)(o->data);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(op);
	sll_string_length_t i=0;
	for (;i<((a->length+7)>>3);i++){
		*(op+i)=*(ap+i);
	}
	const sll_char_t* s=b->data;
	if (a->length&7){
		sll_string_length_t j=8-(a->length&7);
		s+=j;
		do{
			j--;
			o->data[a->length+j]=b->data[j];
		} while (j);
	}
	if (!(a->length&7)||8-(a->length&7)<b->length){
		sll_string_length_t bl=b->length-(a->length&7?8-(a->length&7):0);
		sll_string_length_t j=i+(bl>>3);
		const wide_data_t* bp=(const wide_data_t*)s;
		sll_string_length_t k=0;
		while (i<j){
			*(op+i)=*(bp+k);
			i++;
			k++;
		};
		s+=k<<3;
		j<<=3;
		for (k=0;k<(bl&7);k++){
			o->data[j+k]=*(s+k);
		}
	}
	o->checksum=a->checksum^ROTATE_BITS(b->checksum,(a->length&3)<<3);
}



__SLL_EXTERNAL void sll_string_concat_char(const sll_string_t* s,sll_char_t c,sll_string_t* o){
	o->length=s->length+1;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->length)*sizeof(sll_char_t));
	INIT_PADDING(o->data,o->length);
	const wide_data_t* a=(const wide_data_t*)(s->data);
	wide_data_t* b=(wide_data_t*)(o->data);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	for (sll_string_length_t i=0;i<=(s->length>>3);i++){
		*(b+i)=*(a+i);
	}
	o->data[s->length]=c;
	o->checksum=s->checksum^(((sll_string_checksum_t)c)<<((s->length&3)<<3));
}



__SLL_EXTERNAL void sll_string_concat_chars(sll_char_t a,sll_char_t b,sll_string_t* o){
	o->length=2;
	o->checksum=a|(b<<8);
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(2)*sizeof(sll_char_t));
	STRING_DATA_PTR(o->data);
	*((wide_data_t*)(o->data))=o->checksum;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count(const sll_string_t* a,const sll_string_t* b){
	if (a->length<b->length){
		return 0;
	}
	if (!b->length){
		return a->length+1;
	}
	if (!a->length){
		return 0;
	}
	sll_string_length_t o=0;
	for (sll_string_length_t i=0;i<a->length-b->length+1;i++){
		if (sll_compare_data(a->data+i,b->data,b->length)==SLL_COMPARE_RESULT_EQUAL){
			o++;
		}
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_char(const sll_string_t* s,sll_char_t c){
	if (!s->length){
		return 0;
	}
	sll_string_length_t o=0;
	const wide_data_t* p=(const wide_data_t*)(s->data);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*c;
	for (sll_string_length_t i=0;i<(s->length+7)>>3;i++){
		wide_data_t v=(*(p+i))^m;
		o+=(sll_string_length_t)POPULATION_COUNT((v-0x101010101010101ull)&0x8080808080808080ull&(~v));
	}
	if (!c){
		return o-((s->length&7)?8-(s->length&7):0);
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_left(const sll_string_t* s,sll_char_t c){
	if (!s->length||s->data[0]!=c){
		return 0;
	}
	const wide_data_t* p=(const wide_data_t*)(s->data);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*c;
	for (sll_string_length_t i=0;i<((s->length+7)>>3);i++){
		wide_data_t v=(*p)^m;
		v=((~(v-0x101010101010101ull))|v)&0x8080808080808080ull;
		if (v){
			sll_string_length_t o=(i<<3)+(FIND_FIRST_SET_BIT(v)>>3);
			return (!c&&o>=s->length?s->length:o);
		}
		p++;
	}
	return s->length;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_right(const sll_string_t* s,sll_char_t c){
	if (!s->length||s->data[s->length-1]!=c){
		return 0;
	}
	if (!c){
		SLL_UNIMPLEMENTED();
	}
	sll_string_length_t l=(s->length+7)>>3;
	const wide_data_t* p=((const wide_data_t*)(s->data))+l;
	wide_data_t m=0x101010101010101ull*c;
	wide_data_t n=m<<(((s->length&7))<<3);
	for (sll_string_length_t i=0;i<l;i++){
		p--;
		wide_data_t v=((*p)|n)^m;
		v=(v|(~(v-0x101010101010101ull)))&0x8080808080808080ull;
		if (v){
			return (i<<3)+((s->length&7)?(s->length&7):8)-(FIND_LAST_SET_BIT(v)>>3)-1;
		}
		n=0;
	}
	return s->length;
}



__SLL_EXTERNAL void sll_string_create(sll_string_length_t l,sll_string_t* o){
	o->length=l;
	o->checksum=0;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	INIT_PADDING(o->data,l);
}



__SLL_EXTERNAL void sll_string_decrease(sll_string_t* s,sll_string_length_t l){
	if (l!=s->length){
		s->length=l;
		sll_allocator_resize((void**)(&(s->data)),SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
		SLL_STRING_FORMAT_PADDING(s->data,s->length);
	}
}



__SLL_EXTERNAL void sll_string_duplicate(const sll_string_t* s,sll_integer_t n,sll_string_length_t e,sll_string_t* o){
	if (!s->length){
		SLL_INIT_STRING(o);
		return;
	}
	SLL_ASSERT(e<s->length);
	sll_bool_t r=0;
	if (n<0){
		n=-n;
		r=1;
	}
	if (!n){
		if (!e){
			SLL_INIT_STRING(o);
			return;
		}
		o->length=e;
		o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(e)*sizeof(sll_char_t));
		const wide_data_t* a=(const wide_data_t*)(s->data);
		wide_data_t* b=(wide_data_t*)(o->data);
		STRING_DATA_PTR(a);
		STRING_DATA_PTR(b);
		wide_data_t c=0;
		while (e>7){
			*b=*a;
			c^=*b;
			a++;
			b++;
			e-=8;
		}
		*b=(*a)&((1ull<<(e<<3))-1);
		c^=*b;
		o->checksum=(sll_string_length_t)(c^(c>>32));
		return;
	}
	n*=s->length;
	SLL_ASSERT(n<SLL_MAX_STRING_LENGTH);
	o->length=((sll_string_length_t)n)+e;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->length)*sizeof(sll_char_t));
	wide_data_t* op=(wide_data_t*)(o->data);
	STRING_DATA_PTR(op);
	if (r){
		sll_string_length_t i=s->length-1;
		for (sll_string_length_t j=0;j<i;j++){
			o->data[i]=s->data[j];
			o->data[j]=s->data[i];
			i--;
		}
		if (s->length&1){
			o->data[i]=s->data[i];
		}
	}
	else{
		const wide_data_t* ap=(const wide_data_t*)(s->data);
		STRING_DATA_PTR(ap);
		for (sll_string_length_t i=0;i<((s->length+7)>>3);i++){
			*(op+i)=*(ap+i);
		}
	}
	sll_string_length_t i=s->length;
	if (s->length&7){
		sll_string_length_t j=(o->length<16?o->length:(s->length+15)&0xfffffff0);
		while (i<j){
			o->data[i]=o->data[i-s->length];
			i++;
		}
	}
	n+=e;
	wide_data_t c=0;
	if (i<n){
		const wide_data_t* ap=(const wide_data_t*)(o->data+(s->length&7?8-(s->length&7):0));
		i>>=3;
		for (sll_string_length_t j=0;j<i;j++){
			c^=*(op+j);
		}
		n=(n+7)>>3;
		if (s->length&7){
			n--;
		}
		sll_string_length_t j=0;
		while (i<n){
			*(op+i)=*(ap+j);
			c^=*(op+i);
			i++;
			j++;
		}
		*(op+i)=(*(ap+j))&((1ull<<((o->length&7)<<3))-1);
		c^=*(op+i);
	}
	else{
		SLL_STRING_FORMAT_PADDING(o->data,o->length);
		for (sll_string_length_t j=0;j<(i>>3);j++){
			c^=*(op+j);
		}
	}
	o->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_ends(const sll_string_t* a,const sll_string_t* b){
	if (a->length<b->length){
		return 0;
	}
	if (a->length==b->length){
		return sll_string_equal(a,b);
	}
	return _compare_data(a->data+a->length-b->length,b->data,b->length,0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal(const sll_string_t* a,const sll_string_t* b){
	return STRING_EQUAL(a,b);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_array(const sll_string_t* s,const sll_array_t* a){
	if (s->length!=a->length){
		return 0;
	}
	for (sll_string_length_t i=0;i<s->length;i++){
		sll_object_t* e=a->data[i];
		if ((e->type==SLL_OBJECT_TYPE_CHAR&&e->data.char_==s->data[i])||(e->type==SLL_OBJECT_TYPE_INT&&e->data.int_==s->data[i])||(e->type==SLL_OBJECT_TYPE_STRING&&e->data.string.length==1&&e->data.string.data[0]==s->data[i])){
			continue;
		}
		return 0;
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_map(const sll_string_t* s,const sll_map_t* m){
	if (s->length!=m->length){
		return 0;
	}
	for (sll_map_length_t i=0;i<(m->length<<1);i+=2){
		sll_object_t* e=m->data[i];
		if (e->type!=SLL_OBJECT_TYPE_INT||e->data.int_<0||e->data.int_>=s->length){
			return 0;
		}
		sll_char_t c=s->data[e->data.int_];
		e=m->data[i+1];
		if ((e->type==SLL_OBJECT_TYPE_CHAR&&e->data.char_==c)||(e->type==SLL_OBJECT_TYPE_INT&&e->data.int_==c)||(e->type==SLL_OBJECT_TYPE_STRING&&e->data.string.length==1&&e->data.string.data[0]==c)){
			continue;
		}
		return 0;
	}
	return 1;
}



__SLL_EXTERNAL void sll_string_flip_case(const sll_string_t* s,sll_string_t* o){
	o->length=s->length;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->length)*sizeof(sll_char_t));
	INIT_PADDING(o->data,o->length);
	const wide_data_t* a=(const wide_data_t*)(s->data);
	wide_data_t* b=(wide_data_t*)(o->data);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	wide_data_t c=0;
	for (sll_string_length_t i=0;i<((o->length+7)>>3);i++){
		*(b+i)=(*(a+i))^((((((*(a+i))&0xc0c0c0c0c0c0c0c0ull)^0x4040404040404040ull)-0x101010101010101ull)&(0x9a9a9a9a9a9a9a9aull-((*(a+i))&0x1f1f1f1f1f1f1f1full))&(((*(a+i))&0x1f1f1f1f1f1f1f1full)+0x7f7f7f7f7f7f7f7full)&(~(*(a+i)))&0x8080808080808080ull)>>2);
		c^=*(b+i);
	}
	o->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_from_char(sll_char_t c,sll_string_t* o){
	o->length=1;
	o->checksum=c;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
	STRING_DATA_PTR(o->data);
	*((wide_data_t*)(o->data))=o->checksum;
}



__SLL_EXTERNAL void sll_string_from_data(sll_object_t** v,sll_string_length_t vl,sll_string_t* o){
	o->length=vl;
	o->checksum=0;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(vl)*sizeof(sll_char_t));
	INIT_PADDING(o->data,vl);
	for (sll_string_length_t i=0;i<vl;i++){
		sll_object_t* n=sll_operator_cast(*(v+i),sll_static_int[SLL_OBJECT_TYPE_CHAR]);
		SLL_ASSERT(n->type==SLL_OBJECT_TYPE_CHAR);
		o->data[i]=n->data.char_;
		o->checksum^=ROTATE_BITS(n->data.char_,(i&3)<<3);
		SLL_RELEASE(n);
	}
}



__SLL_EXTERNAL void sll_string_from_int(sll_integer_t v,sll_string_t* o){
	if (!v){
		o->length=1;
		o->checksum='0';
		o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
		STRING_DATA_PTR(o->data);
		*((wide_data_t*)(o->data))='0';
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
	o->length=n+(20-i);
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->length)*sizeof(sll_char_t));
	INIT_PADDING(o->data,o->length);
	if (n){
		o->data[0]='-';
	}
	for (sll_string_length_t j=0;j<20-i;j++){
		o->data[j+n]=bf[i+j];
	}
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_from_pointer(const sll_char_t* s,sll_string_t* o){
	if (!s){
		SLL_INIT_STRING(o);
		return;
	}
	sll_string_from_pointer_length(s,sll_string_length(s),o);
}



__SLL_EXTERNAL void sll_string_from_pointer_length(const sll_char_t* s,sll_string_length_t l,sll_string_t* o){
	if (!l){
		SLL_INIT_STRING(o);
		return;
	}
	o->length=l;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)s;
	wide_data_t* b=(wide_data_t*)(o->data);
	STRING_DATA_PTR(b);
	wide_data_t c=0;
#ifndef __SLL_BUILD_DARWIN
	__m256i c256=_mm256_setzero_si256();
	while (l>31){
		__m256i k=_mm256_lddqu_si256((const __m256i*)a);
		c256=_mm256_xor_si256(c256,k);
		_mm256_storeu_si256((__m256i*)b,k);
		l-=32;
		a+=4;
		b+=4;
	}
	__m128i c128=_mm_xor_si128(_mm256_castsi256_si128(c256),_mm256_extractf128_si256(c256,1));
	if (l>15){
		__m128i k=_mm_lddqu_si128((const __m128i*)a);
		c128=_mm_xor_si128(c128,k);
		_mm_storeu_si128((__m128i*)b,k);
		l-=16;
		a+=2;
		b+=2;
	}
	c=_mm_cvtsi128_si64(_mm_xor_si128(c128,_mm_shuffle_epi32(c128,0x0e)));
#endif
	while (l>7){
		*b=*a;
		c^=*b;
		a++;
		b++;
		l-=8;
	}
	*b=(*a)&((1ull<<(l<<3))-1);
	c^=*b;
	o->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_string_get(const sll_string_t* s,sll_string_length_t i){
	return (i<s->length?s->data[i]:0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_includes(const sll_string_t* a,const sll_string_t* b){
	if (!b->length){
		return 1;
	}
	if (!a->length||b->length>a->length){
		return 0;
	}
	if (b->length==1){
		return sll_string_includes_char(a,b->data[0]);
	}
	if (a->length==b->length){
		return sll_string_equal(a,b);
	}
	wide_data_t c64=0;
	const wide_data_t* p=(const wide_data_t*)(a->data);
	STRING_DATA_PTR(p);
	for (sll_string_length_t i=0;i<((b->length+7)>>3);i++){
		c64^=*p;
		p++;
	}
	if (b->length&7){
		c64^=(*(p-1))&(0xffffffffffffffffull<<((b->length&7)<<3));
	}
	sll_string_checksum_t c=(sll_string_checksum_t)(c64^(c64>>32));
	unsigned int shift=(b->length&3)<<3;
	const sll_char_t* ptr=a->data;
	for (sll_string_length_t i=0;i<a->length-b->length;i++){
		if (c==b->checksum&&sll_compare_data(ptr,b->data,b->length)==SLL_COMPARE_RESULT_EQUAL){
			return 1;
		}
		c=ROTATE_BITS_RIGHT(c^(*ptr)^((*(ptr+b->length))<<shift),8);
		ptr++;
	}
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_includes_char(const sll_string_t* s,sll_char_t c){
	if (!s->length){
		return 0;
	}
	const wide_data_t* p=(const wide_data_t*)(s->data);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*c;
	for (sll_string_length_t i=0;i<((s->length+7)>>3);i++){
		wide_data_t v=(*p)^m;
		v=(v-0x101010101010101ull)&0x8080808080808080ull&(~v);
		if (v){
			return (c?1:((i<<3)+(FIND_FIRST_SET_BIT(v)>>3)<s->length));
		}
		p++;
	}
	return 0;
}



__SLL_EXTERNAL void sll_string_increase(sll_string_t* s,sll_string_length_t l){
	if (!l){
		return;
	}
	l+=s->length;
	if (SLL_STRING_ALIGN_LENGTH(l)>SLL_STRING_ALIGN_LENGTH(s->length)){
		sll_allocator_resize((void**)(&(s->data)),SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
		SLL_STRING_FORMAT_PADDING(s->data,l);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index(const sll_string_t* a,const sll_string_t* b,sll_string_length_t si){
	if (!b->length){
		return 0;
	}
	if (!a->length||b->length>a->length){
		return SLL_MAX_STRING_LENGTH;
	}
	if (b->length==1){
		return sll_string_index_char(a,b->data[0],0,si);
	}
	if (a->length==b->length){
		return (sll_string_equal(a,b)?0:SLL_MAX_STRING_LENGTH);
	}
	wide_data_t c64=0;
	const wide_data_t* p=(const wide_data_t*)(a->data+si);
	for (sll_string_length_t i=0;i<((b->length+7)>>3);i++){
		c64^=*p;
		p++;
	}
	if (b->length&7){
		c64^=(*(p-1))&(0xffffffffffffffffull<<((b->length&7)<<3));
	}
	sll_string_checksum_t c=(sll_string_checksum_t)(c64^(c64>>32));
	unsigned int shift=(b->length&3)<<3;
	const sll_char_t* ptr=a->data+si;
	for (;si<a->length-b->length;si++){
		if (c==b->checksum&&sll_compare_data(ptr,b->data,b->length)==SLL_COMPARE_RESULT_EQUAL){
			return si;
		}
		c=ROTATE_BITS_RIGHT(c^(*ptr)^((*(ptr+b->length))<<shift),8);
		ptr++;
	}
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_char(const sll_string_t* s,sll_char_t c,sll_bool_t inv,sll_string_length_t si){
	if (si>=s->length){
		return SLL_MAX_STRING_LENGTH;
	}
	const wide_data_t* p=(const wide_data_t*)(s->data);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*c;
	wide_data_t n=0x8080808080808080ull<<((si&7)<<3);
	wide_data_t q=0x8080808080808080ull*inv;
	sll_string_length_t i=si>>3;
	p+=i;
	for (;i<((s->length+7)>>3);i++){
		wide_data_t v=(*p)^m;
		v=((v-0x101010101010101ull)&(~v)&n)^q;
		if (v){
			sll_string_length_t o=(i<<3)+(FIND_FIRST_SET_BIT(v)>>3);
			return (!c&&o>=s->length?SLL_MAX_STRING_LENGTH:o);
		}
		p++;
		n=0x8080808080808080ull;
	}
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_multiple(const sll_string_t* s,const sll_char_t* cl,sll_string_length_t cll,sll_bool_t inv){
	if (!s->length||!cll){
		return SLL_MAX_STRING_LENGTH;
	}
	if (cll==1){
		return sll_string_index_char(s,*cl,inv,0);
	}
	const wide_data_t* p=(const wide_data_t*)(s->data);
	STRING_DATA_PTR(p);
#ifdef __SLL_BUILD_DARWIN
	wide_data_t* ml=sll_allocate_stack(cll*sizeof(wide_data_t));
	for (sll_string_length_t i=0;i<cll;i++){
		*(ml+i)=0x101010101010101ull*(*(cl+i));
	}
	wide_data_t n=0x8080808080808080ull*inv;
	for (sll_string_length_t i=0;i<((s->length+7)>>3);i++){
		wide_data_t k=*p;
		wide_data_t v=0;
		for (sll_string_length_t j=0;j<cll;j++){
			wide_data_t e=k^(*(ml+j));
			v|=(e-0x101010101010101ull)&(~e);
		}
		v=(v&0x8080808080808080ull)^n;
		if (v){
			sll_deallocate(ml);
			sll_string_length_t o=(i<<3)+(FIND_FIRST_SET_BIT(v)>>3);
			return (o>=s->length?SLL_MAX_STRING_LENGTH:o);
		}
		p++;
	}
	sll_deallocate(ml);
#else
	sll_string_length_t ln=(cll+3)&0xfffffffc;
	wide_data_t* ml=sll_allocate_stack(ln*sizeof(wide_data_t));
	sll_string_length_t i=0;
	for (;i<cll-1;i++){
		*(ml+i)=0x101010101010101ull*(*(cl+i));
	}
	wide_data_t any=0x101010101010101ull*(*(cl+i));
	for (;i<ln;i++){
		*(ml+i)=any;
	}
	ln>>=2;
	__m256i sub=_mm256_set1_epi8(1);
	wide_data_t n=0x8080808080808080ull*inv;
	for (i=0;i<((s->length+7)>>3);i++){
		__m256i k=_mm256_set1_epi64x(*p);
		__m256i v256=_mm256_setzero_si256();
		const __m256i* m=(const __m256i*)ml;
		for (sll_string_length_t j=0;j<ln;j++){
			__m256i e=_mm256_xor_si256(k,_mm256_lddqu_si256(m));
			m++;
			v256=_mm256_or_si256(v256,_mm256_andnot_si256(e,_mm256_sub_epi64(e,sub)));
		}
		v256=_mm256_or_si256(v256,_mm256_permute2f128_si256(v256,v256,1));
		wide_data_t v=(_mm256_extract_epi64(_mm256_or_si256(v256,_mm256_shuffle_epi32(v256,0x4e)),0)&0x8080808080808080ull)^n;
		if (v){
			sll_deallocate(ml);
			sll_string_length_t o=(i<<3)+(FIND_FIRST_SET_BIT(v)>>3);
			return (o>=s->length?SLL_MAX_STRING_LENGTH:o);
		}
		p++;
	}
	sll_deallocate(ml);
#endif
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse(const sll_string_t* a,const sll_string_t* b){
	if (!b->length){
		return 0;
	}
	if (!a->length){
		return SLL_MAX_STRING_LENGTH;
	}
	if (b->length==1){
		return sll_string_index_reverse_char(a,b->data[0],0);
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse_char(const sll_string_t* s,sll_char_t c,sll_bool_t inv){
	if (!s->length){
		return SLL_MAX_STRING_LENGTH;
	}
	sll_string_length_t l=(s->length+7)>>3;
	const wide_data_t* p=((const wide_data_t*)(s->data))+l;
	wide_data_t m=0x101010101010101ull*c;
	wide_data_t n=0x8080808080808080ull*inv;
	for (sll_string_length_t i=0;i<l;i++){
		p--;
		wide_data_t v=(*p)^m;
		v=((v-0x101010101010101ull)&(~v)&0x8080808080808080ull)^n;
		if (v){
			if (!i&&(s->length&7)){
				v&=0xffffffffffffffffull>>((8-s->length)<<3);
				if (!v){
					continue;
				}
			}
			sll_string_length_t o=((l-i-1)<<3)+(FIND_LAST_SET_BIT(v)>>3);
			return (!c&&o>=s->length?SLL_MAX_STRING_LENGTH:o);
		}
	}
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse_multiple(const sll_string_t* s,const sll_char_t* cl,sll_string_length_t cll,sll_bool_t inv){
	if (!s->length||!cll){
		return SLL_MAX_STRING_LENGTH;
	}
	sll_string_length_t l=(s->length+7)>>3;
	const wide_data_t* p=((const wide_data_t*)(s->data))+l;
	ASSUME_ALIGNED(p,3,0);
#ifdef __SLL_BUILD_DARWIN
	wide_data_t* ml=sll_allocate_stack(cll*sizeof(wide_data_t));
	for (sll_string_length_t i=0;i<cll;i++){
		*(ml+i)=0x101010101010101ull*(*(cl+i));
	}
	wide_data_t n=0x8080808080808080ull*inv;
	for (sll_string_length_t i=0;i<l;i++){
		p--;
		wide_data_t k=*p;
		wide_data_t v=0;
		for (sll_string_length_t j=0;j<cll;j++){
			wide_data_t e=k^(*(ml+j));
			v|=(e-0x101010101010101ull)&(~e);
		}
		v=(v&0x8080808080808080ull)^n;
		if (v){
			if (!i&&(s->length&7)){
				v&=0xffffffffffffffffull>>((8-s->length)<<3);
				if (!v){
					continue;
				}
			}
			sll_deallocate(ml);
			sll_string_length_t o=((l-i-1)<<3)+(FIND_LAST_SET_BIT(v)>>3);
			return (o>=s->length?SLL_MAX_STRING_LENGTH:o);
		}
	}
	sll_deallocate(ml);
#else
	sll_string_length_t ln=(cll+3)&0xfffffffc;
	wide_data_t* ml=sll_allocate_stack(ln*sizeof(wide_data_t));
	sll_string_length_t i=0;
	for (;i<cll-1;i++){
		*(ml+i)=0x101010101010101ull*(*(cl+i));
	}
	wide_data_t any=0x101010101010101ull*(*(cl+i));
	for (;i<ln;i++){
		*(ml+i)=any;
	}
	ln>>=2;
	wide_data_t n=0x8080808080808080ull*inv;
	__m256i sub=_mm256_set1_epi8(1);
	for (i=0;i<l;i++){
		p--;
		__m256i k=_mm256_set1_epi64x(*p);
		__m256i v256=_mm256_setzero_si256();
		const __m256i* m=(const __m256i*)ml;
		for (sll_string_length_t j=0;j<ln;j++){
			__m256i e=_mm256_xor_si256(k,_mm256_lddqu_si256(m));
			m++;
			v256=_mm256_or_si256(v256,_mm256_andnot_si256(e,_mm256_sub_epi64(e,sub)));
		}
		v256=_mm256_or_si256(v256,_mm256_permute2f128_si256(v256,v256,1));
		wide_data_t v=(_mm256_extract_epi64(_mm256_or_si256(v256,_mm256_shuffle_epi32(v256,0x4e)),0)&0x8080808080808080ull)^n;
		if (v){
			if (!i&&(s->length&7)){
				v&=0xffffffffffffffffull>>((8-s->length)<<3);
				if (!v){
					continue;
				}
			}
			sll_deallocate(ml);
			sll_string_length_t o=((l-i-1)<<3)+(FIND_LAST_SET_BIT(v)>>3);
			return (o>=s->length?SLL_MAX_STRING_LENGTH:o);
		}
	}
	sll_deallocate(ml);
#endif
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL sll_string_length_t sll_string_insert_pointer(const sll_char_t* s,sll_string_length_t i,sll_string_t* o){
	return sll_string_insert_pointer_length(s,sll_string_length(s),i,o);
}



__SLL_EXTERNAL sll_string_length_t sll_string_insert_pointer_length(const sll_char_t* s,sll_string_length_t l,sll_string_length_t i,sll_string_t* o){
	if (i+l>o->length){
		l=o->length-i;
	}
	if (l<16){
		STRING_DATA_PTR(o->data);
		while (l){
			o->data[i]=*s;
			l--;
			i++;
			s++;
		}
		return i;
	}
	if (i&7){
		STRING_DATA_PTR(o->data);
		sll_string_length_t j=8-(i&7);
		i+=j;
		l-=j;
		s+=j;
		do{
			o->data[i-j]=*(s-j);
			j--;
		} while (j);
	}
	SLL_ASSERT(!(i&7));
	const wide_data_t* ap=(const wide_data_t*)s;
	STRING_DATA_PTR(o->data);
	wide_data_t* op=(wide_data_t*)(o->data+i);
	sll_string_length_t j=0;
	for (;j<(l>>3);j++){
		*(op+j)=*(ap+j);
	}
	i+=l;
	if (l&7){
		l=(l&7)<<3;
		*(op+j)=((*(op+j))&(0xffffffffffffffffull<<l))|((*(ap+j))&((1ull<<l)-1));
	}
	return i;
}



__SLL_EXTERNAL void sll_string_inv(const sll_string_t* s,sll_string_t* o){
	if (!s->length){
		SLL_INIT_STRING(o);
		return;
	}
	o->length=s->length;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->length)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)(s->data);
	wide_data_t* b=(wide_data_t*)(o->data);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	wide_data_t c=0;
	sll_string_length_t l=s->length;
	while (l>7){
		*b=~(*a);
		c^=*b;
		a++;
		b++;
		l-=8;
	}
	*b=(~(*a))&((1ull<<(l<<3))-1);
	c^=*b;
	o->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_join(const sll_string_t* s,sll_object_t*const* a,sll_array_length_t al,sll_string_t* o){
	STRING_INIT_STACK(o);
	for (sll_array_length_t i=0;i<al;i++){
		if (i){
			sll_string_increase(o,s->length);
			sll_copy_data(s->data,s->length,o->data+o->length);
			o->length+=s->length;
		}
		sll_object_t* n=sll_operator_cast(*(a+i),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_string_increase(o,n->data.string.length);
		sll_copy_data(n->data.string.data,n->data.string.length,o->data+o->length);
		o->length+=n->data.string.length;
		SLL_RELEASE(n);
	}
	sll_allocator_move((void**)(&(o->data)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_join_char(sll_char_t c,sll_object_t*const* a,sll_array_length_t al,sll_string_t* o){
	STRING_INIT_STACK(o);
	for (sll_array_length_t i=0;i<al;i++){
		if (i){
			sll_string_increase(o,1);
			o->data[o->length]=c;
			o->length++;
		}
		sll_object_t* n=sll_operator_cast(*(a+i),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_string_increase(o,n->data.string.length);
		sll_copy_data(n->data.string.data,n->data.string.length,o->data+o->length);
		o->length+=n->data.string.length;
		SLL_RELEASE(n);
	}
	sll_allocator_move((void**)(&(o->data)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_length(const sll_char_t* s){
	addr_t o=ADDR(s);
	while (ADDR(s)&7){
		if (!(*s)){
			return (sll_string_length_t)(ADDR(s)-o);
		}
		s++;
	}
	const wide_data_t* p=(const wide_data_t*)s;
	ASSUME_ALIGNED(p,3,0);
	while (1){
		wide_data_t v=((*p)-0x101010101010101ull)&0x8080808080808080ull&(~(*p));
		if (v){
			return (sll_string_length_t)(ADDR(p)+(FIND_FIRST_SET_BIT(v)>>3)-o);
		}
		p++;
	}
}



__SLL_EXTERNAL void sll_string_lower_case(const sll_string_t* s,sll_string_t* o){
	EXECUTE_CASE_SWITCH(0xdadadadadadadadaull,0x3f3f3f3f3f3f3f3full,ADD);
}



__SLL_EXTERNAL void sll_string_op(const sll_string_t* a,const sll_string_t* b,sll_binary_operator_t f,sll_string_t* o){
	sll_string_length_t e=a->length;
	o->length=b->length;
	if (b->length<a->length){
		e=b->length;
		o->length=a->length;
	}
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->length)*sizeof(sll_char_t));
	INIT_PADDING(o->data,o->length);
	for (sll_string_length_t i=0;i<e;i++){
		sll_object_t* v=f(sll_static_char[a->data[i]],sll_static_char[b->data[i]]);
		sll_object_t* c=sll_operator_cast(v,sll_static_int[SLL_OBJECT_TYPE_CHAR]);
		SLL_RELEASE(v);
		SLL_ASSERT(c->type==SLL_OBJECT_TYPE_CHAR);
		o->data[i]=c->data.char_;
		SLL_RELEASE(c);
	}
	if (a->length==b->length){
		return;
	}
	const sll_char_t* s=(a->length>b->length?a->data:b->data)+e;
	do{
		o->data[e]=s[e];
		e++;
	} while (e<o->length);
}



__SLL_EXTERNAL void sll_string_op_array(const sll_string_t* s,const sll_array_t* a,sll_binary_operator_t f,sll_bool_t inv,sll_array_t* o){
	sll_array_length_t e=a->length;
	o->length=s->length;
	if (s->length<a->length){
		e=s->length;
		o->length=a->length;
	}
	o->data=sll_allocator_init(o->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<e;i++){
		o->data[i]=(inv?f(a->data[i],sll_static_char[s->data[i]]):f(sll_static_char[s->data[i]],a->data[i]));
	}
	if (s->length==a->length){
		return;
	}
	if (s->length>a->length){
		do{
			o->data[e]=SLL_FROM_CHAR(s->data[e]);
			e++;
		} while (e<o->length);
	}
	else{
		do{
			o->data[e]=a->data[e];
			SLL_ACQUIRE(o->data[e]);
			e++;
		} while (e<o->length);
	}
}



__SLL_EXTERNAL void sll_string_op_map(const sll_string_t* s,const sll_map_t* m,sll_binary_operator_t f,sll_bool_t inv,sll_map_t* o){
	o->length=s->length+m->length;
	o->data=sll_allocate((s->length<<1)*sizeof(sll_object_t*));
	sll_string_length_t sml=(s->length+63)>>6;
	bitmap_t* sm=sll_zero_allocate_stack(sml*sizeof(bitmap_t));
	sll_map_length_t i=0;
	for (sll_map_length_t j=0;j<m->length;j++){
		SLL_ACQUIRE(m->data[j<<1]);
		o->data[i]=m->data[j<<1];
		sll_integer_t idx=m->data[j<<1]->data.int_;
		if (m->data[j<<1]->type==SLL_OBJECT_TYPE_INT&&idx>=0&&idx<s->length){
			*(sm+(idx>>6))|=1ull<<(idx&63);
			o->data[i+1]=(inv?f(m->data[(j<<1)+1],sll_static_char[s->data[idx]]):f(sll_static_char[s->data[idx]],m->data[(j<<1)+1]));
		}
		else{
			o->data[i+1]=m->data[(j<<1)+1];
			SLL_ACQUIRE(m->data[(j<<1)+1]);
		}
		i+=2;
	}
	for (sll_string_length_t j=0;j<sml;j++){
		bitmap_t v=~(*(sm+j));
		while (v){
			sll_string_length_t k=FIND_FIRST_SET_BIT(v)|(j<<6);
			if (k>=s->length){
				break;
			}
			v&=v-1;
			o->data[i]=sll_int_to_object(k);
			o->data[i+1]=SLL_FROM_CHAR(s->data[k]);
			i+=2;
		}
	}
	sll_deallocate(sm);
	if ((i>>1)!=o->length){
		o->length=i>>1;
		o->data=sll_reallocate(o->data,i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_string_or(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (a->length<b->length){
		const sll_string_t* c=a;
		a=b;
		b=c;
	}
	o->length=a->length;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(a->length)*sizeof(sll_char_t));
	const wide_data_t* ap=(const wide_data_t*)(a->data);
	const wide_data_t* bp=(const wide_data_t*)(b->data);
	wide_data_t* op=(wide_data_t*)(o->data);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	STRING_DATA_PTR(op);
	wide_data_t c=0;
	sll_string_length_t i=0;
	for (;i<((b->length+7)>>3);i++){
		*(op+i)=(*(ap+i))|(*(bp+i));
		c^=*(op+i);
	}
	for (;i<((a->length+7)>>3);i++){
		*(op+i)=*(ap+i);
		c^=*(op+i);
	}
	o->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_or_char(const sll_string_t* s,sll_char_t v,sll_string_t* o){
	EXECUTE_BINARY_OPERATOR_CHAR(|);
}



__SLL_EXTERNAL void sll_string_pad(const sll_string_t* s,sll_string_length_t l,sll_char_t c,sll_string_t* o){
	if (s->length>=l){
		sll_string_clone(s,o);
		return;
	}
	o->length=l;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	sll_string_length_t i=(l-s->length)>>1;
	sll_set_memory(o->data,i,c);
	sll_copy_data(s->data,s->length,o->data+i);
	sll_set_memory(o->data+i+s->length,l-i-s->length,c);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_pad_left(const sll_string_t* s,sll_string_length_t l,sll_char_t c,sll_string_t* o){
	if (s->length>=l){
		sll_string_clone(s,o);
		return;
	}
	o->length=l;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	sll_copy_data(s->data,s->length,o->data);
	sll_set_memory(o->data+s->length,l-s->length,c);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_pad_right(const sll_string_t* s,sll_string_length_t l,sll_char_t c,sll_string_t* o){
	if (s->length>=l){
		sll_string_clone(s,o);
		return;
	}
	o->length=l;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	sll_set_memory(o->data,l-s->length,c);
	sll_copy_data(s->data,s->length,o->data+l-s->length);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_string_parse_char(const sll_string_t* s){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_string_parse_float(const sll_string_t* s){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_string_parse_int(const sll_string_t* s){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_string_prepend_char(const sll_string_t* s,sll_char_t c,sll_string_t* o){
	o->length=s->length+1;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->length)*sizeof(sll_char_t));
	INIT_PADDING(o->data,o->length);
	const wide_data_t* a=(const wide_data_t*)(s->data+7);
	ASSUME_ALIGNED(a,4,15);
	wide_data_t* b=(wide_data_t*)(o->data);
	STRING_DATA_PTR(b);
	*b=c|((*((wide_data_t*)(s->data)))<<8);
	b++;
	for (sll_string_length_t i=0;i<(s->length>>3);i++){
		*(b+i)=*(a+i);
	}
	o->checksum=c^ROTATE_BITS(s->checksum,8);
}



__SLL_EXTERNAL void sll_string_remove(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (!a->length){
		SLL_INIT_STRING(o);
		return;
	}
	o->length=a->length;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(a->length)*sizeof(sll_char_t));
	sll_string_length_t i=0;
	sll_string_length_t j=0;
	while (j<a->length-b->length+1){
		if (sll_compare_data(a->data+j,b->data,b->length)!=SLL_COMPARE_RESULT_EQUAL){
			o->data[i]=a->data[j];
			i++;
			j++;
		}
		else{
			j+=b->length;
		}
	}
	sll_copy_data(a->data+j,a->length-j,o->data+i);
	o->length=i+a->length-j;
	if (!o->length){
		sll_allocator_release(o->data);
		o->data=NULL;
		return;
	}
	sll_allocator_resize((void**)(&(o->data)),SLL_STRING_ALIGN_LENGTH(o->length)*sizeof(sll_char_t));
	SLL_STRING_FORMAT_PADDING(o->data,o->length);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_replace(const sll_string_t* s,const sll_string_t* k,const sll_string_t* v,sll_string_t* o){
	if (sll_string_equal(s,k)){
		sll_string_clone(v,o);
		return;
	}
	if (k->length>=s->length){
		sll_string_clone(s,o);
		return;
	}
	if (!v->length){
		sll_string_remove(s,k,o);
		return;
	}
	if (k->length==1&&v->length==1){
		sll_string_replace_char(s,k->data[0],v->data[0],o);
		return;
	}
	o->length=s->length;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->length)*sizeof(sll_char_t));
	sll_string_length_t i=0;
	sll_string_length_t j=0;
	if (v->length<=k->length){
		while (j<s->length-k->length){
			if (sll_compare_data(s->data+j,k->data,k->length)!=SLL_COMPARE_RESULT_EQUAL){
				o->data[i]=s->data[j];
				i++;
				j++;
			}
			else{
				sll_copy_data(v->data,v->length,o->data+i);
				i+=v->length;
				j+=k->length;
			}
		}
	}
	else{
		while (j<s->length-k->length+1){
			if (sll_compare_data(s->data+j,k->data,k->length)!=SLL_COMPARE_RESULT_EQUAL){
				o->data[i]=s->data[j];
				i++;
				j++;
			}
			else{
				o->length+=v->length-k->length;
				sll_allocator_resize((void**)(&(o->data)),SLL_STRING_ALIGN_LENGTH(o->length)*sizeof(sll_char_t));
				for (sll_string_length_t e=0;e<v->length;e++){
					o->data[i]=v->data[e];
					i++;
				}
				j+=k->length;
			}
		}
	}
	while (j<s->length){
		o->data[i]=s->data[j];
		i++;
		j++;
	}
	if (i!=o->length){
		SLL_ASSERT(i<o->length);
		o->length=i;
		if (!o->length){
			sll_deallocate(o->data);
			o->data=NULL;
			return;
		}
		sll_allocator_resize((void**)(&(o->data)),SLL_STRING_ALIGN_LENGTH(o->length)*sizeof(sll_char_t));
	}
	SLL_STRING_FORMAT_PADDING(o->data,o->length);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_replace_char(const sll_string_t* s,sll_char_t k,sll_char_t v,sll_string_t* o){
	if (!s->length){
		SLL_INIT_STRING(o);
		return;
	}
	if (k==v){
		sll_string_clone(s,o);
		return;
	}
	o->length=s->length;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->length)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)(s->data);
	wide_data_t* b=(wide_data_t*)(o->data);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	wide_data_t k64=0x101010101010101ull*k;
	wide_data_t v64=0x101010101010101ull*(k^v);
	wide_data_t c=0;
	sll_string_length_t l=s->length;
	while (l>7){
		wide_data_t e=(*a)^k64;
		*b=(*a)^(((((e-0x101010101010101ull)&0x8080808080808080ull&(~e))>>7)*255)&v64);
		c^=*b;
		a++;
		b++;
		l-=8;
	}
	wide_data_t e=(*a)^k64;
	*b=(*a)^(((((e-0x101010101010101ull)&0x8080808080808080ull&(~e))>>7)*255)&((1ull<<(l<<3))-1)&v64);
	c^=*b;
	o->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_resize(const sll_string_t* s,sll_integer_t v,sll_string_t* o){
	if (!v){
		sll_string_clone(s,o);
		return;
	}
	if (v<0){
		v=-v;
		if (v>=s->length){
			SLL_INIT_STRING(o);
		}
		else{
			sll_string_from_pointer_length(s->data+v,(sll_string_length_t)(s->length-v),o);
		}
		return;
	}
	SLL_ASSERT(s->length+v<=SLL_MAX_STRING_LENGTH);
	o->length=(sll_string_length_t)(s->length+v);
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->length)*sizeof(sll_char_t));
	INIT_PADDING(o->data,o->length);
	wide_data_t* p=(wide_data_t*)(o->data);
	for (sll_string_length_t i=0;i<((v+7)>>3);i++){
		*p=0;
		p++;
	}
	sll_copy_data(s->data,s->length,o->data+v);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_reverse(const sll_string_t* s,sll_string_t* o){
	if (!s->length){
		SLL_INIT_STRING(o);
		return;
	}
	o->length=s->length;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->length)*sizeof(sll_char_t));
	sll_string_length_t i=s->length;
	for (sll_string_length_t j=0;j<s->length;j++){
		i--;
		o->data[j]=s->data[i];
	}
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_secure_equal(const sll_string_t* a,const sll_string_t* b){
	if (a->length<b->length){
		const sll_string_t* t=a;
		a=b;
		b=t;
	}
	const wide_data_t* ap=(const wide_data_t*)(a->data);
	const wide_data_t* bp=(const wide_data_t*)(b->data);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	wide_data_t o=0;
	sll_string_length_t i=0;
	for (;i<((b->length+7)>>3);i++){
		o|=(*(ap+i))^(*(bp+i));
	}
	return (a->length==b->length&&!o);
}



__SLL_EXTERNAL void sll_string_select(const sll_string_t* s,sll_integer_t a,sll_integer_t b,sll_integer_t c,sll_string_t* o){
	WRAP_ARRAY_INDEX_VAR(a,s->length);
	WRAP_ARRAY_INDEX_VAR(b,s->length);
	if (!s->length||a==b||!c||(a<=b&&a>=s->length)||(b<=a&&b>=s->length)){
		SLL_INIT_STRING(o);
		return;
	}
	if (a<b){
		if (c<0){
			sll_string_select(s,0,a,1,o);
			return;
		}
		if (a+c>=s->length){
			o->length=1;
			o->checksum=s->data[a];
			o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
			*((wide_data_t*)(o->data))=o->checksum;
			return;
		}
		if (c==1){
			sll_string_from_pointer_length(s->data+a,(sll_string_length_t)(b-a),o);
			return;
		}
		o->length=(sll_string_length_t)((b-a-1)/c+1);
		o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->length)*sizeof(sll_char_t));
		INIT_PADDING(o->data,o->length);
		sll_string_length_t i=0;
		do{
			o->data[i]=s->data[a];
			i++;
			a+=c;
		} while (a<b);
		SLL_ASSERT(i==o->length);
	}
	else{
		if (c>0){
			sll_string_select(s,a,s->length-1,1,o);
			return;
		}
		c=-c;
		if (c>a){
			o->length=1;
			o->checksum=s->data[a];
			o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
			*((wide_data_t*)(o->data))=o->checksum;
			return;
		}
		o->length=(sll_string_length_t)((a-b-1)/c+1);
		o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->length)*sizeof(sll_char_t));
		INIT_PADDING(o->data,o->length);
		sll_string_length_t i=0;
		do{
			o->data[i]=s->data[a];
			i++;
			a-=c;
		} while (a>b);
		SLL_ASSERT(i==o->length);
	}
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_set_char(sll_string_t* o,sll_char_t c,sll_string_length_t i){
	if (i<o->length){
		o->checksum^=((sll_string_checksum_t)(o->data[i]^c))<<((i&3)<<3);
		o->data[i]=c;
	}
}



__SLL_EXTERNAL void sll_string_shift(const sll_string_t* s,sll_integer_t v,sll_string_t* o){
	if (!s->length){
		SLL_INIT_STRING(o);
		return;
	}
	o->length=s->length;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->length)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)(s->data);
	wide_data_t* b=(wide_data_t*)(o->data);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	if (!v){
		o->checksum=s->checksum;
		for (sll_string_length_t i=0;i<((s->length+8)>>3);i++){
			*(b+i)=*(a+i);
		}
		return;
	}
	if (v<-7||v>7){
		o->checksum=0;
		for (sll_string_length_t i=0;i<((s->length+8)>>3);i++){
			*(b+i)=0;
		}
		return;
	}
	INIT_PADDING(o->data,s->length);
	sll_string_length_t l=(s->length+7)>>3;
	wide_data_t c=0;
	if (v<0){
		v=-v;
		wide_data_t m=0x101010101010101ull*((0xff<<v)&0xff);
		do{
			l--;
			*(b+l)=((*(a+l))&m)>>v;
			c^=*(b+l);
		} while (l);
	}
	else{
		wide_data_t m=0x101010101010101ull*(0xff>>v);
		do{
			l--;
			*(b+l)=((*(a+l))&m)<<v;
			c^=*(b+l);
		} while (l);
	}
	o->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_split(const sll_string_t* s,const sll_string_t* p,sll_array_t* o){
	if (!s->length){
		o->length=1;
		o->data=sll_allocator_init(sizeof(sll_object_t*));
		o->data[0]=EMPTY_STRING_TO_OBJECT();
		return;
	}
	if (!p->length){
		sll_string_to_array(s,o);
		return;
	}
	if (p->length==1){
		sll_string_split_char(s,p->data[0],o);
		return;
	}
	if (sll_string_equal(s,p)){
		o->length=2;
		o->data=sll_allocator_init(2*sizeof(sll_object_t*));
		o->data[0]=EMPTY_STRING_TO_OBJECT();
		o->data[1]=EMPTY_STRING_TO_OBJECT();
		return;
	}
	if (p->length>=s->length){
		o->length=1;
		o->data=sll_allocator_init(sizeof(sll_object_t*));
		o->data[0]=STRING_TO_OBJECT(s);
		return;
	}
	SLL_INIT_ARRAY(o);
	sll_string_length_t i=0;
	sll_string_length_t j=0;
	while (j<s->length-p->length){
		if (sll_compare_data(s->data+j,p->data,p->length)==SLL_COMPARE_RESULT_EQUAL){
			sll_object_t* x=(i==j?EMPTY_STRING_TO_OBJECT():STRING_POINTER_LENGTH_TO_OBJECT(s->data+i,j-i));
			sll_array_push(NULL,x,o);
			SLL_RELEASE(x);
			j+=p->length;
			i=j;
		}
		else{
			j++;
		}
	}
	sll_object_t* x=(i==s->length?EMPTY_STRING_TO_OBJECT():STRING_POINTER_LENGTH_TO_OBJECT(s->data+i,s->length-i));
	sll_array_push(NULL,x,o);
	SLL_RELEASE(x);
}



__SLL_EXTERNAL void sll_string_split_char(const sll_string_t* s,sll_char_t c,sll_array_t* o){
	if (!s->length){
		o->length=1;
		o->data=sll_allocator_init(sizeof(sll_object_t*));
		o->data[0]=EMPTY_STRING_TO_OBJECT();
		return;
	}
	SLL_INIT_ARRAY(o);
	const wide_data_t* p=(const wide_data_t*)(s->data);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*c;
	sll_array_length_t i=0;
	sll_string_length_t j=0;
	for (sll_string_length_t k=0;k<(s->length+7)>>3;k++){
		wide_data_t v=(*(p+k))^m;
		v=(v-0x101010101010101ull)&0x8080808080808080ull&(~v);
		while (v){
			sll_string_length_t l=(k<<3)|(FIND_FIRST_SET_BIT(v)>>3);
			o->length++;
			sll_allocator_resize((void**)(&(o->data)),o->length*sizeof(sll_object_t*));
			o->data[i]=(j<l?STRING_POINTER_LENGTH_TO_OBJECT(s->data+j,l-j):EMPTY_STRING_TO_OBJECT());
			i++;
			j=l+1;
			v&=v-1;
		}
	}
	o->length++;
	sll_allocator_resize((void**)(&(o->data)),o->length*sizeof(sll_object_t*));
	o->data[i]=(j<s->length?STRING_POINTER_LENGTH_TO_OBJECT(s->data+j,s->length-j):EMPTY_STRING_TO_OBJECT());
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_starts(const sll_string_t* a,const sll_string_t* b){
	if (a->length<b->length){
		return 0;
	}
	if (a->length==b->length){
		return sll_string_equal(a,b);
	}
	if (!b->length){
		return 1;
	}
	return _compare_data(a->data,b->data,b->length,1);
}



__SLL_EXTERNAL void sll_string_subtract_array(const sll_string_t* s,const sll_array_t* a,sll_array_t* o){
	if (s->length<a->length){
		if (!s->length){
			SLL_INIT_ARRAY(o);
			return;
		}
		o->length=a->length;
		o->data=sll_allocator_init(a->length*sizeof(sll_object_t*));
		sll_string_length_t i=0;
		for (;i<s->length;i++){
			o->data[i]=sll_operator_sub(sll_static_char[s->data[i]],a->data[i]);
		}
		while (i<a->length){
			o->data[i]=sll_operator_sub(sll_static_char[0],a->data[i]);
			i++;
		}
		return;
	}
	if (!a->length){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->length=s->length;
	o->data=sll_allocator_init(s->length*sizeof(sll_object_t*));
	STRING_DATA_PTR(o->data);
	sll_array_length_t i=0;
	do{
		o->data[i]=sll_operator_sub(sll_static_char[s->data[i]],a->data[i]);
		i++;
	} while (i<a->length);
	while (i<s->length){
		o->data[i]=SLL_FROM_CHAR(s->data[i]);
		i++;
	}
}



__SLL_EXTERNAL void sll_string_title_case(const sll_string_t* s,sll_string_t* o){
	o->length=s->length;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->length)*sizeof(sll_char_t));
	INIT_PADDING(o->data,s->length);
	const wide_data_t* p=(const wide_data_t*)(o->data);
	STRING_DATA_PTR(p);
	wide_data_t c=0;
	sll_char_t off=64;
	for (sll_string_length_t i=0;i<o->length;i++){
		sll_char_t e=s->data[i];
		if ((e&64)&&(0x07fffffe&(1u<<(e&31)))){
			o->data[i]=(e&31)|off;
			off=96;
		}
		else{
			o->data[i]=e;
			off=64;
		}
		if ((i&7)==7){
			c^=*p;
			p++;
		}
	}
	if ((o->length&7)<7){
		c^=*p;
	}
	o->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_to_array(const sll_string_t* s,sll_array_t* o){
	o->length=s->length;
	o->data=sll_allocator_init(s->length*sizeof(sll_object_t*));
	for (sll_string_length_t i=0;i<s->length;i++){
		o->data[i]=SLL_FROM_CHAR(s->data[i]);
	}
}



__SLL_EXTERNAL void sll_string_to_map(const sll_string_t* s,sll_map_t* o){
	if (!s->length){
		SLL_INIT_MAP(o);
		return;
	}
	o->length=s->length;
	sll_map_length_t e=s->length<<1;
	o->data=sll_allocate(e*sizeof(sll_object_t*));
	sll_string_length_t i=0;
	for (sll_map_length_t j=0;j<e;j+=2){
		o->data[j]=sll_int_to_object(i);
		o->data[j+1]=SLL_FROM_CHAR(s->data[i]);
		i++;
	}
}



__SLL_EXTERNAL void sll_string_trim(const sll_string_t* s,sll_string_t* o){
	SLL_INIT_STRING(o);
	if (!s->length){
		return;
	}
	sll_string_length_t i=sll_string_index_multiple(s,_string_whitespace,_string_whitespace_count,1);
	if (i==SLL_MAX_STRING_LENGTH){
		return;
	}
	sll_string_length_t j=sll_string_index_reverse_multiple(s,_string_whitespace,_string_whitespace_count,1);
	if (j==SLL_MAX_STRING_LENGTH){
		return;
	}
	sll_string_from_pointer_length(s->data+i,j-i+1,o);
}



__SLL_EXTERNAL void sll_string_trim_left(const sll_string_t* s,sll_string_t* o){
	SLL_INIT_STRING(o);
	if (!s->length){
		return;
	}
	sll_string_length_t i=sll_string_index_multiple(s,_string_whitespace,_string_whitespace_count,1);
	if (i==SLL_MAX_STRING_LENGTH){
		return;
	}
	sll_string_from_pointer_length(s->data+i,s->length-i,o);
}



__SLL_EXTERNAL void sll_string_trim_right(const sll_string_t* s,sll_string_t* o){
	SLL_INIT_STRING(o);
	if (!s->length){
		return;
	}
	sll_string_length_t i=sll_string_index_reverse_multiple(s,_string_whitespace,_string_whitespace_count,1);
	if (i==SLL_MAX_STRING_LENGTH){
		return;
	}
	i++;
	sll_string_from_pointer_length(s->data,i,o);
}



__SLL_EXTERNAL void sll_string_upper_case(const sll_string_t* s,sll_string_t* o){
	EXECUTE_CASE_SWITCH(0xfafafafafafafafaull,0x1f1f1f1f1f1f1f1full,SUB);
}



__SLL_EXTERNAL void sll_string_xor(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (a->length<b->length){
		const sll_string_t* c=a;
		a=b;
		b=c;
	}
	o->length=a->length;
	o->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(a->length)*sizeof(sll_char_t));
	INIT_PADDING(o->data,o->length);
	const wide_data_t* ap=(const wide_data_t*)(a->data);
	const wide_data_t* bp=(const wide_data_t*)(b->data);
	wide_data_t* op=(wide_data_t*)(o->data);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	STRING_DATA_PTR(op);
	wide_data_t c=0;
	sll_string_length_t i=0;
	for (;i<((b->length+7)>>3);i++){
		*(op+i)=(*(ap+i))^(*(bp+i));
		c^=*(op+i);
	}
	for (;i<((a->length+7)>>3);i++){
		*(op+i)=*(ap+i);
		c^=*(op+i);
	}
	o->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_xor_char(const sll_string_t* s,sll_char_t v,sll_string_t* o){
	EXECUTE_BINARY_OPERATOR_CHAR(^);
}

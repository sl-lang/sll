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
	if (!string){ \
		if (!out->length){ \
			return; \
		} \
		wide_data_t* p=(wide_data_t*)(out->data); \
		STRING_DATA_PTR(p); \
		wide_data_t c=0; \
		for (sll_string_length_t i=0;i<((out->length+7)>>3);i++){ \
			*(p+i)=sgn(*(p+i),(((x1-((*(p+i))&0x7f7f7f7f7f7f7f7full))&(~(*(p+i)))&(((*(p+i))&0x7f7f7f7f7f7f7f7full)+x2))&0x8080808080808080ull)>>2); \
			c^=*(p+i); \
		} \
		out->checksum=(sll_string_length_t)(c^(c>>32)); \
		return; \
	} \
	if (!string->length){ \
		SLL_INIT_STRING(out); \
		return; \
	} \
	out->length=string->length; \
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(string->length)*sizeof(sll_char_t)); \
	INIT_PADDING(out->data,out->length); \
	const wide_data_t* a=(const wide_data_t*)(string->data); \
	wide_data_t* b=(wide_data_t*)(out->data); \
	STRING_DATA_PTR(a); \
	STRING_DATA_PTR(b); \
	wide_data_t c=0; \
	for (sll_string_length_t i=0;i<((out->length+7)>>3);i++){ \
		*(b+i)=sgn(*(a+i),(((x1-((*(a+i))&0x7f7f7f7f7f7f7f7full))&(~(*(a+i)))&(((*(a+i))&0x7f7f7f7f7f7f7f7full)+x2))&0x8080808080808080ull)>>2); \
		c^=*(b+i); \
	} \
	out->checksum=(sll_string_length_t)(c^(c>>32));
#define EXECUTE_BINARY_OPERATOR_CHAR(operator) \
	if (!string->length){ \
		SLL_INIT_STRING(out); \
		return; \
	} \
	if (!char_){ \
		sll_string_clone(string,out); \
		return; \
	} \
	out->length=string->length; \
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(string->length)*sizeof(sll_char_t)); \
	const wide_data_t* a=(const wide_data_t*)(string->data); \
	wide_data_t* b=(wide_data_t*)(out->data); \
	STRING_DATA_PTR(a); \
	STRING_DATA_PTR(b); \
	wide_data_t m=0x101010101010101ull*char_; \
	wide_data_t c=0; \
	sll_string_length_t l=string->length; \
	while (l>7){ \
		*b=(*a) operator m; \
		c^=*b; \
		a++; \
		b++; \
		l-=8; \
	} \
	*b=((*a) operator m)&((1ull<<(l<<3))-1); \
	c^=*b; \
	out->checksum=(sll_string_length_t)(c^(c>>32));



static const sll_char_t _string_whitespace[]={'\t','\n','\v','\x0c','\r',' '};
static const sll_string_length_t _string_whitespace_count=sizeof(_string_whitespace)/sizeof(const sll_char_t);



static sll_bool_t _compare_data(const sll_char_t* a,const sll_char_t* b,sll_string_length_t length,sll_bool_t check_align){
	const wide_data_t* ap64=(const wide_data_t*)a;
	const wide_data_t* bp64=(const wide_data_t*)b;
	if (check_align){
		STRING_DATA_PTR(ap64);
	}
	STRING_DATA_PTR(bp64);
	while (length>7){
		if (*ap64!=*bp64){
			return 0;
		}
		ap64++;
		bp64++;
		length-=8;
	}
	const sll_char_t* ap=(const sll_char_t*)ap64;
	const sll_char_t* bp=(const sll_char_t*)bp64;
	while (length){
		if (*ap!=*bp){
			return 0;
		}
		ap++;
		bp++;
		length--;
	}
	return 1;
}



__SLL_EXTERNAL void sll_free_string(sll_string_t* string){
	string->length=0;
	string->checksum=0;
	sll_allocator_release(string->data);
	string->data=NULL;
}



__SLL_EXTERNAL void sll_string_and(const sll_string_t* a,const sll_string_t* b,sll_string_t* out){
	if (a->length<b->length){
		const sll_string_t* c=a;
		a=b;
		b=c;
	}
	out->length=b->length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(b->length)*sizeof(sll_char_t));
	INIT_PADDING(out->data,b->length);
	const wide_data_t* ap=(const wide_data_t*)(a->data);
	const wide_data_t* bp=(const wide_data_t*)(b->data);
	wide_data_t* op=(wide_data_t*)(out->data);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	STRING_DATA_PTR(op);
	sll_string_length_t i=0;
	wide_data_t c=0;
	for (;i<((b->length+7)>>3);i++){
		*(op+i)=(*(ap+i))&(*(bp+i));
		c^=*(op+i);
	}
	out->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_and_char(const sll_string_t* string,sll_char_t char_,sll_string_t* out){
	if (!string->length){
		SLL_INIT_STRING(out);
		return;
	}
	if (char_==255){
		sll_string_clone(string,out);
		return;
	}
	out->length=string->length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(string->length)*sizeof(sll_char_t));
	wide_data_t* b=(wide_data_t*)(out->data);
	STRING_DATA_PTR(b);
	if (!char_){
		for (sll_string_length_t i=0;i<((string->length+7)>>3);i++){
			*b=0;
			b++;
		}
		return;
	}
	const wide_data_t* a=(const wide_data_t*)(string->data);
	STRING_DATA_PTR(a);
	wide_data_t v64=0x101010101010101ull*char_;
	wide_data_t c=0;
	sll_string_length_t l=string->length;
	while (l>7){
		*b=(*a)&v64;
		c^=*b;
		a++;
		b++;
		l-=8;
	}
	*b=(*a)&v64&((1ull<<(l<<3))-1);
	c^=*b;
	out->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_calculate_checksum(sll_string_t* string){
	const wide_data_t* p=(const wide_data_t*)(string->data);
	STRING_DATA_PTR(p);
	sll_string_length_t l=((string->length+7)>>3);
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
	string->checksum=(sll_string_checksum_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_clone(const sll_string_t* string,sll_string_t* out){
	if (!string->length){
		SLL_INIT_STRING(out);
		return;
	}
	out->length=string->length;
	out->checksum=string->checksum;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(string->length)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)(string->data);
	wide_data_t* b=(wide_data_t*)(out->data);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	sll_string_length_t l=(string->length>>3)+1;
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



__SLL_EXTERNAL void sll_string_combinations(const sll_string_t* a,const sll_string_t* b,sll_array_t* out){
	if (!a->length||!b->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=a->length*b->length;
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_string_length_t j=0;j<a->length;j++){
		for (sll_string_length_t k=0;k<b->length;k++){
			sll_char_t bf[2]={
				a->data[j],
				b->data[k]
			};
			out->data[i]=STRING_POINTER_LENGTH_TO_OBJECT(bf,2);
			i++;
		}
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_checksum_t sll_string_combine_checksums(sll_string_checksum_t a,sll_string_length_t length,sll_string_checksum_t b){
	return a^ROTATE_BITS(b,(length&3)<<3);
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_array(const sll_string_t* string,const sll_array_t* array,sll_bool_t inv){
	for (sll_array_length_t i=0;i<(string->length<array->length?string->length:array->length);i++){
		sll_compare_result_t cmp=sll_operator_compare(sll_static_char[string->data[i]],array->data[i]);
		if (cmp!=SLL_COMPARE_RESULT_EQUAL){
			return (((cmp==SLL_COMPARE_RESULT_ABOVE)^inv)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
	}
	return (string->length==array->length?SLL_COMPARE_RESULT_EQUAL:(((string->length<array->length)^inv)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_map(const sll_string_t* string,const sll_map_t* map,sll_bool_t inv){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_pointer(const sll_char_t* a,const sll_char_t* b){
#ifdef __SLL_BUILD_FUZZER
	while (1){
		if (*a!=*b){
			return (*a>*b?SLL_COMPARE_RESULT_ABOVE:SLL_COMPARE_RESULT_BELOW);
		}
		if (!(*a)){
			return SLL_COMPARE_RESULT_EQUAL;
		}
		a++;
		b++;
	}
#else
	if ((ADDR(a)&7)&&(ADDR(b)&7)){
		sll_bool_t inv=0;
		if ((ADDR(a)&7)<(ADDR(b)&7)){
			const sll_char_t* t=a;
			a=b;
			b=t;
			inv=1;
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
		if (inv){
			const sll_char_t* t=a;
			a=b;
			b=t;
		}
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
#endif
}



__SLL_EXTERNAL void sll_string_concat(const sll_string_t* a,const sll_string_t* b,sll_string_t* out){
	if (!a->length){
		sll_string_clone(b,out);
		return;
	}
	if (!b->length){
		sll_string_clone(a,out);
		return;
	}
	out->length=a->length+b->length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(out->length)*sizeof(sll_char_t));
	INIT_PADDING(out->data,out->length);
	const wide_data_t* ap=(const wide_data_t*)(a->data);
	wide_data_t* op=(wide_data_t*)(out->data);
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
			out->data[a->length+j]=b->data[j];
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
			out->data[j+k]=*(s+k);
		}
	}
	out->checksum=a->checksum^ROTATE_BITS(b->checksum,(a->length&3)<<3);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL void sll_string_concat_char(const sll_string_t* string,sll_char_t char_,sll_string_t* out){
	out->length=string->length+1;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(out->length)*sizeof(sll_char_t));
	INIT_PADDING(out->data,out->length);
	const wide_data_t* a=(const wide_data_t*)(string->data);
	wide_data_t* b=(wide_data_t*)(out->data);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	for (sll_string_length_t i=0;i<=(string->length>>3);i++){
		*(b+i)=*(a+i);
	}
	out->data[string->length]=char_;
	out->checksum=string->checksum^(((sll_string_checksum_t)char_)<<((string->length&3)<<3));
}



__SLL_EXTERNAL void sll_string_concat_chars(sll_char_t a,sll_char_t b,sll_string_t* out){
	out->length=2;
	out->checksum=a|(b<<8);
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(2)*sizeof(sll_char_t));
	STRING_DATA_PTR(out->data);
	*((wide_data_t*)(out->data))=out->checksum;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count(const sll_string_t* string,const sll_string_t* substring){
	if (string->length<substring->length){
		return 0;
	}
	if (!substring->length){
		return string->length+1;
	}
	if (!string->length){
		return 0;
	}
	sll_string_length_t o=0;
	for (sll_string_length_t i=0;i<string->length-substring->length+1;i++){
		if (sll_compare_data(string->data+i,substring->data,substring->length)==SLL_COMPARE_RESULT_EQUAL){
			o++;
		}
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_char(const sll_string_t* string,sll_char_t char_){
	if (!string->length){
		return 0;
	}
	sll_string_length_t o=0;
	const wide_data_t* p=(const wide_data_t*)(string->data);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*char_;
	for (sll_string_length_t i=0;i<(string->length+7)>>3;i++){
		wide_data_t v=(*(p+i))^m;
		o+=(sll_string_length_t)POPULATION_COUNT((v-0x101010101010101ull)&0x8080808080808080ull&(~v));
	}
	if (!char_){
		return o-((string->length&7)?8-(string->length&7):0);
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_left(const sll_string_t* string,sll_char_t char_){
	if (!string->length||string->data[0]!=char_){
		return 0;
	}
	const wide_data_t* p=(const wide_data_t*)(string->data);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*char_;
	for (sll_string_length_t i=0;i<((string->length+7)>>3);i++){
		wide_data_t v=(*p)^m;
		v=((~(v-0x101010101010101ull))|v)&0x8080808080808080ull;
		if (v){
			sll_string_length_t o=(i<<3)+(FIND_FIRST_SET_BIT(v)>>3);
			return (!char_&&o>=string->length?string->length:o);
		}
		p++;
	}
	return string->length;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_right(const sll_string_t* string,sll_char_t char_){
	if (!string->length||string->data[string->length-1]!=char_){
		return 0;
	}
	if (!char_){
		SLL_UNIMPLEMENTED();
	}
	sll_string_length_t l=(string->length+7)>>3;
	const wide_data_t* p=((const wide_data_t*)(string->data))+l;
	wide_data_t m=0x101010101010101ull*char_;
	wide_data_t n=m<<(((string->length&7))<<3);
	for (sll_string_length_t i=0;i<l;i++){
		p--;
		wide_data_t v=((*p)|n)^m;
		v=(v|(~(v-0x101010101010101ull)))&0x8080808080808080ull;
		if (v){
			return (i<<3)+((string->length&7)?(string->length&7):8)-(FIND_LAST_SET_BIT(v)>>3)-1;
		}
		n=0;
	}
	return string->length;
}



__SLL_EXTERNAL void sll_string_create(sll_string_length_t length,sll_string_t* out){
	out->length=length;
	out->checksum=0;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(length)*sizeof(sll_char_t));
	INIT_PADDING(out->data,length);
}



__SLL_EXTERNAL void sll_string_decrease(sll_string_t* string,sll_string_length_t length){
	if (length!=string->length){
		string->length=length;
		sll_allocator_resize((void**)(&(string->data)),SLL_STRING_ALIGN_LENGTH(length)*sizeof(sll_char_t));
		SLL_STRING_FORMAT_PADDING(string->data,string->length);
	}
}



__SLL_EXTERNAL void sll_string_duplicate(const sll_string_t* string,sll_integer_t count,sll_string_length_t extra,sll_string_t* out){
	if (!string->length){
		SLL_INIT_STRING(out);
		return;
	}
	SLL_ASSERT(extra<string->length);
	sll_bool_t r=0;
	if (count<0){
		count=-count;
		r=1;
	}
	if (!count){
		if (!extra){
			SLL_INIT_STRING(out);
			return;
		}
		out->length=extra;
		out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(extra)*sizeof(sll_char_t));
		const wide_data_t* a=(const wide_data_t*)(string->data);
		wide_data_t* b=(wide_data_t*)(out->data);
		STRING_DATA_PTR(a);
		STRING_DATA_PTR(b);
		wide_data_t c=0;
		while (extra>7){
			*b=*a;
			c^=*b;
			a++;
			b++;
			extra-=8;
		}
		*b=(*a)&((1ull<<(extra<<3))-1);
		c^=*b;
		out->checksum=(sll_string_length_t)(c^(c>>32));
		return;
	}
	count*=string->length;
	SLL_ASSERT(count<SLL_MAX_STRING_LENGTH);
	out->length=((sll_string_length_t)count)+extra;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(out->length)*sizeof(sll_char_t));
	wide_data_t* op=(wide_data_t*)(out->data);
	STRING_DATA_PTR(op);
	if (r){
		sll_string_length_t i=string->length-1;
		for (sll_string_length_t j=0;j<i;j++){
			out->data[i]=string->data[j];
			out->data[j]=string->data[i];
			i--;
		}
		if (string->length&1){
			out->data[i]=string->data[i];
		}
	}
	else{
		const wide_data_t* ap=(const wide_data_t*)(string->data);
		STRING_DATA_PTR(ap);
		for (sll_string_length_t i=0;i<((string->length+7)>>3);i++){
			*(op+i)=*(ap+i);
		}
	}
	sll_string_length_t i=string->length;
	if (string->length&7){
		sll_string_length_t j=(out->length<16?out->length:(string->length+15)&0xfffffff0);
		while (i<j){
			out->data[i]=out->data[i-string->length];
			i++;
		}
	}
	count+=extra;
	if (i<count){
		const wide_data_t* ap=(const wide_data_t*)(out->data+((string->length&7)?8-(string->length&7):0));
		i>>=3;
		wide_data_t c=0;
		for (sll_string_length_t j=0;j<i;j++){
			c^=*(op+j);
		}
		count=(count+7)>>3;
		if (string->length&7){
			count--;
		}
		sll_string_length_t j=0;
		while (i<count){
			*(op+i)=*(ap+j);
			c^=*(op+i);
			i++;
			j++;
		}
		*(op+i)=(*(ap+j))&((1ull<<((out->length&7)<<3))-1);
		c^=*(op+i);
		out->checksum=(sll_string_length_t)(c^(c>>32));
	}
	else{
		sll_string_calculate_checksum(out);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_ends(const sll_string_t* string,const sll_string_t* suffix){
	if (string->length<suffix->length){
		return 0;
	}
	if (string->length==suffix->length){
		return sll_string_equal(string,suffix);
	}
	return _compare_data(string->data+string->length-suffix->length,suffix->data,suffix->length,0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal(const sll_string_t* a,const sll_string_t* b){
	return STRING_EQUAL(a,b);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_array(const sll_string_t* string,const sll_array_t* array){
	if (string->length!=array->length){
		return 0;
	}
	for (sll_string_length_t i=0;i<string->length;i++){
		sll_object_t* e=array->data[i];
		if ((e->type==SLL_OBJECT_TYPE_CHAR&&e->data.char_==string->data[i])||(e->type==SLL_OBJECT_TYPE_INT&&e->data.int_==string->data[i])||(e->type==SLL_OBJECT_TYPE_STRING&&e->data.string.length==1&&e->data.string.data[0]==string->data[i])){
			continue;
		}
		return 0;
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_map(const sll_string_t* string,const sll_map_t* map){
	if (string->length!=map->length){
		return 0;
	}
	for (sll_map_length_t i=0;i<(map->length<<1);i+=2){
		sll_object_t* e=map->data[i];
		if (e->type!=SLL_OBJECT_TYPE_INT||e->data.int_<0||e->data.int_>=string->length){
			return 0;
		}
		sll_char_t c=string->data[e->data.int_];
		e=map->data[i+1];
		if ((e->type==SLL_OBJECT_TYPE_CHAR&&e->data.char_==c)||(e->type==SLL_OBJECT_TYPE_INT&&e->data.int_==c)||(e->type==SLL_OBJECT_TYPE_STRING&&e->data.string.length==1&&e->data.string.data[0]==c)){
			continue;
		}
		return 0;
	}
	return 1;
}



__SLL_EXTERNAL void sll_string_flip_case(const sll_string_t* string,sll_string_t* out){
	out->length=string->length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(string->length)*sizeof(sll_char_t));
	INIT_PADDING(out->data,out->length);
	const wide_data_t* a=(const wide_data_t*)(string->data);
	wide_data_t* b=(wide_data_t*)(out->data);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	wide_data_t c=0;
	for (sll_string_length_t i=0;i<((out->length+7)>>3);i++){
		*(b+i)=(*(a+i))^((((((*(a+i))&0xc0c0c0c0c0c0c0c0ull)^0x4040404040404040ull)-0x101010101010101ull)&(0x9a9a9a9a9a9a9a9aull-((*(a+i))&0x1f1f1f1f1f1f1f1full))&(((*(a+i))&0x1f1f1f1f1f1f1f1full)+0x7f7f7f7f7f7f7f7full)&(~(*(a+i)))&0x8080808080808080ull)>>2);
		c^=*(b+i);
	}
	out->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_from_char(sll_char_t char_,sll_string_t* out){
	out->length=1;
	out->checksum=char_;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
	STRING_DATA_PTR(out->data);
	*((wide_data_t*)(out->data))=out->checksum;
}



__SLL_EXTERNAL void sll_string_from_data(sll_object_t** object_data,sll_string_length_t object_count,sll_string_t* out){
	out->length=object_count;
	out->checksum=0;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(object_count)*sizeof(sll_char_t));
	INIT_PADDING(out->data,object_count);
	for (sll_string_length_t i=0;i<object_count;i++){
		sll_object_t* n=sll_operator_cast(*(object_data+i),sll_static_int[SLL_OBJECT_TYPE_CHAR]);
		SLL_ASSERT(n->type==SLL_OBJECT_TYPE_CHAR);
		out->data[i]=n->data.char_;
		out->checksum^=ROTATE_BITS(n->data.char_,(i&3)<<3);
		SLL_RELEASE(n);
	}
}



__SLL_EXTERNAL void sll_string_from_int(sll_integer_t int_,sll_string_t* out){
	if (!int_){
		out->length=1;
		out->checksum='0';
		out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
		STRING_DATA_PTR(out->data);
		*((wide_data_t*)(out->data))='0';
		return;
	}
	sll_bool_t n=0;
	if (int_<0){
		int_=-int_;
		n=1;
	}
	sll_char_t bf[20];
	sll_string_length_t i=20;
	do{
		i--;
		bf[i]=int_%10;
		int_/=10;
	} while (int_);
	out->length=n+(20-i);
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(out->length)*sizeof(sll_char_t));
	INIT_PADDING(out->data,out->length);
	if (n){
		out->data[0]='-';
	}
	for (sll_string_length_t j=0;j<20-i;j++){
		out->data[j+n]=bf[i+j];
	}
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL void sll_string_from_pointer(const sll_char_t* pointer,sll_string_t* out){
	if (!pointer){
		SLL_INIT_STRING(out);
		return;
	}
	sll_string_from_pointer_length(pointer,sll_string_length(pointer),out);
}



__SLL_EXTERNAL void sll_string_from_pointer_length(const sll_char_t* pointer,sll_string_length_t length,sll_string_t* out){
	if (!length){
		SLL_INIT_STRING(out);
		return;
	}
	out->length=length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(length)*sizeof(sll_char_t));
#ifdef __SLL_BUILD_FUZZER
	INIT_PADDING(out->data,length);
	sll_copy_data(pointer,length,out->data);
	sll_string_calculate_checksum(out);
#else
	const wide_data_t* a=(const wide_data_t*)pointer;
	wide_data_t* b=(wide_data_t*)(out->data);
	STRING_DATA_PTR(b);
	wide_data_t c=0;
#ifndef __SLL_BUILD_DARWIN
	__m256i c256=_mm256_setzero_si256();
	while (length>31){
		__m256i k=_mm256_lddqu_si256((const __m256i*)a);
		c256=_mm256_xor_si256(c256,k);
		_mm256_storeu_si256((__m256i*)b,k);
		length-=32;
		a+=4;
		b+=4;
	}
	__m128i c128=_mm_xor_si128(_mm256_castsi256_si128(c256),_mm256_extractf128_si256(c256,1));
	if (length>15){
		__m128i k=_mm_lddqu_si128((const __m128i*)a);
		c128=_mm_xor_si128(c128,k);
		_mm_storeu_si128((__m128i*)b,k);
		length-=16;
		a+=2;
		b+=2;
	}
	c=_mm_cvtsi128_si64(_mm_xor_si128(c128,_mm_shuffle_epi32(c128,0x0e)));
#endif
	while (length>7){
		*b=*a;
		c^=*b;
		a++;
		b++;
		length-=8;
	}
	*b=(*a)&((1ull<<(length<<3))-1);
	c^=*b;
	out->checksum=(sll_string_length_t)(c^(c>>32));
#endif
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_string_get(const sll_string_t* string,sll_string_length_t index){
	return (index<string->length?string->data[index]:0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_includes(const sll_string_t* string,const sll_string_t* substring){
	if (!substring->length){
		return 1;
	}
	if (!string->length||substring->length>string->length){
		return 0;
	}
	if (substring->length==1){
		return sll_string_includes_char(string,substring->data[0]);
	}
	if (string->length==substring->length){
		return sll_string_equal(string,substring);
	}
	wide_data_t c64=0;
	const wide_data_t* p=(const wide_data_t*)(string->data);
	STRING_DATA_PTR(p);
	for (sll_string_length_t i=0;i<((substring->length+7)>>3);i++){
		c64^=*p;
		p++;
	}
	if (substring->length&7){
		c64^=(*(p-1))&(0xffffffffffffffffull<<((substring->length&7)<<3));
	}
	sll_string_checksum_t c=(sll_string_checksum_t)(c64^(c64>>32));
	unsigned int shift=(substring->length&3)<<3;
	const sll_char_t* ptr=string->data;
	for (sll_string_length_t i=0;i<string->length-substring->length;i++){
		if (c==substring->checksum&&sll_compare_data(ptr,substring->data,substring->length)==SLL_COMPARE_RESULT_EQUAL){
			return 1;
		}
		c=ROTATE_BITS_RIGHT(c^(*ptr)^((*(ptr+substring->length))<<shift),8);
		ptr++;
	}
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_includes_char(const sll_string_t* string,sll_char_t char_){
	if (!string->length){
		return 0;
	}
	const wide_data_t* p=(const wide_data_t*)(string->data);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*char_;
	for (sll_string_length_t i=0;i<((string->length+7)>>3);i++){
		wide_data_t v=(*p)^m;
		v=(v-0x101010101010101ull)&0x8080808080808080ull&(~v);
		if (v){
			return (char_?1:((i<<3)+(FIND_FIRST_SET_BIT(v)>>3)<string->length));
		}
		p++;
	}
	return 0;
}



__SLL_EXTERNAL void sll_string_increase(sll_string_t* string,sll_string_length_t length){
	if (!length){
		return;
	}
	length+=string->length;
	if (!string->length||SLL_STRING_ALIGN_LENGTH(length)>SLL_STRING_ALIGN_LENGTH(string->length)){
		sll_allocator_resize((void**)(&(string->data)),SLL_STRING_ALIGN_LENGTH(length)*sizeof(sll_char_t));
		SLL_STRING_FORMAT_PADDING(string->data,length);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index(const sll_string_t* string,const sll_string_t* substring,sll_string_length_t start_index){
	if (!substring->length){
		return 0;
	}
	if (!string->length||substring->length>string->length){
		return SLL_MAX_STRING_LENGTH;
	}
	if (substring->length==1){
		return sll_string_index_char(string,substring->data[0],0,start_index);
	}
	if (string->length==substring->length){
		return (sll_string_equal(string,substring)?0:SLL_MAX_STRING_LENGTH);
	}
	wide_data_t c64=0;
	const wide_data_t* p=(const wide_data_t*)(string->data+start_index);
	for (sll_string_length_t i=0;i<((substring->length+7)>>3);i++){
		c64^=*p;
		p++;
	}
	if (substring->length&7){
		c64^=(*(p-1))&(0xffffffffffffffffull<<((substring->length&7)<<3));
	}
	sll_string_checksum_t c=(sll_string_checksum_t)(c64^(c64>>32));
	unsigned int shift=(substring->length&3)<<3;
	const sll_char_t* ptr=string->data+start_index;
	for (;start_index<string->length-substring->length;start_index++){
		if (c==substring->checksum&&sll_compare_data(ptr,substring->data,substring->length)==SLL_COMPARE_RESULT_EQUAL){
			return start_index;
		}
		c=ROTATE_BITS_RIGHT(c^(*ptr)^((*(ptr+substring->length))<<shift),8);
		ptr++;
	}
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_char(const sll_string_t* string,sll_char_t char_,sll_bool_t inv,sll_string_length_t start_index){
	if (start_index>=string->length){
		return SLL_MAX_STRING_LENGTH;
	}
	const wide_data_t* p=(const wide_data_t*)(string->data);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*char_;
	wide_data_t n=0x8080808080808080ull<<((start_index&7)<<3);
	wide_data_t q=0x8080808080808080ull*inv;
	sll_string_length_t i=start_index>>3;
	p+=i;
	for (;i<((string->length+7)>>3);i++){
		wide_data_t v=(*p)^m;
		v=((v-0x101010101010101ull)&(~v)&n)^q;
		if (v){
			sll_string_length_t o=(i<<3)+(FIND_FIRST_SET_BIT(v)>>3);
			return (!char_&&o>=string->length?SLL_MAX_STRING_LENGTH:o);
		}
		p++;
		n=0x8080808080808080ull;
	}
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_multiple(const sll_string_t* string,const sll_char_t* char_data,sll_string_length_t char_count,sll_bool_t inv){
	if (!string->length||!char_count){
		return SLL_MAX_STRING_LENGTH;
	}
	if (char_count==1){
		return sll_string_index_char(string,*char_data,inv,0);
	}
	const wide_data_t* p=(const wide_data_t*)(string->data);
	STRING_DATA_PTR(p);
#ifdef __SLL_BUILD_DARWIN
	wide_data_t* ml=sll_allocate_stack(char_count*sizeof(wide_data_t));
	for (sll_string_length_t i=0;i<char_count;i++){
		*(ml+i)=0x101010101010101ull*(*(char_data+i));
	}
	wide_data_t n=0x8080808080808080ull*inv;
	for (sll_string_length_t i=0;i<((string->length+7)>>3);i++){
		wide_data_t k=*p;
		wide_data_t v=0;
		for (sll_string_length_t j=0;j<char_count;j++){
			wide_data_t e=k^(*(ml+j));
			v|=(e-0x101010101010101ull)&(~e);
		}
		v=(v&0x8080808080808080ull)^n;
		if (v){
			sll_deallocate(ml);
			sll_string_length_t o=(i<<3)+(FIND_FIRST_SET_BIT(v)>>3);
			return (o>=string->length?SLL_MAX_STRING_LENGTH:o);
		}
		p++;
	}
	sll_deallocate(ml);
#else
	sll_string_length_t ln=(char_count+3)&0xfffffffc;
	wide_data_t* ml=sll_allocate_stack(ln*sizeof(wide_data_t));
	sll_string_length_t i=0;
	for (;i<char_count-1;i++){
		*(ml+i)=0x101010101010101ull*(*(char_data+i));
	}
	wide_data_t any=0x101010101010101ull*(*(char_data+i));
	for (;i<ln;i++){
		*(ml+i)=any;
	}
	ln>>=2;
	__m256i sub=_mm256_set1_epi8(1);
	wide_data_t n=0x8080808080808080ull*inv;
	for (i=0;i<((string->length+7)>>3);i++){
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
			return (o>=string->length?SLL_MAX_STRING_LENGTH:o);
		}
		p++;
	}
	sll_deallocate(ml);
#endif
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse(const sll_string_t* string,const sll_string_t* substring){
	if (!substring->length){
		return 0;
	}
	if (!string->length){
		return SLL_MAX_STRING_LENGTH;
	}
	if (substring->length==1){
		return sll_string_index_reverse_char(string,substring->data[0],0);
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse_char(const sll_string_t* string,sll_char_t char_,sll_bool_t inv){
	if (!string->length){
		return SLL_MAX_STRING_LENGTH;
	}
	sll_string_length_t l=(string->length+7)>>3;
	const wide_data_t* p=((const wide_data_t*)(string->data))+l;
	wide_data_t m=0x101010101010101ull*char_;
	wide_data_t n=0x8080808080808080ull*inv;
	for (sll_string_length_t i=0;i<l;i++){
		p--;
		wide_data_t v=(*p)^m;
		v=((v-0x101010101010101ull)&(~v)&0x8080808080808080ull)^n;
		if (v){
			if (!i&&(string->length&7)){
				v&=0xffffffffffffffffull>>((8-string->length)<<3);
				if (!v){
					continue;
				}
			}
			sll_string_length_t o=((l-i-1)<<3)+(FIND_LAST_SET_BIT(v)>>3);
			return (!char_&&o>=string->length?SLL_MAX_STRING_LENGTH:o);
		}
	}
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse_multiple(const sll_string_t* string,const sll_char_t* char_data,sll_string_length_t char_count,sll_bool_t inv){
	if (!string->length||!char_count){
		return SLL_MAX_STRING_LENGTH;
	}
	sll_string_length_t l=(string->length+7)>>3;
	const wide_data_t* p=((const wide_data_t*)(string->data))+l;
	ASSUME_ALIGNED(p,3,0);
#ifdef __SLL_BUILD_DARWIN
	wide_data_t* ml=sll_allocate_stack(char_count*sizeof(wide_data_t));
	for (sll_string_length_t i=0;i<char_count;i++){
		*(ml+i)=0x101010101010101ull*(*(char_data+i));
	}
	wide_data_t n=0x8080808080808080ull*inv;
	for (sll_string_length_t i=0;i<l;i++){
		p--;
		wide_data_t k=*p;
		wide_data_t v=0;
		for (sll_string_length_t j=0;j<char_count;j++){
			wide_data_t e=k^(*(ml+j));
			v|=(e-0x101010101010101ull)&(~e);
		}
		v=(v&0x8080808080808080ull)^n;
		if (v){
			if (!i&&(string->length&7)){
				v&=0xffffffffffffffffull>>((8-string->length)<<3);
				if (!v){
					continue;
				}
			}
			sll_deallocate(ml);
			sll_string_length_t o=((l-i-1)<<3)+(FIND_LAST_SET_BIT(v)>>3);
			return (o>=string->length?SLL_MAX_STRING_LENGTH:o);
		}
	}
	sll_deallocate(ml);
#else
	sll_string_length_t ln=(char_count+3)&0xfffffffc;
	wide_data_t* ml=sll_allocate_stack(ln*sizeof(wide_data_t));
	sll_string_length_t i=0;
	for (;i<char_count-1;i++){
		*(ml+i)=0x101010101010101ull*(*(char_data+i));
	}
	wide_data_t any=0x101010101010101ull*(*(char_data+i));
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
			if (!i&&(string->length&7)){
				v&=0xffffffffffffffffull>>((8-string->length)<<3);
				if (!v){
					continue;
				}
			}
			sll_deallocate(ml);
			sll_string_length_t o=((l-i-1)<<3)+(FIND_LAST_SET_BIT(v)>>3);
			return (o>=string->length?SLL_MAX_STRING_LENGTH:o);
		}
	}
	sll_deallocate(ml);
#endif
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL sll_string_length_t sll_string_insert_pointer(const sll_char_t* pointer,sll_string_length_t index,sll_string_t* out){
	return sll_string_insert_pointer_length(pointer,sll_string_length(pointer),index,out);
}



__SLL_EXTERNAL sll_string_length_t sll_string_insert_pointer_length(const sll_char_t* pointer,sll_string_length_t length,sll_string_length_t index,sll_string_t* out){
	if (index+length>out->length){
		length=out->length-index;
	}
	if (length<16){
		STRING_DATA_PTR(out->data);
		while (length){
			out->data[index]=*pointer;
			length--;
			index++;
			pointer++;
		}
		return index;
	}
	if (index&7){
		STRING_DATA_PTR(out->data);
		sll_string_length_t j=8-(index&7);
		index+=j;
		length-=j;
		pointer+=j;
		do{
			out->data[index-j]=*(pointer-j);
			j--;
		} while (j);
	}
	SLL_ASSERT(!(index&7));
	const wide_data_t* ap=(const wide_data_t*)pointer;
	STRING_DATA_PTR(out->data);
	wide_data_t* op=(wide_data_t*)(out->data+index);
	sll_string_length_t j=0;
	for (;j<(length>>3);j++){
		*(op+j)=*(ap+j);
	}
	index+=length;
	if (length&7){
		length=(length&7)<<3;
		*(op+j)=((*(op+j))&(0xffffffffffffffffull<<length))|((*(ap+j))&((1ull<<length)-1));
	}
	return index;
}



__SLL_EXTERNAL void sll_string_inv(const sll_string_t* string,sll_string_t* out){
	if (!string->length){
		SLL_INIT_STRING(out);
		return;
	}
	out->length=string->length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(string->length)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)(string->data);
	wide_data_t* b=(wide_data_t*)(out->data);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	wide_data_t c=0;
	sll_string_length_t l=string->length;
	while (l>7){
		*b=~(*a);
		c^=*b;
		a++;
		b++;
		l-=8;
	}
	*b=(~(*a))&((1ull<<(l<<3))-1);
	c^=*b;
	out->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_join(const sll_string_t* string,sll_object_t*const* object_data,sll_array_length_t object_count,sll_string_t* out){
	STRING_INIT_STACK(out);
	for (sll_array_length_t i=0;i<object_count;i++){
		if (i){
			sll_string_increase(out,string->length);
			sll_copy_data(string->data,string->length,out->data+out->length);
			out->length+=string->length;
		}
		sll_object_t* n=sll_operator_cast(*(object_data+i),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_string_increase(out,n->data.string.length);
		sll_copy_data(n->data.string.data,n->data.string.length,out->data+out->length);
		out->length+=n->data.string.length;
		SLL_RELEASE(n);
	}
	sll_allocator_move((void**)(&(out->data)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL void sll_string_join_char(sll_char_t char_,sll_object_t*const* object_data,sll_array_length_t object_count,sll_string_t* out){
	STRING_INIT_STACK(out);
	for (sll_array_length_t i=0;i<object_count;i++){
		if (i){
			sll_string_increase(out,1);
			out->data[out->length]=char_;
			out->length++;
		}
		sll_object_t* n=sll_operator_cast(*(object_data+i),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_string_increase(out,n->data.string.length);
		sll_copy_data(n->data.string.data,n->data.string.length,out->data+out->length);
		out->length+=n->data.string.length;
		SLL_RELEASE(n);
	}
	sll_allocator_move((void**)(&(out->data)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_length(const sll_char_t* pointer){
	if (!pointer){
		return 0;
	}
	addr_t base=ADDR(pointer);
#ifdef __SLL_BUILD_FUZZER
	while (*pointer){
		pointer++;
	}
	return (sll_string_length_t)(ADDR(pointer)-base);
#else
	while (ADDR(pointer)&7){
		if (!(*pointer)){
			return (sll_string_length_t)(ADDR(pointer)-base);
		}
		pointer++;
	}
	const wide_data_t* pointer64=(const wide_data_t*)pointer;
	ASSUME_ALIGNED(pointer64,3,0);
	while (1){
		wide_data_t v=((*pointer64)-0x101010101010101ull)&0x8080808080808080ull&(~(*pointer64));
		if (v){
			return (sll_string_length_t)(ADDR(pointer64)+(FIND_FIRST_SET_BIT(v)>>3)-base);
		}
		pointer64++;
	}
#endif
}



__SLL_EXTERNAL void sll_string_lower_case(const sll_string_t* string,sll_string_t* out){
	EXECUTE_CASE_SWITCH(0xdadadadadadadadaull,0x3f3f3f3f3f3f3f3full,ADD);
}



__SLL_EXTERNAL void sll_string_op(const sll_string_t* a,const sll_string_t* b,sll_binary_operator_t operator,sll_string_t* out){
	sll_string_length_t e=a->length;
	out->length=b->length;
	if (b->length<a->length){
		e=b->length;
		out->length=a->length;
	}
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(out->length)*sizeof(sll_char_t));
	INIT_PADDING(out->data,out->length);
	for (sll_string_length_t i=0;i<e;i++){
		sll_object_t* v=operator(sll_static_char[a->data[i]],sll_static_char[b->data[i]]);
		sll_object_t* c=sll_operator_cast(v,sll_static_int[SLL_OBJECT_TYPE_CHAR]);
		SLL_RELEASE(v);
		SLL_ASSERT(c->type==SLL_OBJECT_TYPE_CHAR);
		out->data[i]=c->data.char_;
		SLL_RELEASE(c);
	}
	if (a->length==b->length){
		return;
	}
	const sll_char_t* s=(a->length>b->length?a->data:b->data)+e;
	do{
		out->data[e]=s[e];
		e++;
	} while (e<out->length);
}



__SLL_EXTERNAL void sll_string_op_array(const sll_string_t* string,const sll_array_t* array,sll_binary_operator_t operator,sll_bool_t inv,sll_array_t* out){
	sll_array_length_t e=array->length;
	out->length=string->length;
	if (string->length<array->length){
		e=string->length;
		out->length=array->length;
	}
	out->data=sll_allocator_init(out->length*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<e;i++){
		out->data[i]=(inv?operator(array->data[i],sll_static_char[string->data[i]]):operator(sll_static_char[string->data[i]],array->data[i]));
	}
	if (string->length==array->length){
		return;
	}
	if (string->length>array->length){
		do{
			out->data[e]=SLL_FROM_CHAR(string->data[e]);
			e++;
		} while (e<out->length);
	}
	else{
		do{
			out->data[e]=array->data[e];
			SLL_ACQUIRE(out->data[e]);
			e++;
		} while (e<out->length);
	}
}



__SLL_EXTERNAL void sll_string_op_map(const sll_string_t* string,const sll_map_t* map,sll_binary_operator_t operator,sll_bool_t inv,sll_map_t* out){
	out->length=string->length+map->length;
	out->data=sll_allocate((string->length<<1)*sizeof(sll_object_t*));
	sll_string_length_t sml=(string->length+63)>>6;
	bitmap_t* sm=sll_zero_allocate_stack(sml*sizeof(bitmap_t));
	sll_map_length_t i=0;
	for (sll_map_length_t j=0;j<map->length;j++){
		SLL_ACQUIRE(map->data[j<<1]);
		out->data[i]=map->data[j<<1];
		sll_integer_t idx=map->data[j<<1]->data.int_;
		if (map->data[j<<1]->type==SLL_OBJECT_TYPE_INT&&idx>=0&&idx<string->length){
			*(sm+(idx>>6))|=1ull<<(idx&63);
			out->data[i+1]=(inv?operator(map->data[(j<<1)+1],sll_static_char[string->data[idx]]):operator(sll_static_char[string->data[idx]],map->data[(j<<1)+1]));
		}
		else{
			out->data[i+1]=map->data[(j<<1)+1];
			SLL_ACQUIRE(map->data[(j<<1)+1]);
		}
		i+=2;
	}
	for (sll_string_length_t j=0;j<sml;j++){
		bitmap_t v=~(*(sm+j));
		while (v){
			sll_string_length_t k=FIND_FIRST_SET_BIT(v)|(j<<6);
			if (k>=string->length){
				break;
			}
			v&=v-1;
			out->data[i]=sll_int_to_object(k);
			out->data[i+1]=SLL_FROM_CHAR(string->data[k]);
			i+=2;
		}
	}
	sll_deallocate(sm);
	if ((i>>1)!=out->length){
		out->length=i>>1;
		out->data=sll_reallocate(out->data,i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_string_or(const sll_string_t* a,const sll_string_t* b,sll_string_t* out){
	if (a->length<b->length){
		const sll_string_t* c=a;
		a=b;
		b=c;
	}
	out->length=a->length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(a->length)*sizeof(sll_char_t));
	const wide_data_t* ap=(const wide_data_t*)(a->data);
	const wide_data_t* bp=(const wide_data_t*)(b->data);
	wide_data_t* op=(wide_data_t*)(out->data);
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
	out->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_or_char(const sll_string_t* string,sll_char_t char_,sll_string_t* out){
	EXECUTE_BINARY_OPERATOR_CHAR(|);
}



__SLL_EXTERNAL void sll_string_pad(const sll_string_t* string,sll_string_length_t length,sll_char_t char_,sll_string_t* out){
	if (string->length>=length){
		sll_string_clone(string,out);
		return;
	}
	out->length=length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(length)*sizeof(sll_char_t));
	sll_string_length_t i=(length-string->length)>>1;
	sll_set_memory(out->data,i,char_);
	sll_copy_data(string->data,string->length,out->data+i);
	sll_set_memory(out->data+i+string->length,length-i-string->length,char_);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL void sll_string_pad_left(const sll_string_t* string,sll_string_length_t length,sll_char_t char_,sll_string_t* out){
	if (string->length>=length){
		sll_string_clone(string,out);
		return;
	}
	out->length=length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(length)*sizeof(sll_char_t));
	sll_copy_data(string->data,string->length,out->data);
	sll_set_memory(out->data+string->length,length-string->length,char_);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL void sll_string_pad_right(const sll_string_t* string,sll_string_length_t length,sll_char_t char_,sll_string_t* out){
	if (string->length>=length){
		sll_string_clone(string,out);
		return;
	}
	out->length=length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(length)*sizeof(sll_char_t));
	sll_set_memory(out->data,length-string->length,char_);
	sll_copy_data(string->data,string->length,out->data+length-string->length);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_string_parse_char(const sll_string_t* string){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_string_parse_float(const sll_string_t* string){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_string_parse_int(const sll_string_t* string){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_string_prepend_char(const sll_string_t* string,sll_char_t char_,sll_string_t* out){
	out->length=string->length+1;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(out->length)*sizeof(sll_char_t));
	INIT_PADDING(out->data,out->length);
	const wide_data_t* a=(const wide_data_t*)(string->data+1);
	wide_data_t* b=(wide_data_t*)(out->data);
	STRING_DATA_PTR(b);
	*b=char_|((*((wide_data_t*)(string->data)))<<8);
	b++;
	for (sll_string_length_t i=0;i<(string->length>>3);i++){
		*(b+i)=*(a+i);
	}
	out->checksum=char_^ROTATE_BITS(string->checksum,8);
}



__SLL_EXTERNAL void sll_string_remove(const sll_string_t* string,const sll_string_t* substring,sll_string_t* out){
	if (!string->length){
		SLL_INIT_STRING(out);
		return;
	}
	out->length=string->length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(string->length)*sizeof(sll_char_t));
	sll_string_length_t i=0;
	sll_string_length_t j=0;
	while (j<string->length-substring->length+1){
		if (sll_compare_data(string->data+j,substring->data,substring->length)!=SLL_COMPARE_RESULT_EQUAL){
			out->data[i]=string->data[j];
			i++;
			j++;
		}
		else{
			j+=substring->length;
		}
	}
	sll_copy_data(string->data+j,string->length-j,out->data+i);
	out->length=i+string->length-j;
	if (!out->length){
		sll_allocator_release(out->data);
		out->data=NULL;
		return;
	}
	sll_allocator_resize((void**)(&(out->data)),SLL_STRING_ALIGN_LENGTH(out->length)*sizeof(sll_char_t));
	SLL_STRING_FORMAT_PADDING(out->data,out->length);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL void sll_string_replace(const sll_string_t* string,const sll_string_t* old,const sll_string_t* new,sll_string_t* out){
	if (sll_string_equal(string,old)){
		sll_string_clone(new,out);
		return;
	}
	if (old->length>=string->length){
		sll_string_clone(string,out);
		return;
	}
	if (!new->length){
		sll_string_remove(string,old,out);
		return;
	}
	if (old->length==1&&new->length==1){
		sll_string_replace_char(string,old->data[0],new->data[0],out);
		return;
	}
	out->length=string->length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(string->length)*sizeof(sll_char_t));
	sll_string_length_t i=0;
	sll_string_length_t j=0;
	if (new->length<=old->length){
		while (j<string->length-old->length){
			if (sll_compare_data(string->data+j,old->data,old->length)!=SLL_COMPARE_RESULT_EQUAL){
				out->data[i]=string->data[j];
				i++;
				j++;
			}
			else{
				sll_copy_data(new->data,new->length,out->data+i);
				i+=new->length;
				j+=old->length;
			}
		}
	}
	else{
		while (j<string->length-old->length+1){
			if (sll_compare_data(string->data+j,old->data,old->length)!=SLL_COMPARE_RESULT_EQUAL){
				out->data[i]=string->data[j];
				i++;
				j++;
			}
			else{
				out->length+=new->length-old->length;
				sll_allocator_resize((void**)(&(out->data)),SLL_STRING_ALIGN_LENGTH(out->length)*sizeof(sll_char_t));
				for (sll_string_length_t e=0;e<new->length;e++){
					out->data[i]=new->data[e];
					i++;
				}
				j+=old->length;
			}
		}
	}
	while (j<string->length){
		out->data[i]=string->data[j];
		i++;
		j++;
	}
	if (i!=out->length){
		SLL_ASSERT(i<out->length);
		out->length=i;
		if (!out->length){
			sll_deallocate(out->data);
			out->data=NULL;
			return;
		}
		sll_allocator_resize((void**)(&(out->data)),SLL_STRING_ALIGN_LENGTH(out->length)*sizeof(sll_char_t));
	}
	SLL_STRING_FORMAT_PADDING(out->data,out->length);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL void sll_string_replace_char(const sll_string_t* string,sll_char_t old,sll_char_t new,sll_string_t* out){
	if (!string->length){
		SLL_INIT_STRING(out);
		return;
	}
	if (old==new){
		sll_string_clone(string,out);
		return;
	}
	out->length=string->length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(string->length)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)(string->data);
	wide_data_t* b=(wide_data_t*)(out->data);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	wide_data_t k64=0x101010101010101ull*old;
	wide_data_t v64=0x101010101010101ull*(old^new);
	wide_data_t c=0;
	sll_string_length_t l=string->length;
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
	out->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_resize(const sll_string_t* string,sll_integer_t delta,sll_string_t* out){
	if (!delta){
		sll_string_clone(string,out);
		return;
	}
	if (delta<0){
		delta=-delta;
		if (delta>=string->length){
			SLL_INIT_STRING(out);
		}
		else{
			sll_string_from_pointer_length(string->data+delta,(sll_string_length_t)(string->length-delta),out);
		}
		return;
	}
	SLL_ASSERT(string->length+delta<=SLL_MAX_STRING_LENGTH);
	out->length=(sll_string_length_t)(string->length+delta);
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(out->length)*sizeof(sll_char_t));
	INIT_PADDING(out->data,out->length);
	wide_data_t* p=(wide_data_t*)(out->data);
	for (sll_string_length_t i=0;i<((delta+7)>>3);i++){
		*p=0;
		p++;
	}
	sll_copy_data(string->data,string->length,out->data+delta);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL void sll_string_reverse(const sll_string_t* string,sll_string_t* out){
	if (!string->length){
		SLL_INIT_STRING(out);
		return;
	}
	out->length=string->length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(string->length)*sizeof(sll_char_t));
	sll_string_length_t i=string->length;
	for (sll_string_length_t j=0;j<string->length;j++){
		i--;
		out->data[j]=string->data[i];
	}
	sll_string_calculate_checksum(out);
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



__SLL_EXTERNAL void sll_string_select(const sll_string_t* string,sll_integer_t start,sll_integer_t end,sll_integer_t step,sll_string_t* out){
	WRAP_ARRAY_INDEX_VAR(start,string->length);
	WRAP_ARRAY_INDEX_VAR(end,string->length);
	SLL_INIT_STRING(out);
	if (!string->length||start==end||!step||(start<=end&&start>=string->length)||(end<=start&&end>=string->length)){
		return;
	}
	if (start<end){
		if (step<0){
			return;
		}
		if (start+step>=string->length){
			out->length=1;
			out->checksum=string->data[start];
			out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
			*((wide_data_t*)(out->data))=out->checksum;
			return;
		}
		if (step==1){
			sll_string_from_pointer_length(string->data+start,(sll_string_length_t)(end-start),out);
			return;
		}
		out->length=(sll_string_length_t)((end-start-1)/step+1);
		out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(out->length)*sizeof(sll_char_t));
		INIT_PADDING(out->data,out->length);
		sll_string_length_t i=0;
		do{
			out->data[i]=string->data[start];
			i++;
			start+=step;
		} while (start<end);
		SLL_ASSERT(i==out->length);
	}
	else{
		if (step>0){
			return;
		}
		step=-step;
		if (step>start){
			out->length=1;
			out->checksum=string->data[start];
			out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
			*((wide_data_t*)(out->data))=out->checksum;
			return;
		}
		out->length=(sll_string_length_t)((start-end-1)/step+1);
		out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(out->length)*sizeof(sll_char_t));
		INIT_PADDING(out->data,out->length);
		sll_string_length_t i=0;
		do{
			out->data[i]=string->data[start];
			i++;
			start-=step;
		} while (start>end);
		SLL_ASSERT(i==out->length);
	}
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL void sll_string_set_char(sll_string_t* string,sll_char_t char_,sll_string_length_t index){
	if (index<string->length){
		string->checksum^=((sll_string_checksum_t)(string->data[index]^char_))<<((index&3)<<3);
		string->data[index]=char_;
	}
}



__SLL_EXTERNAL void sll_string_shift(const sll_string_t* string,sll_integer_t delta,sll_string_t* out){
	if (!string->length){
		SLL_INIT_STRING(out);
		return;
	}
	out->length=string->length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(string->length)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)(string->data);
	wide_data_t* b=(wide_data_t*)(out->data);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	if (!delta){
		out->checksum=string->checksum;
		for (sll_string_length_t i=0;i<((string->length+8)>>3);i++){
			*(b+i)=*(a+i);
		}
		return;
	}
	if (delta<-7||delta>7){
		out->checksum=0;
		for (sll_string_length_t i=0;i<((string->length+8)>>3);i++){
			*(b+i)=0;
		}
		return;
	}
	INIT_PADDING(out->data,string->length);
	sll_string_length_t l=(string->length+7)>>3;
	wide_data_t c=0;
	if (delta<0){
		delta=-delta;
		wide_data_t m=0x101010101010101ull*((0xff<<delta)&0xff);
		do{
			l--;
			*(b+l)=((*(a+l))&m)>>delta;
			c^=*(b+l);
		} while (l);
	}
	else{
		wide_data_t m=0x101010101010101ull*(0xff>>delta);
		do{
			l--;
			*(b+l)=((*(a+l))&m)<<delta;
			c^=*(b+l);
		} while (l);
	}
	out->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_split(const sll_string_t* string,const sll_string_t* substring,sll_array_t* out){
	if (!string->length){
		out->length=1;
		out->data=sll_allocator_init(sizeof(sll_object_t*));
		out->data[0]=EMPTY_STRING_TO_OBJECT();
		return;
	}
	if (!substring->length){
		sll_string_to_array(string,out);
		return;
	}
	if (substring->length==1){
		sll_string_split_char(string,substring->data[0],out);
		return;
	}
	if (sll_string_equal(string,substring)){
		out->length=2;
		out->data=sll_allocator_init(2*sizeof(sll_object_t*));
		out->data[0]=EMPTY_STRING_TO_OBJECT();
		out->data[1]=EMPTY_STRING_TO_OBJECT();
		return;
	}
	if (substring->length>=string->length){
		out->length=1;
		out->data=sll_allocator_init(sizeof(sll_object_t*));
		out->data[0]=STRING_TO_OBJECT(string);
		return;
	}
	SLL_INIT_ARRAY(out);
	sll_string_length_t i=0;
	sll_string_length_t j=0;
	while (j<string->length-substring->length){
		if (sll_compare_data(string->data+j,substring->data,substring->length)==SLL_COMPARE_RESULT_EQUAL){
			sll_object_t* x=(i==j?EMPTY_STRING_TO_OBJECT():STRING_POINTER_LENGTH_TO_OBJECT(string->data+i,j-i));
			sll_array_push(NULL,x,out);
			SLL_RELEASE(x);
			j+=substring->length;
			i=j;
		}
		else{
			j++;
		}
	}
	sll_object_t* x=(i==string->length?EMPTY_STRING_TO_OBJECT():STRING_POINTER_LENGTH_TO_OBJECT(string->data+i,string->length-i));
	sll_array_push(NULL,x,out);
	SLL_RELEASE(x);
}



__SLL_EXTERNAL void sll_string_split_char(const sll_string_t* string,sll_char_t char_,sll_array_t* out){
	if (!string->length){
		out->length=1;
		out->data=sll_allocator_init(sizeof(sll_object_t*));
		out->data[0]=EMPTY_STRING_TO_OBJECT();
		return;
	}
	SLL_INIT_ARRAY(out);
	const wide_data_t* p=(const wide_data_t*)(string->data);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*char_;
	sll_array_length_t i=0;
	sll_string_length_t j=0;
	for (sll_string_length_t k=0;k<(string->length+7)>>3;k++){
		wide_data_t v=(*(p+k))^m;
		v=(v-0x101010101010101ull)&0x8080808080808080ull&(~v);
		while (v){
			sll_string_length_t l=(k<<3)|(FIND_FIRST_SET_BIT(v)>>3);
			out->length++;
			sll_allocator_resize((void**)(&(out->data)),out->length*sizeof(sll_object_t*));
			out->data[i]=(j<l?STRING_POINTER_LENGTH_TO_OBJECT(string->data+j,l-j):EMPTY_STRING_TO_OBJECT());
			i++;
			j=l+1;
			v&=v-1;
		}
	}
	out->length++;
	sll_allocator_resize((void**)(&(out->data)),out->length*sizeof(sll_object_t*));
	out->data[i]=(j<string->length?STRING_POINTER_LENGTH_TO_OBJECT(string->data+j,string->length-j):EMPTY_STRING_TO_OBJECT());
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_starts(const sll_string_t* string,const sll_string_t* prefix){
	if (string->length<prefix->length){
		return 0;
	}
	if (string->length==prefix->length){
		return sll_string_equal(string,prefix);
	}
	if (!prefix->length){
		return 1;
	}
	return _compare_data(string->data,prefix->data,prefix->length,1);
}



__SLL_EXTERNAL void sll_string_subtract_array(const sll_string_t* string,const sll_array_t* array,sll_array_t* out){
	if (string->length<array->length){
		if (!string->length){
			SLL_INIT_ARRAY(out);
			return;
		}
		out->length=array->length;
		out->data=sll_allocator_init(array->length*sizeof(sll_object_t*));
		sll_string_length_t i=0;
		for (;i<string->length;i++){
			out->data[i]=sll_operator_sub(sll_static_char[string->data[i]],array->data[i]);
		}
		while (i<array->length){
			out->data[i]=sll_operator_sub(sll_static_char[0],array->data[i]);
			i++;
		}
		return;
	}
	if (!array->length){
		SLL_INIT_ARRAY(out);
		return;
	}
	out->length=string->length;
	out->data=sll_allocator_init(string->length*sizeof(sll_object_t*));
	STRING_DATA_PTR(out->data);
	sll_array_length_t i=0;
	do{
		out->data[i]=sll_operator_sub(sll_static_char[string->data[i]],array->data[i]);
		i++;
	} while (i<array->length);
	while (i<string->length){
		out->data[i]=SLL_FROM_CHAR(string->data[i]);
		i++;
	}
}



__SLL_EXTERNAL void sll_string_title_case(const sll_string_t* string,sll_string_t* out){
	out->length=string->length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(string->length)*sizeof(sll_char_t));
	INIT_PADDING(out->data,string->length);
	const wide_data_t* p=(const wide_data_t*)(out->data);
	STRING_DATA_PTR(p);
	wide_data_t c=0;
	sll_char_t off=64;
	for (sll_string_length_t i=0;i<out->length;i++){
		sll_char_t e=string->data[i];
		if ((e&64)&&(0x07fffffe&(1u<<(e&31)))){
			out->data[i]=(e&31)|off;
			off=96;
		}
		else{
			out->data[i]=e;
			off=64;
		}
		if ((i&7)==7){
			c^=*p;
			p++;
		}
	}
	if ((out->length&7)<7){
		c^=*p;
	}
	out->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_to_array(const sll_string_t* string,sll_array_t* out){
	out->length=string->length;
	out->data=sll_allocator_init(string->length*sizeof(sll_object_t*));
	for (sll_string_length_t i=0;i<string->length;i++){
		out->data[i]=SLL_FROM_CHAR(string->data[i]);
	}
}



__SLL_EXTERNAL void sll_string_to_map(const sll_string_t* string,sll_map_t* out){
	if (!string->length){
		SLL_INIT_MAP(out);
		return;
	}
	out->length=string->length;
	sll_map_length_t e=string->length<<1;
	out->data=sll_allocate(e*sizeof(sll_object_t*));
	sll_string_length_t i=0;
	for (sll_map_length_t j=0;j<e;j+=2){
		out->data[j]=sll_int_to_object(i);
		out->data[j+1]=SLL_FROM_CHAR(string->data[i]);
		i++;
	}
}



__SLL_EXTERNAL void sll_string_trim(const sll_string_t* string,sll_string_t* out){
	SLL_INIT_STRING(out);
	if (!string->length){
		return;
	}
	sll_string_length_t i=sll_string_index_multiple(string,_string_whitespace,_string_whitespace_count,1);
	if (i==SLL_MAX_STRING_LENGTH){
		return;
	}
	sll_string_length_t j=sll_string_index_reverse_multiple(string,_string_whitespace,_string_whitespace_count,1);
	if (j==SLL_MAX_STRING_LENGTH){
		return;
	}
	sll_string_from_pointer_length(string->data+i,j-i+1,out);
}



__SLL_EXTERNAL void sll_string_trim_left(const sll_string_t* string,sll_string_t* out){
	SLL_INIT_STRING(out);
	if (!string->length){
		return;
	}
	sll_string_length_t i=sll_string_index_multiple(string,_string_whitespace,_string_whitespace_count,1);
	if (i==SLL_MAX_STRING_LENGTH){
		return;
	}
	sll_string_from_pointer_length(string->data+i,string->length-i,out);
}



__SLL_EXTERNAL void sll_string_trim_right(const sll_string_t* string,sll_string_t* out){
	SLL_INIT_STRING(out);
	if (!string->length){
		return;
	}
	sll_string_length_t i=sll_string_index_reverse_multiple(string,_string_whitespace,_string_whitespace_count,1);
	if (i==SLL_MAX_STRING_LENGTH){
		return;
	}
	i++;
	sll_string_from_pointer_length(string->data,i,out);
}



__SLL_EXTERNAL void sll_string_upper_case(const sll_string_t* string,sll_string_t* out){
	EXECUTE_CASE_SWITCH(0xfafafafafafafafaull,0x1f1f1f1f1f1f1f1full,SUB);
}



__SLL_EXTERNAL void sll_string_xor(const sll_string_t* a,const sll_string_t* b,sll_string_t* out){
	if (a->length<b->length){
		const sll_string_t* c=a;
		a=b;
		b=c;
	}
	out->length=a->length;
	out->data=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(a->length)*sizeof(sll_char_t));
	INIT_PADDING(out->data,out->length);
	const wide_data_t* ap=(const wide_data_t*)(a->data);
	const wide_data_t* bp=(const wide_data_t*)(b->data);
	wide_data_t* op=(wide_data_t*)(out->data);
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
	out->checksum=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_xor_char(const sll_string_t* string,sll_char_t char_,sll_string_t* out){
	EXECUTE_BINARY_OPERATOR_CHAR(^);
}

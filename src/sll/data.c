#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_compare_data(const void* a,const void* b,sll_size_t length){
	if (!length){
		return SLL_COMPARE_RESULT_EQUAL;
	}
	const sll_char_t* ap=(const sll_char_t*)a;
	const sll_char_t* bp=(const sll_char_t*)b;
	if (length<16){
		for (sll_size_t i=0;i<length;i++){
			if (*(ap+i)!=*(bp+i)){
				return (*(ap+i)<*(bp+i)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
			}
		}
		return SLL_COMPARE_RESULT_EQUAL;
	}
	if ((ADDR(ap)&7)&&(ADDR(bp)&7)){
		if ((ADDR(ap)&7)<(ADDR(bp)&7)){
			ap=(const sll_char_t*)b;
			bp=(const sll_char_t*)a;
		}
		sll_size_t i=8-(ADDR(ap)&7);
		length-=i;
		do{
			if (*ap!=*bp){
				return (*ap<*bp?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
			}
			ap++;
			bp++;
			i--;
		} while (i);
	}
	else if (ADDR(ap)&7){
		ap=(const sll_char_t*)b;
		bp=(const sll_char_t*)a;
	}
	const wide_data_t* ap64=(const wide_data_t*)ap;
	const wide_data_t* bp64=(const wide_data_t*)bp;
#ifndef __SLL_BUILD_DARWIN
	while (length>31){
		unsigned int v=~_mm256_movemask_epi8(_mm256_cmpeq_epi8(_mm256_lddqu_si256((const __m256i*)ap64),_mm256_lddqu_si256((const __m256i*)bp64)));
		if (v){
			sll_size_t i=FIND_FIRST_SET_BIT(v);
			return (*(((const sll_char_t*)ap64)+i)<*(((const sll_char_t*)bp64)+i)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
		length-=32;
		ap64+=4;
		bp64+=4;
	}
	if (length>15){
		unsigned int v=_mm_movemask_epi8(_mm_cmpeq_epi8(_mm_lddqu_si128((const __m128i*)ap64),_mm_lddqu_si128((const __m128i*)bp64)))^0xffff;
		if (v){
			sll_size_t i=FIND_FIRST_SET_BIT(v);
			return (*(((const sll_char_t*)ap64)+i)<*(((const sll_char_t*)bp64)+i)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
		length-=16;
		ap64+=2;
		bp64+=2;
	}
#endif
	ASSUME_ALIGNED(ap64,3,0);
	while (length>7){
		if (*ap64!=*bp64){
			return (SWAP_BYTES64(*ap64)<SWAP_BYTES64(*bp64)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
		ap64++;
		bp64++;
		length-=8;
	}
	ap=(const sll_char_t*)ap64;
	bp=(const sll_char_t*)bp64;
	while (length){
		length--;
		if (*ap!=*bp){
			return (*ap<*bp?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
		ap++;
		bp++;
	}
	return SLL_COMPARE_RESULT_EQUAL;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_contains_character(const void* pointer,sll_size_t length,sll_char_t char_){
	if (!length){
		return 0;
	}
	const sll_char_t* ptr=pointer;
	if (length<16){
		for (sll_size_t i=0;i<length;i++){
			if (*ptr==char_){
				return 1;
			}
			ptr++;
		}
		return 0;
	}
	if (ADDR(ptr)&7){
		sll_size_t i=8-(ADDR(ptr)&7);
		length-=i;
		do{
			if (*ptr==char_){
				return 1;
			}
			ptr++;
			i--;
		} while (i);
	}
	const wide_data_t* ptr64=(const wide_data_t*)ptr;
#ifndef __SLL_BUILD_DARWIN
	__m256i m=_mm256_set1_epi8(char_);
	__m256i n=_mm256_set1_epi64x(0x101010101010101ull);
	while (length>31){
		__m256i v=_mm256_xor_si256(_mm256_lddqu_si256((const __m256i*)ptr64),m);
		if (~_mm256_movemask_epi8(_mm256_andnot_si256(v,_mm256_sub_epi64(v,n)))){
			return 1;
		}
		length-=32;
		ptr64+=4;
	}
	if (length>15){
		__m128i v=_mm_xor_si128(_mm_lddqu_si128((const __m128i*)ptr64),_mm256_castsi256_si128(m));
		if (_mm_movemask_epi8(_mm_andnot_si128(v,_mm_sub_epi64(v,_mm256_castsi256_si128(n))))^0xffff){
			return 1;
		}
		length-=16;
		ptr64+=2;
	}
#endif
	ASSUME_ALIGNED(ptr64,3,0);
	if (length>7){
		wide_data_t m64=0x101010101010101ull*char_;
		do{
			wide_data_t v=(*ptr64)^m64;
			if ((v-0x101010101010101ull)&(~v)&0x8080808080808080ull){
				return 1;
			}
			ptr64++;
			length-=8;
		} while (length>7);
	}
	ptr=(const sll_char_t*)ptr64;
	while (length){
		length--;
		if (*ptr==char_){
			return 1;
		}
		ptr++;
	}
	return 0;
}



__SLL_EXTERNAL void sll_copy_data(const void* source,sll_size_t length,void* target){
	const sll_char_t* a=(const sll_char_t*)source;
	sll_char_t* b=(sll_char_t*)target;
	if (length<16){
		while (length){
			*b=*a;
			a++;
			b++;
			length--;
		}
		return;
	}
	if (ADDR(b)&7){
		sll_size_t i=8-(ADDR(b)&7);
		*((wide_data_t*)b)=*((const wide_data_t*)a);
		a+=i;
		b+=i;
		length-=i;
	}
	SLL_ASSERT(!(ADDR(b)&7));
#ifndef __SLL_BUILD_DARWIN
	while (length>31){
		_mm256_storeu_si256((__m256i*)b,_mm256_lddqu_si256((const __m256i*)a));
		length-=32;
		a+=32;
		b+=32;
	}
	if (length>15){
		_mm_storeu_si128((__m128i*)b,_mm_lddqu_si128((const __m128i*)a));
		length-=16;
		a+=16;
		b+=16;
	}
#endif
	const wide_data_t* ap=(const wide_data_t*)a;
	wide_data_t* bp=(wide_data_t*)b;
	ASSUME_ALIGNED(bp,3,0);
	while (length>7){
		*bp=*ap;
		ap++;
		bp++;
		length-=8;
	}
	a=(const sll_char_t*)ap;
	b=(sll_char_t*)bp;
	while (length){
		*b=*a;
		a++;
		b++;
		length--;
	}
}



__SLL_EXTERNAL void sll_copy_objects(const sll_object_t* source,sll_size_t count,sll_object_t* target){
	while (count){
		*target=*source;
		SLL_ACQUIRE(*target);
		source++;
		target++;
		count--;
	}
}



__SLL_EXTERNAL void* sll_copy_string(const sll_char_t* source,void* target){
	if (!(*source)){
		return target;
	}
	sll_char_t* o=(sll_char_t*)target;
	while (ADDR(o)&7){
		*o=*source;
		source++;
		o++;
		if (!(*source)){
			return o;
		}
	}
	const wide_data_t* sp=(const wide_data_t*)source;
	wide_data_t* op=(wide_data_t*)o;
	ASSUME_ALIGNED(op,3,0);
	while (1){
		wide_data_t v=((*sp)-0x101010101010101ull)&0x8080808080808080ull&(~(*sp));
		if (v){
			SLL_ASSERT(FIND_FIRST_SET_BIT(v)>6&&(FIND_FIRST_SET_BIT(v)&7)==7);
			v=FIND_FIRST_SET_BIT(v);
			o=PTR(ADDR(op)+(v>>3));
			if (v>7){
				v-=7;
				*op=((*op)&(0xffffffffffffffffull<<v))|((*sp)&((1ull<<v)-1));
			}
			return o;
		}
		*op=*sp;
		sp++;
		op++;
	}
}



__SLL_EXTERNAL void sll_copy_string_null(const sll_char_t* source,void* target){
	*((sll_char_t*)sll_copy_string(source,target))=0;
}



__SLL_EXTERNAL void sll_set_memory(void* pointer,sll_size_t length,sll_char_t value){
	sll_char_t* o=(sll_char_t*)pointer;
	if (length<16){
		while (length){
			*o=value;
			o++;
			length--;
		}
		return;
	}
	if (ADDR(o)&7){
		sll_size_t i=8-(ADDR(o)&7);
		length-=i;
		do{
			*o=value;
			o++;
			i--;
		} while (i);
	}
	SLL_ASSERT(!(ADDR(o)&7));
	wide_data_t* op=(wide_data_t*)o;
	ASSUME_ALIGNED(op,3,0);
	sll_size_t i=0;
	wide_data_t v64=0x101010101010101ull*value;
	for (;i<(length>>3);i++){
		*(op+i)=v64;
	}
	if (length&7){
		length=(length&7)<<3;
		*(op+i)=((*(op+i))&(0xffffffffffffffffull<<length))|(v64&((1ull<<length)-1));
	}
}



__SLL_EXTERNAL void sll_zero_memory(void* pointer,sll_size_t length){
	sll_char_t* o=(sll_char_t*)pointer;
	if (length<16){
		while (length){
			*o=0;
			o++;
			length--;
		}
		return;
	}
	if (ADDR(o)&7){
		sll_size_t i=8-(ADDR(o)&7);
		length-=i;
		do{
			*o=0;
			o++;
			i--;
		} while (i);
	}
	SLL_ASSERT(!(ADDR(o)&7));
	wide_data_t* op=(wide_data_t*)o;
	ASSUME_ALIGNED(op,3,0);
	sll_size_t i=0;
	for (;i<(length>>3);i++){
		*(op+i)=0;
	}
	if (length&7){
		*(op+i)&=0xffffffffffffffffull<<((length&7)<<3);
	}
}

#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_compare_data(const void* a,const void* b,sll_size_t l){
	if (!l){
		return SLL_COMPARE_RESULT_EQUAL;
	}
	const sll_char_t* ap=(const sll_char_t*)a;
	const sll_char_t* bp=(const sll_char_t*)b;
	if (l<16){
		for (sll_size_t i=0;i<l;i++){
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
		SLL_UNIMPLEMENTED();
	}
	else if (ADDR(ap)&7){
		ap=(const sll_char_t*)b;
		bp=(const sll_char_t*)a;
	}
	const wide_data_t* ap64=(const wide_data_t*)ap;
	const wide_data_t* bp64=(const wide_data_t*)bp;
	ASSUME_ALIGNED(ap64,3,0);
	do{
		if (*ap64!=*bp64){
			wide_data_t av=ROTATE_BITS64(*ap64,32);
			wide_data_t bv=ROTATE_BITS64(*bp64,32);
			av=((av&0xffff0000ffffull)<<16)|((av&0xffff0000ffff0000ull)>>16);
			bv=((bv&0xffff0000ffffull)<<16)|((bv&0xffff0000ffff0000ull)>>16);
			return ((((av&0xff00ff00ff00ffull)<<8)|((av&0xff00ff00ff00ff00ull)>>8))<(((bv&0xff00ff00ff00ffull)<<8)|((bv&0xff00ff00ff00ff00ull)>>8))?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
		ap64++;
		bp64++;
		l-=8;
	} while (l>7);
	ap=(const sll_char_t*)ap64;
	bp=(const sll_char_t*)bp64;
	while (l){
		l--;
		if (*ap!=*bp){
			return (*ap<*bp?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
		ap++;
		bp++;
	}
	return SLL_COMPARE_RESULT_EQUAL;
}



__SLL_EXTERNAL void sll_copy_data(const void* s,sll_size_t l,void* d){
	const sll_char_t* a=(const sll_char_t*)s;
	sll_char_t* b=(sll_char_t*)d;
	if (l<16){
		while (l){
			*b=*a;
			a++;
			b++;
			l--;
		}
		return;
	}
	if (ADDR(d)&7){
		sll_size_t i=8-(ADDR(d)&7);
		a+=i;
		b+=i;
		l-=i;
		do{
			*(b-i)=*(a-i);
			i--;
		} while (i);
	}
	SLL_ASSERT(!(ADDR(b)&7));
	const wide_data_t* ap=(const wide_data_t*)a;
	wide_data_t* bp=(wide_data_t*)b;
	ASSUME_ALIGNED(bp,3,0);
	sll_size_t i=0;
	for (;i<(l>>3);i++){
		*(bp+i)=*(ap+i);
	}
	if (l&7){
		l=(l&7)<<3;
		*(bp+i)=((*(bp+i))&(0xffffffffffffffffull<<l))|((*(ap+i))&((1ull<<l)-1));
	}
}



__SLL_EXTERNAL void* sll_copy_string(const sll_char_t* s,void* d){
	if (!(*s)){
		return d;
	}
	sll_char_t* o=(sll_char_t*)d;
	while (ADDR(o)&7){
		*o=*s;
		s++;
		o++;
		if (!(*s)){
			return o;
		}
	}
	SLL_ASSERT(!(ADDR(o)&7));
	const wide_data_t* sp=(const wide_data_t*)s;
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



__SLL_EXTERNAL void sll_copy_string_null(const sll_char_t* s,void* d){
	*((sll_char_t*)sll_copy_string(s,d))=0;
}



__SLL_EXTERNAL void sll_set_memory(void* p,sll_size_t l,sll_char_t v){
	sll_char_t* o=(sll_char_t*)p;
	if (l<16){
		while (l){
			*o=v;
			o++;
			l--;
		}
		return;
	}
	if (ADDR(o)&7){
		sll_size_t i=8-(ADDR(o)&7);
		l-=i;
		do{
			*o=v;
			o++;
			i--;
		} while (i);
	}
	SLL_ASSERT(!(ADDR(o)&7));
	wide_data_t* op=(wide_data_t*)o;
	ASSUME_ALIGNED(op,3,0);
	sll_size_t i=0;
	wide_data_t v64=0x101010101010101ull*v;
	for (;i<(l>>3);i++){
		*(op+i)=v64;
	}
	if (l&7){
		l=(l&7)<<3;
		*(op+i)=((*(op+i))&(0xffffffffffffffffull<<l))|(v64&((1ull<<l)-1));
	}
}



__SLL_EXTERNAL void sll_zero_memory(void* p,sll_size_t l){
	sll_char_t* o=(sll_char_t*)p;
	if (l<16){
		while (l){
			*o=0;
			o++;
			l--;
		}
		return;
	}
	if (ADDR(o)&7){
		sll_size_t i=8-(ADDR(o)&7);
		l-=i;
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
	for (;i<(l>>3);i++){
		*(op+i)=0;
	}
	if (l&7){
		*(op+i)&=0xffffffffffffffffull<<((l&7)<<3);
	}
}

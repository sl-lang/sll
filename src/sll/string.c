#include <sll/_internal/gc.h>
#include <sll/_internal/intrinsics.h>
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



#define INIT_PADDING(v,l) (*((wide_data_t*)((v)+((l)&0xfffffffffffffff8ull)))=0)

#define STRING_DATA_PTR(x) ASSUME_ALIGNED(x,4,0)

#define EXECUTE_CASE_SWITCH(x1,x2) \
	if (!s->l){ \
		SLL_INIT_STRING(o); \
		return; \
	} \
	o->l=s->l; \
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t)); \
	INIT_PADDING(o->v,o->l); \
	const wide_data_t* a=(const wide_data_t*)(s->v); \
	wide_data_t* b=(wide_data_t*)(o->v); \
	STRING_DATA_PTR(a); \
	STRING_DATA_PTR(b); \
	wide_data_t c=0; \
	for (sll_string_length_t i=0;i<((o->l+7)>>3);i++){ \
		*(b+i)=(*(a+i))-((((x1-((*(a+i))&0x7f7f7f7f7f7f7f7full))&(~(*(a+i)))&(((*(a+i))&0x7f7f7f7f7f7f7f7full)+x2))&0x8080808080808080ull)>>2); \
		c^=*(b+i); \
	} \
	o->c=(sll_string_length_t)(c^(c>>32));
#define EXECUTE_BINARY_OPERATOR_CHAR(operator) \
	if (!s->l){ \
		SLL_INIT_STRING(o); \
		return; \
	} \
	if (!v){ \
		sll_string_clone(s,o); \
		return; \
	} \
	o->l=s->l; \
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t)); \
	const wide_data_t* a=(const wide_data_t*)(s->v); \
	wide_data_t* b=(wide_data_t*)(o->v); \
	STRING_DATA_PTR(a); \
	STRING_DATA_PTR(b); \
	wide_data_t v64=0x101010101010101ull*v; \
	wide_data_t c=0; \
	sll_string_length_t l=s->l; \
	while (l>7){ \
		*b=(*a) operator v64; \
		c^=*b; \
		a++; \
		b++; \
		l-=8; \
	} \
	*b=((*a) operator v64)&((1ull<<(l<<3))-1); \
	c^=*b; \
	o->c=(sll_string_length_t)(c^(c>>32));



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



__SLL_EXTERNAL void sll_free_string(sll_string_t* s){
	s->l=0;
	s->c=0;
	sll_allocator_release(s->v);
	s->v=NULL;
}



__SLL_EXTERNAL void sll_string_and(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (a->l<b->l){
		const sll_string_t* c=a;
		a=b;
		b=c;
	}
	o->l=b->l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(b->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,b->l);
	const wide_data_t* ap=(const wide_data_t*)(a->v);
	const wide_data_t* bp=(const wide_data_t*)(b->v);
	wide_data_t* op=(wide_data_t*)(o->v);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	STRING_DATA_PTR(op);
	sll_string_length_t i=0;
	wide_data_t c=0;
	for (;i<((b->l+7)>>3);i++){
		*(op+i)=(*(ap+i))&(*(bp+i));
		c^=*(op+i);
	}
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_and_char(const sll_string_t* s,sll_char_t v,sll_string_t* o){
	if (!s->l){
		SLL_INIT_STRING(o);
		return;
	}
	if (v==255){
		sll_string_clone(s,o);
		return;
	}
	o->l=s->l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	wide_data_t* b=(wide_data_t*)(o->v);
	STRING_DATA_PTR(b);
	if (!v){
		for (sll_string_length_t i=0;i<((s->l+7)>>3);i++){
			*b=0;
			b++;
		}
		return;
	}
	const wide_data_t* a=(const wide_data_t*)(s->v);
	STRING_DATA_PTR(a);
	wide_data_t v64=0x101010101010101ull*v;
	wide_data_t c=0;
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



__SLL_EXTERNAL void sll_string_calculate_checksum(sll_string_t* s){
	const wide_data_t* p=(const wide_data_t*)(s->v);
	STRING_DATA_PTR(p);
	wide_data_t c=0;
	for (sll_string_length_t i=0;i<((s->l+7)>>3);i++){
		c^=*(p+i);
	}
	s->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_clone(const sll_string_t* s,sll_string_t* d){
	if (!s->l){
		SLL_INIT_STRING(d);
		return;
	}
	d->l=s->l;
	d->c=s->c;
	d->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)(s->v);
	wide_data_t* b=(wide_data_t*)(d->v);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	for (sll_string_length_t i=0;i<=(s->l>>3);i++){
		*(b+i)=*(a+i);
	}
}



__SLL_EXTERNAL void sll_string_combinations(const sll_string_t* a,const sll_string_t* b,sll_array_t* o){
	if (!a->l||!b->l){
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=a->l*b->l;
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	sll_array_length_t i=0;
	for (sll_string_length_t j=0;j<a->l;j++){
		for (sll_string_length_t k=0;k<b->l;k++){
			sll_char_t bf[2]={
				a->v[j],
				b->v[k]
			};
			o->v[i]=sll_string_pointer_to_object(bf,2);
			i++;
		}
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_checksum_t sll_string_combine_checksums(sll_string_checksum_t a,sll_string_length_t l,sll_string_checksum_t b){
	return a^ROTATE_BITS(b,(l&3)<<3);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare(const sll_string_t* a,const sll_string_t* b){
	const wide_data_t* ap=(const wide_data_t*)(a->v);
	const wide_data_t* bp=(const wide_data_t*)(b->v);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	for (sll_string_length_t i=0;i<(((a->l<b->l?a->l:b->l)+7)<<3);i++){
		wide_data_t av=*(ap+i);
		wide_data_t bv=*(bp+i);
		if (av!=bv){
			return (SWAP_BYTES64(av)<SWAP_BYTES64(bv)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
	}
	return (a->l==b->l?SLL_COMPARE_RESULT_EQUAL:(a->l<b->l?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_string_compare_array(const sll_string_t* s,const sll_array_t* a,sll_bool_t inv){
	for (sll_array_length_t i=0;i<(s->l<a->l?s->l:a->l);i++){
		sll_compare_result_t cmp=sll_operator_compare(sll_static_char[s->v[i]],a->v[i]);
		if (cmp!=SLL_COMPARE_RESULT_EQUAL){
			return (((cmp==SLL_COMPARE_RESULT_ABOVE)^inv)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE);
		}
	}
	return (s->l==a->l?SLL_COMPARE_RESULT_EQUAL:(((s->l<a->l)^inv)?SLL_COMPARE_RESULT_BELOW:SLL_COMPARE_RESULT_ABOVE));
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
	if (!a->l){
		if (!b->l){
			SLL_INIT_STRING(o);
		}
		else{
			sll_string_clone(b,o);
		}
		return;
	}
	if (!b->l){
		sll_string_clone(a,o);
		return;
	}
	o->l=a->l+b->l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,o->l);
	const wide_data_t* ap=(const wide_data_t*)(a->v);
	wide_data_t* op=(wide_data_t*)(o->v);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(op);
	sll_string_length_t i=0;
	for (;i<((a->l+7)>>3);i++){
		*(op+i)=*(ap+i);
	}
	const sll_char_t* s=b->v;
	if (a->l&7){
		sll_string_length_t j=8-(a->l&7);
		s+=j;
		do{
			j--;
			o->v[a->l+j]=b->v[j];
		} while (j);
	}
	if (!(a->l&7)||8-(a->l&7)<b->l){
		sll_string_length_t bl=b->l-(a->l&7?8-(a->l&7):0);
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
			o->v[j+k]=*(s+k);
		}
	}
	o->c=a->c^ROTATE_BITS(b->c,(a->l&3)<<3);
}



__SLL_EXTERNAL void sll_string_concat_char(const sll_string_t* s,sll_char_t c,sll_string_t* o){
	o->l=s->l+1;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,o->l);
	const wide_data_t* a=(const wide_data_t*)(s->v);
	wide_data_t* b=(wide_data_t*)(o->v);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	for (sll_string_length_t i=0;i<=(s->l>>3);i++){
		*(b+i)=*(a+i);
	}
	o->v[s->l]=c;
	o->c=s->c^(((sll_string_checksum_t)c)<<((s->l&3)<<3));
}



__SLL_EXTERNAL void sll_string_concat_chars(sll_char_t a,sll_char_t b,sll_string_t* o){
	o->l=2;
	o->c=a|(b<<8);
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(2)*sizeof(sll_char_t));
	STRING_DATA_PTR(o->v);
	*((wide_data_t*)(o->v))=o->c;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count(const sll_string_t* a,const sll_string_t* b){
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
		if (sll_compare_data(a->v+i,b->v,b->l)==SLL_COMPARE_RESULT_EQUAL){
			o++;
		}
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_char(const sll_string_t* s,sll_char_t c){
	if (!s->l){
		return 0;
	}
	sll_string_length_t o=0;
	const wide_data_t* p=(const wide_data_t*)(s->v);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*c;
	for (sll_string_length_t i=0;i<(s->l+7)>>3;i++){
		wide_data_t v=(*(p+i))^m;
		o+=(sll_string_length_t)POPULATION_COUNT((v-0x101010101010101ull)&0x8080808080808080ull&(~v));
	}
	if (!c){
		return o-((s->l&7)?8-(s->l&7):0);
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_left(const sll_string_t* s,sll_char_t c){
	if (!s->l||s->v[0]!=c){
		return 0;
	}
	const wide_data_t* p=(const wide_data_t*)(s->v);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*c;
	for (sll_string_length_t i=0;i<((s->l+7)>>3);i++){
		wide_data_t v=(*p)^m;
		v=((~(v-0x101010101010101ull))|v)&0x8080808080808080ull;
		if (v){
			sll_string_length_t o=(i<<3)+(FIND_FIRST_SET_BIT(v)>>3);
			return (!c&&o>=s->l?s->l:o);
		}
		p++;
	}
	return s->l;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_count_right(const sll_string_t* s,sll_char_t c){
	if (!s->l||s->v[s->l-1]!=c){
		return 0;
	}
	if (!c){
		SLL_UNIMPLEMENTED();
	}
	sll_string_length_t l=(s->l+7)>>3;
	const wide_data_t* p=((const wide_data_t*)(s->v))+l;
	wide_data_t m=0x101010101010101ull*c;
	wide_data_t n=m<<(((s->l&7))<<3);
	for (sll_string_length_t i=0;i<l;i++){
		p--;
		wide_data_t v=((*p)|n)^m;
		v=((~(v-0x101010101010101ull))|v)&0x8080808080808080ull;
		if (v){
			return (i<<3)+(s->l&7)-(FIND_LAST_SET_BIT(v)>>3)-1;
		}
		n=0;
	}
	return s->l;
}



__SLL_EXTERNAL void sll_string_create(sll_string_length_t l,sll_string_t* o){
	o->l=l;
	o->c=0;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	wide_data_t* p=(wide_data_t*)(o->v);
	STRING_DATA_PTR(p);
	for (sll_string_length_t i=0;i<SLL_STRING_ALIGN_LENGTH(l)>>3;i++){
		*(p+i)=0;
	}
}



__SLL_EXTERNAL void sll_string_decrease(sll_string_t* s,sll_string_length_t l){
	if (l!=s->l){
		s->l=l;
		sll_allocator_resize((void**)(&(s->v)),SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
		SLL_STRING_FORMAT_PADDING(s->v,s->l);
	}
}



__SLL_EXTERNAL void sll_string_duplicate(const sll_string_t* s,sll_integer_t n,sll_string_length_t e,sll_string_t* o){
	if (!s->l){
		SLL_INIT_STRING(o);
		return;
	}
	SLL_ASSERT(e<s->l);
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
		o->l=e;
		o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(e)*sizeof(sll_char_t));
		const wide_data_t* a=(const wide_data_t*)(s->v);
		wide_data_t* b=(wide_data_t*)(o->v);
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
		o->c=(sll_string_length_t)(c^(c>>32));
		return;
	}
	n*=s->l;
	SLL_ASSERT(n<SLL_MAX_STRING_LENGTH);
	o->l=((sll_string_length_t)n)+e;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	wide_data_t* op=(wide_data_t*)(o->v);
	STRING_DATA_PTR(op);
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
		const wide_data_t* ap=(const wide_data_t*)(s->v);
		STRING_DATA_PTR(ap);
		for (sll_string_length_t i=0;i<((s->l+7)>>3);i++){
			*(op+i)=*(ap+i);
		}
	}
	sll_string_length_t i=s->l;
	if (s->l&7){
		sll_string_length_t j=(o->l<16?o->l:(s->l+15)&0xfffffff0);
		while (i<j){
			o->v[i]=o->v[i-s->l];
			i++;
		}
	}
	n+=e;
	wide_data_t c=0;
	if (i<n){
		const wide_data_t* ap=(const wide_data_t*)(o->v+(s->l&7?8-(s->l&7):0));
		i>>=3;
		for (sll_string_length_t j=0;j<i;j++){
			c^=*(op+j);
		}
		n=(n+7)>>3;
		if (s->l&7){
			n--;
		}
		sll_string_length_t j=0;
		while (i<n){
			*(op+i)=*(ap+j);
			c^=*(op+i);
			i++;
			j++;
		}
		*(op+i)=(*(ap+j))&((1ull<<((o->l&7)<<3))-1);
		c^=*(op+i);
	}
	else{
		SLL_STRING_FORMAT_PADDING(o->v,o->l);
		for (sll_string_length_t j=0;j<(i>>3);j++){
			c^=*(op+j);
		}
	}
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_ends(const sll_string_t* a,const sll_string_t* b){
	if (a->l<b->l){
		return 0;
	}
	if (a->l==b->l){
		return sll_string_equal(a,b);
	}
	return _compare_data(a->v+a->l-b->l,b->v,b->l,0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal(const sll_string_t* a,const sll_string_t* b){
	if (a->l!=b->l||a->c!=b->c){
		return 0;
	}
	if (!a->l){
		return 1;
	}
	const wide_data_t* ap=(const wide_data_t*)(a->v);
	const wide_data_t* bp=(const wide_data_t*)(b->v);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	for (sll_string_length_t i=0;i<((a->l+7)>>3);i++){
		if (*(ap+i)!=*(bp+i)){
			return 0;
		}
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_array(const sll_string_t* s,const sll_array_t* a){
	if (s->l!=a->l){
		return 0;
	}
	for (sll_string_length_t i=0;i<s->l;i++){
		sll_object_t* e=a->v[i];
		if ((e->t==SLL_OBJECT_TYPE_CHAR&&e->dt.c==s->v[i])||(e->t==SLL_OBJECT_TYPE_INT&&e->dt.i==s->v[i])||(e->t==SLL_OBJECT_TYPE_STRING&&e->dt.s.l==1&&e->dt.s.v[0]==s->v[i])){
			continue;
		}
		return 0;
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_equal_map(const sll_string_t* s,const sll_map_t* m){
	if (s->l!=m->l){
		return 0;
	}
	for (sll_map_length_t i=0;i<(m->l<<1);i+=2){
		sll_object_t* e=m->v[i];
		if (e->t!=SLL_OBJECT_TYPE_INT||e->dt.i<0||e->dt.i>=s->l){
			return 0;
		}
		sll_char_t c=s->v[e->dt.i];
		e=m->v[i+1];
		if ((e->t==SLL_OBJECT_TYPE_CHAR&&e->dt.c==c)||(e->t==SLL_OBJECT_TYPE_INT&&e->dt.i==c)||(e->t==SLL_OBJECT_TYPE_STRING&&e->dt.s.l==1&&e->dt.s.v[0]==c)){
			continue;
		}
		return 0;
	}
	return 1;
}



__SLL_EXTERNAL void sll_string_flip_case(const sll_string_t* s,sll_string_t* o){
	o->l=s->l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,o->l);
	const wide_data_t* a=(const wide_data_t*)(s->v);
	wide_data_t* b=(wide_data_t*)(o->v);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	wide_data_t c=0;
	for (sll_string_length_t i=0;i<((o->l+7)>>3);i++){
		*(b+i)=(*(a+i))^((((((*(a+i))&0xc0c0c0c0c0c0c0c0ull)^0x4040404040404040ull)-0x101010101010101ull)&(0x9a9a9a9a9a9a9a9aull-((*(a+i))&0x1f1f1f1f1f1f1f1full))&(((*(a+i))&0x1f1f1f1f1f1f1f1full)+0x7f7f7f7f7f7f7f7full)&(~(*(a+i)))&0x8080808080808080ull)>>2);
		c^=*(b+i);
	}
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_from_char(sll_char_t c,sll_string_t* o){
	o->l=1;
	o->c=c;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
	STRING_DATA_PTR(o->v);
	*((wide_data_t*)(o->v))=o->c;
}



__SLL_EXTERNAL void sll_string_from_data(sll_object_t** v,sll_string_length_t vl,sll_string_t* o){
	o->l=vl;
	o->c=0;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(vl)*sizeof(sll_char_t));
	INIT_PADDING(o->v,vl);
	for (sll_string_length_t i=0;i<vl;i++){
		sll_object_t* n=sll_operator_cast(*(v+i),sll_static_int[SLL_OBJECT_TYPE_CHAR]);
		SLL_ASSERT(n->t==SLL_OBJECT_TYPE_CHAR);
		o->v[i]=n->dt.c;
		o->c^=ROTATE_BITS(n->dt.c,(i&3)<<3);
		GC_RELEASE(n);
	}
}



__SLL_EXTERNAL void sll_string_from_int(sll_integer_t v,sll_string_t* o){
	if (!v){
		o->l=1;
		o->c='0';
		o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
		STRING_DATA_PTR(o->v);
		*((wide_data_t*)(o->v))='0';
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
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,o->l);
	if (n){
		o->v[0]='-';
	}
	for (sll_string_length_t j=0;j<20-i;j++){
		o->v[j+n]=bf[i+j];
	}
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_from_pointer(const sll_char_t* s,sll_string_t* o){
	if (!s){
		SLL_INIT_STRING(o);
	}
	else{
		sll_string_from_pointer_length(s,sll_string_length_unaligned(s),o);
	}
}



__SLL_EXTERNAL void sll_string_from_pointer_length(const sll_char_t* s,sll_string_length_t l,sll_string_t* o){
	if (!l){
		SLL_INIT_STRING(o);
		return;
	}
	o->l=l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)s;
	wide_data_t* b=(wide_data_t*)(o->v);
	STRING_DATA_PTR(b);
	wide_data_t c=0;
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_char_t sll_string_get(const sll_string_t* s,sll_string_length_t i){
	return (i<s->l?s->v[i]:0);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_includes(const sll_string_t* a,const sll_string_t* b){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_includes_char(const sll_string_t* s,sll_char_t c){
	if (!s->l){
		return 0;
	}
	const wide_data_t* p=(const wide_data_t*)(s->v);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*c;
	for (sll_string_length_t i=0;i<((s->l+7)>>3);i++){
		wide_data_t v=(*p)^m;
		v=(v-0x101010101010101ull)&0x8080808080808080ull&(~v);
		if (v){
			return (c?1:((i<<3)+(FIND_FIRST_SET_BIT(v)>>3)<s->l));
		}
		p++;
	}
	return 0;
}



__SLL_EXTERNAL void sll_string_increase(sll_string_t* s,sll_string_length_t l){
	if (!l){
		return;
	}
	l+=s->l;
	if (SLL_STRING_ALIGN_LENGTH(l)>SLL_STRING_ALIGN_LENGTH(s->l)){
		sll_allocator_resize((void**)(&(s->v)),SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
		SLL_STRING_FORMAT_PADDING(s->v,l);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index(const sll_string_t* a,const sll_string_t* b,sll_string_length_t si){
	if (!b->l){
		return 0;
	}
	if (!a->l||b->l>a->l){
		return SLL_MAX_STRING_LENGTH;
	}
	if (b->l==1){
		return sll_string_index_char(a,b->v[0],0,si);
	}
	if (a->l==b->l){
		return (sll_string_equal(a,b)?0:SLL_MAX_STRING_LENGTH);
	}
	for (;si<a->l-b->l;si++){
		if (sll_compare_data(a->v+si,b->v,b->l)==SLL_COMPARE_RESULT_EQUAL){
			return si;
		}
	}
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_char(const sll_string_t* s,sll_char_t c,sll_bool_t inv,sll_string_length_t si){
	if (!s->l){
		return SLL_MAX_STRING_LENGTH;
	}
	if (si){
		SLL_UNIMPLEMENTED();
	}
	const wide_data_t* p=(const wide_data_t*)(s->v);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*c;
	for (sll_string_length_t i=0;i<((s->l+7)>>3);i++){
		wide_data_t v=(*p)^m;
		v=(v-0x101010101010101ull)&(~v);
		if (inv){
			v=~v;
		}
		v&=0x8080808080808080ull;
		if (v){
			sll_string_length_t o=(i<<3)+(FIND_FIRST_SET_BIT(v)>>3);
			return (!c&&o>=s->l?SLL_MAX_STRING_LENGTH:o);
		}
		p++;
	}
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_multiple(const sll_string_t* s,const sll_char_t* cl,sll_string_length_t cll,sll_bool_t inv){
	if (!s->l||!cll){
		return SLL_MAX_STRING_LENGTH;
	}
	const wide_data_t* p=(const wide_data_t*)(s->v);
	wide_data_t* ml=sll_allocate_stack(cll*sizeof(wide_data_t));
	STRING_DATA_PTR(p);
	for (sll_string_length_t i=0;i<cll;i++){
		*(ml+i)=0x101010101010101ull*(*(cl+i));
	}
	for (sll_string_length_t i=0;i<((s->l+7)>>3);i++){
		wide_data_t k=*p;
		wide_data_t v=0;
		for (sll_string_length_t j=0;j<cll;j++){
			wide_data_t e=k^(*(ml+j));
			v|=(e-0x101010101010101ull)&(~e);
		}
		if (inv){
			v=~v;
		}
		v&=0x8080808080808080ull;
		if (v){
			sll_string_length_t o=(i<<3)+(FIND_FIRST_SET_BIT(v)>>3);
			return (o>=s->l?SLL_MAX_STRING_LENGTH:o);
		}
		p++;
	}
	sll_deallocate(ml);
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse(const sll_string_t* a,const sll_string_t* b){
	if (!b->l){
		return 0;
	}
	if (!a->l){
		return SLL_MAX_STRING_LENGTH;
	}
	if (b->l==1){
		return sll_string_index_reverse_char(a,b->v[0],0);
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse_char(const sll_string_t* s,sll_char_t c,sll_bool_t inv){
	if (!s->l){
		return SLL_MAX_STRING_LENGTH;
	}
	sll_string_length_t l=(s->l+7)>>3;
	const wide_data_t* p=((const wide_data_t*)(s->v))+l;
	wide_data_t m=0x101010101010101ull*c;
	for (sll_string_length_t i=0;i<l;i++){
		p--;
		wide_data_t v=(*p)^m;
		v=(v-0x101010101010101ull)&(~v);
		if (inv){
			v=~v;
		}
		v&=0x8080808080808080ull;
		if (v){
			if (!i&&(s->l&7)){
				v&=0xffffffffffffffffull>>((8-s->l)<<3);
				if (!v){
					continue;
				}
			}
			sll_string_length_t o=((l-i-1)<<3)+(FIND_LAST_SET_BIT(v)>>3);
			return (!c&&o>=s->l?SLL_MAX_STRING_LENGTH:o);
		}
	}
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_index_reverse_multiple(const sll_string_t* s,const sll_char_t* cl,sll_string_length_t cll,sll_bool_t inv){
	if (!s->l||!cll){
		return SLL_MAX_STRING_LENGTH;
	}
	sll_string_length_t l=(s->l+7)>>3;
	const wide_data_t* p=((const wide_data_t*)(s->v))+l;
	ASSUME_ALIGNED(p,3,0);
	wide_data_t* ml=sll_allocate_stack(cll*sizeof(wide_data_t));
	for (sll_string_length_t i=0;i<cll;i++){
		*(ml+i)=0x101010101010101ull*(*(cl+i));
	}
	for (sll_string_length_t i=0;i<l;i++){
		p--;
		wide_data_t k=*p;
		wide_data_t v=0;
		for (sll_string_length_t j=0;j<cll;j++){
			wide_data_t e=k^(*(ml+j));
			v|=(e-0x101010101010101ull)&(~e);
		}
		if (inv){
			v=~v;
		}
		v&=0x8080808080808080ull;
		if (v){
			if (!i&&(s->l&7)){
				v&=0xffffffffffffffffull>>((8-s->l)<<3);
				if (!v){
					continue;
				}
			}
			sll_string_length_t o=((l-i-1)<<3)+(FIND_LAST_SET_BIT(v)>>3);
			return (o>=s->l?SLL_MAX_STRING_LENGTH:o);
		}
	}
	sll_deallocate(ml);
	return SLL_MAX_STRING_LENGTH;
}



__SLL_EXTERNAL sll_string_length_t sll_string_insert_pointer(const sll_char_t* s,sll_string_length_t i,sll_string_t* o){
	return sll_string_insert_pointer_length(s,sll_string_length_unaligned(s),i,o);
}



__SLL_EXTERNAL sll_string_length_t sll_string_insert_pointer_length(const sll_char_t* s,sll_string_length_t l,sll_string_length_t i,sll_string_t* o){
	if (i+l>o->l){
		l=o->l-i;
	}
	if (l<16){
		STRING_DATA_PTR(o->v);
		while (l){
			o->v[i]=*s;
			l--;
			i++;
			s++;
		}
		return i;
	}
	if (i&7){
		STRING_DATA_PTR(o->v);
		sll_string_length_t j=8-(i&7);
		i+=j;
		l-=j;
		s+=j;
		do{
			o->v[i-j]=*(s-j);
			j--;
		} while (j);
	}
	SLL_ASSERT(!(i&7));
	const wide_data_t* ap=(const wide_data_t*)s;
	STRING_DATA_PTR(o->v);
	wide_data_t* op=(wide_data_t*)(o->v+i);
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
	if (!s->l){
		SLL_INIT_STRING(o);
		return;
	}
	o->l=s->l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)(s->v);
	wide_data_t* b=(wide_data_t*)(o->v);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	wide_data_t c=0;
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



__SLL_EXTERNAL void sll_string_join(const sll_string_t* s,sll_object_t*const* a,sll_array_length_t al,sll_string_t* o){
	sll_string_create(0,o);
	sll_allocator_move((void**)(&(o->v)),SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
	for (sll_array_length_t i=0;i<al;i++){
		if (i){
			sll_string_increase(o,s->l);
			sll_copy_data(s->v,s->l,o->v+o->l);
			o->l+=s->l;
		}
		sll_object_t* n=sll_operator_cast(*(a+i),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_string_increase(o,n->dt.s.l);
		sll_copy_data(n->dt.s.v,n->dt.s.l,o->v+o->l);
		o->l+=n->dt.s.l;
		GC_RELEASE(n);
	}
	sll_allocator_move((void**)(&(o->v)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_join_char(sll_char_t c,sll_object_t*const* a,sll_array_length_t al,sll_string_t* o){
	sll_string_create(0,o);
	sll_allocator_move((void**)(&(o->v)),SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
	for (sll_array_length_t i=0;i<al;i++){
		if (i){
			sll_string_increase(o,1);
			o->v[o->l]=c;
			o->l++;
		}
		sll_object_t* n=sll_operator_cast(*(a+i),sll_static_int[SLL_OBJECT_TYPE_STRING]);
		sll_string_increase(o,n->dt.s.l);
		sll_copy_data(n->dt.s.v,n->dt.s.l,o->v+o->l);
		o->l+=n->dt.s.l;
		GC_RELEASE(n);
	}
	sll_allocator_move((void**)(&(o->v)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_length(const sll_char_t* s){
	const wide_data_t* p=(const wide_data_t*)s;
	ASSUME_ALIGNED(p,3,0);
	while (1){
		wide_data_t v=((*p)-0x101010101010101ull)&0x8080808080808080ull&(~(*p));
		if (v){
			return (sll_string_length_t)(ADDR(p)-ADDR(s)+(FIND_FIRST_SET_BIT(v)>>3));
		}
		p++;
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_string_length_unaligned(const sll_char_t* s){
	addr_t o=ADDR(s);
	while (ADDR(s)&7){
		if (!(*s)){
			return (sll_string_length_t)(ADDR(s)-o);
		}
		s++;
	}
	return sll_string_length(s)+(sll_string_length_t)(ADDR(s)-o);
}



__SLL_EXTERNAL void sll_string_lower_case(const sll_string_t* s,sll_string_t* o){
	EXECUTE_CASE_SWITCH(0xdadadadadadadadaull,0x3f3f3f3f3f3f3f3full);
}



__SLL_EXTERNAL void sll_string_op(const sll_string_t* a,const sll_string_t* b,sll_binary_operator_t f,sll_string_t* o){
	sll_string_length_t e=a->l;
	o->l=b->l;
	if (b->l<a->l){
		e=b->l;
		o->l=a->l;
	}
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,o->l);
	for (sll_string_length_t i=0;i<e;i++){
		sll_object_t* v=f(sll_static_char[a->v[i]],sll_static_char[b->v[i]]);
		sll_object_t* c=sll_operator_cast(v,sll_static_int[SLL_OBJECT_TYPE_CHAR]);
		GC_RELEASE(v);
		SLL_ASSERT(c->t==SLL_OBJECT_TYPE_CHAR);
		o->v[i]=c->dt.c;
		GC_RELEASE(c);
	}
	if (a->l==b->l){
		return;
	}
	const sll_char_t* s=(a->l>b->l?a->v:b->v)+e;
	do{
		o->v[e]=s[e];
		e++;
	} while (e<o->l);
}



__SLL_EXTERNAL void sll_string_op_array(const sll_string_t* s,const sll_array_t* a,sll_binary_operator_t f,sll_bool_t inv,sll_array_t* o){
	sll_array_length_t e=a->l;
	o->l=s->l;
	if (s->l<a->l){
		e=s->l;
		o->l=a->l;
	}
	o->v=sll_allocator_init(o->l*sizeof(sll_object_t*));
	for (sll_array_length_t i=0;i<e;i++){
		o->v[i]=(inv?f(a->v[i],sll_static_char[s->v[i]]):f(sll_static_char[s->v[i]],a->v[i]));
	}
	if (s->l==a->l){
		return;
	}
	if (s->l>a->l){
		do{
			o->v[e]=SLL_FROM_CHAR(s->v[e]);
			e++;
		} while (e<o->l);
	}
	else{
		do{
			o->v[e]=a->v[e];
			SLL_ACQUIRE(o->v[e]);
			e++;
		} while (e<o->l);
	}
}



__SLL_EXTERNAL void sll_string_op_map(const sll_string_t* s,const sll_map_t* m,sll_binary_operator_t f,sll_bool_t inv,sll_map_t* o){
	o->l=s->l+m->l;
	o->v=sll_allocate((s->l<<1)*sizeof(sll_object_t*));
	sll_string_length_t sml=(s->l+63)>>6;
	bitmap_t* sm=sll_zero_allocate_stack(sml*sizeof(bitmap_t));
	sll_map_length_t i=0;
	for (sll_map_length_t j=0;j<m->l;j++){
		SLL_ACQUIRE(m->v[j<<1]);
		o->v[i]=m->v[j<<1];
		sll_integer_t idx=m->v[j<<1]->dt.i;
		if (m->v[j<<1]->t==SLL_OBJECT_TYPE_INT&&idx>=0&&idx<s->l){
			*(sm+(idx>>6))|=1ull<<(idx&63);
			o->v[i+1]=(inv?f(m->v[(j<<1)+1],sll_static_char[s->v[idx]]):f(sll_static_char[s->v[idx]],m->v[(j<<1)+1]));
		}
		else{
			o->v[i+1]=m->v[(j<<1)+1];
			SLL_ACQUIRE(m->v[(j<<1)+1]);
		}
		i+=2;
	}
	for (sll_string_length_t j=0;j<sml;j++){
		bitmap_t v=~(*(sm+j));
		while (v){
			sll_string_length_t k=FIND_FIRST_SET_BIT(v)|(j<<6);
			if (k>=s->l){
				break;
			}
			v&=v-1;
			o->v[i]=sll_int_to_object(k);
			o->v[i+1]=SLL_FROM_CHAR(s->v[k]);
			i+=2;
		}
	}
	sll_deallocate(sm);
	if ((i>>1)!=o->l){
		o->l=i>>1;
		o->v=sll_reallocate(o->v,i*sizeof(sll_object_t*));
	}
}



__SLL_EXTERNAL void sll_string_or(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (a->l<b->l){
		const sll_string_t* c=a;
		a=b;
		b=c;
	}
	o->l=a->l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(a->l)*sizeof(sll_char_t));
	const wide_data_t* ap=(const wide_data_t*)(a->v);
	const wide_data_t* bp=(const wide_data_t*)(b->v);
	wide_data_t* op=(wide_data_t*)(o->v);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	STRING_DATA_PTR(op);
	wide_data_t c=0;
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



__SLL_EXTERNAL void sll_string_or_char(const sll_string_t* s,sll_char_t v,sll_string_t* o){
	EXECUTE_BINARY_OPERATOR_CHAR(|);
}



__SLL_EXTERNAL void sll_string_pad(const sll_string_t* s,sll_string_length_t l,sll_char_t c,sll_string_t* o){
	if (s->l>=l){
		sll_string_clone(s,o);
		return;
	}
	o->l=l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	sll_string_length_t i=(l-s->l)>>1;
	sll_set_memory(o->v,i,c);
	sll_copy_data(s->v,s->l,o->v+i);
	sll_set_memory(o->v+i+s->l,l-i-s->l,c);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_pad_left(const sll_string_t* s,sll_string_length_t l,sll_char_t c,sll_string_t* o){
	if (s->l>=l){
		sll_string_clone(s,o);
		return;
	}
	o->l=l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	sll_copy_data(s->v,s->l,o->v);
	sll_set_memory(o->v+s->l,l-s->l,c);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_pad_right(const sll_string_t* s,sll_string_length_t l,sll_char_t c,sll_string_t* o){
	if (s->l>=l){
		sll_string_clone(s,o);
		return;
	}
	o->l=l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(l)*sizeof(sll_char_t));
	sll_set_memory(o->v,l-s->l,c);
	sll_copy_data(s->v,s->l,o->v+l-s->l);
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
	o->l=s->l+1;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,o->l);
	const wide_data_t* a=(const wide_data_t*)(s->v+7);
	ASSUME_ALIGNED(a,4,15);
	wide_data_t* b=(wide_data_t*)(o->v);
	STRING_DATA_PTR(b);
	*b=c|((*((wide_data_t*)(s->v)))<<8);
	b++;
	for (sll_string_length_t i=0;i<(s->l>>3);i++){
		*(b+i)=*(a+i);
	}
	o->c=c^ROTATE_BITS(s->c,8);
}



__SLL_EXTERNAL void sll_string_remove(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (!a->l){
		SLL_INIT_STRING(o);
		return;
	}
	o->l=a->l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(a->l)*sizeof(sll_char_t));
	sll_string_length_t i=0;
	sll_string_length_t j=0;
	while (j<a->l-b->l+1){
		if (sll_compare_data(a->v+j,b->v,b->l)!=SLL_COMPARE_RESULT_EQUAL){
			o->v[i]=a->v[j];
			i++;
		}
		else{
			j+=b->l-1;
		}
		j++;
	}
	sll_copy_data(a->v+a->l-b->l+1,b->l-1,o->v+i);
	o->l=i+b->l-1;
	if (!o->l){
		sll_allocator_release(o->v);
		o->v=NULL;
		return;
	}
	sll_allocator_resize((void**)(&(o->v)),SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_replace(const sll_string_t* s,const sll_string_t* k,const sll_string_t* v,sll_string_t* o){
	if (!v->l){
		sll_string_remove(s,k,o);
		return;
	}
	if (k->l==1&&v->l==1){
		sll_string_replace_char(s,k->v[0],v->v[0],o);
		return;
	}
	o->l=s->l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	sll_string_length_t i=0;
	if (v->l<=k->l){
		sll_string_length_t j=0;
		while (j<s->l-k->l+1){
			if (sll_compare_data(s->v+j,k->v,k->l)!=SLL_COMPARE_RESULT_EQUAL){
				o->v[i]=s->v[j];
				i++;
				j++;
			}
			else{
				for (sll_string_length_t e=0;e<v->l;e++){
					o->v[i+e]=v->v[e];
				}
				i+=v->l;
				j+=k->l;
			}
		}
	}
	else{
		sll_string_length_t j=0;
		while (j<s->l-k->l+1){
			if (sll_compare_data(s->v+j,k->v,k->l)!=SLL_COMPARE_RESULT_EQUAL){
				o->v[i]=s->v[j];
				i++;
				j++;
			}
			else{
				o->l+=v->l-k->l;
				sll_allocator_resize((void**)(&(o->v)),SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
				for (sll_string_length_t e=0;e<v->l;e++){
					o->v[i+e]=v->v[e];
				}
				i+=v->l;
				j+=k->l;
			}
		}
	}
	const sll_char_t* t=s->v+s->l-k->l+1;
	for (sll_string_length_t j=0;j<k->l-1;j++){
		o->v[i+j]=t[j];
	}
	i+=k->l-1;
	if (i!=o->l){
		SLL_ASSERT(i<o->l);
		o->l=i;
		if (!o->l){
			sll_deallocate(o->v);
			o->v=NULL;
			return;
		}
		sll_allocator_resize((void**)(&(o->v)),SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	}
	SLL_STRING_FORMAT_PADDING(o->v,o->l);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_replace_char(const sll_string_t* s,sll_char_t k,sll_char_t v,sll_string_t* o){
	if (!s->l){
		SLL_INIT_STRING(o);
		return;
	}
	if (k==v){
		sll_string_clone(s,o);
		return;
	}
	o->l=s->l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)(s->v);
	wide_data_t* b=(wide_data_t*)(o->v);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
	wide_data_t k64=0x101010101010101ull*k;
	wide_data_t v64=0x101010101010101ull*(k^v);
	wide_data_t c=0;
	sll_string_length_t l=s->l;
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
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_resize(const sll_string_t* s,sll_integer_t v,sll_string_t* o){
	if (!v){
		sll_string_clone(s,o);
		return;
	}
	if (v<0){
		v=-v;
		if (v>=s->l){
			SLL_INIT_STRING(o);
			return;
		}
		o->l=(sll_string_length_t)(s->l-v);
		o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
		INIT_PADDING(o->v,o->l);
		sll_copy_data(s->v+v,o->l,o->v);
		sll_string_calculate_checksum(o);
		return;
	}
	SLL_ASSERT(s->l+v<=SLL_MAX_STRING_LENGTH);
	o->l=(sll_string_length_t)(s->l+v);
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,o->l);
	wide_data_t* p=(wide_data_t*)(o->v);
	for (sll_string_length_t i=0;i<((v+7)>>3);i++){
		*p=0;
		p++;
	}
	sll_copy_data(s->v,s->l,o->v+v);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_reverse(const sll_string_t* s,sll_string_t* o){
	if (!s->l){
		SLL_INIT_STRING(o);
		return;
	}
	o->l=s->l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	sll_string_length_t i=s->l;
	for (sll_string_length_t j=0;j<s->l;j++){
		i--;
		o->v[j]=s->v[i];
	}
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_secure_equal(const sll_string_t* a,const sll_string_t* b){
	if (a->l<b->l){
		const sll_string_t* t=a;
		a=b;
		b=t;
	}
	const wide_data_t* ap=(const wide_data_t*)(a->v);
	const wide_data_t* bp=(const wide_data_t*)(b->v);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	wide_data_t o=0;
	sll_string_length_t i=0;
	for (;i<((b->l+7)>>3);i++){
		o|=(*(ap+i))^(*(bp+i));
	}
	return (a->l==b->l&&!o);
}



__SLL_EXTERNAL void sll_string_select(const sll_string_t* s,sll_integer_t a,sll_integer_t b,sll_integer_t c,sll_string_t* o){
	WRAP_ARRAY_INDEX_VAR(a,s->l);
	WRAP_ARRAY_INDEX_VAR(b,s->l);
	if (!s->l||a==b||!c||(a<=b&&a>=s->l)||(b<=a&&b>=s->l)){
		SLL_INIT_STRING(o);
		return;
	}
	if (a<b){
		if (c<0){
			sll_string_select(s,0,a,1,o);
			return;
		}
		if (a+c>=s->l){
			o->l=1;
			o->c=s->v[a];
			o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
			*((wide_data_t*)(o->v))=s->c;
			return;
		}
		o->l=(sll_string_length_t)((b-a-1)/c+1);
		o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
		INIT_PADDING(o->v,o->l);
		sll_string_length_t i=0;
		do{
			o->v[i]=s->v[a];
			i++;
			a+=c;
		} while (a<b);
		SLL_ASSERT(i==o->l);
	}
	else{
		if (c>0){
			sll_string_select(s,a,s->l-1,1,o);
			return;
		}
		c=-c;
		if (c>a){
			o->l=1;
			o->c=s->v[a];
			o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(1)*sizeof(sll_char_t));
			*((wide_data_t*)(o->v))=s->c;
			return;
		}
		o->l=(sll_string_length_t)((a-b-1)/c+1);
		o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(o->l)*sizeof(sll_char_t));
		INIT_PADDING(o->v,o->l);
		sll_string_length_t i=0;
		do{
			o->v[i]=s->v[a];
			i++;
			a-=c;
		} while (a>b);
		SLL_ASSERT(i==o->l);
	}
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_set_char(sll_char_t c,sll_string_length_t i,sll_string_t* o){
	if (i<o->l){
		o->c^=((sll_string_checksum_t)(o->v[i]^c))<<((i&3)<<3);
		o->v[i]=c;
	}
}



__SLL_EXTERNAL void sll_string_shift(const sll_string_t* s,sll_integer_t v,sll_string_t* o){
	if (!s->l){
		SLL_INIT_STRING(o);
		return;
	}
	o->l=s->l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	const wide_data_t* a=(const wide_data_t*)(s->v);
	wide_data_t* b=(wide_data_t*)(o->v);
	STRING_DATA_PTR(a);
	STRING_DATA_PTR(b);
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
	o->c=(sll_string_length_t)(c^(c>>32));
}



__SLL_EXTERNAL void sll_string_split(const sll_string_t* s,const sll_string_t* p,sll_array_t* o){
	if (!s->l){
		o->l=1;
		o->v=sll_allocator_init(sizeof(sll_object_t*));
		o->v[0]=sll_string_to_object(NULL);
		return;
	}
	if (!p->l){
		sll_string_to_array(s,o);
		return;
	}
	if (p->l==1){
		sll_string_split_char(s,p->v[0],o);
		return;
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL void sll_string_split_char(const sll_string_t* s,sll_char_t c,sll_array_t* o){
	if (!s->l){
		o->l=1;
		o->v=sll_allocator_init(sizeof(sll_object_t*));
		o->v[0]=sll_string_to_object(NULL);
		return;
	}
	o->l=0;
	o->v=NULL;
	const wide_data_t* p=(const wide_data_t*)(s->v);
	STRING_DATA_PTR(p);
	wide_data_t m=0x101010101010101ull*c;
	sll_array_length_t i=0;
	sll_string_length_t j=0;
	for (sll_string_length_t k=0;k<(s->l+7)>>3;k++){
		wide_data_t v=(*(p+k))^m;
		v=(v-0x101010101010101ull)&0x8080808080808080ull&(~v);
		while (v){
			sll_string_length_t l=(k<<3)|(FIND_FIRST_SET_BIT(v)>>3);
			o->l++;
			sll_allocator_resize((void**)(&(o->v)),o->l*sizeof(sll_object_t*));
			o->v[i]=(j<l?sll_string_pointer_to_object(s->v+j,l-j):sll_string_to_object(NULL));
			i++;
			j=l+1;
			v&=v-1;
		}
	}
	o->l++;
	sll_allocator_resize((void**)(&(o->v)),o->l*sizeof(sll_object_t*));
	o->v[i]=(j<s->l?sll_string_pointer_to_object(s->v+j,s->l-j):sll_string_to_object(NULL));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_starts(const sll_string_t* a,const sll_string_t* b){
	if (a->l<b->l){
		return 0;
	}
	if (a->l==b->l){
		return sll_string_equal(a,b);
	}
	return _compare_data(a->v,b->v,b->l,1);
}



__SLL_EXTERNAL void sll_string_subtract_array(const sll_string_t* s,const sll_array_t* a,sll_array_t* o){
	if (s->l<a->l){
		if (!s->l){
			SLL_INIT_ARRAY(o);
			return;
		}
		o->l=a->l;
		o->v=sll_allocator_init(a->l*sizeof(sll_object_t*));
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
		SLL_INIT_ARRAY(o);
		return;
	}
	o->l=s->l;
	o->v=sll_allocator_init(s->l*sizeof(sll_object_t*));
	STRING_DATA_PTR(o->v);
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



__SLL_EXTERNAL void sll_string_title_case(const sll_string_t* s,sll_string_t* o){
	o->l=s->l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(s->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,s->l);
	const wide_data_t* p=(const wide_data_t*)(o->v);
	STRING_DATA_PTR(p);
	wide_data_t c=0;
	sll_char_t off=64;
	for (sll_string_length_t i=0;i<o->l;i++){
		sll_char_t e=s->v[i];
		if ((e&64)&&(0x07fffffe&(1u<<(e&31)))){
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



__SLL_EXTERNAL void sll_string_to_array(const sll_string_t* s,sll_array_t* o){
	o->l=s->l;
	o->v=sll_allocator_init(s->l*sizeof(sll_object_t*));
	for (sll_string_length_t i=0;i<s->l;i++){
		o->v[i]=SLL_FROM_CHAR(s->v[i]);
	}
}



__SLL_EXTERNAL void sll_string_to_map(const sll_string_t* s,sll_map_t* o){
	if (!s->l){
		SLL_INIT_MAP(o);
		return;
	}
	o->l=s->l;
	sll_map_length_t e=s->l<<1;
	o->v=sll_allocate(e*sizeof(sll_object_t*));
	sll_string_length_t i=0;
	for (sll_map_length_t j=0;j<e;j+=2){
		o->v[j]=sll_int_to_object(i);
		o->v[j+1]=SLL_FROM_CHAR(s->v[i]);
		i++;
	}
}



__SLL_EXTERNAL void sll_string_trim(const sll_string_t* s,sll_string_t* o){
	SLL_INIT_STRING(o);
	if (!s->l){
		return;
	}
	sll_string_length_t i=sll_string_index_multiple(s,_string_whitespace,_string_whitespace_count,1);
	sll_string_length_t j=sll_string_index_reverse_multiple(s,_string_whitespace,_string_whitespace_count,1);
	if (i==SLL_MAX_STRING_LENGTH&&j==SLL_MAX_STRING_LENGTH){
		return;
	}
	j++;
	sll_string_create(j-i,o);
	sll_copy_data(s->v+i,j-i,o->v);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_trim_left(const sll_string_t* s,sll_string_t* o){
	SLL_INIT_STRING(o);
	if (!s->l){
		return;
	}
	sll_string_length_t i=sll_string_index_multiple(s,_string_whitespace,_string_whitespace_count,1);
	if (i==SLL_MAX_STRING_LENGTH){
		return;
	}
	sll_string_create(s->l-i,o);
	sll_copy_data(s->v+i,o->l,o->v);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_trim_right(const sll_string_t* s,sll_string_t* o){
	SLL_INIT_STRING(o);
	if (!s->l){
		return;
	}
	sll_string_length_t i=sll_string_index_reverse_multiple(s,_string_whitespace,_string_whitespace_count,1);
	if (i==SLL_MAX_STRING_LENGTH){
		return;
	}
	i++;
	sll_string_create(i,o);
	sll_copy_data(s->v,i,o->v);
	sll_string_calculate_checksum(o);
}



__SLL_EXTERNAL void sll_string_upper_case(const sll_string_t* s,sll_string_t* o){
	EXECUTE_CASE_SWITCH(0xfafafafafafafafaull,0x1f1f1f1f1f1f1f1full);
}



__SLL_EXTERNAL void sll_string_xor(const sll_string_t* a,const sll_string_t* b,sll_string_t* o){
	if (a->l<b->l){
		const sll_string_t* c=a;
		a=b;
		b=c;
	}
	o->l=a->l;
	o->v=sll_allocator_init(SLL_STRING_ALIGN_LENGTH(a->l)*sizeof(sll_char_t));
	INIT_PADDING(o->v,o->l);
	const wide_data_t* ap=(const wide_data_t*)(a->v);
	const wide_data_t* bp=(const wide_data_t*)(b->v);
	wide_data_t* op=(wide_data_t*)(o->v);
	STRING_DATA_PTR(ap);
	STRING_DATA_PTR(bp);
	STRING_DATA_PTR(op);
	wide_data_t c=0;
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



__SLL_EXTERNAL void sll_string_xor_char(const sll_string_t* s,sll_char_t v,sll_string_t* o){
	EXECUTE_BINARY_OPERATOR_CHAR(^);
}

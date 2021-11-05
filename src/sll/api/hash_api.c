#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/runtime_object.h>
#include <sll/types.h>
#include <sll/util.h>
#include <string.h>
#include <stdint.h>



#define APPEND_SMALL(p,sz,st) \
	do{ \
		if ((st)->l+(sz)>63){ \
			sll_copy_data((p),64-(st)->l,(st)->bf+(st)->l); \
			(st)->l=0; \
			_process_chunk((st),(st)->bf); \
		} \
		sll_copy_data((p),(sz),(st)->bf+(st)->l); \
		(st)->l+=(sz); \
	} while (0)
#define MD5_STAGE0(a,b,c,d,i,r,k) a=b+ROTATE_BITS(((b&c)|((~b)&d))+a+k+dt[i],r)
#define MD5_STAGE1(a,b,c,d,i,r,k) a=b+ROTATE_BITS(((b&d)|(c&(~d)))+a+k+dt[(5*i+1)&15],r)
#define MD5_STAGE2(a,b,c,d,i,r,k) a=b+ROTATE_BITS((b^c^d)+a+k+dt[(3*i+5)&15],r)
#define MD5_STAGE3(a,b,c,d,i,r,k) a=b+ROTATE_BITS((c^(b|(~d)))+a+k+dt[(7*i)&15],r)



typedef struct __HASH_STATE{
	uint64_t sz;
	uint8_t bf[64];
	uint8_t l;
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
} hash_state_t;



static void _process_chunk(hash_state_t* st,const uint8_t* bf){
	st->sz++;
	const uint32_t* dt=(const uint32_t*)bf;
	uint32_t a=st->a;
	uint32_t b=st->b;
	uint32_t c=st->c;
	uint32_t d=st->d;
	MD5_STAGE0(a,b,c,d,0,7,0xd76aa478);
	MD5_STAGE0(d,a,b,c,1,12,0xe8c7b756);
	MD5_STAGE0(c,d,a,b,2,17,0x242070db);
	MD5_STAGE0(b,c,d,a,3,22,0xc1bdceee);
	MD5_STAGE0(a,b,c,d,4,7,0xf57c0faf);
	MD5_STAGE0(d,a,b,c,5,12,0x4787c62a);
	MD5_STAGE0(c,d,a,b,6,17,0xa8304613);
	MD5_STAGE0(b,c,d,a,7,22,0xfd469501);
	MD5_STAGE0(a,b,c,d,8,7,0x698098d8);
	MD5_STAGE0(d,a,b,c,9,12,0x8b44f7af);
	MD5_STAGE0(c,d,a,b,10,17,0xffff5bb1);
	MD5_STAGE0(b,c,d,a,11,22,0x895cd7be);
	MD5_STAGE0(a,b,c,d,12,7,0x6b901122);
	MD5_STAGE0(d,a,b,c,13,12,0xfd987193);
	MD5_STAGE0(c,d,a,b,14,17,0xa679438e);
	MD5_STAGE0(b,c,d,a,15,22,0x49b40821);
	MD5_STAGE1(a,b,c,d,16,5,0xf61e2562);
	MD5_STAGE1(d,a,b,c,17,9,0xc040b340);
	MD5_STAGE1(c,d,a,b,18,14,0x265e5a51);
	MD5_STAGE1(b,c,d,a,19,20,0xe9b6c7aa);
	MD5_STAGE1(a,b,c,d,20,5,0xd62f105d);
	MD5_STAGE1(d,a,b,c,21,9,0x02441453);
	MD5_STAGE1(c,d,a,b,22,14,0xd8a1e681);
	MD5_STAGE1(b,c,d,a,23,20,0xe7d3fbc8);
	MD5_STAGE1(a,b,c,d,24,5,0x21e1cde6);
	MD5_STAGE1(d,a,b,c,25,9,0xc33707d6);
	MD5_STAGE1(c,d,a,b,26,14,0xf4d50d87);
	MD5_STAGE1(b,c,d,a,27,20,0x455a14ed);
	MD5_STAGE1(a,b,c,d,28,5,0xa9e3e905);
	MD5_STAGE1(d,a,b,c,29,9,0xfcefa3f8);
	MD5_STAGE1(c,d,a,b,30,14,0x676f02d9);
	MD5_STAGE1(b,c,d,a,31,20,0x8d2a4c8a);
	MD5_STAGE2(a,b,c,d,32,4,0xfffa3942);
	MD5_STAGE2(d,a,b,c,33,11,0x8771f681);
	MD5_STAGE2(c,d,a,b,34,16,0x6d9d6122);
	MD5_STAGE2(b,c,d,a,35,23,0xfde5380c);
	MD5_STAGE2(a,b,c,d,36,4,0xa4beea44);
	MD5_STAGE2(d,a,b,c,37,11,0x4bdecfa9);
	MD5_STAGE2(c,d,a,b,38,16,0xf6bb4b60);
	MD5_STAGE2(b,c,d,a,39,23,0xbebfbc70);
	MD5_STAGE2(a,b,c,d,40,4,0x289b7ec6);
	MD5_STAGE2(d,a,b,c,41,11,0xeaa127fa);
	MD5_STAGE2(c,d,a,b,42,16,0xd4ef3085);
	MD5_STAGE2(b,c,d,a,43,23,0x04881d05);
	MD5_STAGE2(a,b,c,d,44,4,0xd9d4d039);
	MD5_STAGE2(d,a,b,c,45,11,0xe6db99e5);
	MD5_STAGE2(c,d,a,b,46,16,0x1fa27cf8);
	MD5_STAGE2(b,c,d,a,47,23,0xc4ac5665);
	MD5_STAGE3(a,b,c,d,48,6,0xf4292244);
	MD5_STAGE3(d,a,b,c,49,10,0x432aff97);
	MD5_STAGE3(c,d,a,b,50,15,0xab9423a7);
	MD5_STAGE3(b,c,d,a,51,21,0xfc93a039);
	MD5_STAGE3(a,b,c,d,52,6,0x655b59c3);
	MD5_STAGE3(d,a,b,c,53,10,0x8f0ccc92);
	MD5_STAGE3(c,d,a,b,54,15,0xffeff47d);
	MD5_STAGE3(b,c,d,a,55,21,0x85845dd1);
	MD5_STAGE3(a,b,c,d,56,6,0x6fa87e4f);
	MD5_STAGE3(d,a,b,c,57,10,0xfe2ce6e0);
	MD5_STAGE3(c,d,a,b,58,15,0xa3014314);
	MD5_STAGE3(b,c,d,a,59,21,0x4e0811a1);
	MD5_STAGE3(a,b,c,d,60,6,0xf7537e82);
	MD5_STAGE3(d,a,b,c,61,10,0xbd3af235);
	MD5_STAGE3(c,d,a,b,62,15,0x2ad7d2bb);
	MD5_STAGE3(b,c,d,a,63,21,0xeb86d391);
	st->a+=a;
	st->b+=b;
	st->c+=c;
	st->d+=d;
}



static void _hash_object(const sll_runtime_object_t* o,hash_state_t* st){
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(o)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			APPEND_SMALL(&(o->dt.i),sizeof(sll_integer_t),st);
			return;
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			APPEND_SMALL(&(o->dt.f),sizeof(sll_float_t),st);
			return;
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			APPEND_SMALL(&(o->dt.c),sizeof(sll_char_t),st);
			return;
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			{
				sll_string_length_t l=o->dt.s.l;
				sll_char_t* p=o->dt.s.v;
				APPEND_SMALL(&l,sizeof(sll_string_length_t),st);
				if (!l){
					return;
				}
				if (st->l){
					if (l+st->l<64){
						sll_copy_data(p,l,st->bf+st->l);
						st->l+=l;
						return;
					}
					sll_copy_data(p,64-st->l,st->bf+st->l);
					p+=64-st->l;
					_process_chunk(st,st->bf);
				}
				while (l>63){
					_process_chunk(st,p);
					p+=64;
					l-=64;
				}
				sll_copy_data(p,l,st->bf);
				st->l=l;
				return;
			}
		case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
			APPEND_SMALL(&(o->dt.a.l),sizeof(sll_array_length_t),st);
			for (sll_array_length_t i=0;i<o->dt.a.l;i++){
				_hash_object(o->dt.a.v[i],st);
			}
			return;
		case SLL_RUNTIME_OBJECT_TYPE_HANDLE:
			APPEND_SMALL(&(o->dt.h.t),sizeof(sll_handle_type_t),st);
			APPEND_SMALL(&(o->dt.h.h),sizeof(sll_handle_t),st);
			return;
		case SLL_RUNTIME_OBJECT_TYPE_MAP:
			SLL_UNIMPLEMENTED();
		default:
			SLL_UNREACHABLE();
	}
}



__API_FUNC(hash_create){
	hash_state_t st={
		0,
		.l=0,
		.a=0x67452301,
		.b=0xefcdab89,
		.c=0x98badcfe,
		.d=0x10325476
	};
	_hash_object(a,&st);
	uint64_t sz=(st.sz<<6)+st.l;
	st.bf[st.l]=0x80;
	st.l++;
	if (st.l>55){
		memset(st.bf+st.l,0,64-st.l);
		_process_chunk(&st,st.bf);
		st.l=0;
	}
	memset(st.bf+st.l,0,56-st.l);
	uint8_t bf[8]={(sz<<3)&0xff,(sz>>5)&0xff,(sz>>13)&0xff,(sz>>21)&0xff,(sz>>29)&0xff,(sz>>37)&0xff,(sz>>45)&0xff,(sz>>53)&0xff};
	sll_copy_data(bf,8,st.bf+56);
	_process_chunk(&st,st.bf);
	return (sll_integer_t)((((uint64_t)(st.a^st.b))<<32)|(st.c^st.d));
}

#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>



#define MD5_STAGE0(b,c,d) (d^(b&(c^d)))
#define MD5_STAGE1(b,c,d) (c^(d&(b^c)))
#define MD5_STAGE2(b,c,d) (b^c^d)
#define MD5_STAGE3(b,c,d) (c^(b|(~d)))

#define MD5_STAGE(a,b,c,d,t,s,v) a=ROTATE_BITS(a+MD5_STAGE##t(b,c,d)+v,s)+b



#define SHA1_STAGE0(b,c,d) (d^(b&(c^d)))+0x5a827999
#define SHA1_STAGE1(b,c,d) (b^c^d)+0x6ed9eba1
#define SHA1_STAGE2(b,c,d) ((b&c)|(b&d)|(c&d))+0x8f1bbcdc
#define SHA1_STAGE3(b,c,d) (b^c^d)+0xca62c1d6

#define SHA1_STAGE(t,i) \
	if (i>3){ \
		w[i*5]=ROTATE_BITS(w[i*5-3]^w[i*5-8]^w[i*5-14]^w[i*5-16],1); \
		w[i*5+1]=ROTATE_BITS(w[i*5-2]^w[i*5-7]^w[i*5-13]^w[i*5-15],1); \
		w[i*5+2]=ROTATE_BITS(w[i*5-1]^w[i*5-6]^w[i*5-12]^w[i*5-14],1); \
		w[i*5+3]=ROTATE_BITS(w[i*5]^w[i*5-5]^w[i*5-11]^w[i*5-13],1); \
		w[i*5+4]=ROTATE_BITS(w[i*5+1]^w[i*5-4]^w[i*5-10]^w[i*5-12],1); \
	} \
	de=ROTATE_BITS(da,5)+SHA1_STAGE##t(db,dc,dd)+de+w[i*5]; \
	db=ROTATE_BITS(db,30); \
	dd=ROTATE_BITS(de,5)+SHA1_STAGE##t(da,db,dc)+dd+w[i*5+1]; \
	da=ROTATE_BITS(da,30); \
	dc=ROTATE_BITS(dd,5)+SHA1_STAGE##t(de,da,db)+dc+w[i*5+2]; \
	de=ROTATE_BITS(de,30); \
	db=ROTATE_BITS(dc,5)+SHA1_STAGE##t(dd,de,da)+db+w[i*5+3]; \
	dd=ROTATE_BITS(dd,30); \
	da=ROTATE_BITS(db,5)+SHA1_STAGE##t(dc,dd,de)+da+w[i*5+4]; \
	dc=ROTATE_BITS(dc,30);



__API_FUNC(hash_md5){
	uint32_t av=(uint32_t)a;
	uint32_t bv=(uint32_t)b;
	uint32_t cv=(uint32_t)c;
	uint32_t dv=(uint32_t)d;
	SLL_ASSERT(!(e->l&63));
	const uint32_t* ptr=(const uint32_t*)(e->v);
	for (sll_string_length_t i=0;i<(e->l>>6);i++){
		uint32_t da=av;
		uint32_t db=bv;
		uint32_t dc=cv;
		uint32_t dd=dv;
		MD5_STAGE(da,db,dc,dd,0,7,(*ptr)+0xd76aa478);
		MD5_STAGE(dd,da,db,dc,0,12,(*(ptr+1))+0xe8c7b756);
		MD5_STAGE(dc,dd,da,db,0,17,(*(ptr+2))+0x242070db);
		MD5_STAGE(db,dc,dd,da,0,22,(*(ptr+3))+0xc1bdceee);
		MD5_STAGE(da,db,dc,dd,0,7,(*(ptr+4))+0xf57c0faf);
		MD5_STAGE(dd,da,db,dc,0,12,(*(ptr+5))+0x4787c62a);
		MD5_STAGE(dc,dd,da,db,0,17,(*(ptr+6))+0xa8304613);
		MD5_STAGE(db,dc,dd,da,0,22,(*(ptr+7))+0xfd469501);
		MD5_STAGE(da,db,dc,dd,0,7,(*(ptr+8))+0x698098d8);
		MD5_STAGE(dd,da,db,dc,0,12,(*(ptr+9))+0x8b44f7af);
		MD5_STAGE(dc,dd,da,db,0,17,(*(ptr+10))+0xffff5bb1);
		MD5_STAGE(db,dc,dd,da,0,22,(*(ptr+11))+0x895cd7be);
		MD5_STAGE(da,db,dc,dd,0,7,(*(ptr+12))+0x6b901122);
		MD5_STAGE(dd,da,db,dc,0,12,(*(ptr+13))+0xfd987193);
		MD5_STAGE(dc,dd,da,db,0,17,(*(ptr+14))+0xa679438e);
		MD5_STAGE(db,dc,dd,da,0,22,(*(ptr+15))+0x49b40821);
		MD5_STAGE(da,db,dc,dd,1,5,(*(ptr+1))+0xf61e2562);
		MD5_STAGE(dd,da,db,dc,1,9,(*(ptr+6))+0xc040b340);
		MD5_STAGE(dc,dd,da,db,1,14,(*(ptr+11))+0x265e5a51);
		MD5_STAGE(db,dc,dd,da,1,20,(*ptr)+0xe9b6c7aa);
		MD5_STAGE(da,db,dc,dd,1,5,(*(ptr+5))+0xd62f105d);
		MD5_STAGE(dd,da,db,dc,1,9,(*(ptr+10))+0x02441453);
		MD5_STAGE(dc,dd,da,db,1,14,(*(ptr+15))+0xd8a1e681);
		MD5_STAGE(db,dc,dd,da,1,20,(*(ptr+4))+0xe7d3fbc8);
		MD5_STAGE(da,db,dc,dd,1,5,(*(ptr+9))+0x21e1cde6);
		MD5_STAGE(dd,da,db,dc,1,9,(*(ptr+14))+0xc33707d6);
		MD5_STAGE(dc,dd,da,db,1,14,(*(ptr+3))+0xf4d50d87);
		MD5_STAGE(db,dc,dd,da,1,20,(*(ptr+8))+0x455a14ed);
		MD5_STAGE(da,db,dc,dd,1,5,(*(ptr+13))+0xa9e3e905);
		MD5_STAGE(dd,da,db,dc,1,9,(*(ptr+2))+0xfcefa3f8);
		MD5_STAGE(dc,dd,da,db,1,14,(*(ptr+7))+0x676f02d9);
		MD5_STAGE(db,dc,dd,da,1,20,(*(ptr+12))+0x8d2a4c8a);
		MD5_STAGE(da,db,dc,dd,2,4,(*(ptr+5))+0xfffa3942);
		MD5_STAGE(dd,da,db,dc,2,11,(*(ptr+8))+0x8771f681);
		MD5_STAGE(dc,dd,da,db,2,16,(*(ptr+11))+0x6d9d6122);
		MD5_STAGE(db,dc,dd,da,2,23,(*(ptr+14))+0xfde5380c);
		MD5_STAGE(da,db,dc,dd,2,4,(*(ptr+1))+0xa4beea44);
		MD5_STAGE(dd,da,db,dc,2,11,(*(ptr+4))+0x4bdecfa9);
		MD5_STAGE(dc,dd,da,db,2,16,(*(ptr+7))+0xf6bb4b60);
		MD5_STAGE(db,dc,dd,da,2,23,(*(ptr+10))+0xbebfbc70);
		MD5_STAGE(da,db,dc,dd,2,4,(*(ptr+13))+0x289b7ec6);
		MD5_STAGE(dd,da,db,dc,2,11,(*ptr)+0xeaa127fa);
		MD5_STAGE(dc,dd,da,db,2,16,(*(ptr+3))+0xd4ef3085);
		MD5_STAGE(db,dc,dd,da,2,23,(*(ptr+6))+0x04881d05);
		MD5_STAGE(da,db,dc,dd,2,4,(*(ptr+9))+0xd9d4d039);
		MD5_STAGE(dd,da,db,dc,2,11,(*(ptr+12))+0xe6db99e5);
		MD5_STAGE(dc,dd,da,db,2,16,(*(ptr+15))+0x1fa27cf8);
		MD5_STAGE(db,dc,dd,da,2,23,(*(ptr+2))+0xc4ac5665);
		MD5_STAGE(da,db,dc,dd,3,6,(*ptr)+0xf4292244);
		MD5_STAGE(dd,da,db,dc,3,10,(*(ptr+7))+0x432aff97);
		MD5_STAGE(dc,dd,da,db,3,15,(*(ptr+14))+0xab9423a7);
		MD5_STAGE(db,dc,dd,da,3,21,(*(ptr+5))+0xfc93a039);
		MD5_STAGE(da,db,dc,dd,3,6,(*(ptr+12))+0x655b59c3);
		MD5_STAGE(dd,da,db,dc,3,10,(*(ptr+3))+0x8f0ccc92);
		MD5_STAGE(dc,dd,da,db,3,15,(*(ptr+10))+0xffeff47d);
		MD5_STAGE(db,dc,dd,da,3,21,(*(ptr+1))+0x85845dd1);
		MD5_STAGE(da,db,dc,dd,3,6,(*(ptr+8))+0x6fa87e4f);
		MD5_STAGE(dd,da,db,dc,3,10,(*(ptr+15))+0xfe2ce6e0);
		MD5_STAGE(dc,dd,da,db,3,15,(*(ptr+6))+0xa3014314);
		MD5_STAGE(db,dc,dd,da,3,21,(*(ptr+13))+0x4e0811a1);
		MD5_STAGE(da,db,dc,dd,3,6,(*(ptr+4))+0xf7537e82);
		MD5_STAGE(dd,da,db,dc,3,10,(*(ptr+11))+0xbd3af235);
		MD5_STAGE(dc,dd,da,db,3,15,(*(ptr+2))+0x2ad7d2bb);
		MD5_STAGE(db,dc,dd,da,3,21,(*(ptr+9))+0xeb86d391);
		av+=da;
		bv+=db;
		cv+=dc;
		dv+=dd;
		ptr+=16;
	}
	if (!sll_array_create(4,out)){
		SLL_UNIMPLEMENTED();
	}
	out->v[0]=SLL_FROM_INT(av);
	out->v[1]=SLL_FROM_INT(bv);
	out->v[2]=SLL_FROM_INT(cv);
	out->v[3]=SLL_FROM_INT(dv);
}



__API_FUNC(hash_sha1){
	uint32_t av=(uint32_t)a;
	uint32_t bv=(uint32_t)b;
	uint32_t cv=(uint32_t)c;
	uint32_t dv=(uint32_t)d;
	uint32_t ev=(uint32_t)e;
	SLL_ASSERT(!(f->l&63));
	uint32_t w[80];
	const uint32_t* ptr=(const uint32_t*)(f->v);
	for (sll_string_length_t i=0;i<(f->l>>6);i++){
		for (uint8_t j=0;j<16;j++){
			w[j]=SWAP_BYTES(*ptr);
			ptr++;
		}
		w[16]=ROTATE_BITS(w[13]^w[8]^w[2]^w[0],1);
		w[17]=ROTATE_BITS(w[14]^w[9]^w[3]^w[1],1);
		w[18]=ROTATE_BITS(w[15]^w[10]^w[4]^w[2],1);
		w[19]=ROTATE_BITS(w[16]^w[11]^w[5]^w[3],1);
		uint32_t da=av;
		uint32_t db=bv;
		uint32_t dc=cv;
		uint32_t dd=dv;
		uint32_t de=ev;
		SHA1_STAGE(0,0);
		SHA1_STAGE(0,1);
		SHA1_STAGE(0,2);
		SHA1_STAGE(0,3);
		SHA1_STAGE(1,4);
		SHA1_STAGE(1,5);
		SHA1_STAGE(1,6);
		SHA1_STAGE(1,7);
		SHA1_STAGE(2,8);
		SHA1_STAGE(2,9);
		SHA1_STAGE(2,10);
		SHA1_STAGE(2,11);
		SHA1_STAGE(3,12);
		SHA1_STAGE(3,13);
		SHA1_STAGE(3,14);
		SHA1_STAGE(3,15);
		av+=da;
		bv+=db;
		cv+=dc;
		dv+=dd;
		ev+=de;
	}
	if (!sll_array_create(5,out)){
		SLL_UNIMPLEMENTED();
	}
	out->v[0]=SLL_FROM_INT(av);
	out->v[1]=SLL_FROM_INT(bv);
	out->v[2]=SLL_FROM_INT(cv);
	out->v[3]=SLL_FROM_INT(dv);
	out->v[4]=SLL_FROM_INT(ev);
}

#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>



#define MD5_STEP0(b,c,d) (d^(b&(c^d)))
#define MD5_STEP1(b,c,d) (c^(d&(b^c)))
#define MD5_STEP2(b,c,d) (b^c^d)
#define MD5_STEP3(b,c,d) (c^(b|(~d)))

#define MD5_STEP(a,b,c,d,t,s,v) a=ROTATE_BITS(a+MD5_STEP##t(b,c,d)+v,s)+b



#define SHA1_STEP0(b,c,d) (d^(b&(c^d)))+0x5a827999
#define SHA1_STEP1(b,c,d) (b^c^d)+0x6ed9eba1
#define SHA1_STEP2(b,c,d) ((b&c)|(b&d)|(c&d))+0x8f1bbcdc
#define SHA1_STEP3(b,c,d) (b^c^d)+0xca62c1d6

#define SHA1_STEP(t,i) \
	if (i>3){ \
		w[i*5]=ROTATE_BITS(w[i*5-3]^w[i*5-8]^w[i*5-14]^w[i*5-16],1); \
		w[i*5+1]=ROTATE_BITS(w[i*5-2]^w[i*5-7]^w[i*5-13]^w[i*5-15],1); \
		w[i*5+2]=ROTATE_BITS(w[i*5-1]^w[i*5-6]^w[i*5-12]^w[i*5-14],1); \
		w[i*5+3]=ROTATE_BITS(w[i*5]^w[i*5-5]^w[i*5-11]^w[i*5-13],1); \
		w[i*5+4]=ROTATE_BITS(w[i*5+1]^w[i*5-4]^w[i*5-10]^w[i*5-12],1); \
	} \
	de=ROTATE_BITS(da,5)+SHA1_STEP##t(db,dc,dd)+de+w[i*5]; \
	db=ROTATE_BITS(db,30); \
	dd=ROTATE_BITS(de,5)+SHA1_STEP##t(da,db,dc)+dd+w[i*5+1]; \
	da=ROTATE_BITS(da,30); \
	dc=ROTATE_BITS(dd,5)+SHA1_STEP##t(de,da,db)+dc+w[i*5+2]; \
	de=ROTATE_BITS(de,30); \
	db=ROTATE_BITS(dc,5)+SHA1_STEP##t(dd,de,da)+db+w[i*5+3]; \
	dd=ROTATE_BITS(dd,30); \
	da=ROTATE_BITS(db,5)+SHA1_STEP##t(dc,dd,de)+da+w[i*5+4]; \
	dc=ROTATE_BITS(dc,30);



#define SHA256_STEP(a,b,c,d,e,f,g,h,v) \
	t=(ROTATE_BITS_RIGHT(e,6)^ROTATE_BITS_RIGHT(e,11)^ROTATE_BITS_RIGHT(e,25))+(g^(e&(f^g)))+h+v; \
	d+=t; \
	h=t+(ROTATE_BITS_RIGHT(a,2)^ROTATE_BITS_RIGHT(a,13)^ROTATE_BITS_RIGHT(a,22))+((a&b)|((a|b)&c));




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
		MD5_STEP(da,db,dc,dd,0,7,(*ptr)+0xd76aa478);
		MD5_STEP(dd,da,db,dc,0,12,(*(ptr+1))+0xe8c7b756);
		MD5_STEP(dc,dd,da,db,0,17,(*(ptr+2))+0x242070db);
		MD5_STEP(db,dc,dd,da,0,22,(*(ptr+3))+0xc1bdceee);
		MD5_STEP(da,db,dc,dd,0,7,(*(ptr+4))+0xf57c0faf);
		MD5_STEP(dd,da,db,dc,0,12,(*(ptr+5))+0x4787c62a);
		MD5_STEP(dc,dd,da,db,0,17,(*(ptr+6))+0xa8304613);
		MD5_STEP(db,dc,dd,da,0,22,(*(ptr+7))+0xfd469501);
		MD5_STEP(da,db,dc,dd,0,7,(*(ptr+8))+0x698098d8);
		MD5_STEP(dd,da,db,dc,0,12,(*(ptr+9))+0x8b44f7af);
		MD5_STEP(dc,dd,da,db,0,17,(*(ptr+10))+0xffff5bb1);
		MD5_STEP(db,dc,dd,da,0,22,(*(ptr+11))+0x895cd7be);
		MD5_STEP(da,db,dc,dd,0,7,(*(ptr+12))+0x6b901122);
		MD5_STEP(dd,da,db,dc,0,12,(*(ptr+13))+0xfd987193);
		MD5_STEP(dc,dd,da,db,0,17,(*(ptr+14))+0xa679438e);
		MD5_STEP(db,dc,dd,da,0,22,(*(ptr+15))+0x49b40821);
		MD5_STEP(da,db,dc,dd,1,5,(*(ptr+1))+0xf61e2562);
		MD5_STEP(dd,da,db,dc,1,9,(*(ptr+6))+0xc040b340);
		MD5_STEP(dc,dd,da,db,1,14,(*(ptr+11))+0x265e5a51);
		MD5_STEP(db,dc,dd,da,1,20,(*ptr)+0xe9b6c7aa);
		MD5_STEP(da,db,dc,dd,1,5,(*(ptr+5))+0xd62f105d);
		MD5_STEP(dd,da,db,dc,1,9,(*(ptr+10))+0x02441453);
		MD5_STEP(dc,dd,da,db,1,14,(*(ptr+15))+0xd8a1e681);
		MD5_STEP(db,dc,dd,da,1,20,(*(ptr+4))+0xe7d3fbc8);
		MD5_STEP(da,db,dc,dd,1,5,(*(ptr+9))+0x21e1cde6);
		MD5_STEP(dd,da,db,dc,1,9,(*(ptr+14))+0xc33707d6);
		MD5_STEP(dc,dd,da,db,1,14,(*(ptr+3))+0xf4d50d87);
		MD5_STEP(db,dc,dd,da,1,20,(*(ptr+8))+0x455a14ed);
		MD5_STEP(da,db,dc,dd,1,5,(*(ptr+13))+0xa9e3e905);
		MD5_STEP(dd,da,db,dc,1,9,(*(ptr+2))+0xfcefa3f8);
		MD5_STEP(dc,dd,da,db,1,14,(*(ptr+7))+0x676f02d9);
		MD5_STEP(db,dc,dd,da,1,20,(*(ptr+12))+0x8d2a4c8a);
		MD5_STEP(da,db,dc,dd,2,4,(*(ptr+5))+0xfffa3942);
		MD5_STEP(dd,da,db,dc,2,11,(*(ptr+8))+0x8771f681);
		MD5_STEP(dc,dd,da,db,2,16,(*(ptr+11))+0x6d9d6122);
		MD5_STEP(db,dc,dd,da,2,23,(*(ptr+14))+0xfde5380c);
		MD5_STEP(da,db,dc,dd,2,4,(*(ptr+1))+0xa4beea44);
		MD5_STEP(dd,da,db,dc,2,11,(*(ptr+4))+0x4bdecfa9);
		MD5_STEP(dc,dd,da,db,2,16,(*(ptr+7))+0xf6bb4b60);
		MD5_STEP(db,dc,dd,da,2,23,(*(ptr+10))+0xbebfbc70);
		MD5_STEP(da,db,dc,dd,2,4,(*(ptr+13))+0x289b7ec6);
		MD5_STEP(dd,da,db,dc,2,11,(*ptr)+0xeaa127fa);
		MD5_STEP(dc,dd,da,db,2,16,(*(ptr+3))+0xd4ef3085);
		MD5_STEP(db,dc,dd,da,2,23,(*(ptr+6))+0x04881d05);
		MD5_STEP(da,db,dc,dd,2,4,(*(ptr+9))+0xd9d4d039);
		MD5_STEP(dd,da,db,dc,2,11,(*(ptr+12))+0xe6db99e5);
		MD5_STEP(dc,dd,da,db,2,16,(*(ptr+15))+0x1fa27cf8);
		MD5_STEP(db,dc,dd,da,2,23,(*(ptr+2))+0xc4ac5665);
		MD5_STEP(da,db,dc,dd,3,6,(*ptr)+0xf4292244);
		MD5_STEP(dd,da,db,dc,3,10,(*(ptr+7))+0x432aff97);
		MD5_STEP(dc,dd,da,db,3,15,(*(ptr+14))+0xab9423a7);
		MD5_STEP(db,dc,dd,da,3,21,(*(ptr+5))+0xfc93a039);
		MD5_STEP(da,db,dc,dd,3,6,(*(ptr+12))+0x655b59c3);
		MD5_STEP(dd,da,db,dc,3,10,(*(ptr+3))+0x8f0ccc92);
		MD5_STEP(dc,dd,da,db,3,15,(*(ptr+10))+0xffeff47d);
		MD5_STEP(db,dc,dd,da,3,21,(*(ptr+1))+0x85845dd1);
		MD5_STEP(da,db,dc,dd,3,6,(*(ptr+8))+0x6fa87e4f);
		MD5_STEP(dd,da,db,dc,3,10,(*(ptr+15))+0xfe2ce6e0);
		MD5_STEP(dc,dd,da,db,3,15,(*(ptr+6))+0xa3014314);
		MD5_STEP(db,dc,dd,da,3,21,(*(ptr+13))+0x4e0811a1);
		MD5_STEP(da,db,dc,dd,3,6,(*(ptr+4))+0xf7537e82);
		MD5_STEP(dd,da,db,dc,3,10,(*(ptr+11))+0xbd3af235);
		MD5_STEP(dc,dd,da,db,3,15,(*(ptr+2))+0x2ad7d2bb);
		MD5_STEP(db,dc,dd,da,3,21,(*(ptr+9))+0xeb86d391);
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
		SHA1_STEP(0,0);
		SHA1_STEP(0,1);
		SHA1_STEP(0,2);
		SHA1_STEP(0,3);
		SHA1_STEP(1,4);
		SHA1_STEP(1,5);
		SHA1_STEP(1,6);
		SHA1_STEP(1,7);
		SHA1_STEP(2,8);
		SHA1_STEP(2,9);
		SHA1_STEP(2,10);
		SHA1_STEP(2,11);
		SHA1_STEP(3,12);
		SHA1_STEP(3,13);
		SHA1_STEP(3,14);
		SHA1_STEP(3,15);
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



__API_FUNC(hash_sha256){
	uint32_t av=(uint32_t)a;
	uint32_t bv=(uint32_t)b;
	uint32_t cv=(uint32_t)c;
	uint32_t dv=(uint32_t)d;
	uint32_t ev=(uint32_t)e;
	uint32_t fv=(uint32_t)f;
	uint32_t gv=(uint32_t)g;
	uint32_t hv=(uint32_t)h;
	SLL_ASSERT(!(i->l&63));
	uint32_t w[80];
	uint32_t t;
	const uint32_t* ptr=(const uint32_t*)(i->v);
	for (sll_string_length_t idx=0;idx<(i->l>>6);idx++){
		for (uint8_t j=0;j<16;j++){
			w[j]=SWAP_BYTES(*ptr);
			ptr++;
		}
		for (uint8_t j=16;j<64;j++){
			w[j]=(ROTATE_BITS_RIGHT(w[j-2],17)^ROTATE_BITS_RIGHT(w[j-2],19)^(w[j-2]>>10))+w[j-7]+(ROTATE_BITS_RIGHT(w[j-15],7)^ROTATE_BITS_RIGHT(w[j-15],18)^(w[j-15]>>3))+w[j-16];
		}
		uint32_t da=av;
		uint32_t db=bv;
		uint32_t dc=cv;
		uint32_t dd=dv;
		uint32_t de=ev;
		uint32_t df=fv;
		uint32_t dg=gv;
		uint32_t dh=hv;
		SHA256_STEP(da,db,dc,dd,de,df,dg,dh,w[0]+0x428a2f98);
		SHA256_STEP(dh,da,db,dc,dd,de,df,dg,w[1]+0x71374491);
		SHA256_STEP(dg,dh,da,db,dc,dd,de,df,w[2]+0xb5c0fbcf);
		SHA256_STEP(df,dg,dh,da,db,dc,dd,de,w[3]+0xe9b5dba5);
		SHA256_STEP(de,df,dg,dh,da,db,dc,dd,w[4]+0x3956c25b);
		SHA256_STEP(dd,de,df,dg,dh,da,db,dc,w[5]+0x59f111f1);
		SHA256_STEP(dc,dd,de,df,dg,dh,da,db,w[6]+0x923f82a4);
		SHA256_STEP(db,dc,dd,de,df,dg,dh,da,w[7]+0xab1c5ed5);
		SHA256_STEP(da,db,dc,dd,de,df,dg,dh,w[8]+0xd807aa98);
		SHA256_STEP(dh,da,db,dc,dd,de,df,dg,w[9]+0x12835b01);
		SHA256_STEP(dg,dh,da,db,dc,dd,de,df,w[10]+0x243185be);
		SHA256_STEP(df,dg,dh,da,db,dc,dd,de,w[11]+0x550c7dc3);
		SHA256_STEP(de,df,dg,dh,da,db,dc,dd,w[12]+0x72be5d74);
		SHA256_STEP(dd,de,df,dg,dh,da,db,dc,w[13]+0x80deb1fe);
		SHA256_STEP(dc,dd,de,df,dg,dh,da,db,w[14]+0x9bdc06a7);
		SHA256_STEP(db,dc,dd,de,df,dg,dh,da,w[15]+0xc19bf174);
		SHA256_STEP(da,db,dc,dd,de,df,dg,dh,w[16]+0xe49b69c1);
		SHA256_STEP(dh,da,db,dc,dd,de,df,dg,w[17]+0xefbe4786);
		SHA256_STEP(dg,dh,da,db,dc,dd,de,df,w[18]+0x0fc19dc6);
		SHA256_STEP(df,dg,dh,da,db,dc,dd,de,w[19]+0x240ca1cc);
		SHA256_STEP(de,df,dg,dh,da,db,dc,dd,w[20]+0x2de92c6f);
		SHA256_STEP(dd,de,df,dg,dh,da,db,dc,w[21]+0x4a7484aa);
		SHA256_STEP(dc,dd,de,df,dg,dh,da,db,w[22]+0x5cb0a9dc);
		SHA256_STEP(db,dc,dd,de,df,dg,dh,da,w[23]+0x76f988da);
		SHA256_STEP(da,db,dc,dd,de,df,dg,dh,w[24]+0x983e5152);
		SHA256_STEP(dh,da,db,dc,dd,de,df,dg,w[25]+0xa831c66d);
		SHA256_STEP(dg,dh,da,db,dc,dd,de,df,w[26]+0xb00327c8);
		SHA256_STEP(df,dg,dh,da,db,dc,dd,de,w[27]+0xbf597fc7);
		SHA256_STEP(de,df,dg,dh,da,db,dc,dd,w[28]+0xc6e00bf3);
		SHA256_STEP(dd,de,df,dg,dh,da,db,dc,w[29]+0xd5a79147);
		SHA256_STEP(dc,dd,de,df,dg,dh,da,db,w[30]+0x06ca6351);
		SHA256_STEP(db,dc,dd,de,df,dg,dh,da,w[31]+0x14292967);
		SHA256_STEP(da,db,dc,dd,de,df,dg,dh,w[32]+0x27b70a85);
		SHA256_STEP(dh,da,db,dc,dd,de,df,dg,w[33]+0x2e1b2138);
		SHA256_STEP(dg,dh,da,db,dc,dd,de,df,w[34]+0x4d2c6dfc);
		SHA256_STEP(df,dg,dh,da,db,dc,dd,de,w[35]+0x53380d13);
		SHA256_STEP(de,df,dg,dh,da,db,dc,dd,w[36]+0x650a7354);
		SHA256_STEP(dd,de,df,dg,dh,da,db,dc,w[37]+0x766a0abb);
		SHA256_STEP(dc,dd,de,df,dg,dh,da,db,w[38]+0x81c2c92e);
		SHA256_STEP(db,dc,dd,de,df,dg,dh,da,w[39]+0x92722c85);
		SHA256_STEP(da,db,dc,dd,de,df,dg,dh,w[40]+0xa2bfe8a1);
		SHA256_STEP(dh,da,db,dc,dd,de,df,dg,w[41]+0xa81a664b);
		SHA256_STEP(dg,dh,da,db,dc,dd,de,df,w[42]+0xc24b8b70);
		SHA256_STEP(df,dg,dh,da,db,dc,dd,de,w[43]+0xc76c51a3);
		SHA256_STEP(de,df,dg,dh,da,db,dc,dd,w[44]+0xd192e819);
		SHA256_STEP(dd,de,df,dg,dh,da,db,dc,w[45]+0xd6990624);
		SHA256_STEP(dc,dd,de,df,dg,dh,da,db,w[46]+0xf40e3585);
		SHA256_STEP(db,dc,dd,de,df,dg,dh,da,w[47]+0x106aa070);
		SHA256_STEP(da,db,dc,dd,de,df,dg,dh,w[48]+0x19a4c116);
		SHA256_STEP(dh,da,db,dc,dd,de,df,dg,w[49]+0x1e376c08);
		SHA256_STEP(dg,dh,da,db,dc,dd,de,df,w[50]+0x2748774c);
		SHA256_STEP(df,dg,dh,da,db,dc,dd,de,w[51]+0x34b0bcb5);
		SHA256_STEP(de,df,dg,dh,da,db,dc,dd,w[52]+0x391c0cb3);
		SHA256_STEP(dd,de,df,dg,dh,da,db,dc,w[53]+0x4ed8aa4a);
		SHA256_STEP(dc,dd,de,df,dg,dh,da,db,w[54]+0x5b9cca4f);
		SHA256_STEP(db,dc,dd,de,df,dg,dh,da,w[55]+0x682e6ff3);
		SHA256_STEP(da,db,dc,dd,de,df,dg,dh,w[56]+0x748f82ee);
		SHA256_STEP(dh,da,db,dc,dd,de,df,dg,w[57]+0x78a5636f);
		SHA256_STEP(dg,dh,da,db,dc,dd,de,df,w[58]+0x84c87814);
		SHA256_STEP(df,dg,dh,da,db,dc,dd,de,w[59]+0x8cc70208);
		SHA256_STEP(de,df,dg,dh,da,db,dc,dd,w[60]+0x90befffa);
		SHA256_STEP(dd,de,df,dg,dh,da,db,dc,w[61]+0xa4506ceb);
		SHA256_STEP(dc,dd,de,df,dg,dh,da,db,w[62]+0xbef9a3f7);
		SHA256_STEP(db,dc,dd,de,df,dg,dh,da,w[63]+0xc67178f2);
		av+=da;
		bv+=db;
		cv+=dc;
		dv+=dd;
		ev+=de;
		fv+=df;
		gv+=dg;
		hv+=dh;
	}
	if (!sll_array_create(8,out)){
		SLL_UNIMPLEMENTED();
	}
	out->v[0]=SLL_FROM_INT(av);
	out->v[1]=SLL_FROM_INT(bv);
	out->v[2]=SLL_FROM_INT(cv);
	out->v[3]=SLL_FROM_INT(dv);
	out->v[4]=SLL_FROM_INT(ev);
	out->v[5]=SLL_FROM_INT(fv);
	out->v[6]=SLL_FROM_INT(gv);
	out->v[7]=SLL_FROM_INT(hv);
}

#include <sll/_size_types.h>
#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/static_object.h>
#include <sll/types.h>



#define u32 __SLL_U32
#define u64 __SLL_U64



#define MD5_STEP0(b,c,d) (d^(b&(c^d)))
#define MD5_STEP1(b,c,d) (c^(d&(b^c)))
#define MD5_STEP2(b,c,d) (b^c^d)
#define MD5_STEP3(b,c,d) (c^(b|(~d)))
#define MD5_STEP(a,b,c,d,t,s,v) a=ROTATE_BITS(a+MD5_STEP##t(b,c,d)+v,s)+b



#define SHA1_STEP0(b,c,d) (d^(b&(c^d)))+0x5a827999
#define SHA1_STEP1(b,c,d) (b^c^d)+0x6ed9eba1
#define SHA1_STEP2(b,c,d) ((b&c)|(b&d)|(c&d))+0x8f1bbcdc
#define SHA1_STEP3(b,c,d) (b^c^d)+0xca62c1d6
#define SHA1_STEP(a,b,c,d,e,fn,v) \
	e=ROTATE_BITS(a,5)+SHA1_STEP##fn(b,c,d)+e+v; \
	b=ROTATE_BITS(b,30);



#define SHA256_STEP(a,b,c,d,e,f,g,h,v) \
	t=(ROTATE_BITS_RIGHT(e,6)^ROTATE_BITS_RIGHT(e,11)^ROTATE_BITS_RIGHT(e,25))+(g^(e&(f^g)))+h+v; \
	d+=t; \
	h=t+(ROTATE_BITS_RIGHT(a,2)^ROTATE_BITS_RIGHT(a,13)^ROTATE_BITS_RIGHT(a,22))+((a&b)|((a|b)&c));



#define SHA512_STEP(a,b,c,d,e,f,g,h,v) \
	t=(ROTATE_BITS_RIGHT64(e,14)^ROTATE_BITS_RIGHT64(e,18)^ROTATE_BITS_RIGHT64(e,41))+(g^(e&(f^g)))+h+v; \
	d+=t; \
	h=t+(ROTATE_BITS_RIGHT64(a,28)^ROTATE_BITS_RIGHT64(a,34)^ROTATE_BITS_RIGHT64(a,39))+((a&b)|((a|b)&c));



__SLL_EXTERNAL void sll_hash_md5(sll_md5_data_t* dt,void* bf,sll_file_offset_t bfl){
	SLL_ASSERT(!(bfl&63));
	const u32* ptr=(const u32*)bf;
	for (sll_file_offset_t i=0;i<(bfl>>6);i++){
		u32 a=dt->a;
		u32 b=dt->b;
		u32 c=dt->c;
		u32 d=dt->d;
		MD5_STEP(a,b,c,d,0,7,(*ptr)+0xd76aa478);
		MD5_STEP(d,a,b,c,0,12,(*(ptr+1))+0xe8c7b756);
		MD5_STEP(c,d,a,b,0,17,(*(ptr+2))+0x242070db);
		MD5_STEP(b,c,d,a,0,22,(*(ptr+3))+0xc1bdceee);
		MD5_STEP(a,b,c,d,0,7,(*(ptr+4))+0xf57c0faf);
		MD5_STEP(d,a,b,c,0,12,(*(ptr+5))+0x4787c62a);
		MD5_STEP(c,d,a,b,0,17,(*(ptr+6))+0xa8304613);
		MD5_STEP(b,c,d,a,0,22,(*(ptr+7))+0xfd469501);
		MD5_STEP(a,b,c,d,0,7,(*(ptr+8))+0x698098d8);
		MD5_STEP(d,a,b,c,0,12,(*(ptr+9))+0x8b44f7af);
		MD5_STEP(c,d,a,b,0,17,(*(ptr+10))+0xffff5bb1);
		MD5_STEP(b,c,d,a,0,22,(*(ptr+11))+0x895cd7be);
		MD5_STEP(a,b,c,d,0,7,(*(ptr+12))+0x6b901122);
		MD5_STEP(d,a,b,c,0,12,(*(ptr+13))+0xfd987193);
		MD5_STEP(c,d,a,b,0,17,(*(ptr+14))+0xa679438e);
		MD5_STEP(b,c,d,a,0,22,(*(ptr+15))+0x49b40821);
		MD5_STEP(a,b,c,d,1,5,(*(ptr+1))+0xf61e2562);
		MD5_STEP(d,a,b,c,1,9,(*(ptr+6))+0xc040b340);
		MD5_STEP(c,d,a,b,1,14,(*(ptr+11))+0x265e5a51);
		MD5_STEP(b,c,d,a,1,20,(*ptr)+0xe9b6c7aa);
		MD5_STEP(a,b,c,d,1,5,(*(ptr+5))+0xd62f105d);
		MD5_STEP(d,a,b,c,1,9,(*(ptr+10))+0x02441453);
		MD5_STEP(c,d,a,b,1,14,(*(ptr+15))+0xd8a1e681);
		MD5_STEP(b,c,d,a,1,20,(*(ptr+4))+0xe7d3fbc8);
		MD5_STEP(a,b,c,d,1,5,(*(ptr+9))+0x21e1cde6);
		MD5_STEP(d,a,b,c,1,9,(*(ptr+14))+0xc33707d6);
		MD5_STEP(c,d,a,b,1,14,(*(ptr+3))+0xf4d50d87);
		MD5_STEP(b,c,d,a,1,20,(*(ptr+8))+0x455a14ed);
		MD5_STEP(a,b,c,d,1,5,(*(ptr+13))+0xa9e3e905);
		MD5_STEP(d,a,b,c,1,9,(*(ptr+2))+0xfcefa3f8);
		MD5_STEP(c,d,a,b,1,14,(*(ptr+7))+0x676f02d9);
		MD5_STEP(b,c,d,a,1,20,(*(ptr+12))+0x8d2a4c8a);
		MD5_STEP(a,b,c,d,2,4,(*(ptr+5))+0xfffa3942);
		MD5_STEP(d,a,b,c,2,11,(*(ptr+8))+0x8771f681);
		MD5_STEP(c,d,a,b,2,16,(*(ptr+11))+0x6d9d6122);
		MD5_STEP(b,c,d,a,2,23,(*(ptr+14))+0xfde5380c);
		MD5_STEP(a,b,c,d,2,4,(*(ptr+1))+0xa4beea44);
		MD5_STEP(d,a,b,c,2,11,(*(ptr+4))+0x4bdecfa9);
		MD5_STEP(c,d,a,b,2,16,(*(ptr+7))+0xf6bb4b60);
		MD5_STEP(b,c,d,a,2,23,(*(ptr+10))+0xbebfbc70);
		MD5_STEP(a,b,c,d,2,4,(*(ptr+13))+0x289b7ec6);
		MD5_STEP(d,a,b,c,2,11,(*ptr)+0xeaa127fa);
		MD5_STEP(c,d,a,b,2,16,(*(ptr+3))+0xd4ef3085);
		MD5_STEP(b,c,d,a,2,23,(*(ptr+6))+0x04881d05);
		MD5_STEP(a,b,c,d,2,4,(*(ptr+9))+0xd9d4d039);
		MD5_STEP(d,a,b,c,2,11,(*(ptr+12))+0xe6db99e5);
		MD5_STEP(c,d,a,b,2,16,(*(ptr+15))+0x1fa27cf8);
		MD5_STEP(b,c,d,a,2,23,(*(ptr+2))+0xc4ac5665);
		MD5_STEP(a,b,c,d,3,6,(*ptr)+0xf4292244);
		MD5_STEP(d,a,b,c,3,10,(*(ptr+7))+0x432aff97);
		MD5_STEP(c,d,a,b,3,15,(*(ptr+14))+0xab9423a7);
		MD5_STEP(b,c,d,a,3,21,(*(ptr+5))+0xfc93a039);
		MD5_STEP(a,b,c,d,3,6,(*(ptr+12))+0x655b59c3);
		MD5_STEP(d,a,b,c,3,10,(*(ptr+3))+0x8f0ccc92);
		MD5_STEP(c,d,a,b,3,15,(*(ptr+10))+0xffeff47d);
		MD5_STEP(b,c,d,a,3,21,(*(ptr+1))+0x85845dd1);
		MD5_STEP(a,b,c,d,3,6,(*(ptr+8))+0x6fa87e4f);
		MD5_STEP(d,a,b,c,3,10,(*(ptr+15))+0xfe2ce6e0);
		MD5_STEP(c,d,a,b,3,15,(*(ptr+6))+0xa3014314);
		MD5_STEP(b,c,d,a,3,21,(*(ptr+13))+0x4e0811a1);
		MD5_STEP(a,b,c,d,3,6,(*(ptr+4))+0xf7537e82);
		MD5_STEP(d,a,b,c,3,10,(*(ptr+11))+0xbd3af235);
		MD5_STEP(c,d,a,b,3,15,(*(ptr+2))+0x2ad7d2bb);
		MD5_STEP(b,c,d,a,3,21,(*(ptr+9))+0xeb86d391);
		dt->a+=a;
		dt->b+=b;
		dt->c+=c;
		dt->d+=d;
		ptr+=16;
	}
}



__SLL_EXTERNAL void sll_hash_sha1(sll_sha1_data_t* dt,void* bf,sll_file_offset_t bfl){
	SLL_ASSERT(!(bfl&63));
	u32 w[80];
	const u32* ptr=(const u32*)bf;
	for (sll_file_offset_t i=0;i<(bfl>>6);i++){
		for (unsigned int j=0;j<16;j++){
			w[j]=SWAP_BYTES(*ptr);
			ptr++;
		}
		for (unsigned int j=16;j<80;j++){
			w[j]=ROTATE_BITS(w[j-3]^w[j-8]^w[j-14]^w[j-16],1);
		}
		u32 a=dt->a;
		u32 b=dt->b;
		u32 c=dt->c;
		u32 d=dt->d;
		u32 e=dt->e;
		SHA1_STEP(a,b,c,d,e,0,w[0]);
		SHA1_STEP(e,a,b,c,d,0,w[1]);
		SHA1_STEP(d,e,a,b,c,0,w[2]);
		SHA1_STEP(c,d,e,a,b,0,w[3]);
		SHA1_STEP(b,c,d,e,a,0,w[4]);
		SHA1_STEP(a,b,c,d,e,0,w[5]);
		SHA1_STEP(e,a,b,c,d,0,w[6]);
		SHA1_STEP(d,e,a,b,c,0,w[7]);
		SHA1_STEP(c,d,e,a,b,0,w[8]);
		SHA1_STEP(b,c,d,e,a,0,w[9]);
		SHA1_STEP(a,b,c,d,e,0,w[10]);
		SHA1_STEP(e,a,b,c,d,0,w[11]);
		SHA1_STEP(d,e,a,b,c,0,w[12]);
		SHA1_STEP(c,d,e,a,b,0,w[13]);
		SHA1_STEP(b,c,d,e,a,0,w[14]);
		SHA1_STEP(a,b,c,d,e,0,w[15]);
		SHA1_STEP(e,a,b,c,d,0,w[16]);
		SHA1_STEP(d,e,a,b,c,0,w[17]);
		SHA1_STEP(c,d,e,a,b,0,w[18]);
		SHA1_STEP(b,c,d,e,a,0,w[19]);
		SHA1_STEP(a,b,c,d,e,1,w[20]);
		SHA1_STEP(e,a,b,c,d,1,w[21]);
		SHA1_STEP(d,e,a,b,c,1,w[22]);
		SHA1_STEP(c,d,e,a,b,1,w[23]);
		SHA1_STEP(b,c,d,e,a,1,w[24]);
		SHA1_STEP(a,b,c,d,e,1,w[25]);
		SHA1_STEP(e,a,b,c,d,1,w[26]);
		SHA1_STEP(d,e,a,b,c,1,w[27]);
		SHA1_STEP(c,d,e,a,b,1,w[28]);
		SHA1_STEP(b,c,d,e,a,1,w[29]);
		SHA1_STEP(a,b,c,d,e,1,w[30]);
		SHA1_STEP(e,a,b,c,d,1,w[31]);
		SHA1_STEP(d,e,a,b,c,1,w[32]);
		SHA1_STEP(c,d,e,a,b,1,w[33]);
		SHA1_STEP(b,c,d,e,a,1,w[34]);
		SHA1_STEP(a,b,c,d,e,1,w[35]);
		SHA1_STEP(e,a,b,c,d,1,w[36]);
		SHA1_STEP(d,e,a,b,c,1,w[37]);
		SHA1_STEP(c,d,e,a,b,1,w[38]);
		SHA1_STEP(b,c,d,e,a,1,w[39]);
		SHA1_STEP(a,b,c,d,e,2,w[40]);
		SHA1_STEP(e,a,b,c,d,2,w[41]);
		SHA1_STEP(d,e,a,b,c,2,w[42]);
		SHA1_STEP(c,d,e,a,b,2,w[43]);
		SHA1_STEP(b,c,d,e,a,2,w[44]);
		SHA1_STEP(a,b,c,d,e,2,w[45]);
		SHA1_STEP(e,a,b,c,d,2,w[46]);
		SHA1_STEP(d,e,a,b,c,2,w[47]);
		SHA1_STEP(c,d,e,a,b,2,w[48]);
		SHA1_STEP(b,c,d,e,a,2,w[49]);
		SHA1_STEP(a,b,c,d,e,2,w[50]);
		SHA1_STEP(e,a,b,c,d,2,w[51]);
		SHA1_STEP(d,e,a,b,c,2,w[52]);
		SHA1_STEP(c,d,e,a,b,2,w[53]);
		SHA1_STEP(b,c,d,e,a,2,w[54]);
		SHA1_STEP(a,b,c,d,e,2,w[55]);
		SHA1_STEP(e,a,b,c,d,2,w[56]);
		SHA1_STEP(d,e,a,b,c,2,w[57]);
		SHA1_STEP(c,d,e,a,b,2,w[58]);
		SHA1_STEP(b,c,d,e,a,2,w[59]);
		SHA1_STEP(a,b,c,d,e,3,w[60]);
		SHA1_STEP(e,a,b,c,d,3,w[61]);
		SHA1_STEP(d,e,a,b,c,3,w[62]);
		SHA1_STEP(c,d,e,a,b,3,w[63]);
		SHA1_STEP(b,c,d,e,a,3,w[64]);
		SHA1_STEP(a,b,c,d,e,3,w[65]);
		SHA1_STEP(e,a,b,c,d,3,w[66]);
		SHA1_STEP(d,e,a,b,c,3,w[67]);
		SHA1_STEP(c,d,e,a,b,3,w[68]);
		SHA1_STEP(b,c,d,e,a,3,w[69]);
		SHA1_STEP(a,b,c,d,e,3,w[70]);
		SHA1_STEP(e,a,b,c,d,3,w[71]);
		SHA1_STEP(d,e,a,b,c,3,w[72]);
		SHA1_STEP(c,d,e,a,b,3,w[73]);
		SHA1_STEP(b,c,d,e,a,3,w[74]);
		SHA1_STEP(a,b,c,d,e,3,w[75]);
		SHA1_STEP(e,a,b,c,d,3,w[76]);
		SHA1_STEP(d,e,a,b,c,3,w[77]);
		SHA1_STEP(c,d,e,a,b,3,w[78]);
		SHA1_STEP(b,c,d,e,a,3,w[79]);
		dt->a+=a;
		dt->b+=b;
		dt->c+=c;
		dt->d+=d;
		dt->e+=e;
	}
}



__SLL_EXTERNAL void sll_hash_sha256(sll_sha256_data_t* dt,void* bf,sll_file_offset_t bfl){
	SLL_ASSERT(!(bfl&63));
	u32 w[64];
	const u32* ptr=(const u32*)bf;
	for (sll_file_offset_t idx=0;idx<(bfl>>6);idx++){
		for (unsigned int j=0;j<16;j++){
			w[j]=SWAP_BYTES(*ptr);
			ptr++;
		}
		for (unsigned int j=16;j<64;j++){
			w[j]=(ROTATE_BITS_RIGHT(w[j-2],17)^ROTATE_BITS_RIGHT(w[j-2],19)^(w[j-2]>>10))+w[j-7]+(ROTATE_BITS_RIGHT(w[j-15],7)^ROTATE_BITS_RIGHT(w[j-15],18)^(w[j-15]>>3))+w[j-16];
		}
		u32 a=dt->a;
		u32 b=dt->b;
		u32 c=dt->c;
		u32 d=dt->d;
		u32 e=dt->e;
		u32 f=dt->f;
		u32 g=dt->g;
		u32 h=dt->h;
		u32 t;
		SHA256_STEP(a,b,c,d,e,f,g,h,w[0]+0x428a2f98);
		SHA256_STEP(h,a,b,c,d,e,f,g,w[1]+0x71374491);
		SHA256_STEP(g,h,a,b,c,d,e,f,w[2]+0xb5c0fbcf);
		SHA256_STEP(f,g,h,a,b,c,d,e,w[3]+0xe9b5dba5);
		SHA256_STEP(e,f,g,h,a,b,c,d,w[4]+0x3956c25b);
		SHA256_STEP(d,e,f,g,h,a,b,c,w[5]+0x59f111f1);
		SHA256_STEP(c,d,e,f,g,h,a,b,w[6]+0x923f82a4);
		SHA256_STEP(b,c,d,e,f,g,h,a,w[7]+0xab1c5ed5);
		SHA256_STEP(a,b,c,d,e,f,g,h,w[8]+0xd807aa98);
		SHA256_STEP(h,a,b,c,d,e,f,g,w[9]+0x12835b01);
		SHA256_STEP(g,h,a,b,c,d,e,f,w[10]+0x243185be);
		SHA256_STEP(f,g,h,a,b,c,d,e,w[11]+0x550c7dc3);
		SHA256_STEP(e,f,g,h,a,b,c,d,w[12]+0x72be5d74);
		SHA256_STEP(d,e,f,g,h,a,b,c,w[13]+0x80deb1fe);
		SHA256_STEP(c,d,e,f,g,h,a,b,w[14]+0x9bdc06a7);
		SHA256_STEP(b,c,d,e,f,g,h,a,w[15]+0xc19bf174);
		SHA256_STEP(a,b,c,d,e,f,g,h,w[16]+0xe49b69c1);
		SHA256_STEP(h,a,b,c,d,e,f,g,w[17]+0xefbe4786);
		SHA256_STEP(g,h,a,b,c,d,e,f,w[18]+0x0fc19dc6);
		SHA256_STEP(f,g,h,a,b,c,d,e,w[19]+0x240ca1cc);
		SHA256_STEP(e,f,g,h,a,b,c,d,w[20]+0x2de92c6f);
		SHA256_STEP(d,e,f,g,h,a,b,c,w[21]+0x4a7484aa);
		SHA256_STEP(c,d,e,f,g,h,a,b,w[22]+0x5cb0a9dc);
		SHA256_STEP(b,c,d,e,f,g,h,a,w[23]+0x76f988da);
		SHA256_STEP(a,b,c,d,e,f,g,h,w[24]+0x983e5152);
		SHA256_STEP(h,a,b,c,d,e,f,g,w[25]+0xa831c66d);
		SHA256_STEP(g,h,a,b,c,d,e,f,w[26]+0xb00327c8);
		SHA256_STEP(f,g,h,a,b,c,d,e,w[27]+0xbf597fc7);
		SHA256_STEP(e,f,g,h,a,b,c,d,w[28]+0xc6e00bf3);
		SHA256_STEP(d,e,f,g,h,a,b,c,w[29]+0xd5a79147);
		SHA256_STEP(c,d,e,f,g,h,a,b,w[30]+0x06ca6351);
		SHA256_STEP(b,c,d,e,f,g,h,a,w[31]+0x14292967);
		SHA256_STEP(a,b,c,d,e,f,g,h,w[32]+0x27b70a85);
		SHA256_STEP(h,a,b,c,d,e,f,g,w[33]+0x2e1b2138);
		SHA256_STEP(g,h,a,b,c,d,e,f,w[34]+0x4d2c6dfc);
		SHA256_STEP(f,g,h,a,b,c,d,e,w[35]+0x53380d13);
		SHA256_STEP(e,f,g,h,a,b,c,d,w[36]+0x650a7354);
		SHA256_STEP(d,e,f,g,h,a,b,c,w[37]+0x766a0abb);
		SHA256_STEP(c,d,e,f,g,h,a,b,w[38]+0x81c2c92e);
		SHA256_STEP(b,c,d,e,f,g,h,a,w[39]+0x92722c85);
		SHA256_STEP(a,b,c,d,e,f,g,h,w[40]+0xa2bfe8a1);
		SHA256_STEP(h,a,b,c,d,e,f,g,w[41]+0xa81a664b);
		SHA256_STEP(g,h,a,b,c,d,e,f,w[42]+0xc24b8b70);
		SHA256_STEP(f,g,h,a,b,c,d,e,w[43]+0xc76c51a3);
		SHA256_STEP(e,f,g,h,a,b,c,d,w[44]+0xd192e819);
		SHA256_STEP(d,e,f,g,h,a,b,c,w[45]+0xd6990624);
		SHA256_STEP(c,d,e,f,g,h,a,b,w[46]+0xf40e3585);
		SHA256_STEP(b,c,d,e,f,g,h,a,w[47]+0x106aa070);
		SHA256_STEP(a,b,c,d,e,f,g,h,w[48]+0x19a4c116);
		SHA256_STEP(h,a,b,c,d,e,f,g,w[49]+0x1e376c08);
		SHA256_STEP(g,h,a,b,c,d,e,f,w[50]+0x2748774c);
		SHA256_STEP(f,g,h,a,b,c,d,e,w[51]+0x34b0bcb5);
		SHA256_STEP(e,f,g,h,a,b,c,d,w[52]+0x391c0cb3);
		SHA256_STEP(d,e,f,g,h,a,b,c,w[53]+0x4ed8aa4a);
		SHA256_STEP(c,d,e,f,g,h,a,b,w[54]+0x5b9cca4f);
		SHA256_STEP(b,c,d,e,f,g,h,a,w[55]+0x682e6ff3);
		SHA256_STEP(a,b,c,d,e,f,g,h,w[56]+0x748f82ee);
		SHA256_STEP(h,a,b,c,d,e,f,g,w[57]+0x78a5636f);
		SHA256_STEP(g,h,a,b,c,d,e,f,w[58]+0x84c87814);
		SHA256_STEP(f,g,h,a,b,c,d,e,w[59]+0x8cc70208);
		SHA256_STEP(e,f,g,h,a,b,c,d,w[60]+0x90befffa);
		SHA256_STEP(d,e,f,g,h,a,b,c,w[61]+0xa4506ceb);
		SHA256_STEP(c,d,e,f,g,h,a,b,w[62]+0xbef9a3f7);
		SHA256_STEP(b,c,d,e,f,g,h,a,w[63]+0xc67178f2);
		dt->a+=a;
		dt->b+=b;
		dt->c+=c;
		dt->d+=d;
		dt->e+=e;
		dt->f+=f;
		dt->g+=g;
		dt->h+=h;
	}
}



__SLL_EXTERNAL void sll_hash_sha512(sll_sha512_data_t* dt,void* bf,sll_file_offset_t bfl){
	SLL_ASSERT(!(bfl&127));
	u64 w[80];
	const u64* ptr=(const u64*)bf;
	for (sll_file_offset_t idx=0;idx<(bfl>>7);idx++){
		for (unsigned int j=0;j<16;j++){
			w[j]=SWAP_BYTES64(*ptr);
			ptr++;
		}
		for (unsigned int j=16;j<80;j++){
			w[j]=(ROTATE_BITS_RIGHT64(w[j-2],19)^ROTATE_BITS_RIGHT64(w[j-2],61)^(w[j-2]>>6))+w[j-7]+(ROTATE_BITS_RIGHT64(w[j-15],1)^ROTATE_BITS_RIGHT64(w[j-15],8)^(w[j-15]>>7))+w[j-16];
		}
		u64 a=dt->a;
		u64 b=dt->b;
		u64 c=dt->c;
		u64 d=dt->d;
		u64 e=dt->e;
		u64 f=dt->f;
		u64 g=dt->g;
		u64 h=dt->h;
		u64 t;
		SHA512_STEP(a,b,c,d,e,f,g,h,w[0]+0x428a2f98d728ae22ull);
		SHA512_STEP(h,a,b,c,d,e,f,g,w[1]+0x7137449123ef65cdull);
		SHA512_STEP(g,h,a,b,c,d,e,f,w[2]+0xb5c0fbcfec4d3b2full);
		SHA512_STEP(f,g,h,a,b,c,d,e,w[3]+0xe9b5dba58189dbbcull);
		SHA512_STEP(e,f,g,h,a,b,c,d,w[4]+0x3956c25bf348b538ull);
		SHA512_STEP(d,e,f,g,h,a,b,c,w[5]+0x59f111f1b605d019ull);
		SHA512_STEP(c,d,e,f,g,h,a,b,w[6]+0x923f82a4af194f9bull);
		SHA512_STEP(b,c,d,e,f,g,h,a,w[7]+0xab1c5ed5da6d8118ull);
		SHA512_STEP(a,b,c,d,e,f,g,h,w[8]+0xd807aa98a3030242ull);
		SHA512_STEP(h,a,b,c,d,e,f,g,w[9]+0x12835b0145706fbeull);
		SHA512_STEP(g,h,a,b,c,d,e,f,w[10]+0x243185be4ee4b28cull);
		SHA512_STEP(f,g,h,a,b,c,d,e,w[11]+0x550c7dc3d5ffb4e2ull);
		SHA512_STEP(e,f,g,h,a,b,c,d,w[12]+0x72be5d74f27b896full);
		SHA512_STEP(d,e,f,g,h,a,b,c,w[13]+0x80deb1fe3b1696b1ull);
		SHA512_STEP(c,d,e,f,g,h,a,b,w[14]+0x9bdc06a725c71235ull);
		SHA512_STEP(b,c,d,e,f,g,h,a,w[15]+0xc19bf174cf692694ull);
		SHA512_STEP(a,b,c,d,e,f,g,h,w[16]+0xe49b69c19ef14ad2ull);
		SHA512_STEP(h,a,b,c,d,e,f,g,w[17]+0xefbe4786384f25e3ull);
		SHA512_STEP(g,h,a,b,c,d,e,f,w[18]+0x0fc19dc68b8cd5b5ull);
		SHA512_STEP(f,g,h,a,b,c,d,e,w[19]+0x240ca1cc77ac9c65ull);
		SHA512_STEP(e,f,g,h,a,b,c,d,w[20]+0x2de92c6f592b0275ull);
		SHA512_STEP(d,e,f,g,h,a,b,c,w[21]+0x4a7484aa6ea6e483ull);
		SHA512_STEP(c,d,e,f,g,h,a,b,w[22]+0x5cb0a9dcbd41fbd4ull);
		SHA512_STEP(b,c,d,e,f,g,h,a,w[23]+0x76f988da831153b5ull);
		SHA512_STEP(a,b,c,d,e,f,g,h,w[24]+0x983e5152ee66dfabull);
		SHA512_STEP(h,a,b,c,d,e,f,g,w[25]+0xa831c66d2db43210ull);
		SHA512_STEP(g,h,a,b,c,d,e,f,w[26]+0xb00327c898fb213full);
		SHA512_STEP(f,g,h,a,b,c,d,e,w[27]+0xbf597fc7beef0ee4ull);
		SHA512_STEP(e,f,g,h,a,b,c,d,w[28]+0xc6e00bf33da88fc2ull);
		SHA512_STEP(d,e,f,g,h,a,b,c,w[29]+0xd5a79147930aa725ull);
		SHA512_STEP(c,d,e,f,g,h,a,b,w[30]+0x06ca6351e003826full);
		SHA512_STEP(b,c,d,e,f,g,h,a,w[31]+0x142929670a0e6e70ull);
		SHA512_STEP(a,b,c,d,e,f,g,h,w[32]+0x27b70a8546d22ffcull);
		SHA512_STEP(h,a,b,c,d,e,f,g,w[33]+0x2e1b21385c26c926ull);
		SHA512_STEP(g,h,a,b,c,d,e,f,w[34]+0x4d2c6dfc5ac42aedull);
		SHA512_STEP(f,g,h,a,b,c,d,e,w[35]+0x53380d139d95b3dfull);
		SHA512_STEP(e,f,g,h,a,b,c,d,w[36]+0x650a73548baf63deull);
		SHA512_STEP(d,e,f,g,h,a,b,c,w[37]+0x766a0abb3c77b2a8ull);
		SHA512_STEP(c,d,e,f,g,h,a,b,w[38]+0x81c2c92e47edaee6ull);
		SHA512_STEP(b,c,d,e,f,g,h,a,w[39]+0x92722c851482353bull);
		SHA512_STEP(a,b,c,d,e,f,g,h,w[40]+0xa2bfe8a14cf10364ull);
		SHA512_STEP(h,a,b,c,d,e,f,g,w[41]+0xa81a664bbc423001ull);
		SHA512_STEP(g,h,a,b,c,d,e,f,w[42]+0xc24b8b70d0f89791ull);
		SHA512_STEP(f,g,h,a,b,c,d,e,w[43]+0xc76c51a30654be30ull);
		SHA512_STEP(e,f,g,h,a,b,c,d,w[44]+0xd192e819d6ef5218ull);
		SHA512_STEP(d,e,f,g,h,a,b,c,w[45]+0xd69906245565a910ull);
		SHA512_STEP(c,d,e,f,g,h,a,b,w[46]+0xf40e35855771202aull);
		SHA512_STEP(b,c,d,e,f,g,h,a,w[47]+0x106aa07032bbd1b8ull);
		SHA512_STEP(a,b,c,d,e,f,g,h,w[48]+0x19a4c116b8d2d0c8ull);
		SHA512_STEP(h,a,b,c,d,e,f,g,w[49]+0x1e376c085141ab53ull);
		SHA512_STEP(g,h,a,b,c,d,e,f,w[50]+0x2748774cdf8eeb99ull);
		SHA512_STEP(f,g,h,a,b,c,d,e,w[51]+0x34b0bcb5e19b48a8ull);
		SHA512_STEP(e,f,g,h,a,b,c,d,w[52]+0x391c0cb3c5c95a63ull);
		SHA512_STEP(d,e,f,g,h,a,b,c,w[53]+0x4ed8aa4ae3418acbull);
		SHA512_STEP(c,d,e,f,g,h,a,b,w[54]+0x5b9cca4f7763e373ull);
		SHA512_STEP(b,c,d,e,f,g,h,a,w[55]+0x682e6ff3d6b2b8a3ull);
		SHA512_STEP(a,b,c,d,e,f,g,h,w[56]+0x748f82ee5defb2fcull);
		SHA512_STEP(h,a,b,c,d,e,f,g,w[57]+0x78a5636f43172f60ull);
		SHA512_STEP(g,h,a,b,c,d,e,f,w[58]+0x84c87814a1f0ab72ull);
		SHA512_STEP(f,g,h,a,b,c,d,e,w[59]+0x8cc702081a6439ecull);
		SHA512_STEP(e,f,g,h,a,b,c,d,w[60]+0x90befffa23631e28ull);
		SHA512_STEP(d,e,f,g,h,a,b,c,w[61]+0xa4506cebde82bde9ull);
		SHA512_STEP(c,d,e,f,g,h,a,b,w[62]+0xbef9a3f7b2c67915ull);
		SHA512_STEP(b,c,d,e,f,g,h,a,w[63]+0xc67178f2e372532bull);
		SHA512_STEP(a,b,c,d,e,f,g,h,w[64]+0xca273eceea26619cull);
		SHA512_STEP(h,a,b,c,d,e,f,g,w[65]+0xd186b8c721c0c207ull);
		SHA512_STEP(g,h,a,b,c,d,e,f,w[66]+0xeada7dd6cde0eb1eull);
		SHA512_STEP(f,g,h,a,b,c,d,e,w[67]+0xf57d4f7fee6ed178ull);
		SHA512_STEP(e,f,g,h,a,b,c,d,w[68]+0x06f067aa72176fbaull);
		SHA512_STEP(d,e,f,g,h,a,b,c,w[69]+0x0a637dc5a2c898a6ull);
		SHA512_STEP(c,d,e,f,g,h,a,b,w[70]+0x113f9804bef90daeull);
		SHA512_STEP(b,c,d,e,f,g,h,a,w[71]+0x1b710b35131c471bull);
		SHA512_STEP(a,b,c,d,e,f,g,h,w[72]+0x28db77f523047d84ull);
		SHA512_STEP(h,a,b,c,d,e,f,g,w[73]+0x32caab7b40c72493ull);
		SHA512_STEP(g,h,a,b,c,d,e,f,w[74]+0x3c9ebe0a15c9bebcull);
		SHA512_STEP(f,g,h,a,b,c,d,e,w[75]+0x431d67c49c100d4cull);
		SHA512_STEP(e,f,g,h,a,b,c,d,w[76]+0x4cc5d4becb3e42b6ull);
		SHA512_STEP(d,e,f,g,h,a,b,c,w[77]+0x597f299cfc657e2aull);
		SHA512_STEP(c,d,e,f,g,h,a,b,w[78]+0x5fcb6fab3ad6faecull);
		SHA512_STEP(b,c,d,e,f,g,h,a,w[79]+0x6c44198c4a475817ull);
		dt->a+=a;
		dt->b+=b;
		dt->c+=c;
		dt->d+=d;
		dt->e+=e;
		dt->f+=f;
		dt->g+=g;
		dt->h+=h;
	}
}



__API_FUNC(hash_md5){
	sll_md5_data_t dt={
		(u32)a,
		(u32)b,
		(u32)c,
		(u32)d
	};
	sll_hash_md5(&dt,e->v,e->l);
	sll_array_create(4,out);
	out->v[0]=sll_int_to_object(dt.a);
	out->v[1]=sll_int_to_object(dt.b);
	out->v[2]=sll_int_to_object(dt.c);
	out->v[3]=sll_int_to_object(dt.d);
}



__API_FUNC(hash_sha1){
	sll_sha1_data_t dt={
		(u32)a,
		(u32)b,
		(u32)c,
		(u32)d,
		(u32)e
	};
	sll_hash_sha1(&dt,f->v,f->l);
	sll_array_create(5,out);
	out->v[0]=sll_int_to_object(dt.a);
	out->v[1]=sll_int_to_object(dt.b);
	out->v[2]=sll_int_to_object(dt.c);
	out->v[3]=sll_int_to_object(dt.d);
	out->v[4]=sll_int_to_object(dt.e);
}



__API_FUNC(hash_sha256){
	sll_sha256_data_t dt={
		(u32)a,
		(u32)b,
		(u32)c,
		(u32)d,
		(u32)e,
		(u32)f,
		(u32)g,
		(u32)h
	};
	sll_hash_sha256(&dt,i->v,i->l);
	sll_array_create(8,out);
	out->v[0]=sll_int_to_object(dt.a);
	out->v[1]=sll_int_to_object(dt.b);
	out->v[2]=sll_int_to_object(dt.c);
	out->v[3]=sll_int_to_object(dt.d);
	out->v[4]=sll_int_to_object(dt.e);
	out->v[5]=sll_int_to_object(dt.f);
	out->v[6]=sll_int_to_object(dt.g);
	out->v[7]=sll_int_to_object(dt.h);
}



__API_FUNC(hash_sha512){
	sll_sha512_data_t dt={
		(u64)a,
		(u64)b,
		(u64)c,
		(u64)d,
		(u64)e,
		(u64)f,
		(u64)g,
		(u64)h
	};
	sll_hash_sha512(&dt,i->v,i->l);
	sll_array_create(8,out);
	out->v[0]=sll_int_to_object(dt.a);
	out->v[1]=sll_int_to_object(dt.b);
	out->v[2]=sll_int_to_object(dt.c);
	out->v[3]=sll_int_to_object(dt.d);
	out->v[4]=sll_int_to_object(dt.e);
	out->v[5]=sll_int_to_object(dt.f);
	out->v[6]=sll_int_to_object(dt.g);
	out->v[7]=sll_int_to_object(dt.h);
}

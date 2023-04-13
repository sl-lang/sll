#ifndef __SLL__INTERNAL_API_HASH_H__
#define __SLL__INTERNAL_API_HASH_H__ 1



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



#endif

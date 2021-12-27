#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdint.h>



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

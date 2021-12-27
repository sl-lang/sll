#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <stdint.h>



__API_FUNC(hash_sha1){
	uint32_t av=(uint32_t)a;
	uint32_t bv=(uint32_t)b;
	uint32_t cv=(uint32_t)c;
	uint32_t dv=(uint32_t)d;
	uint32_t ev=(uint32_t)e;
	SLL_ASSERT(!(f->l&63));
	uint32_t W[80];
	const uint32_t* ptr=(const uint32_t*)(f->v);
	for (sll_string_length_t i=0;i<(f->l>>6);i++){
		uint8_t j=0;
		for (;j<16;j++){
			W[j]=SWAP_BYTES(*ptr);
			ptr++;
		}
		for (;j<80;j++){
			W[j]=ROTATE_BITS(W[j-3]^W[j-8]^W[j-14]^W[j-16],1);
		}
		uint32_t da=av;
		uint32_t db=bv;
		uint32_t dc=cv;
		uint32_t dd=dv;
		uint32_t de=ev;
		for (j=0;j<80;j++){
			uint32_t t=de;
			if (j<20){
				t+=(dd^(db&(dc^dd)))+0x5a827999;
			}
			else if (j<40){
				t+=(db^dc^dd)+0x6ed9eba1;
			}
			else if (j<60){
				t+=((db&dc)|(db&dd)|(dc&dd))+0x8f1bbcdc;
			}
			else{
				t+=(db^dc^dd)+0xca62c1d6;
			}
			de=dd;
			dd=dc;
			dc=ROTATE_BITS(db,30);
			db=da;
			da=ROTATE_BITS(da,5)+t+W[j];
		}
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

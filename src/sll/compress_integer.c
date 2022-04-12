#include <sll/_internal/intrinsics.h>
#include <sll/common.h>
#include <sll/_size_types.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_compress_integer(sll_integer_t n){
	sll_integer_t v=n;
	__SLL_U32 sgn=0;
	if (v<0){
		v=-v;
		sgn=1;
	}
	__SLL_S32 off=0;
	__SLL_U32 s=FIND_LAST_SET_BIT((__SLL_U64)v);
	if (v&(v-1)){
		sll_size_t pw=1ull<<s;
		__SLL_S32 n_pw=(__SLL_S32)((pw<<1)-v);
		off=(__SLL_S32)(v-pw);
		if (n_pw<off){
			off=n_pw;
			sgn|=2;
			s++;
		}
		if (off>>24){
			return n;
		}
	}
	sll_compressed_integer_t ci=s|(sgn<<6)|(off<<8);
	return (ci>v?n:(sll_integer_t)ci);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_integer_t sll_decompress_integer(sll_compressed_integer_t v){
	sll_integer_t off=v>>8;
	if (v&128){
		off=-off;
	}
	sll_integer_t o=(1ull<<(v&63))+off;
	if (v&64){
		o=-o;
	}
	return o;
}

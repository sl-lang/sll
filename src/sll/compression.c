#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_compress_integer(sll_size_t v){
	if (v<128){
		return v;
	}
	sll_size_t off=0;
	__SLL_U32 s=FIND_LAST_SET_BIT(v);
	SLL_ASSERT(s>=7);
	if (v&(v-1)){
		__SLL_U32 m=s-1;
		sll_size_t n=1ull<<s;
		if ((3ull<<m)<v){
			s++;
			off=(((n<<1)-v)<<1)-1;
		}
		else{
			off=(v-n)<<1;
		}
		if (off>>58||FIND_LAST_SET_BIT(off)>=m-5){
			return v;
		}
	}
	return (s==64?0:s-6)|(off<<6);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_decompress_integer(sll_compressed_integer_t v){
	sll_size_t o=v>>7;
	if (v&64){
		o=~o;
	}
	sll_size_t s=v&63;
	if (s){
		o+=1ull<<(s+6);
	}
	return o;
}

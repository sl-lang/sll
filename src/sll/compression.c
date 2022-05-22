#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/compression.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compressed_integer_t sll_compress_integer(sll_size_t int_){
	if (int_<128){
		return int_;
	}
	sll_size_t off=0;
	__SLL_U32 s=FIND_LAST_SET_BIT(int_);
	SLL_ASSERT(s>=7);
	if (int_&(int_-1)){
		__SLL_U32 m=s-1;
		sll_size_t n=1ull<<s;
		if ((3ull<<m)<int_){
			s++;
			off=(((n<<1)-int_)<<1)-1;
		}
		else{
			off=(int_-n)<<1;
		}
		if (off>>58||FIND_LAST_SET_BIT(off)>=m-5){
			return int_;
		}
	}
	return (s==64?0:s-6)|(off<<6);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_decompress_integer(sll_compressed_integer_t compressed_int){
	sll_size_t o=compressed_int>>7;
	if (compressed_int&64){
		o=~o;
	}
	sll_size_t s=compressed_int&63;
	if (s){
		o+=1ull<<(s+6);
	}
	return o;
}

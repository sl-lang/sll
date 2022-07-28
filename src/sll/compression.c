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
	__SLL_U32 shift=FIND_LAST_SET_BIT(int_);
	SLL_ASSERT(shift>=7);
	if (int_&(int_-1)){
		__SLL_U32 new_shift=shift-1;
		sll_size_t n=1ull<<shift;
		if ((3ull<<new_shift)<int_){
			shift++;
			off=(((n<<1)-int_)<<1)-1;
		}
		else{
			off=(int_-n)<<1;
		}
		if (off>>58||FIND_LAST_SET_BIT(off)>=new_shift-5){
			return int_;
		}
	}
	return (shift==64?0:shift-6)|(off<<6);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_decompress_integer(sll_compressed_integer_t compressed_int){
	sll_size_t out=compressed_int>>7;
	if (compressed_int&64){
		out=~out;
	}
	sll_size_t shift=compressed_int&63;
	if (shift){
		out+=1ull<<(shift+6);
	}
	return out;
}

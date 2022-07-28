#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/_internal/util.h>
#include <sll/common.h>
#include <sll/platform/util.h>
#include <sll/sandbox.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_random_get_float(sll_float_t min,sll_float_t max){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_RANDOM)){
		return 0;
	}
	if (max<min){
		sll_float_t tmp=min;
		min=max;
		max=tmp;
	}
	else if (min==max){
		return min;
	}
	sll_size_t v;
	SLL_RANDOM_BITS(v);
	v&=0xfffffffffffffull;
	sll_size_t l=FIND_LAST_SET_BIT(v);
	f64_data_t out={
		.data=((l+971)<<52)|((v<<(52-l))&0xfffffffffffffull)
	};
	return out.value*(max-min)+min;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_random_get_int(sll_integer_t min,sll_integer_t max){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_RANDOM)){
		return 0;
	}
	if (max<min){
		sll_integer_t tmp=min;
		min=max;
		max=tmp;
	}
	else if (min==max){
		return min;
	}
	sll_size_t v=(sll_size_t)(max-min);
	sll_size_t m=0xffffffffffffffffull>>(63-FIND_LAST_SET_BIT(v));
	sll_size_t out;
	do{
		SLL_RANDOM_BITS(out);
		out&=m;
	} while (out>v);
	return min+out;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_random_get_string(sll_string_length_t len,sll_char_t min,sll_char_t max,sll_string_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_RANDOM)){
		SLL_INIT_STRING(out);
		return;
	}
	sll_string_create(len,out);
	if (max<min){
		sll_char_t tmp=min;
		min=max;
		max=tmp;
	}
	else if (min==max){
		sll_set_memory(out->data,len,min);
		sll_string_calculate_checksum(out);
		return;
	}
	sll_platform_random(out->data,len);
	if (!min&&max==255){
		return;
	}
	for (sll_string_length_t i=0;i<len;i++){
		while (out->data[i]<min||out->data[i]>max){
			SLL_RANDOM_BITS(out->data[i]);
		}
	}
	sll_string_calculate_checksum(out);
}

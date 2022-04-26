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
	sll_size_t v;
	sll_platform_random(&v,sizeof(sll_size_t));
	v&=0xfffffffffffffull;
	sll_size_t l=FIND_LAST_SET_BIT(v);
	f64_data_t o={
		.dt=((l+971)<<52)|((v<<(52-l))&0xfffffffffffffull)
	};
	return o.v*(max-min)+min;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_random_get_int(sll_integer_t min,sll_integer_t max){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_RANDOM)){
		return 0;
	}
	SLL_ASSERT(min<max);
	sll_size_t v=(sll_size_t)(max-min);
	sll_size_t m=0xffffffffffffffffull>>(63-FIND_LAST_SET_BIT(v));
	sll_size_t o;
	do{
		sll_platform_random(&o,sizeof(sll_size_t));
		o&=m;
	} while (o>v);
	return min+o;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_random_get_string(sll_string_length_t len,sll_char_t min,sll_char_t max,sll_string_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_RANDOM)){
		SLL_INIT_STRING(out);
		return;
	}
	SLL_ASSERT(min<max);
	sll_string_create(len,out);
	sll_platform_random(out->v,len);
	if (!min&&max==255){
		return;
	}
	for (sll_string_length_t i=0;i<len;i++){
		while (out->v[i]<min||out->v[i]>max){
			sll_platform_random(out->v+i,sizeof(sll_char_t));
		}
	}
	sll_string_calculate_checksum(out);
}

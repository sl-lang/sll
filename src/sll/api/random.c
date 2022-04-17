#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/_internal/util.h>
#include <sll/common.h>
#include <sll/platform/util.h>
#include <sll/sandbox.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_random_get_float(sll_float_t a,sll_float_t b){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_RANDOM)){
		return 0;
	}
	sll_size_t v;
	sll_platform_random(&v,sizeof(sll_size_t));
	v&=0xfffffffffffffull;
	sll_size_t l=FIND_LAST_SET_BIT(v);
	double_data_t o={
		.dt=((l+971)<<52)|((v<<(52-l))&0xfffffffffffffull)
	};
	return o.v*(b-a)+a;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_random_get_int(sll_integer_t a,sll_integer_t b){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_RANDOM)){
		return 0;
	}
	SLL_ASSERT(a<b);
	sll_size_t v=(sll_size_t)(b-a);
	sll_size_t m=0xffffffffffffffffull>>(63-FIND_LAST_SET_BIT(v));
	sll_size_t o;
	do{
		sll_platform_random(&o,sizeof(sll_size_t));
		o&=m;
	} while (o>v);
	return a+o;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_random_get_string(sll_integer_t a,sll_char_t b,sll_char_t c,sll_string_t* out){
	if (sll_get_sandbox_flag(SLL_SANDBOX_FLAG_DISABLE_RANDOM)){
		SLL_INIT_STRING(out);
		return;
	}
	SLL_ASSERT(b<c);
	sll_string_length_t l=(sll_string_length_t)a;
	sll_string_create(l,out);
	sll_platform_random(out->v,l);
	if (!b&&c==255){
		return;
	}
	for (sll_string_length_t i=0;i<l;i++){
		while (out->v[i]<b||out->v[i]>c){
			sll_platform_random(out->v+i,sizeof(sll_char_t));
		}
	}
	sll_string_calculate_checksum(out);
}

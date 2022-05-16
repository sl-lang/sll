#include <sll/_internal/util.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_struct_double_from_bits(__SLL_U64 v){
	f64_data_t dt={
		.dt=v
	};
	return dt.v;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_struct_float_from_bits(__SLL_U32 v){
	f32_data_t dt={
		.dt=v
	};
	return dt.v;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_struct_double_to_bits(sll_float_t v){
	f64_data_t dt={
		.v=v
	};
	return dt.dt;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_struct_float_to_bits(sll_float_t v){
	f32_data_t dt={
		.v=(__SLL_F32)v
	};
	return dt.dt;
}

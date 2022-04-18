#include <sll/_internal/util.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_struct_float_from_bits(__SLL_U32 v){
	float_data_t dt={
		.dt=v
	};
	return dt.v;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_struct_double_from_bits(__SLL_U64 v){
	double_data_t dt={
		.dt=v
	};
	return dt.v;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_struct_float_to_bits(sll_float_t v){
	float_data_t dt={
		.v=(float)v
	};
	return dt.dt;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_struct_double_to_bits(sll_float_t v){
	double_data_t dt={
		.v=v
	};
	return dt.dt;
}

#include <sll/_internal/util.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_struct_float_from_bits(sll_integer_t a){
	float_data_t dt={
		.dt=(__SLL_U32)a
	};
	return dt.v;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_struct_double_from_bits(sll_integer_t a){
	double_data_t dt={
		.dt=a
	};
	return dt.v;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_struct_float_to_bits(sll_float_t a){
	float_data_t dt={
		.v=(float)a
	};
	return dt.dt;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_struct_double_to_bits(sll_float_t a){
	double_data_t dt={
		.v=a
	};
	return dt.dt;
}

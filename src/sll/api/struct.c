#include <sll/_internal/util.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_struct_double_from_bits(__SLL_U64 value){
	f64_data_t dt={
		.data=value
	};
	return dt.value;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_struct_float_from_bits(__SLL_U32 value){
	f32_data_t dt={
		.data=value
	};
	return dt.value;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_struct_double_to_bits(sll_float_t value){
	f64_data_t dt={
		.value=value
	};
	return dt.data;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_struct_float_to_bits(sll_float_t value){
	f32_data_t dt={
		.value=(__SLL_F32)value
	};
	return dt.data;
}

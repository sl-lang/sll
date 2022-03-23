#include <sll/_internal/api.h>
#include <sll/_internal/util.h>
#include <sll/_size_types.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



__API_FUNC(struct_float_from_bits){
	float_data_t dt={
		.dt=(__SLL_U32)a
	};
	return dt.v;
}



__API_FUNC(struct_double_from_bits){
	double_data_t dt={
		.dt=a
	};
	return dt.v;
}



__API_FUNC(struct_float_to_bits){
	float_data_t dt={
		.v=(float)a
	};
	return dt.dt;
}



__API_FUNC(struct_double_to_bits){
	double_data_t dt={
		.v=a
	};
	return dt.dt;
}

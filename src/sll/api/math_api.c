#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
#include <math.h>



__API_FUNC(math_abs){
	float_data_t dt={
		.v=a
	};
	dt.dt&=0x7fffffffffffffffull;
	return dt.v;
}



__API_FUNC(math_ceil){
	return ceil(a);
}



__API_FUNC(math_copy_sign){
	float_data_t dt_a={
		.v=a
	};
	float_data_t dt_b={
		.v=b
	};
	dt_b.dt=(dt_b.dt&0x7fffffffffffffffull)|(dt_a.dt&0x8000000000000000ull);
	return dt_b.v;
}



__API_FUNC(math_floor){
	return floor(a);
}



__API_FUNC(math_round){
	return round(a);
}

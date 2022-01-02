#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/math.h>
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



__API_FUNC(math_acos){
	return acos(a);
}



__API_FUNC(math_acosh){
	return acosh(a);
}



__API_FUNC(math_asin){
	return asin(a);
}



__API_FUNC(math_asinh){
	return asinh(a);
}



__API_FUNC(math_atan){
	return atan(a);
}



__API_FUNC(math_atan2){
	return atan2(a,b);
}



__API_FUNC(math_atanh){
	return atanh(a);
}



__API_FUNC(math_cbrt){
	return cbrt(a);
}



__API_FUNC(math_combinations){
	if (b>a||b<=0){
		return 0;
	}
	if (a==b){
		return 1;
	}
	if (b==1){
		return 1;
	}
	uint64_t av=(sll_integer_t)a;
	uint64_t bv=(sll_integer_t)b;
	uint64_t o=1;
	uint64_t i=a-b;
	uint64_t j=2;
	do{
		i++;
		o*=i;
		if (j&&!(o%j)){
			o/=j;
			j=(j==b?0:j+1);
		}
	} while (i<av);
	if (j){
		while (j<=bv){
			o/=j;
			j++;
		}
	}
	return o;
}



__API_FUNC(math_cos){
	return cos(a);
}



__API_FUNC(math_cosh){
	return cosh(a);
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



__API_FUNC(math_factorial){
	if (a<0){
		SLL_UNIMPLEMENTED();
	}
	uint64_t o=1;
	while (a>1){
		o*=a;
		a--;
	}
	return o;
}



__API_FUNC(math_floor){
	return floor(a);
}



__API_FUNC(math_gcd){
	uint64_t t;
	while (b){
		t=a%b;
		a=b;
		b=t;
	}
	return a;
}



__API_FUNC(math_int_log2){
	if (a<0){
		SLL_UNIMPLEMENTED();
	}
	return FIND_FIRST_SET_BIT(a);
}



__API_FUNC(math_int_pow){
	if (b<0){
		SLL_UNIMPLEMENTED();
	}
	sll_integer_t o=1;
	if (a<0){
		a=-a;
		if (b&1){
			o=-1;
		}
	}
	uint64_t v=(uint64_t)a;
	uint64_t e=(uint64_t)b;
	while (1){
		if (e&1){
			o*=v;
		}
		e>>=1;
		if (!e){
			break;
		}
		v*=v;
	}
	return o;
}



__API_FUNC(math_int_sqrt){
	if (a<0){
		SLL_UNIMPLEMENTED();
	}
	uint64_t v=(uint64_t)a;
	uint64_t i=v>>1;
	if (!i){
		return v;
	}
	uint64_t j=i;
	do{
		i=j;
		j=(i+v/i)>>1;
	} while (j<i);
	return i;
}



__API_FUNC(math_log){
	return log(a);
}



__API_FUNC(math_log2){
	return log2(a);
}



__API_FUNC(math_log10){
	return log10(a);
}



__API_FUNC(math_permutations){
	if (b>a||b<=0){
		return 0;
	}
	if (a==b){
		return sll_api_math_factorial(a);
	}
	if (b==1){
		return a;
	}
	uint64_t v=(sll_integer_t)a;
	uint64_t o=1;
	uint64_t i=a-b;
	do{
		i++;
		o*=i;
	} while (i<v);
	return o;
}



__API_FUNC(math_pow){
	return pow(a,b);
}



__API_FUNC(math_round){
	return round(a);
}



__API_FUNC(math_sin){
	return sin(a);
}



__API_FUNC(math_sinh){
	return sinh(a);
}



__API_FUNC(math_sqrt){
	return sqrt(a);
}



__API_FUNC(math_tan){
	return tan(a);
}



__API_FUNC(math_tanh){
	return tanh(a);
}

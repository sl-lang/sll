#include <sll/_internal/api.h>
#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/_internal/util.h>
#include <sll/api.h>
#include <sll/api/math.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <math.h>



#define PUSH_FACTOR(v,c) \
	do{ \
		i++; \
		o=sll_reallocate(o,i*sizeof(sll_factor_t)); \
		(o+i-1)->n=v; \
		(o+i-1)->pw=c; \
	} while (0)
#define WHEEL_STEP(x) \
	if (!(n%f)){ \
		j=0; \
		do{ \
			j++; \
			n/=f; \
		} while (!(n%f)); \
		PUSH_FACTOR(f,j); \
	} \
	f+=x; \
	if (f*f>n){ \
		break; \
	}
#define WHEEL_STEP_EULER(x) \
	if (!(n%f)){ \
		do{ \
			n/=f; \
		} while (!(n%f)); \
		o-=o/f; \
	} \
	f+=x; \
	if (f*f>n){ \
		break; \
	}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_math_euler_phi(sll_size_t n){
	if (n<2){
		return 0;
	}
	sll_size_t o=n;
	sll_size_t c=FIND_FIRST_SET_BIT(n);
	if (c){
		n>>=c;
		o>>=1;
	}
	if (!(n%3)){
		do{
			n/=3;
		} while (!(n%3));
		o-=o/3;
	}
	if (!(n%5)){
		do{
			n/=5;
		} while (!(n%5));
		o-=o/5;
	}
	sll_size_t f=7;
	if (n>48){
		while (1){
			WHEEL_STEP_EULER(4);
			WHEEL_STEP_EULER(2);
			WHEEL_STEP_EULER(4);
			WHEEL_STEP_EULER(2);
			WHEEL_STEP_EULER(4);
			WHEEL_STEP_EULER(6);
			WHEEL_STEP_EULER(2);
			WHEEL_STEP_EULER(6);
		}
	}
	if (n!=1){
		o-=o/n;
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_factor_t* sll_math_factors(sll_integer_t v,sll_array_length_t* ol){
	sll_factor_t* o=sll_allocate_stack(1);
	sll_array_length_t i=0;
	if (v<0){
		v=-v;
		i=1;
		o=sll_reallocate(o,sizeof(sll_factor_t));
		o->n=-1;
		o->pw=1;
	}
	if (v<2){
		*ol=i;
		if (i){
			return o;
		}
		sll_deallocate(o);
		return NULL;
	}
	sll_size_t n=v;
	sll_size_t j=FIND_FIRST_SET_BIT(n);
	if (j){
		n>>=j;
		PUSH_FACTOR(2,j);
	}
	if (!(n%3)){
		j=0;
		do{
			j++;
			n/=3;
		} while (!(n%3));
		PUSH_FACTOR(3,j);
	}
	if (!(n%5)){
		j=0;
		do{
			j++;
			n/=5;
		} while (!(n%5));
		PUSH_FACTOR(5,j);
	}
	sll_size_t f=7;
	if (n>48){
		while (1){
			WHEEL_STEP(4);
			WHEEL_STEP(2);
			WHEEL_STEP(4);
			WHEEL_STEP(2);
			WHEEL_STEP(4);
			WHEEL_STEP(6);
			WHEEL_STEP(2);
			WHEEL_STEP(6);
		}
	}
	if (n!=1){
		PUSH_FACTOR(n,1);
	}
	*ol=i;
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_math_mod(sll_float_t a,sll_float_t b){
	return fmod(a,b);
}



__API_FUNC(math_abs){
	double_data_t dt={
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
	__SLL_U64 av=(__SLL_U64)a;
	__SLL_U64 bv=(__SLL_U64)b;
	__SLL_U64 o=1;
	__SLL_U64 i=a-b;
	__SLL_U64 j=2;
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
	return (sll_integer_t)ceil(a);
}



__API_FUNC(math_copy_sign){
	double_data_t dt_a={
		.v=a
	};
	double_data_t dt_b={
		.v=b
	};
	dt_b.dt=(dt_b.dt&0x7fffffffffffffffull)|(dt_a.dt&0x8000000000000000ull);
	return dt_b.v;
}



__API_FUNC(math_euler_phi){
	return (a<0?0:sll_math_euler_phi((sll_size_t)a));
}



__API_FUNC(math_exp){
	return exp(a);
}



__API_FUNC(math_factorial){
	if (a<0){
		SLL_UNIMPLEMENTED();
	}
	__SLL_U64 o=1;
	while (a>1){
		o*=a;
		a--;
	}
	return o;
}



__API_FUNC(math_factors){
	sll_array_length_t l;
	sll_factor_t* dt=sll_math_factors(a,&l);
	sll_object_t* o=sll_new_object(SLL_CHAR("{ii}"),dt,l,sizeof(sll_factor_t),SLL_OFFSETOF(sll_factor_t,n),SLL_OFFSETOF(sll_factor_t,pw));
	sll_deallocate(dt);
	return o;
}



__API_FUNC(math_floor){
	return (sll_integer_t)floor(a);
}



__API_FUNC(math_gcd){
	while (b){
		sll_integer_t t=a%b;
		a=b;
		b=t;
	}
	return a;
}



__API_FUNC(math_int_log2){
	if (a<0){
		SLL_UNIMPLEMENTED();
	}
	return FIND_LAST_SET_BIT(a);
}



__API_FUNC(math_int_pow){
	if (b<0||c<0){
		SLL_UNIMPLEMENTED();
	}
	if (!b){
		return 1;
	}
	__SLL_U64 mod=(c?c:__SLL_U64_MAX);
	if (mod==1){
		return 0;
	}
	__SLL_U64 o=1;
	if (a<0){
		a=-a;
		if (b&1){
			o=-1;
		}
	}
	__SLL_U64 v=((__SLL_U64)a)%mod;
	__SLL_U64 e=(__SLL_U64)b;
	while (1){
		if (e&1){
			o=(o*v)%mod;
		}
		e>>=1;
		if (!e){
			break;
		}
		v=(v*v)%mod;
	}
	return (sll_integer_t)o;
}



__API_FUNC(math_int_sqrt){
	if (a<0){
		SLL_UNIMPLEMENTED();
	}
	__SLL_U64 v=(__SLL_U64)a;
	__SLL_U64 i=v>>1;
	if (!i){
		return v;
	}
	__SLL_U64 j=i;
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
	__SLL_U64 v=(__SLL_U64)a;
	__SLL_U64 o=1;
	__SLL_U64 i=a-b;
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
	return (sll_integer_t)round(a);
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

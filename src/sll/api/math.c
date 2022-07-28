#include <sll/_internal/api/math.h>
#include <sll/_internal/common.h>
#include <sll/_internal/complex.h>
#include <sll/_internal/intrinsics.h>
#include <sll/_internal/parse_args.h>
#include <sll/_internal/util.h>
#include <sll/api/math.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/complex.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/new_object.h>
#include <sll/static_object.h>
#include <sll/types.h>
#include <math.h>



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_abs(const sll_number_t* a,sll_number_t* out){
	if (a->type==SLL_PARSE_ARGS_TYPE_INT){
		out->type=SLL_PARSE_ARGS_TYPE_INT;
		out->data.int_=(a->data.int_<0?-a->data.int_:a->data.int_);
		return;
	}
	out->type=SLL_PARSE_ARGS_TYPE_FLOAT;
	if (a->type==SLL_PARSE_ARGS_TYPE_COMPLEX){
		out->data.float_=COMPLEX_ABS(a->data.complex_);
	}
	else{
		f64_data_t dt={
			.value=a->data.float_
		};
		dt.data&=0x7fffffffffffffffull;
		out->data.float_=dt.value;
	}
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_acos(sll_float_t a){
	return acos(a);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_acosh(sll_float_t a){
	return acosh(a);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_asin(sll_float_t a){
	return asin(a);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_asinh(sll_float_t a){
	return asinh(a);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_atan(sll_float_t a){
	return atan(a);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_atan2(sll_float_t y,sll_float_t x){
	return atan2(y,x);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_atanh(sll_float_t a){
	return atanh(a);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_cbrt(sll_float_t a){
	return cbrt(a);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_ceil(const sll_number_t* a,sll_number_t* out){
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->type=SLL_PARSE_ARGS_TYPE_INT;
		out->data.int_=(sll_integer_t)ceil(a->data.float_);
	}
	else{
		out->type=SLL_PARSE_ARGS_TYPE_COMPLEX;
		out->data.complex_.real=ceil(a->data.complex_.real);
		out->data.complex_.imag=ceil(a->data.complex_.imag);
	}
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_math_combinations(sll_size_t a,sll_size_t b){
	if (b>a){
		return 0;
	}
	if (a==b){
		return 1;
	}
	if (b==1){
		return 1;
	}
	sll_size_t out=1;
	sll_size_t i=a-b;
	sll_size_t j=2;
	do{
		i++;
		out*=i;
		if (j&&!(out%j)){
			out/=j;
			j=(j==b?0:j+1);
		}
	} while (i<a);
	if (j){
		while (j<=b){
			out/=j;
			j++;
		}
	}
	return out;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_copy_sign(const sll_number_t* a,const sll_number_t* b,sll_number_t* out){
	out->type=a->type;
	switch (PARSE_ARGS_TYPE_COMBINE(a->type,b->type)){
		case PARSE_ARGS_TYPE_COMBINE(SLL_PARSE_ARGS_TYPE_INT,SLL_PARSE_ARGS_TYPE_INT):
		case PARSE_ARGS_TYPE_COMBINE(SLL_PARSE_ARGS_TYPE_INT,SLL_PARSE_ARGS_TYPE_FLOAT):
		case PARSE_ARGS_TYPE_COMBINE(SLL_PARSE_ARGS_TYPE_INT,SLL_PARSE_ARGS_TYPE_COMPLEX):
			{
				sll_integer_t neg=((a->data.int_^b->data.int_)<0);
				out->data.int_=(a->data.int_^(-neg))+neg;
				break;
			}
		case PARSE_ARGS_TYPE_COMBINE(SLL_PARSE_ARGS_TYPE_FLOAT,SLL_PARSE_ARGS_TYPE_INT):
			SLL_UNIMPLEMENTED();
		case PARSE_ARGS_TYPE_COMBINE(SLL_PARSE_ARGS_TYPE_FLOAT,SLL_PARSE_ARGS_TYPE_FLOAT):
			{
				f64_data_t dt_a={
					.value=a->data.float_
				};
				f64_data_t dt_b={
					.value=b->data.float_
				};
				dt_a.data=(dt_a.data&0x7fffffffffffffffull)|(dt_b.data&0x8000000000000000ull);
				out->data.float_=dt_a.value;
				break;
			}
		case PARSE_ARGS_TYPE_COMBINE(SLL_PARSE_ARGS_TYPE_FLOAT,SLL_PARSE_ARGS_TYPE_COMPLEX):
			SLL_UNIMPLEMENTED();
		case PARSE_ARGS_TYPE_COMBINE(SLL_PARSE_ARGS_TYPE_COMPLEX,SLL_PARSE_ARGS_TYPE_INT):
			SLL_UNIMPLEMENTED();
		case PARSE_ARGS_TYPE_COMBINE(SLL_PARSE_ARGS_TYPE_COMPLEX,SLL_PARSE_ARGS_TYPE_FLOAT):
			SLL_UNIMPLEMENTED();
		case PARSE_ARGS_TYPE_COMBINE(SLL_PARSE_ARGS_TYPE_COMPLEX,SLL_PARSE_ARGS_TYPE_COMPLEX):
			{
				f64_data_t dt_a={
					.value=a->data.complex_.real
				};
				f64_data_t dt_b={
					.value=b->data.complex_.real
				};
				dt_a.data=(dt_a.data&0x7fffffffffffffffull)|(dt_b.data&0x8000000000000000ull);
				out->data.complex_.real=dt_a.value;
				dt_a.value=a->data.complex_.imag;
				dt_b.value=b->data.complex_.imag;
				dt_a.data=(dt_a.data&0x7fffffffffffffffull)|(dt_b.data&0x8000000000000000ull);
				out->data.complex_.imag=dt_a.value;
				break;
			}
		default:
			SLL_UNREACHABLE();
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_cos(const sll_number_t* a,sll_number_t* out){
	out->type=a->type;
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->data.float_=cos(a->data.float_);
	}
	else{
		out->data.complex_.real=cos(a->data.complex_.real)*cosh(a->data.complex_.imag);
		out->data.complex_.imag=sin(a->data.complex_.real)*sinh(a->data.complex_.imag);
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_cosh(const sll_number_t* a,sll_number_t* out){
	out->type=a->type;
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->data.float_=cosh(a->data.float_);
	}
	else{
		out->data.complex_.real=cosh(a->data.complex_.real)*cos(a->data.complex_.imag);
		out->data.complex_.imag=sinh(a->data.complex_.real)*sin(a->data.complex_.imag);
	}
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_math_euler_phi(sll_size_t n){
	if (n<2){
		return 0;
	}
	sll_size_t out=n;
	sll_size_t c=FIND_FIRST_SET_BIT(n);
	if (c){
		n>>=c;
		out>>=1;
	}
	if (!(n%3)){
		do{
			n/=3;
		} while (!(n%3));
		out-=out/3;
	}
	if (!(n%5)){
		do{
			n/=5;
		} while (!(n%5));
		out-=out/5;
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
		out-=out/n;
	}
	return out;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_exp(const sll_number_t* a,sll_number_t* out){
	out->type=a->type;
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->data.float_=exp(a->data.float_);
	}
	else{
		sll_complex_exp(&(a->data.complex_),&(out->data.complex_));
	}
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_math_factorial(sll_size_t a){
	sll_size_t out=1;
	while (a>1){
		out*=a;
		a--;
	}
	return out;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_factors(sll_size_t a,sll_array_t* out){
	sll_array_length_t l;
	sll_factor_t* dt=sll_math_factors(a,&l);
	sll_object_t object=sll_new_object(SLL_CHAR("{ii}"),dt,l,sizeof(sll_factor_t),SLL_OFFSETOF(sll_factor_t,number),SLL_OFFSETOF(sll_factor_t,power));
	sll_deallocate(dt);
	*out=object->data.array;
	SLL_CRITICAL(sll_destroy_object(object));
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_floor(const sll_number_t* a,sll_number_t* out){
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->type=SLL_PARSE_ARGS_TYPE_INT;
		out->data.int_=(sll_integer_t)floor(a->data.float_);
	}
	else{
		out->type=SLL_PARSE_ARGS_TYPE_COMPLEX;
		out->data.complex_.real=floor(a->data.complex_.real);
		out->data.complex_.imag=floor(a->data.complex_.imag);
	}
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_math_gcd(sll_integer_t a,sll_integer_t b){
	while (b){
		sll_integer_t t=a%b;
		a=b;
		b=t;
	}
	return a;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_math_int_log2(sll_size_t a){
	return FIND_LAST_SET_BIT(a);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_math_int_pow(sll_integer_t a,sll_size_t b,sll_size_t c){
	if (!b){
		return 1;
	}
	if (c==1){
		return 0;
	}
	sll_integer_t out=1;
	if (a<0){
		a=-a;
		if (b&1){
			out=-1;
		}
	}
	if (!c){
		while (1){
			if (b&1){
				out*=a;
			}
			b>>=1;
			if (!b){
				break;
			}
			a*=a;
		}
	}
	else{
		a%=c;
		while (1){
			if (b&1){
				out=(out*a)%c;
			}
			b>>=1;
			if (!b){
				break;
			}
			a=(a*a)%c;
		}
	}
	return out;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_math_int_sqrt(sll_size_t v){
	sll_size_t i=v>>1;
	if (!i){
		return v;
	}
	sll_size_t j=i;
	do{
		i=j;
		j=(i+v/i)>>1;
	} while (j<i);
	return i;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_log(const sll_number_t* a,sll_number_t* out){
	out->type=a->type;
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->data.float_=log(a->data.float_);
	}
	else{
		sll_complex_log(&(a->data.complex_),&(out->data.complex_));
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_log10(const sll_number_t* a,sll_number_t* out){
	out->type=a->type;
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->data.float_=log10(a->data.float_);
	}
	else{
		sll_complex_t tmp;
		sll_complex_log(&(a->data.complex_),&tmp);
		out->data.complex_=COMPLEX_DIV_FLOAT(tmp,2.302585092994046);
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_log2(const sll_number_t* a,sll_number_t* out){
	out->type=a->type;
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->data.float_=log2(a->data.float_);
	}
	else{
		sll_complex_t tmp;
		sll_complex_log(&(a->data.complex_),&tmp);
		out->data.complex_=COMPLEX_DIV_FLOAT(tmp,0.6931471805599453);
	}
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_math_permutations(sll_size_t a,sll_size_t b){
	if (b>a){
		return 0;
	}
	if (a==b){
		return sll_api_math_factorial(a);
	}
	if (b==1){
		return a;
	}
	sll_size_t out=1;
	sll_size_t i=a-b;
	do{
		i++;
		out*=i;
	} while (i<a);
	return out;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_pow(const sll_number_t* a,const sll_number_t* b,sll_number_t* out){
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT&&b->type==SLL_OBJECT_TYPE_FLOAT){
		out->type=SLL_PARSE_ARGS_TYPE_FLOAT;
		out->data.float_=pow(a->data.float_,b->data.float_);
	}
	else{
		out->type=SLL_PARSE_ARGS_TYPE_COMPLEX;
		if (a->type==SLL_PARSE_ARGS_TYPE_COMPLEX){
			if (b->type==SLL_PARSE_ARGS_TYPE_COMPLEX){
				sll_complex_pow(&(a->data.complex_),&(b->data.complex_),&(out->data.complex_));
			}
			else{
				sll_complex_pow_float(&(a->data.complex_),b->data.float_,&(out->data.complex_));
			}
		}
		else{
			sll_complex_t tmp={
				a->data.float_,
				0
			};
			sll_complex_pow(&tmp,&(b->data.complex_),&(out->data.complex_));
		}
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_round(const sll_number_t* a,sll_number_t* out){
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->type=SLL_PARSE_ARGS_TYPE_INT;
		out->data.int_=(sll_integer_t)round(a->data.float_);
	}
	else{
		out->type=SLL_PARSE_ARGS_TYPE_COMPLEX;
		out->data.complex_.real=round(a->data.complex_.real);
		out->data.complex_.imag=round(a->data.complex_.imag);
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_sin(const sll_number_t* a,sll_number_t* out){
	out->type=a->type;
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->data.float_=sin(a->data.float_);
	}
	else{
		out->data.complex_.real=sin(a->data.complex_.real)*cosh(a->data.complex_.imag);
		out->data.complex_.imag=cos(a->data.complex_.real)*sinh(a->data.complex_.imag);
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_sinh(const sll_number_t* a,sll_number_t* out){
	out->type=a->type;
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->data.float_=sinh(a->data.float_);
	}
	else{
		out->data.complex_.real=sinh(a->data.complex_.real)*cos(a->data.complex_.imag);
		out->data.complex_.imag=cosh(a->data.complex_.real)*sin(a->data.complex_.imag);
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_sqrt(const sll_number_t* a,sll_number_t* out){
	out->type=a->type;
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->data.float_=sqrt(a->data.float_);
	}
	else{
		sll_float_t x=(hypot(a->data.complex_.real,a->data.complex_.imag)+a->data.complex_.real)/2;
		out->data.complex_.real=sqrt(x);
		out->data.complex_.imag=sll_math_copy_sign(sqrt(x-a->data.complex_.real),a->data.complex_.imag);
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_tan(const sll_number_t* a,sll_number_t* out){
	out->type=a->type;
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->data.float_=tan(a->data.float_);
	}
	else{
		sll_float_t t=tan(a->data.complex_.real);
		sll_float_t th=tanh(a->data.complex_.imag);
		sll_complex_t num={
			t,
			th
		};
		sll_complex_t denom={
			1,
			-t*th
		};
		out->data.complex_=COMPLEX_DIV(num,denom);
	}
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_tanh(const sll_number_t* a,sll_number_t* out){
	out->type=a->type;
	if (a->type==SLL_PARSE_ARGS_TYPE_FLOAT){
		out->data.float_=tanh(a->data.float_);
	}
	else{
		sll_float_t t=tan(a->data.complex_.imag);
		sll_float_t th=tanh(a->data.complex_.real);
		sll_complex_t num={
			th,
			t
		};
		sll_complex_t denom={
			1,
			t*th
		};
		out->data.complex_=COMPLEX_DIV(num,denom);
	}
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_math_abs(sll_float_t a){
	f64_data_t dt={
		.value=a
	};
	dt.data&=0x7fffffffffffffffull;
	return dt.value;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_math_copy_sign(sll_float_t a,sll_float_t b){
	f64_data_t dt_a={
		.value=a
	};
	f64_data_t dt_b={
		.value=b
	};
	dt_a.data=(dt_a.data&0x7fffffffffffffffull)|(dt_b.data&0x8000000000000000ull);
	return dt_a.value;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_factor_t* sll_math_factors(sll_size_t v,sll_math_factor_count_t* ol){
	if (v<2){
		*ol=0;
		return NULL;
	}
	sll_factor_t* out=sll_allocate_stack(1);
	sll_math_factor_count_t i=0;
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
	return out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_math_mod(sll_float_t a,sll_float_t b){
	return fmod(a,b);
}

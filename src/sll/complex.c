#include <sll/_internal/complex.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_complex_abs(sll_complex_t a){
	return COMPLEX_ABS(a);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_add(sll_complex_t a,sll_complex_t b){
	return COMPLEX_ADD(a,b);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_conjugate(sll_complex_t a){
	return COMPLEX_CONJUGATE(a);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_div(sll_complex_t a,sll_complex_t b){
	return COMPLEX_DIV(a,b);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_mult(sll_complex_t a,sll_complex_t b){
	return COMPLEX_MULT(a,b);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_neg(sll_complex_t a){
	return COMPLEX_NEG(a);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_pow(sll_complex_t a,sll_complex_t b){
	sll_complex_t o;
	if (!b.real&&!b.imag){
		o.real=1;
		o.imag=0;
	}
	else if (!a.real&&!a.imag){
		o.real=0;
		o.imag=0;
	}
	else{
		sll_float_t abs=sll_api_math_sqrt(a.real*a.real+a.imag*a.imag);
		sll_float_t len=sll_api_math_pow(abs,b.real);
		sll_float_t rot=sll_api_math_atan2(a.imag,a.real);
		sll_float_t n_rot=rot*b.real;
		if (b.imag){
			len/=sll_api_math_exp(rot*b.imag);
			n_rot+=sll_api_math_log(abs)*b.imag;
		}
		o.real=len*sll_api_math_cos(n_rot);
		o.imag=len*sll_api_math_sin(n_rot);
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_pow_float(sll_complex_t a,sll_float_t b){
	sll_complex_t o;
	if (!b){
		o.real=1;
		o.imag=0;
	}
	else if (!a.real&&!a.imag){
		o.real=0;
		o.imag=0;
	}
	else{
		sll_float_t len=sll_api_math_pow(a.real*a.real+a.imag*a.imag,b/2);
		sll_float_t rot=sll_api_math_atan2(a.imag,a.real)*b;
		o.real=len*sll_api_math_cos(rot);
		o.imag=len*sll_api_math_sin(rot);
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_pow_int(sll_complex_t a,sll_integer_t b){
	sll_bool_t inv=0;
	if (b<0){
		b=-b;
		inv=1;
	}
	sll_complex_t o={
		1,
		0
	};
	sll_complex_t n=a;
	sll_size_t v=b;
	sll_size_t m=1;
	while (m&&m<=v){
		if (v&m){
			sll_float_t tmp=o.real*n.real-o.imag*n.imag;
			o.imag=o.real*n.imag+o.imag*n.real;
			o.real=tmp;
		}
		m<<=1;
		sll_float_t tmp=n.real*n.real-n.imag*n.imag;
		n.imag=n.real*n.imag+n.imag*n.real;
		n.real=tmp;
	}
	if (inv){
		sll_float_t d=1/(o.real*o.real+o.imag*o.imag);
		o.real*=d;
		o.imag*=-d;
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_reciprocal(sll_complex_t a){
	return COMPLEX_RECIPROCAL(a);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_sub(sll_complex_t a,sll_complex_t b){
	return COMPLEX_SUB(a,b);
}

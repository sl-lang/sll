#ifndef __SLL__INTERNAL_COMPLEX_H__
#define __SLL__INTERNAL_COMPLEX_H__ 1
#include <sll/_internal/common.h>
#include <sll/api/math.h>
#include <sll/types.h>



static __SLL_FORCE_INLINE sll_float_t COMPLEX_ABS(sll_complex_t a){
	return sll_api_math_sqrt(a.real*a.real+a.imag*a.imag);
}



static __SLL_FORCE_INLINE sll_complex_t COMPLEX_ADD(sll_complex_t a,sll_complex_t b){
	sll_complex_t o={
		a.real+b.real,
		a.imag+b.imag
	};
	return o;
}



static __SLL_FORCE_INLINE sll_complex_t COMPLEX_CONJUGATE(sll_complex_t a){
	sll_complex_t o={
		a.real,
		-a.imag
	};
	return o;
}



static __SLL_FORCE_INLINE sll_complex_t COMPLEX_DIV(sll_complex_t a,sll_complex_t b){
	sll_complex_t o;
	sll_float_t d=b.real*b.real+b.imag*b.imag;
	if (!d){
		o.real=0;
		o.imag=0;
	}
	else{
		d=1/d;
		o.real=d*(a.real*b.real+a.imag*b.imag);
		o.imag=d*(a.imag*b.real-a.real*b.imag);
	}
	return o;
}



static __SLL_FORCE_INLINE sll_complex_t COMPLEX_DIV_FLOAT(sll_complex_t a,sll_float_t b){
	b=1/b;
	sll_complex_t o={
		a.real*b,
		a.imag*b
	};
	return o;
}



static __SLL_FORCE_INLINE sll_complex_t COMPLEX_EXP(sll_complex_t a){
	sll_float_t r=sll_api_math_exp(a.real);
	sll_complex_t o={
		r*sll_api_math_cos(a.imag),
		r*sll_api_math_sin(a.imag)
	};
	return o;
}



static __SLL_FORCE_INLINE sll_complex_t COMPLEX_LOG(sll_complex_t a){
	sll_complex_t o={
		sll_api_math_log(sll_api_math_sqrt(a.real*a.real+a.imag*a.imag)),
		sll_api_math_atan2(a.imag,a.real)
	};
	return o;
}



static __SLL_FORCE_INLINE sll_complex_t COMPLEX_MULT(sll_complex_t a,sll_complex_t b){
	sll_complex_t o={
		a.real*b.real-a.imag*b.imag,
		a.real*b.imag+a.imag*b.real
	};
	return o;
}



static __SLL_FORCE_INLINE sll_complex_t COMPLEX_MULT_FLOAT(sll_complex_t a,sll_float_t b){
	sll_complex_t o={
		a.real*b,
		a.imag*b
	};
	return o;
}



static __SLL_FORCE_INLINE sll_complex_t COMPLEX_NEG(sll_complex_t a){
	sll_complex_t o={
		-a.real,
		-a.imag
	};
	return o;
}



static __SLL_FORCE_INLINE sll_complex_t COMPLEX_RECIPROCAL(sll_complex_t a){
	sll_complex_t o;
	sll_float_t d=a.real*a.real+a.imag*a.imag;
	if (!d){
		o.real=0;
		o.imag=0;
	}
	else{
		d=1/d;
		o.real=d*a.real;
		o.imag=-d*a.imag;
	}
	return o;
}



static __SLL_FORCE_INLINE sll_complex_t COMPLEX_SUB(sll_complex_t a,sll_complex_t b){
	sll_complex_t o={
		a.real-b.real,
		a.imag-b.imag
	};
	return o;
}



#endif

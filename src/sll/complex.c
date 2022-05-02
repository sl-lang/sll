#include <sll/_internal/complex.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_complex_abs(const sll_complex_t* a){
	return COMPLEX_ABS(*a);
}



__SLL_EXTERNAL void sll_complex_add(const sll_complex_t* a,const sll_complex_t* b,sll_complex_t* out){
	*out=COMPLEX_ADD(*a,*b);
}



__SLL_EXTERNAL void sll_complex_conjugate(const sll_complex_t* a,sll_complex_t* out){
	*out=COMPLEX_CONJUGATE(*a);
}



__SLL_EXTERNAL void sll_complex_div(const sll_complex_t* a,const sll_complex_t* b,sll_complex_t* out){
	*out=COMPLEX_DIV(*a,*b);
}



__SLL_EXTERNAL void sll_complex_div_float(const sll_complex_t* a,sll_float_t b,sll_complex_t* out){
	*out=COMPLEX_DIV_FLOAT(*a,b);
}



__SLL_EXTERNAL void sll_complex_exp(const sll_complex_t* a,sll_complex_t* out){
	*out=COMPLEX_EXP(*a);
}



__SLL_EXTERNAL void sll_complex_log(const sll_complex_t* a,sll_complex_t* out){
	*out=COMPLEX_LOG(*a);
}



__SLL_EXTERNAL void sll_complex_mult(const sll_complex_t* a,const sll_complex_t* b,sll_complex_t* out){
	*out=COMPLEX_MULT(*a,*b);
}



__SLL_EXTERNAL void sll_complex_mult_float(const sll_complex_t* a,sll_float_t b,sll_complex_t* out){
	*out=COMPLEX_MULT_FLOAT(*a,b);
}



__SLL_EXTERNAL void sll_complex_neg(const sll_complex_t* a,sll_complex_t* out){
	*out=COMPLEX_NEG(*a);
}



__SLL_EXTERNAL void sll_complex_pow(const sll_complex_t* a,const sll_complex_t* b,sll_complex_t* out){
	if (!b->real&&!b->imag){
		out->real=1;
		out->imag=0;
	}
	else if (!a->real&&!a->imag){
		out->real=0;
		out->imag=0;
	}
	else{
		sll_float_t abs=sll_api_math_sqrt(a->real*a->real+a->imag*a->imag);
		sll_float_t len=sll_api_math_pow(abs,b->real);
		sll_float_t rot=sll_api_math_atan2(a->imag,a->real);
		sll_float_t n_rot=rot*b->real;
		if (b->imag){
			len/=sll_api_math_exp(rot*b->imag);
			n_rot+=sll_api_math_log(abs)*b->imag;
		}
		out->real=len*sll_api_math_cos(n_rot);
		out->imag=len*sll_api_math_sin(n_rot);
	}
}



__SLL_EXTERNAL void sll_complex_pow_float(const sll_complex_t* a,sll_float_t b,sll_complex_t* out){
	if (!b){
		out->real=1;
		out->imag=0;
	}
	else if (!a->real&&!a->imag){
		out->real=0;
		out->imag=0;
	}
	else{
		sll_float_t len=sll_api_math_pow(a->real*a->real+a->imag*a->imag,b/2);
		sll_float_t rot=sll_api_math_atan2(a->imag,a->real)*b;
		out->real=len*sll_api_math_cos(rot);
		out->imag=len*sll_api_math_sin(rot);
	}
}



__SLL_EXTERNAL void sll_complex_pow_int(const sll_complex_t* a,sll_integer_t b,sll_complex_t* out){
	sll_bool_t inv=0;
	if (b<0){
		b=-b;
		inv=1;
	}
	out->real=1;
	out->imag=0;
	sll_complex_t n=*a;
	sll_size_t v=b;
	sll_size_t m=1;
	while (m&&m<=v){
		if (v&m){
			sll_float_t tmp=out->real*n.real-out->imag*n.imag;
			out->imag=out->real*n.imag+out->imag*n.real;
			out->real=tmp;
		}
		m<<=1;
		sll_float_t tmp=n.real*n.real-n.imag*n.imag;
		n.imag=n.real*n.imag+n.imag*n.real;
		n.real=tmp;
	}
	if (inv){
		sll_float_t d=1/(out->real*out->real+out->imag*out->imag);
		out->real*=d;
		out->imag*=-d;
	}
}



__SLL_EXTERNAL void sll_complex_reciprocal(const sll_complex_t* a,sll_complex_t* out){
	*out=COMPLEX_RECIPROCAL(*a);
}



__SLL_EXTERNAL void sll_complex_sub(const sll_complex_t* a,const sll_complex_t* b,sll_complex_t* out){
	*out=COMPLEX_SUB(*a,*b);
}

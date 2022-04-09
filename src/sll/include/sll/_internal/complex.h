#ifndef __SLL__INTERNAL_COMPLEX_H__
#define __SLL__INTERNAL_COMPLEX_H__ 1
#include <sll/_internal/common.h>
#include <sll/types.h>



#define COMPLEX_ADD(a,b,o) \
	do{ \
		sll_complex_t __a=(a); \
		sll_complex_t __b=(b); \
		(o).real=__a.real+__b.real; \
		(o).imag=__a.imag+__b.imag; \
	} while (0)



#define COMPLEX_CONJUGATE(a,o) \
	do{ \
		sll_complex_t __a=(a); \
		(o).real=__a.real; \
		(o).imag=-__a.imag; \
	} while (0)



#define COMPLEX_DIV(a,b,o) \
	do{ \
		sll_complex_t __a=(a); \
		sll_complex_t __b=(b); \
		sll_float_t __d=__b.real*__b.real+__b.imag*__b.imag; \
		if (!__d){ \
			(o).real=0; \
			(o).imag=0; \
		} \
		else{ \
			__d=1/__d; \
			(o).real=__d*(__a.real*__b.real+__a.imag*__b.imag); \
			(o).imag=__d*(__a.imag*__b.real-__a.real*__b.imag); \
		} \
	} while (0)



#define COMPLEX_MULT(a,b,o) \
	do{ \
		sll_complex_t __a=(a); \
		sll_complex_t __b=(b); \
		(o).real=__a.real*__b.real-__a.imag*__b.imag; \
		(o).imag=__a.real*__b.imag+__a.imag*__b.real; \
	} while (0)



#define COMPLEX_RECIPROCAL(a,o) \
	do{ \
		sll_complex_t __a=(a); \
		sll_float_t __d=__a.real*__a.real+__a.imag*__a.imag; \
		if (!__d){ \
			(o).real=0; \
			(o).imag=0; \
		} \
		else{ \
			__d=1/__d; \
			(o).real=__d*__a.real; \
			(o).imag=-__d*__a.imag; \
		} \
	} while (0)



#define COMPLEX_SUB(a,b,o) \
	do{ \
		sll_complex_t __a=(a); \
		sll_complex_t __b=(b); \
		(o).real=__a.real-__b.real; \
		(o).imag=__a.imag-__b.imag; \
	} while (0)




#endif

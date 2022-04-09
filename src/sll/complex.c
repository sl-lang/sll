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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_reciprocal(sll_complex_t a){
	return COMPLEX_RECIPROCAL(a);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_sub(sll_complex_t a,sll_complex_t b){
	return COMPLEX_SUB(a,b);
}

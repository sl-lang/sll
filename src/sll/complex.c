#include <sll/_internal/complex.h>
#include <sll/common.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_complex_add(sll_complex_t* a,sll_complex_t* b,sll_complex_t* o){
	COMPLEX_ADD(*a,*b,*o);
}



__SLL_EXTERNAL void sll_complex_conjugate(sll_complex_t* a,sll_complex_t* o){
	COMPLEX_CONJUGATE(*a,*o);
}



__SLL_EXTERNAL void sll_complex_div(sll_complex_t* a,sll_complex_t* b,sll_complex_t* o){
	COMPLEX_DIV(*a,*b,*o);
}



__SLL_EXTERNAL void sll_complex_mult(sll_complex_t* a,sll_complex_t* b,sll_complex_t* o){
	COMPLEX_MULT(*a,*b,*o);
}



__SLL_EXTERNAL void sll_complex_reciprocal(sll_complex_t* a,sll_complex_t* o){
	COMPLEX_RECIPROCAL(*a,*o);
}



__SLL_EXTERNAL void sll_complex_sub(sll_complex_t* a,sll_complex_t* b,sll_complex_t* o){
	COMPLEX_SUB(*a,*b,*o);
}

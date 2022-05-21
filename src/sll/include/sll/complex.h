#ifndef __SLL_COMPLEX_H__
#define __SLL_COMPLEX_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Complex Numbers
 * \group complex
 * \desc Docs!
 */



/**
 * \flags func macro
 * \name SLL_INIT_COMPLEX
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t* c
 */
#define SLL_INIT_COMPLEX(c) \
	do{ \
		(c)->real=0; \
		(c)->imag=0; \
	} while (0)



/**
 * \flags macro var
 * \name SLL_INIT_COMPLEX_STRUCT
 * \group complex
 * \desc Docs!
 * \type sll_complex_t
 */
#define SLL_INIT_COMPLEX_STRUCT {0,0}



/**
 * \flags check_output func
 * \name sll_complex_abs
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_complex_abs(const sll_complex_t* a);



/**
 * \flags func
 * \name sll_complex_add
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg const sll_complex_t* b
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_add(const sll_complex_t* a,const sll_complex_t* b,sll_complex_t* out);



/**
 * \flags func
 * \name sll_complex_conjugate
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_conjugate(const sll_complex_t* a,sll_complex_t* out);



/**
 * \flags func
 * \name sll_complex_div
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg const sll_complex_t* b
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_div(const sll_complex_t* a,const sll_complex_t* b,sll_complex_t* out);



/**
 * \flags func
 * \name sll_complex_div_float
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg sll_float_t b
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_div_float(const sll_complex_t* a,sll_float_t b,sll_complex_t* out);



/**
 * \flags func
 * \name sll_complex_exp
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_exp(const sll_complex_t* a,sll_complex_t* out);



/**
 * \flags func
 * \name sll_complex_log
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_log(const sll_complex_t* a,sll_complex_t* out);



/**
 * \flags func
 * \name sll_complex_mult
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg const sll_complex_t* b
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_mult(const sll_complex_t* a,const sll_complex_t* b,sll_complex_t* out);



/**
 * \flags func
 * \name sll_complex_mult_float
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg sll_float_t b
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_mult_float(const sll_complex_t* a,sll_float_t b,sll_complex_t* out);



/**
 * \flags func
 * \name sll_complex_neg
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_neg(const sll_complex_t* a,sll_complex_t* out);



/**
 * \flags func
 * \name sll_complex_pow
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg const sll_complex_t* b
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_pow(const sll_complex_t* a,const sll_complex_t* b,sll_complex_t* out);



/**
 * \flags func
 * \name sll_complex_pow_float
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg sll_float_t b
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_pow_float(const sll_complex_t* a,sll_float_t b,sll_complex_t* out);



/**
 * \flags func
 * \name sll_complex_pow_int
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg sll_integer_t b
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_pow_int(const sll_complex_t* a,sll_integer_t b,sll_complex_t* out);



/**
 * \flags func
 * \name sll_complex_reciprocal
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_reciprocal(const sll_complex_t* a,sll_complex_t* out);



/**
 * \flags func
 * \name sll_complex_sub
 * \group complex
 * \desc Docs!
 * \arg const sll_complex_t* a
 * \arg const sll_complex_t* b
 * \arg sll_complex_t* out
 */
__SLL_EXTERNAL void sll_complex_sub(const sll_complex_t* a,const sll_complex_t* b,sll_complex_t* out);



#endif

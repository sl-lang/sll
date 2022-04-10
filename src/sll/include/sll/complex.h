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
 * \arg sll_complex_t a
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_complex_abs(sll_complex_t a);



/**
 * \flags check_output func
 * \name sll_complex_add
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t a
 * \arg sll_complex_t b
 * \ret sll_complex_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_add(sll_complex_t a,sll_complex_t b);



/**
 * \flags check_output func
 * \name sll_complex_conjugate
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t a
 * \ret sll_complex_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_conjugate(sll_complex_t a);



/**
 * \flags check_output func
 * \name sll_complex_div
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t a
 * \arg sll_complex_t b
 * \ret sll_complex_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_div(sll_complex_t a,sll_complex_t b);



/**
 * \flags check_output func
 * \name sll_complex_div_float
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t a
 * \arg sll_float_t b
 * \ret sll_complex_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_div_float(sll_complex_t a,sll_float_t b);



/**
 * \flags check_output func
 * \name sll_complex_mult
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t a
 * \arg sll_complex_t b
 * \ret sll_complex_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_mult(sll_complex_t a,sll_complex_t b);



/**
 * \flags check_output func
 * \name sll_complex_mult_float
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t a
 * \arg sll_float_t b
 * \ret sll_complex_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_mult_float(sll_complex_t a,sll_float_t b);



/**
 * \flags check_output func
 * \name sll_complex_neg
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t a
 * \ret sll_complex_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_neg(sll_complex_t a);



/**
 * \flags check_output func
 * \name sll_complex_pow
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t a
 * \arg sll_complex_t b
 * \ret sll_complex_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_pow(sll_complex_t a,sll_complex_t b);



/**
 * \flags check_output func
 * \name sll_complex_pow_float
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t a
 * \arg sll_float_t b
 * \ret sll_complex_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_pow_float(sll_complex_t a,sll_float_t b);



/**
 * \flags check_output func
 * \name sll_complex_pow_int
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t a
 * \arg sll_integer_t b
 * \ret sll_complex_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_pow_int(sll_complex_t a,sll_integer_t b);



/**
 * \flags check_output func
 * \name sll_complex_reciprocal
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t a
 * \ret sll_complex_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_reciprocal(sll_complex_t a);



/**
 * \flags check_output func
 * \name sll_complex_sub
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t a
 * \arg sll_complex_t b
 * \ret sll_complex_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_complex_t sll_complex_sub(sll_complex_t a,sll_complex_t b);



#endif

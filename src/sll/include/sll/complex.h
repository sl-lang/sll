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

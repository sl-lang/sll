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
 * \flags func
 * \name sll_complex_add
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t* a
 * \arg sll_complex_t* b
 * \arg sll_complex_t* o
 */
__SLL_EXTERNAL void sll_complex_add(sll_complex_t* a,sll_complex_t* b,sll_complex_t* o);



/**
 * \flags func
 * \name sll_complex_conjugate
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t* a
 * \arg sll_complex_t* o
 */
__SLL_EXTERNAL void sll_complex_conjugate(sll_complex_t* a,sll_complex_t* o);



/**
 * \flags func
 * \name sll_complex_div
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t* a
 * \arg sll_complex_t* b
 * \arg sll_complex_t* o
 */
__SLL_EXTERNAL void sll_complex_div(sll_complex_t* a,sll_complex_t* b,sll_complex_t* o);



/**
 * \flags func
 * \name sll_complex_mult
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t* a
 * \arg sll_complex_t* b
 * \arg sll_complex_t* o
 */
__SLL_EXTERNAL void sll_complex_mult(sll_complex_t* a,sll_complex_t* b,sll_complex_t* o);



/**
 * \flags func
 * \name sll_complex_reciprocal
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t* a
 * \arg sll_complex_t* o
 */
__SLL_EXTERNAL void sll_complex_reciprocal(sll_complex_t* a,sll_complex_t* o);



/**
 * \flags func
 * \name sll_complex_sub
 * \group complex
 * \desc Docs!
 * \arg sll_complex_t* a
 * \arg sll_complex_t* b
 * \arg sll_complex_t* o
 */
__SLL_EXTERNAL void sll_complex_sub(sll_complex_t* a,sll_complex_t* b,sll_complex_t* o);



#endif

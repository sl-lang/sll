#ifndef __SLL_OPERATOR_H__
#define __SLL_OPERATOR_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <math.h>
/**
 * \flags group
 * \name Operator
 * \group operator
 * \desc Docs!
 */



#define SLL_FLOAT_ABSOLUTE(v) fabs((v))
#define SLL_FLOAT_ROUND(v) llround((v))



/**
 * \flags var
 * \name sll_float_compare_error
 * \group operator
 * \desc Docs!
 * \type const sll_float_t
 */
__SLL_EXTERNAL extern const sll_float_t sll_float_compare_error;



/**
 * \flags check_output func
 * \name sll_operator_access
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_access(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_access_range
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \arg sll_object_t* c
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_access_range(sll_object_t* a,sll_object_t* b,sll_object_t* c);



/**
 * \flags check_output func
 * \name sll_operator_access_range_step
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \arg sll_object_t* c
 * \arg sll_object_t* d
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_access_range_step(sll_object_t* a,sll_object_t* b,sll_object_t* c,sll_object_t* d);



/**
 * \flags check_output func
 * \name sll_operator_add
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_add(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_and
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_and(sll_object_t* a,sll_object_t* b);



/**
 * \flags func
 * \name sll_operator_assign
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \arg sll_object_t* v
 */
__SLL_EXTERNAL void sll_operator_assign(sll_object_t* a,sll_object_t* b,sll_object_t* v);



/**
 * \flags func
 * \name sll_operator_assign_range
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \arg sll_object_t* c
 * \arg sll_object_t* v
 */
__SLL_EXTERNAL void sll_operator_assign_range(sll_object_t* a,sll_object_t* b,sll_object_t* c,sll_object_t* v);



/**
 * \flags func
 * \name sll_operator_assign_range_step
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \arg sll_object_t* c
 * \arg sll_object_t* d
 * \arg sll_object_t* v
 */
__SLL_EXTERNAL void sll_operator_assign_range_step(sll_object_t* a,sll_object_t* b,sll_object_t* c,sll_object_t* d,sll_object_t* v);



/**
 * \flags check_output func
 * \name sll_operator_bool
 * \group operator
 * \desc Docs!
 * \arg const sll_object_t* a
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_bool(const sll_object_t* a);



/**
 * \flags check_output func
 * \name sll_operator_cast
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_cast(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_compare
 * \group operator
 * \desc Docs!
 * \arg const sll_object_t* a
 * \arg const sll_object_t* b
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_operator_compare(const sll_object_t* a,const sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_dec
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_dec(sll_object_t* a);



/**
 * \flags check_output func
 * \name sll_operator_div
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_div(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_dup
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_dup(sll_object_t* a);



/**
 * \flags check_output func
 * \name sll_operator_equal
 * \group operator
 * \desc Docs!
 * \arg const sll_object_t* a
 * \arg const sll_object_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_equal(const sll_object_t* a,const sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_floor_div
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_floor_div(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_inc
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_inc(sll_object_t* a);



/**
 * \flags check_output func
 * \name sll_operator_inv
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_inv(sll_object_t* a);



/**
 * \flags check_output func
 * \name sll_operator_len
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_len(sll_object_t* a);



/**
 * \flags check_output func
 * \name sll_operator_mod
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_mod(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_mult
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_mult(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_or
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_or(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_shl
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_shl(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_shr
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_shr(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_strict_equal
 * \group operator
 * \desc Docs!
 * \arg const sll_object_t* a
 * \arg const sll_object_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_strict_equal(const sll_object_t* a,const sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_sub
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_sub(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_xor
 * \group operator
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_xor(sll_object_t* a,sll_object_t* b);



#endif

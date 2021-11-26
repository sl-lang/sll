#ifndef __SLL_OPERATOR_H__
#define __SLL_OPERATOR_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <math.h>



#define SLL_FLOAT_ABSOLUTE(v) fabs((v))
#define SLL_FLOAT_ROUND(v) llround((v))



/**
 * \flags var
 * \name sll_float_compare_error
 * \desc Docs!
 * \type const sll_float_t
 */
__SLL_EXTERNAL extern const sll_float_t sll_float_compare_error;



/**
 * \flags check_output func
 * \name sll_operator_access
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_access(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_access_range
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
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_add(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_and
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_and(sll_object_t* a,sll_object_t* b);



/**
 * \flags func
 * \name sll_operator_assign
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \arg sll_object_t* v
 */
__SLL_EXTERNAL void sll_operator_assign(sll_object_t* a,sll_object_t* b,sll_object_t* v);



/**
 * \flags func
 * \name sll_operator_assign_range
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
 * \desc Docs!
 * \arg const sll_object_t* a
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_bool(const sll_object_t* a);



/**
 * \flags check_output func
 * \name sll_operator_cast
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_cast(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_compare
 * \desc Docs!
 * \arg const sll_object_t* a
 * \arg const sll_object_t* b
 * \ret sll_compare_result_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_compare_result_t sll_operator_compare(const sll_object_t* a,const sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_dec
 * \desc Docs!
 * \arg sll_object_t* a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_dec(sll_object_t* a);



/**
 * \flags check_output func
 * \name sll_operator_div
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_div(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_dup
 * \desc Docs!
 * \arg sll_object_t* a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_dup(sll_object_t* a);



/**
 * \flags check_output func
 * \name sll_operator_equal
 * \desc Docs!
 * \arg const sll_object_t* a
 * \arg const sll_object_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_equal(const sll_object_t* a,const sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_floor_div
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_floor_div(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_inc
 * \desc Docs!
 * \arg sll_object_t* a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_inc(sll_object_t* a);



/**
 * \flags check_output func
 * \name sll_operator_inv
 * \desc Docs!
 * \arg sll_object_t* a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_inv(sll_object_t* a);



/**
 * \flags check_output func
 * \name sll_operator_len
 * \desc Docs!
 * \arg sll_object_t* a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_len(sll_object_t* a);



/**
 * \flags check_output func
 * \name sll_operator_mod
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_mod(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_mult
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_mult(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_or
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_or(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_shl
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_shl(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_shr
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_shr(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_strict_equal
 * \desc Docs!
 * \arg const sll_object_t* a
 * \arg const sll_object_t* b
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_operator_strict_equal(const sll_object_t* a,const sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_sub
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_sub(sll_object_t* a,sll_object_t* b);



/**
 * \flags check_output func
 * \name sll_operator_xor
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_object_t* sll_operator_xor(sll_object_t* a,sll_object_t* b);



#endif

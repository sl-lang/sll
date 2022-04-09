#ifndef __SLL_API_MATH_H__
#define __SLL_API_MATH_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Math API
 * \group math-api
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Inverse Trigonometry
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Trigonometry
 * \group math-api
 * \subgroup math-api-trig
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Exponents
 * \group math-api
 * \subgroup math-api-exp
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Logarithms
 * \group math-api
 * \subgroup math-api-log
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_math_euler_phi
 * \group math-api
 * \desc Docs!
 * \arg sll_size_t n
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_size_t sll_math_euler_phi(sll_size_t n);



/**
 * \flags check_output func
 * \name sll_math_factors
 * \group math-api
 * \desc Docs!
 * \arg sll_integer_t n
 * \arg sll_array_length_t* ol
 * \ret sll_factor_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_factor_t* sll_math_factors(sll_integer_t n,sll_array_length_t* ol);



/**
 * \flags check_output func
 * \name sll_math_mod
 * \group math-api
 * \desc Docs!
 * \arg sll_float_t a
 * \arg sll_float_t b
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_float_t sll_math_mod(sll_float_t a,sll_float_t b);



/**
 * \flags api func
 * \name sll_api_math_abs
 * \group math-api
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_abs);



/**
 * \flags api func
 * \name sll_api_math_acos
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_acos);



/**
 * \flags api func
 * \name sll_api_math_acosh
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_acosh);



/**
 * \flags api func
 * \name sll_api_math_asin
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_asin);



/**
 * \flags api func
 * \name sll_api_math_asinh
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_asinh);



/**
 * \flags api func
 * \name sll_api_math_atan
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_atan);



/**
 * \flags api func
 * \name sll_api_math_atan2
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \arg F
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_atan2);



/**
 * \flags api func
 * \name sll_api_math_atanh
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_atanh);



/**
 * \flags api func
 * \name sll_api_math_cbrt
 * \group math-api
 * \subgroup math-api-exp
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_cbrt);



/**
 * \flags api func
 * \name sll_api_math_combinations
 * \group math-api
 * \desc Docs!
 * \arg I
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(math_combinations);



/**
 * \flags api func
 * \name sll_api_math_cos
 * \group math-api
 * \subgroup math-api-trig
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_cos);



/**
 * \flags api func
 * \name sll_api_math_cosh
 * \group math-api
 * \subgroup math-api-trig
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_cosh);



/**
 * \flags api func
 * \name sll_api_math_ceil
 * \group math-api
 * \desc Docs!
 * \arg F
 * \ret I
 */
__API_FUNC_DECL(math_ceil);



/**
 * \flags api func
 * \name sll_api_math_copy_sign
 * \group math-api
 * \desc Docs!
 * \arg F
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_copy_sign);



/**
 * \flags api func
 * \name sll_api_math_euler_phi
 * \group math-api
 * \desc Docs!
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(math_euler_phi);



/**
 * \flags api func
 * \name sll_api_math_exp
 * \group math-api
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_exp);



/**
 * \flags api func
 * \name sll_api_math_factorial
 * \group math-api
 * \desc Docs!
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(math_factorial);



/**
 * \flags api func
 * \name sll_api_math_factors
 * \group math-api
 * \desc Docs!
 * \arg I
 * \ret O
 */
__API_FUNC_DECL(math_factors);



/**
 * \flags api func
 * \name sll_api_math_gcd
 * \group math-api
 * \desc Docs!
 * \arg I
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(math_gcd);



/**
 * \flags api func
 * \name sll_api_math_floor
 * \group math-api
 * \desc Docs!
 * \arg F
 * \ret I
 */
__API_FUNC_DECL(math_floor);



/**
 * \flags api func
 * \name sll_api_math_int_log2
 * \group math-api
 * \subgroup math-api-log
 * \desc Docs!
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(math_int_log2);



/**
 * \flags api func
 * \name sll_api_math_int_pow
 * \group math-api
 * \subgroup math-api-exp
 * \desc Docs!
 * \arg I
 * \arg I
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(math_int_pow);



/**
 * \flags api func
 * \name sll_api_math_int_sqrt
 * \group math-api
 * \subgroup math-api-exp
 * \desc Docs!
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(math_int_sqrt);



/**
 * \flags api func
 * \name sll_api_math_log
 * \group math-api
 * \subgroup math-api-log
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_log);



/**
 * \flags api func
 * \name sll_api_math_log2
 * \group math-api
 * \subgroup math-api-log
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_log2);



/**
 * \flags api func
 * \name sll_api_math_log10
 * \group math-api
 * \subgroup math-api-log
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_log10);



/**
 * \flags api func
 * \name sll_api_math_permutations
 * \group math-api
 * \desc Docs!
 * \arg I
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(math_permutations);



/**
 * \flags api func
 * \name sll_api_math_pow
 * \group math-api
 * \subgroup math-api-exp
 * \desc Docs!
 * \arg F
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_pow);



/**
 * \flags api func
 * \name sll_api_math_round
 * \group math-api
 * \desc Docs!
 * \arg F
 * \ret I
 */
__API_FUNC_DECL(math_round);



/**
 * \flags api func
 * \name sll_api_math_sin
 * \group math-api
 * \subgroup math-api-trig
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_sin);



/**
 * \flags api func
 * \name sll_api_math_sinh
 * \group math-api
 * \subgroup math-api-trig
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_sinh);



/**
 * \flags api func
 * \name sll_api_math_sqrt
 * \group math-api
 * \subgroup math-api-exp
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_sqrt);



/**
 * \flags api func
 * \name sll_api_math_tan
 * \group math-api
 * \subgroup math-api-trig
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_tan);



/**
 * \flags api func
 * \name sll_api_math_tanh
 * \group math-api
 * \subgroup math-api-trig
 * \desc Docs!
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(math_tanh);



#endif

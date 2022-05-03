#ifndef __SLL_API_MATH_H__
#define __SLL_API_MATH_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/parse_args.h>
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
 * \flags type
 * \name sll_factor_t
 * \group math-api
 * \desc Docs!
 * \arg sll_size_t n
 * \arg sll_size_t pw
 */
typedef struct _SLL_FACTOR{
	sll_size_t n;
	sll_size_t pw;
} sll_factor_t;



/**
 * \flags func
 * \name sll_api_math_abs
 * \group math-api
 * \desc Docs!
 * \api z|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_abs(const sll_number_t* a,sll_number_t* out);



/**
 * \flags check_output func
 * \name sll_api_math_acos
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \api f|f
 * \arg sll_float_t a
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_acos(sll_float_t a);



/**
 * \flags check_output func
 * \name sll_api_math_acosh
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \api f|f
 * \arg sll_float_t a
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_acosh(sll_float_t a);



/**
 * \flags check_output func
 * \name sll_api_math_asin
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \api f|f
 * \arg sll_float_t a
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_asin(sll_float_t a);



/**
 * \flags check_output func
 * \name sll_api_math_asinh
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \api f|f
 * \arg sll_float_t a
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_asinh(sll_float_t a);



/**
 * \flags check_output func
 * \name sll_api_math_atan
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \api f|f
 * \arg sll_float_t a
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_atan(sll_float_t a);



/**
 * \flags check_output func
 * \name sll_api_math_atan2
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \api ff|f
 * \arg sll_float_t y
 * \arg sll_float_t x
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_atan2(sll_float_t y,sll_float_t x);



/**
 * \flags check_output func
 * \name sll_api_math_atanh
 * \group math-api
 * \subgroup math-api-inv-trig
 * \desc Docs!
 * \api f|f
 * \arg sll_float_t a
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_atanh(sll_float_t a);



/**
 * \flags check_output func
 * \name sll_api_math_cbrt
 * \group math-api
 * \subgroup math-api-exp
 * \desc Docs!
 * \api f|f
 * \arg sll_float_t a
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_math_cbrt(sll_float_t a);



/**
 * \flags check_output func
 * \name sll_api_math_combinations
 * \group math-api
 * \desc Docs!
 * \api QQ|Q
 * \arg sll_size_t a
 * \arg sll_size_t b
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_math_combinations(sll_size_t a,sll_size_t b);



/**
 * \flags func
 * \name sll_api_math_cos
 * \group math-api
 * \subgroup math-api-trig
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_cos(const sll_number_t* a,sll_number_t* out);



/**
 * \flags func
 * \name sll_api_math_cosh
 * \group math-api
 * \subgroup math-api-trig
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_cosh(const sll_number_t* a,sll_number_t* out);



/**
 * \flags func
 * \name sll_api_math_ceil
 * \group math-api
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_ceil(const sll_number_t* a,sll_number_t* out);



/**
 * \flags func
 * \name sll_api_math_copy_sign
 * \group math-api
 * \desc Docs!
 * \api zz|x
 * \arg const sll_number_t* a
 * \arg const sll_number_t* b
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_copy_sign(const sll_number_t* a,const sll_number_t* b,sll_number_t* out);



/**
 * \flags check_output func
 * \name sll_api_math_euler_phi
 * \group math-api
 * \desc Docs!
 * \api Q|Q
 * \arg sll_size_t n
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_math_euler_phi(sll_size_t n);



/**
 * \flags func
 * \name sll_api_math_exp
 * \group math-api
 * \subgroup math-api-exp
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_exp(const sll_number_t* a,sll_number_t* out);



/**
 * \flags check_output func
 * \name sll_api_math_factorial
 * \group math-api
 * \desc Docs!
 * \api Q|Q
 * \arg sll_size_t a
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_math_factorial(sll_size_t a);



/**
 * \flags check_output func
 * \name sll_api_math_factors
 * \group math-api
 * \desc Docs!
 * \api Q|a
 * \arg sll_size_t a
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_factors(sll_size_t a,sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_math_gcd
 * \group math-api
 * \desc Docs!
 * \api ii|i
 * \arg sll_integer_t a
 * \arg sll_integer_t b
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_math_gcd(sll_integer_t a,sll_integer_t b);



/**
 * \flags func
 * \name sll_api_math_floor
 * \group math-api
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_floor(const sll_number_t* a,sll_number_t* out);



/**
 * \flags check_output func
 * \name sll_api_math_int_log2
 * \group math-api
 * \subgroup math-api-log
 * \desc Docs!
 * \api Q|Q
 * \arg sll_size_t a
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_math_int_log2(sll_size_t a);



/**
 * \flags check_output func
 * \name sll_api_math_int_pow
 * \group math-api
 * \subgroup math-api-exp
 * \desc Docs!
 * \api iQQ|i
 * \arg sll_integer_t a
 * \arg sll_size_t b
 * \arg sll_size_t c
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_math_int_pow(sll_integer_t a,sll_size_t b,sll_size_t c);



/**
 * \flags check_output func
 * \name sll_api_math_int_sqrt
 * \group math-api
 * \subgroup math-api-exp
 * \desc Docs!
 * \api Q|Q
 * \arg sll_size_t v
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_math_int_sqrt(sll_size_t v);



/**
 * \flags func
 * \name sll_api_math_log
 * \group math-api
 * \subgroup math-api-log
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_log(const sll_number_t* a,sll_number_t* out);



/**
 * \flags func
 * \name sll_api_math_log10
 * \group math-api
 * \subgroup math-api-log
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_log10(const sll_number_t* a,sll_number_t* out);



/**
 * \flags func
 * \name sll_api_math_log2
 * \group math-api
 * \subgroup math-api-log
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_log2(const sll_number_t* a,sll_number_t* out);



/**
 * \flags check_output func
 * \name sll_api_math_permutations
 * \group math-api
 * \desc Docs!
 * \api QQ|Q
 * \arg sll_size_t a
 * \arg sll_size_t b
 * \ret sll_size_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_size_t sll_api_math_permutations(sll_size_t a,sll_size_t b);



/**
 * \flags func
 * \name sll_api_math_pow
 * \group math-api
 * \subgroup math-api-exp
 * \desc Docs!
 * \api XX|x
 * \arg const sll_number_t* a
 * \arg const sll_number_t* b
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_pow(const sll_number_t* a,const sll_number_t* b,sll_number_t* out);



/**
 * \flags func
 * \name sll_api_math_round
 * \group math-api
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_round(const sll_number_t* a,sll_number_t* out);



/**
 * \flags func
 * \name sll_api_math_sin
 * \group math-api
 * \subgroup math-api-trig
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_sin(const sll_number_t* a,sll_number_t* out);



/**
 * \flags func
 * \name sll_api_math_sinh
 * \group math-api
 * \subgroup math-api-trig
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_sinh(const sll_number_t* a,sll_number_t* out);



/**
 * \flags func
 * \name sll_api_math_sqrt
 * \group math-api
 * \subgroup math-api-exp
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_sqrt(const sll_number_t* a,sll_number_t* out);



/**
 * \flags func
 * \name sll_api_math_tan
 * \group math-api
 * \subgroup math-api-trig
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_tan(const sll_number_t* a,sll_number_t* out);



/**
 * \flags func
 * \name sll_api_math_tanh
 * \group math-api
 * \subgroup math-api-trig
 * \desc Docs!
 * \api X|x
 * \arg const sll_number_t* a
 * \arg sll_number_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_math_tanh(const sll_number_t* a,sll_number_t* out);



/**
 * \flags check_output func
 * \name sll_math_abs
 * \group math-api
 * \desc Docs!
 * \arg sll_float_t a
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_math_abs(sll_float_t a);



/**
 * \flags check_output func
 * \name sll_math_copy_sign
 * \group math-api
 * \desc Docs!
 * \arg sll_float_t a
 * \arg sll_float_t b
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_math_copy_sign(sll_float_t a,sll_float_t b);



/**
 * \flags check_output func
 * \name sll_math_factors
 * \group math-api
 * \desc Docs!
 * \arg sll_size_t n
 * \arg sll_array_length_t* ol
 * \ret sll_factor_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_factor_t* sll_math_factors(sll_size_t n,sll_array_length_t* ol);



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



#endif

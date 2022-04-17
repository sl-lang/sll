#ifndef __SLL_API_RANDOM_H__
#define __SLL_API_RANDOM_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Random API
 * \group random-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_random_get_float
 * \group random-api
 * \desc Docs!
 * \api ff|f
 * \arg sll_float_t a
 * \arg sll_float_t b
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_random_get_float(sll_float_t a,sll_float_t b);



/**
 * \flags func
 * \name sll_api_random_get_int
 * \group random-api
 * \desc Docs!
 * \api ii|i
 * \arg sll_integer_t a
 * \arg sll_integer_t b
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_random_get_int(sll_integer_t a,sll_integer_t b);



/**
 * \flags func
 * \name sll_api_random_get_string
 * \group random-api
 * \desc Docs!
 * \api icc|s
 * \arg sll_integer_t a
 * \arg sll_char_t b
 * \arg sll_char_t c
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_random_get_string(sll_integer_t a,sll_char_t b,sll_char_t c,sll_string_t* out);



#endif

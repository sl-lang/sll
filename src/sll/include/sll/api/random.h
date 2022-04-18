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
 * \arg sll_float_t min
 * \arg sll_float_t max
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_random_get_float(sll_float_t min,sll_float_t max);



/**
 * \flags func
 * \name sll_api_random_get_int
 * \group random-api
 * \desc Docs!
 * \api ii|i
 * \arg sll_integer_t min
 * \arg sll_integer_t max
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_random_get_int(sll_integer_t min,sll_integer_t max);



/**
 * \flags func
 * \name sll_api_random_get_string
 * \group random-api
 * \desc Docs!
 * \api Dcc|s
 * \arg sll_string_length_t len
 * \arg sll_char_t min
 * \arg sll_char_t max
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_random_get_string(sll_string_length_t len,sll_char_t min,sll_char_t max,sll_string_t* out);



#endif

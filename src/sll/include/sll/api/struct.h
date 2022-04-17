#ifndef __SLL_API_STRUCT_H__
#define __SLL_API_STRUCT_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Memory Structure API
 * \group struct-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_struct_float_from_bits
 * \group struct-api
 * \desc Docs!
 * \api i|f
 * \arg sll_integer_t a
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_struct_float_from_bits(sll_integer_t a);



/**
 * \flags func
 * \name sll_api_struct_double_from_bits
 * \group struct-api
 * \desc Docs!
 * \api i|f
 * \arg sll_integer_t a
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_struct_double_from_bits(sll_integer_t a);



/**
 * \flags func
 * \name sll_api_struct_float_to_bits
 * \group struct-api
 * \desc Docs!
 * \api f|i
 * \arg sll_float_t a
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_struct_float_to_bits(sll_float_t a);



/**
 * \flags func
 * \name sll_api_struct_double_to_bits
 * \group struct-api
 * \desc Docs!
 * \api f|i
 * \arg sll_float_t a
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_struct_double_to_bits(sll_float_t a);



#endif

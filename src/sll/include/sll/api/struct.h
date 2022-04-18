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
 * \api D|f
 * \arg __SLL_U32 v
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_struct_float_from_bits(__SLL_U32 v);



/**
 * \flags func
 * \name sll_api_struct_double_from_bits
 * \group struct-api
 * \desc Docs!
 * \api Q|f
 * \arg __SLL_U64 v
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_struct_double_from_bits(__SLL_U64 v);



/**
 * \flags func
 * \name sll_api_struct_float_to_bits
 * \group struct-api
 * \desc Docs!
 * \api f|i
 * \arg sll_float_t v
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_struct_float_to_bits(sll_float_t v);



/**
 * \flags func
 * \name sll_api_struct_double_to_bits
 * \group struct-api
 * \desc Docs!
 * \api f|i
 * \arg sll_float_t v
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_struct_double_to_bits(sll_float_t v);



#endif

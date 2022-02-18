#ifndef __SLL_API_STRUCT_H__
#define __SLL_API_STRUCT_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Memory Structure API
 * \group struct-api
 * \desc Docs!
 */



/**
 * \flags api func
 * \name sll_api_struct_float_from_bits
 * \group struct-api
 * \desc Docs!
 * \arg I
 * \ret F
 */
__API_FUNC_DECL(struct_float_from_bits);



/**
 * \flags api func
 * \name sll_api_struct_double_from_bits
 * \group struct-api
 * \desc Docs!
 * \arg I
 * \ret F
 */
__API_FUNC_DECL(struct_double_from_bits);



/**
 * \flags api func
 * \name sll_api_struct_float_to_bits
 * \group struct-api
 * \desc Docs!
 * \arg F
 * \ret I
 */
__API_FUNC_DECL(struct_float_to_bits);



/**
 * \flags api func
 * \name sll_api_struct_double_to_bits
 * \group struct-api
 * \desc Docs!
 * \arg F
 * \ret I
 */
__API_FUNC_DECL(struct_double_to_bits);



#endif

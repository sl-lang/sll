#ifndef __SLL_API_INT_H__
#define __SLL_API_INT_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Integer API
 * \group int-api
 * \desc Docs!
 */



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_int_to_bin
 * \group int-api
 * \desc Docs!
 * \arg I
 * \ret S
 */
__API_FUNC_DECL(int_to_bin);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_int_to_hex
 * \group int-api
 * \desc Docs!
 * \arg I
 * \ret S
 */
__API_FUNC_DECL(int_to_hex);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_int_to_oct
 * \group int-api
 * \desc Docs!
 * \arg I
 * \ret S
 */
__API_FUNC_DECL(int_to_oct);



#endif

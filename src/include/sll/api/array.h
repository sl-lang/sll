#ifndef __SLL_API_ARRAY_H__
#define __SLL_API_ARRAY_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Array API
 * \group array-api
 * \desc Docs!
 */



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_array_count
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg O
 * \ret 0 -> Failure
 * \ret I -> Success
 */
__API_FUNC_DECL(array_count);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_array_create
 * \group array-api
 * \desc Docs!
 * \arg I
 * \ret E -> Failure
 * \ret A -> Success
 */
__API_FUNC_DECL(array_create);



#endif

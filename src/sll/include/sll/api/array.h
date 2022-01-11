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
 * \ret I
 */
__API_FUNC_DECL(array_count);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_array_create
 * \group array-api
 * \desc Docs!
 * \arg I
 * \ret A
 */
__API_FUNC_DECL(array_create);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_array_extend
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg A
 * \ret V
 */
__API_FUNC_DECL(array_extend);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_array_join
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg O
 * \ret A
 */
__API_FUNC_DECL(array_join);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_array_pop
 * \group array-api
 * \desc Docs!
 * \arg A
 * \ret O
 */
__API_FUNC_DECL(array_pop);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_array_push
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg O
 * \ret V
 */
__API_FUNC_DECL(array_push);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_array_remove
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg O
 * \ret V
 */
__API_FUNC_DECL(array_remove);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_array_replace
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg O
 * \arg O
 * \ret A
 */
__API_FUNC_DECL(array_replace);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_array_reverse
 * \group array-api
 * \desc Docs!
 * \arg A
 * \ret A
 */
__API_FUNC_DECL(array_reverse);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_array_split
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg O
 * \ret A
 */
__API_FUNC_DECL(array_split);



#endif

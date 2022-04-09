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
 * \flags api func
 * \name sll_api_array_count
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg O
 * \ret I
 */
__API_FUNC_DECL(array_count);



/**
 * \flags api func
 * \name sll_api_array_create
 * \group array-api
 * \desc Docs!
 * \arg I
 * \ret A
 */
__API_FUNC_DECL(array_create);



/**
 * \flags api func
 * \name sll_api_array_extend
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg A
 */
__API_FUNC_DECL(array_extend);



/**
 * \flags api func
 * \name sll_api_array_index
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg O
 * \ret I
 */
__API_FUNC_DECL(array_index);



/**
 * \flags api func
 * \name sll_api_array_join
 * \group array-api
 * \desc Docs!
 * \arg A+
 * \arg O
 * \ret A
 */
__API_FUNC_DECL(array_join);



/**
 * \flags api func
 * \name sll_api_array_pop
 * \group array-api
 * \desc Docs!
 * \arg A
 * \ret O
 */
__API_FUNC_DECL(array_pop);



/**
 * \flags api func
 * \name sll_api_array_push
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg O
 */
__API_FUNC_DECL(array_push);



/**
 * \flags api func
 * \name sll_api_array_remove
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg O
 */
__API_FUNC_DECL(array_remove);



/**
 * \flags api func
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
 * \flags api func
 * \name sll_api_array_reverse
 * \group array-api
 * \desc Docs!
 * \arg A
 * \ret A
 */
__API_FUNC_DECL(array_reverse);



/**
 * \flags api func
 * \name sll_api_array_shift
 * \group array-api
 * \desc Docs!
 * \arg A
 * \ret O
 */
__API_FUNC_DECL(array_shift);



/**
 * \flags api func
 * \name sll_api_array_split
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg O
 * \ret A
 */
__API_FUNC_DECL(array_split);



/**
 * \flags api func
 * \name sll_api_array_unshift
 * \group array-api
 * \desc Docs!
 * \arg A
 * \arg O
 */
__API_FUNC_DECL(array_unshift);



#endif

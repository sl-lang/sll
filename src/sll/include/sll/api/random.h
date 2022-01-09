#ifndef __SLL_API_RANDOM_H__
#define __SLL_API_RANDOM_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Random API
 * \group random-api
 * \desc Docs!
 */



/**
 * \flags api func optimizable
 * \name sll_api_random_get_float
 * \group random-api
 * \desc Docs!
 * \arg F
 * \arg F
 * \ret F
 */
__API_FUNC_DECL(random_get_float);



/**
 * \flags api func optimizable
 * \name sll_api_random_get_int
 * \group random-api
 * \desc Docs!
 * \arg I
 * \arg I
 * \ret I
 */
__API_FUNC_DECL(random_get_int);



/**
 * \flags api func optimizable
 * \name sll_api_random_get_string
 * \group random-api
 * \desc Docs!
 * \arg I
 * \arg C
 * \arg C
 * \ret S
 */
__API_FUNC_DECL(random_get_string);



#endif

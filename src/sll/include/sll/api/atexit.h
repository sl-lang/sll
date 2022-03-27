#ifndef __SLL_API_ATEXIT_H__
#define __SLL_API_ATEXIT_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Atexit API
 * \group atexit-api
 * \desc Docs!
 */



/**
 * \flags api func var_arg
 * \name sll_api_atexit_register
 * \group atexit-api
 * \desc Docs!
 * \arg I
 * \arg O
 */
__API_FUNC_DECL(atexit_register);



/**
 * \flags api func
 * \name sll_api_atexit_unregister
 * \group atexit-api
 * \desc Docs!
 * \arg I
 * \ret B
 */
__API_FUNC_DECL(atexit_unregister);



#endif

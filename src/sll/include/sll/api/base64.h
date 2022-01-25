#ifndef __SLL_API_BASE64_H__
#define __SLL_API_BASE64_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Base 64 API
 * \group base64-api
 * \desc Docs!
 */



/**
 * \flags api func
 * \name sll_api_base64_decode
 * \group base64-api
 * \desc Docs!
 * \arg S
 * \ret O
 */
__API_FUNC_DECL(base64_decode);



/**
 * \flags api func
 * \name sll_api_base64_encode
 * \group base64-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(base64_encode);



#endif

#ifndef __SLL_API_BASE64_H__
#define __SLL_API_BASE64_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Base 64 API
 * \group base64-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_base64_decode
 * \group base64-api
 * \desc Docs!
 * \api s|o
 * \arg sll_string_t* a
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_base64_decode(sll_string_t* a);



/**
 * \flags func
 * \name sll_api_base64_encode
 * \group base64-api
 * \desc Docs!
 * \api s|s
 * \arg sll_string_t* a
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_base64_encode(sll_string_t* a,sll_string_t* out);



#endif

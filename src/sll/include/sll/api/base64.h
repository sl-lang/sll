#ifndef __SLL_API_BASE64_H__
#define __SLL_API_BASE64_H__ 1
#include <sll/common.h>
#include <sll/error.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Base 64 API
 * \group base64-api
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_api_base64_decode
 * \group base64-api
 * \desc Docs!
 * \api #s|~s
 * \arg const sll_string_t* string
 * \arg sll_string_t* out
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_base64_decode(const sll_string_t* string,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_base64_encode
 * \group base64-api
 * \desc Docs!
 * \api #s|s
 * \arg const sll_string_t* string
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_base64_encode(const sll_string_t* string,sll_string_t* out);



#endif

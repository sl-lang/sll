#ifndef __SLL_API_ERROR_H__
#define __SLL_API_ERROR_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Error API
 * \group error-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_error_get_call_stack
 * \group error-api
 * \desc Docs!
 * \api ii|o
 * \arg sll_integer_t a
 * \arg sll_integer_t b
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_error_get_call_stack(sll_integer_t a,sll_integer_t b);



#endif

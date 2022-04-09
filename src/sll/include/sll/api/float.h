#ifndef __SLL_API_FLOAT_H__
#define __SLL_API_FLOAT_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Floating-Point API
 * \group float-api
 * \desc Docs!
 */



/**
 * \flags var
 * \name sll_float_compare_error
 * \group float-api
 * \desc Docs!
 * \type sll_float_t
 */
__SLL_EXTERNAL extern sll_float_t sll_float_compare_error;



/**
 * \flags api func
 * \name sll_api_float_get_compare_error
 * \group float-api
 * \desc Docs!
 * \ret F
 */
__API_FUNC_DECL(float_get_compare_error);



/**
 * \flags api func
 * \name sll_api_float_set_compare_error
 * \group float-api
 * \desc Docs!
 * \arg F
 */
__API_FUNC_DECL(float_set_compare_error);



#endif

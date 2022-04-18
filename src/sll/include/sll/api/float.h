#ifndef __SLL_API_FLOAT_H__
#define __SLL_API_FLOAT_H__ 1
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
 * \flags check_output func
 * \name sll_api_float_get_compare_error
 * \group float-api
 * \desc Docs!
 * \api |f
 * \ret sll_float_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_float_t sll_api_float_get_compare_error(void);



/**
 * \flags func
 * \name sll_api_float_set_compare_error
 * \group float-api
 * \desc Docs!
 * \api f|v
 * \arg sll_float_t err
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_float_set_compare_error(sll_float_t err);



#endif

#ifndef __SLL_API_ATEXIT_H__
#define __SLL_API_ATEXIT_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Atexit API
 * \group atexit-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_atexit_register
 * \group atexit-api
 * \desc Docs!
 * \api io!|
 * \arg sll_integer_t function
 * \arg sll_object_t*const* args
 * \arg sll_arg_count_t arg_count
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_atexit_register(sll_integer_t function,sll_object_t*const* args,sll_arg_count_t arg_count);



/**
 * \flags check_output func
 * \name sll_api_atexit_unregister
 * \api i|b
 * \group atexit-api
 * \desc Docs!
 * \arg sll_integer_t function
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_atexit_unregister(sll_integer_t function);



#endif

#ifndef __SLL_API_LOG_H__
#define __SLL_API_LOG_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Logging API
 * \group log-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_log_log
 * \group log-api
 * \desc Docs!
 * \api o!|v
 * \arg sll_object_t*const* a
 * \arg sll_arg_count_t ac
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_log_log(sll_object_t*const* a,sll_arg_count_t ac);



/**
 * \flags func
 * \name sll_api_log_set_default
 * \group log-api
 * \desc Docs!
 * \api ib|v
 * \arg sll_integer_t a
 * \arg sll_bool_t b
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_log_set_default(sll_integer_t a,sll_bool_t b);



/**
 * \flags func
 * \name sll_api_log_set_file
 * \group log-api
 * \desc Docs!
 * \api sib|v
 * \arg sll_string_t* a
 * \arg sll_integer_t b
 * \arg sll_bool_t c
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_log_set_file(sll_string_t* a,sll_integer_t b,sll_bool_t c);



/**
 * \flags func
 * \name sll_api_log_set_function
 * \group log-api
 * \desc Docs!
 * \api ssib|v
 * \arg sll_string_t* a
 * \arg sll_string_t* b
 * \arg sll_integer_t c
 * \arg sll_bool_t d
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_log_set_function(sll_string_t* a,sll_string_t* b,sll_integer_t c,sll_bool_t d);



#endif

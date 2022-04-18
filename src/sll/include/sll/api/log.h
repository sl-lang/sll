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
 * \arg sll_object_t*const* data
 * \arg sll_arg_count_t len
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_log_log(sll_object_t*const* data,sll_arg_count_t len);



/**
 * \flags func
 * \name sll_api_log_set_default
 * \group log-api
 * \desc Docs!
 * \api Bb|v
 * \arg sll_flags_t flag
 * \arg sll_bool_t state
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_log_set_default(sll_flags_t flag,sll_bool_t state);



/**
 * \flags func
 * \name sll_api_log_set_file
 * \group log-api
 * \desc Docs!
 * \api sBb|v
 * \arg sll_string_t* file
 * \arg sll_flags_t flags
 * \arg sll_bool_t state
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_log_set_file(const sll_string_t* file,sll_flags_t flags,sll_bool_t state);



/**
 * \flags func
 * \name sll_api_log_set_function
 * \group log-api
 * \desc Docs!
 * \api ssBb|v
 * \arg sll_string_t* file
 * \arg sll_string_t* func
 * \arg sll_flags_t flags
 * \arg sll_bool_t state
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_log_set_function(const sll_string_t* file,const sll_string_t* func,sll_flags_t flags,sll_bool_t state);



#endif

#ifndef __SLL_API_LOG_H__
#define __SLL_API_LOG_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/log.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Logging API
 * \group log-api
 * \desc Docs!
 */



/**
 * \flags check_output func
 * \name sll_api_log_log
 * \group log-api
 * \desc Docs!
 * \api o!|b
 * \arg sll_object_t*const* data
 * \arg sll_arg_count_t len
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_log_log(sll_object_t*const* data,sll_arg_count_t len);



/**
 * \flags check_output func
 * \name sll_api_log_set
 * \group log-api
 * \desc Docs!
 * \api #@s#@sBb|b
 * \arg sll_string_t* file
 * \arg sll_string_t* func
 * \arg sll_logger_flags_t flags
 * \arg sll_bool_t state
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_log_set(const sll_string_t* file,const sll_string_t* func,sll_logger_flags_t flags,sll_bool_t state);



#endif

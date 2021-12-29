#ifndef __SLL_API_LOG_H__
#define __SLL_API_LOG_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Logging API
 * \group log-api
 * \desc Docs!
 */



/**
 * \flags api func optimizable var_arg
 * \name sll_api_log_log
 * \group log-api
 * \desc Docs!
 * \arg O
 * \ret V
 */
__API_FUNC_DECL(log_log);



/**
 * \flags api func optimizable
 * \name sll_api_log_set_default
 * \group log-api
 * \desc Docs!
 * \arg B
 * \ret V
 */
__API_FUNC_DECL(log_set_default);



/**
 * \flags api func optimizable
 * \name sll_api_log_set_file
 * \group log-api
 * \desc Docs!
 * \arg S
 * \arg B
 * \ret V
 */
__API_FUNC_DECL(log_set_file);



/**
 * \flags api func optimizable
 * \name sll_api_log_set_function
 * \group log-api
 * \desc Docs!
 * \arg S
 * \arg S
 * \arg B
 * \ret V
 */
__API_FUNC_DECL(log_set_function);



#endif

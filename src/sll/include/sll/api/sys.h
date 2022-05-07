#ifndef __SLL_API_SYS_H__
#define __SLL_API_SYS_H__ 1
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/platform/thread.h>
#include <sll/sandbox.h>
#include <sll/string.h>
#include <sll/types.h>
/**
 * \flags group
 * \name System API
 * \group sys-api
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_api_sys_get_args
 * \group sys-api
 * \desc Docs!
 * \api |a
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_args(sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_sys_get_cpu_count
 * \group sys-api
 * \desc Docs!
 * \api |W
 * \ret sll_cpu_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_cpu_t sll_api_sys_get_cpu_count(void);



/**
 * \flags func
 * \name sll_api_sys_get_env
 * \group sys-api
 * \desc Docs!
 * \api |m
 * \arg sll_map_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_env(sll_map_t* out);



/**
 * \flags func
 * \name sll_api_sys_get_executable
 * \group sys-api
 * \desc Docs!
 * \api |s
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_executable(sll_string_t* out);



/**
 * \flags func
 * \name sll_api_sys_get_library
 * \group sys-api
 * \desc Docs!
 * \api |s
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_library(sll_string_t* out);



/**
 * \flags func
 * \name sll_api_sys_get_platform
 * \group sys-api
 * \desc Docs!
 * \api |s
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_platform(sll_string_t* out);



/**
 * \flags check_output func
 * \name sll_api_sys_get_sandbox_flags
 * \group sys-api
 * \desc Docs!
 * \api |Q
 * \ret sll_sandbox_flags_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_sandbox_flags_t sll_api_sys_get_sandbox_flags(void);



/**
 * \flags func
 * \name sll_api_sys_get_version
 * \group sys-api
 * \desc Docs!
 * \api |a
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_get_version(sll_array_t* out);



/**
 * \flags check_output func
 * \name sll_api_sys_load_library
 * \group sys-api
 * \desc Docs!
 * \api #sQQQQQ|b
 * \arg const sll_string_t* path
 * \arg sll_size_t sz
 * \arg __SLL_U64 h0
 * \arg __SLL_U64 h1
 * \arg __SLL_U64 h2
 * \arg __SLL_U64 h3
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_bool_t sll_api_sys_load_library(const sll_string_t* path,sll_size_t sz,__SLL_U64 h0,__SLL_U64 h1,__SLL_U64 h2,__SLL_U64 h3);



/**
 * \flags func
 * \name sll_api_sys_remove_env
 * \group sys-api
 * \desc Docs!
 * \api #s|
 * \arg const sll_string_t* key
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_remove_env(const sll_string_t* key);



/**
 * \flags func
 * \name sll_api_sys_set_sandbox_flag
 * \group sys-api
 * \desc Docs!
 * \api B|
 * \arg sll_sandbox_flag_t flag
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_set_sandbox_flag(sll_sandbox_flag_t flag);



/**
 * \flags func
 * \name sll_api_sys_set_env
 * \group sys-api
 * \desc Docs!
 * \api #s#s|
 * \arg const sll_string_t* key
 * \arg const sll_string_t* value
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_set_env(const sll_string_t* key,const sll_string_t* value);



/**
 * \flags func
 * \name sll_set_argument
 * \group sys-api
 * \desc Docs!
 * \arg sll_array_length_t i
 * \arg const sll_char_t* a
 */
__SLL_EXTERNAL void sll_set_argument(sll_array_length_t i,const sll_char_t* a);



/**
 * \flags func
 * \name sll_set_argument_count
 * \group sys-api
 * \desc Docs!
 * \arg sll_array_length_t ac
 */
__SLL_EXTERNAL void sll_set_argument_count(sll_array_length_t ac);



#endif

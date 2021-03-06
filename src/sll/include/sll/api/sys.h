#ifndef __SLL_API_SYS_H__
#define __SLL_API_SYS_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/platform/library.h>
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
 * \flags type var
 * \name sll_argc_t
 * \group sys-api
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_argc_t;



/**
 * \flags type
 * \name sll_loaded_library_t
 * \group sys-api
 * \desc Docs!
 * \arg const sll_string_t name
 * \arg sll_library_handle_t handle
 */
typedef struct _SLL_LOADED_LIBRARY{
	const sll_string_t name;
	sll_library_handle_t handle;
} sll_loaded_library_t;



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
 * \api #s|Q
 * \arg const sll_string_t* path
 * \ret sll_error_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_sys_load_library(const sll_string_t* path);



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
 * \name sll_api_sys_set_sandbox_flag
 * \group sys-api
 * \desc Docs!
 * \api B|
 * \arg sll_sandbox_flag_t flag
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_sys_set_sandbox_flag(sll_sandbox_flag_t flag);



/**
 * \flags func
 * \name sll_get_loaded_libraries
 * \group sys-api
 * \desc Docs!
 * \arg sll_size_t* count
 * \ret const sll_loaded_library_t*const*
 */
__SLL_EXTERNAL const sll_loaded_library_t*const* sll_get_loaded_libraries(sll_size_t* count);



/**
 * \flags func
 * \name sll_set_argument
 * \group sys-api
 * \desc Docs!
 * \arg sll_argc_t index
 * \arg const sll_char_t* value
 */
__SLL_EXTERNAL void sll_set_argument(sll_argc_t index,const sll_char_t* value);



/**
 * \flags func
 * \name sll_set_argument_count
 * \group sys-api
 * \desc Docs!
 * \arg sll_argc_t value
 */
__SLL_EXTERNAL void sll_set_argument_count(sll_argc_t value);



#endif

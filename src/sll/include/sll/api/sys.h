#ifndef __SLL_API_SYS_H__
#define __SLL_API_SYS_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name System API
 * \group sys-api
 * \desc Docs!
 */



#define SLL_UNKNOWN_LIBRARY_HANDLE ((void*)0xffffffffffffffffull)



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



/**
 * \flags api func optimizable
 * \name sll_api_sys_get_args
 * \group sys-api
 * \desc Docs!
 * \ret A
 */
__API_FUNC_DECL(sys_get_args);



/**
 * \flags api func optimizable
 * \name sll_api_sys_get_executable
 * \group sys-api
 * \desc Docs!
 * \ret S
 */
__API_FUNC_DECL(sys_get_executable);



/**
 * \flags api func optimizable
 * \name sll_api_sys_get_library
 * \group sys-api
 * \desc Docs!
 * \ret S
 */
__API_FUNC_DECL(sys_get_library);



/**
 * \flags api func optimizable
 * \name sll_api_sys_get_platform
 * \group sys-api
 * \desc Docs!
 * \ret S
 */
__API_FUNC_DECL(sys_get_platform);



/**
 * \flags api func optimizable
 * \name sll_api_sys_get_version
 * \group sys-api
 * \desc Docs!
 * \arg I
 * \ret O
 */
__API_FUNC_DECL(sys_get_version);



/**
 * \flags api func optimizable
 * \name sll_api_sys_load_library
 * \group sys-api
 * \desc Docs!
 * \arg S
 * \ret B
 */
__API_FUNC_DECL(sys_load_library);



#endif

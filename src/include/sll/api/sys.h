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



/**
 * \flags func
 * \name sll_set_argument
 * \group sys-api
 * \desc Docs!
 * \arg sll_integer_t i
 * \arg const sll_char_t* a
 */
__SLL_EXTERNAL void sll_set_argument(sll_integer_t i,const sll_char_t* a);



/**
 * \flags func
 * \name sll_set_argument_count
 * \group sys-api
 * \desc Docs!
 * \arg sll_integer_t ac
 */
__SLL_EXTERNAL void sll_set_argument_count(sll_integer_t ac);



/**
 * \flags api func optimizable
 * \name sll_api_sys_arg_get
 * \group sys-api
 * \desc Docs!
 * \arg I
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(sys_arg_get);



/**
 * \flags api func optimizable
 * \name sll_api_sys_arg_get_count
 * \group sys-api
 * \desc Docs!
 * \ret I
 */
__API_FUNC_DECL(sys_arg_get_count);



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
 * \name sll_api_sys_get_platform
 * \group sys-api
 * \desc Docs!
 * \ret S
 */
__API_FUNC_DECL(sys_get_platform);



#endif

#ifndef __SLL_API_SYS_H__
#define __SLL_API_SYS_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



/**
 * \flags func
 * \name sll_set_argument
 * \desc Docs!
 * \arg sll_integer_t i
 * \arg const sll_char_t* a
 */
__SLL_EXTERNAL void sll_set_argument(sll_integer_t i,const sll_char_t* a);



/**
 * \flags func
 * \name sll_set_argument_count
 * \desc Docs!
 * \arg sll_integer_t ac
 */
__SLL_EXTERNAL void sll_set_argument_count(sll_integer_t ac);



/**
 * \flags api func optimizable
 * \name sll_api_sys_arg_get
 * \desc Docs!
 * \arg I
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(sys_arg_get);



/**
 * \flags api func optimizable
 * \name sll_api_sys_arg_get_count
 * \desc Docs!
 * \ret I
 */
__API_FUNC_DECL(sys_arg_get_count);



/**
 * \flags api func optimizable
 * \name sll_api_sys_get_executable
 * \desc Docs!
 * \ret S
 */
__API_FUNC_DECL(sys_get_executable);



/**
 * \flags api func optimizable
 * \name sll_api_sys_get_platform
 * \desc Docs!
 * \ret S
 */
__API_FUNC_DECL(sys_get_platform);



#endif

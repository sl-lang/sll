#ifndef __SLL_API_STRING_H__
#define __SLL_API_STRING_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



/**
 * \flags api compilation_call func optimizable var_arg
 * \name sll_api_string_convert
 * \desc Docs!
 * \arg O
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_convert);



/**
 * \flags api compilation_call func optimizable var_arg
 * \name sll_api_string_format
 * \desc Docs!
 * \arg S
 * \arg O?
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_format);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_string_replace
 * \desc Docs!
 * \arg S
 * \arg CS?
 * \arg CS?
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_replace);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_string_to_lower_case
 * \desc Docs!
 * \arg S
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_to_lower_case);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_string_to_title_case
 * \desc Docs!
 * \arg S
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_to_title_case);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_string_to_upper_case
 * \desc Docs!
 * \arg S
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_to_upper_case);



#endif

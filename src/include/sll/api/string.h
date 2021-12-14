#ifndef __SLL_API_STRING_H__
#define __SLL_API_STRING_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name String API
 * \group string-api
 * \desc Docs!
 */



/**
 * \flags api compilation_call func optimizable var_arg
 * \name sll_api_string_convert
 * \group string-api
 * \desc Docs!
 * \arg O
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_convert);



/**
 * \flags api compilation_call func optimizable var_arg
 * \name sll_api_string_format
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg O?
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_format);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_string_flip_case
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_flip_case);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_string_pad_left
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg I
 * \arg C?
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_pad_left);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_string_pad_right
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg I
 * \arg C?
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_pad_right);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_string_replace
 * \group string-api
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
 * \name sll_api_string_reverse
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_reverse);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_string_split
 * \group string-api
 * \desc Docs!
 * \arg S
 * \arg CS
 * \ret E -> Failure
 * \ret A -> Success
 */
__API_FUNC_DECL(string_split);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_string_to_lower_case
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_to_lower_case);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_string_to_title_case
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_to_title_case);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_string_to_upper_case
 * \group string-api
 * \desc Docs!
 * \arg S
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(string_to_upper_case);



#endif

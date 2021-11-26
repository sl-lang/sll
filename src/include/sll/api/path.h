#ifndef __SLL_API_PATH_H__
#define __SLL_API_PATH_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



#ifdef _MSC_VER
#define SLL_API_FILE_PATH_SEPARATOR '\\'
#define SLL_API_MAX_FILE_PATH_LENGTH 260
#else
#define SLL_API_FILE_PATH_SEPARATOR '/'
#define SLL_API_MAX_FILE_PATH_LENGTH 4096
#endif



__SLL_EXTERNAL sll_string_length_t sll_path_absolute(const sll_char_t* s,sll_char_t* o,sll_string_length_t ol);



/**
 * \flags api func optimizable
 * \name sll_api_path_absolute
 * \desc Docs!
 * \arg S
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(path_absolute);



/**
 * \flags api func optimizable
 * \name sll_api_path_exists
 * \desc Docs!
 * \arg S
 * \ret 0 -> Failure
 * \ret I -> Success
 */
__API_FUNC_DECL(path_exists);



/**
 * \flags api func optimizable
 * \name sll_api_path_get_cwd
 * \desc Docs!
 * \ret S -> Docs!
 */
__API_FUNC_DECL(path_get_cwd);



/**
 * \flags api func optimizable
 * \name sll_api_path_is_dir
 * \desc Docs!
 * \arg S
 * \ret 0 -> Failure
 * \ret I -> Success
 */
__API_FUNC_DECL(path_is_dir);



/**
 * \flags api func optimizable var_arg
 * \name sll_api_path_join
 * \desc Docs!
 * \arg S
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(path_join);



/**
 * \flags api func optimizable
 * \name sll_api_path_list_dir
 * \desc Docs!
 * \arg S
 * \ret E -> Failure
 * \ret A -> Success
 */
__API_FUNC_DECL(path_list_dir);



/**
 * \flags api func optimizable
 * \name sll_api_path_recursive_list_dir
 * \desc Docs!
 * \arg S
 * \ret E -> Failure
 * \ret A -> Success
 */
__API_FUNC_DECL(path_recursive_list_dir);



/**
 * \flags api func optimizable
 * \name sll_api_path_relative
 * \desc Docs!
 * \arg S
 * \ret Z -> Failure
 * \ret S -> Success
 */
__API_FUNC_DECL(path_relative);



/**
 * \flags api func optimizable
 * \name sll_api_path_set_cwd
 * \arg S
 * \ret 0 -> Failure
 * \ret I -> Docs!
 */
__API_FUNC_DECL(path_set_cwd);



/**
 * \flags api func optimizable
 * \name sll_api_path_size
 * \desc Docs!
 * \arg S
 * \ret 0 -> Failure
 * \ret I -> Success
 */
__API_FUNC_DECL(path_size);



#endif

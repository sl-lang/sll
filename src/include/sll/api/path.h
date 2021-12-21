#ifndef __SLL_API_PATH_H__
#define __SLL_API_PATH_H__ 1
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Path API
 * \group path-api
 * \desc Docs!
 */



#ifdef _MSC_VER
#define SLL_API_FILE_PATH_SEPARATOR '\\'
#define SLL_API_MAX_FILE_PATH_LENGTH 260
#else
#define SLL_API_FILE_PATH_SEPARATOR '/'
#define SLL_API_MAX_FILE_PATH_LENGTH 4096
#endif



/**
 * \flags func
 * \name sll_path_absolute
 * \group path-api
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg sll_char_t* o
 * \arg sll_string_length_t ol
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL sll_string_length_t sll_path_absolute(const sll_char_t* s,sll_char_t* o,sll_string_length_t ol);



/**
 * \flags func
 * \name sll_path_split
 * \group path-api
 * \desc Docs!
 * \arg const sll_string_t* s
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL sll_string_length_t sll_path_split(const sll_string_t* s);



/**
 * \flags api func optimizable
 * \name sll_api_path_absolute
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(path_absolute);



/**
 * \flags api func optimizable
 * \name sll_api_path_exists
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret I
 */
__API_FUNC_DECL(path_exists);



/**
 * \flags api func optimizable
 * \name sll_api_path_get_cwd
 * \group path-api
 * \desc Docs!
 * \ret S
 */
__API_FUNC_DECL(path_get_cwd);



/**
 * \flags api func optimizable
 * \name sll_api_path_is_dir
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret I
 */
__API_FUNC_DECL(path_is_dir);



/**
 * \flags api func optimizable var_arg
 * \name sll_api_path_join
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(path_join);



/**
 * \flags api func optimizable
 * \name sll_api_path_list_dir
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret A
 */
__API_FUNC_DECL(path_list_dir);



/**
 * \flags api func optimizable
 * \name sll_api_path_recursive_list_dir
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret A
 */
__API_FUNC_DECL(path_recursive_list_dir);



/**
 * \flags api func optimizable
 * \name sll_api_path_relative
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(path_relative);



/**
 * \flags api func optimizable
 * \name sll_api_path_set_cwd
 * \group json-api
 * \desc Docs!
 * \arg S
 * \ret I
 */
__API_FUNC_DECL(path_set_cwd);



/**
 * \flags api func optimizable
 * \name sll_api_path_size
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret I
 */
__API_FUNC_DECL(path_size);



/**
 * \flags api compilation_call func optimizable
 * \name sll_api_path_split
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret A
 */
__API_FUNC_DECL(path_split);



#endif

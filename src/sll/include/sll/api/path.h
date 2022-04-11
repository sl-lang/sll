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



/**
 * \flags macro var
 * \name SLL_API_FILE_PATH_SEPARATOR
 * \group version
 * \desc Docs!
 * \type sll_char_t
 */
/**
 * \flags macro var
 * \name SLL_API_MAX_FILE_PATH_LENGTH
 * \group version
 * \desc Docs!
 * \type sll_string_length_t
 */
#ifdef __SLL_BUILD_WINDOWS
#define SLL_API_FILE_PATH_SEPARATOR '\\'
#define SLL_API_MAX_FILE_PATH_LENGTH 261
#else
#define SLL_API_FILE_PATH_SEPARATOR '/'
#ifdef __SLL_BUILD_DARWIN
#define SLL_API_MAX_FILE_PATH_LENGTH 1024
#else
#define SLL_API_MAX_FILE_PATH_LENGTH 4096
#endif
#endif



/**
 * \flags func
 * \name sll_path_relative
 * \group path-api
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg const sll_char_t* b
 * \arg sll_string_t* o
 */
__SLL_EXTERNAL void sll_path_relative(const sll_char_t* s,const sll_char_t* b,sll_string_t* o);



/**
 * \flags check_output func
 * \name sll_path_split
 * \group path-api
 * \desc Docs!
 * \arg const sll_string_t* s
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_path_split(const sll_string_t* s);



/**
 * \flags check_output func
 * \name sll_path_split_drive
 * \group path-api
 * \desc Docs!
 * \arg const sll_string_t* s
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_path_split_drive(const sll_string_t* s);



/**
 * \flags api func
 * \name sll_api_path_absolute
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(path_absolute);



/**
 * \flags api func
 * \name sll_api_path_exists
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret I
 */
__API_FUNC_DECL(path_exists);



/**
 * \flags api func
 * \name sll_api_path_get_cwd
 * \group path-api
 * \desc Docs!
 * \ret S
 */
__API_FUNC_DECL(path_get_cwd);



/**
 * \flags api func
 * \name sll_api_path_is_dir
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret I
 */
__API_FUNC_DECL(path_is_dir);



/**
 * \flags api func var_arg
 * \name sll_api_path_join
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(path_join);



/**
 * \flags api func
 * \name sll_api_path_list_dir
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret A
 */
__API_FUNC_DECL(path_list_dir);



/**
 * \flags api func
 * \name sll_api_path_mkdir
 * \group path-api
 * \desc Docs!
 * \arg S
 * \arg B
 * \ret I
 */
__API_FUNC_DECL(path_mkdir);



/**
 * \flags api func
 * \name sll_api_path_recursive_list_dir
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret A
 */
__API_FUNC_DECL(path_recursive_list_dir);



/**
 * \flags api func
 * \name sll_api_path_relative
 * \group path-api
 * \desc Docs!
 * \arg S
 * \arg S
 * \ret S
 */
__API_FUNC_DECL(path_relative);



/**
 * \flags api func
 * \name sll_api_path_set_cwd
 * \group json-api
 * \desc Docs!
 * \arg S
 * \ret I
 */
__API_FUNC_DECL(path_set_cwd);



/**
 * \flags api func
 * \name sll_api_path_size
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret I
 */
__API_FUNC_DECL(path_size);



/**
 * \flags api func
 * \name sll_api_path_split
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret A
 */
__API_FUNC_DECL(path_split);



/**
 * \flags api func
 * \name sll_api_path_split_drive
 * \group path-api
 * \desc Docs!
 * \arg S
 * \ret A
 */
__API_FUNC_DECL(path_split_drive);



#endif

#ifndef __SLL_API_PATH_H__
#define __SLL_API_PATH_H__ 1
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
 * \flags func
 * \name sll_api_path_absolute
 * \group path-api
 * \desc Docs!
 * \api s|s
 * \arg const sll_string_t* path
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_absolute(const sll_string_t* path,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_path_exists
 * \group path-api
 * \desc Docs!
 * \api s|i
 * \arg const sll_string_t* path
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_path_exists(const sll_string_t* path);



/**
 * \flags func
 * \name sll_api_path_get_cwd
 * \group path-api
 * \desc Docs!
 * \api |o
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_get_cwd(void);



/**
 * \flags func
 * \name sll_api_path_is_dir
 * \group path-api
 * \desc Docs!
 * \api s|i
 * \arg const sll_string_t* path
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_path_is_dir(const sll_string_t* path);



/**
 * \flags func
 * \name sll_api_path_join
 * \group path-api
 * \desc Docs!
 * \api s!|s
 * \arg const sll_string_t*const* parts
 * \arg sll_arg_count_t len
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_join(const sll_string_t*const* parts,sll_arg_count_t len,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_path_list_dir
 * \group path-api
 * \desc Docs!
 * \api s|o
 * \arg const sll_string_t* path
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_list_dir(const sll_string_t* path);



/**
 * \flags func
 * \name sll_api_path_mkdir
 * \group path-api
 * \desc Docs!
 * \api sb|i
 * \arg const sll_string_t* path
 * \arg sll_bool_t all
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_path_mkdir(const sll_string_t* path,sll_bool_t all);



/**
 * \flags func
 * \name sll_api_path_recursive_list_dir
 * \group path-api
 * \desc Docs!
 * \api s|o
 * \arg const sll_string_t* path
 * \ret sll_object_t*
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_path_recursive_list_dir(const sll_string_t* path);



/**
 * \flags func
 * \name sll_api_path_relative
 * \group path-api
 * \desc Docs!
 * \api ss|s
 * \arg const sll_string_t* path
 * \arg const sll_string_t* base
 * \arg sll_string_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_relative(const sll_string_t* path,const sll_string_t* base,sll_string_t* out);



/**
 * \flags func
 * \name sll_api_path_set_cwd
 * \group json-api
 * \desc Docs!
 * \api s|i
 * \arg const sll_string_t* path
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_path_set_cwd(const sll_string_t* path);



/**
 * \flags func
 * \name sll_api_path_size
 * \group path-api
 * \desc Docs!
 * \api s|i
 * \arg const sll_string_t* path
 * \ret sll_integer_t
 */
__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_integer_t sll_api_path_size(const sll_string_t* path);



/**
 * \flags func
 * \name sll_api_path_split
 * \group path-api
 * \desc Docs!
 * \api s|a
 * \arg const sll_string_t* path
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_split(const sll_string_t* path,sll_array_t* out);



/**
 * \flags func
 * \name sll_api_path_split_drive
 * \group path-api
 * \desc Docs!
 * \api s|a
 * \arg const sll_string_t* path
 * \arg sll_array_t* out
 */
__SLL_EXTERNAL __SLL_API_CALL void sll_api_path_split_drive(const sll_string_t* path,sll_array_t* out);



#endif

#ifndef __SLL_PLATFORM_PATH_H__
#define __SLL_PLATFORM_PATH_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags subgroup
 * \name Paths
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 */



/**
 * \flags var
 * \name sll_executable_file_path
 * \group platform
 * \desc Docs!
 * \type const sll_string_t*
 */
__SLL_EXTERNAL extern const sll_string_t* sll_executable_file_path;



/**
 * \flags var
 * \name sll_library_file_path
 * \group platform
 * \desc Docs!
 * \type const sll_string_t*
 */
__SLL_EXTERNAL extern const sll_string_t* sll_library_file_path;



/**
 * \flags var
 * \name sll_temporary_file_path
 * \group platform
 * \desc Docs!
 * \type const sll_string_t*
 */
__SLL_EXTERNAL extern const sll_string_t* sll_temporary_file_path;



/**
 * \flags func
 * \name sll_platform_absolute_path
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg sll_char_t* o
 * \arg sll_string_length_t ol
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL sll_string_length_t sll_platform_absolute_path(const sll_char_t* fp,sll_char_t* o,sll_string_length_t ol);



/**
 * \flags check_output func
 * \name sll_platform_create_directory
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg sll_bool_t all
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_create_directory(const sll_char_t* fp,sll_bool_t all);



/**
 * \flags check_output func
 * \name sll_platform_get_current_working_directory
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg sll_char_t* o
 * \arg sll_string_length_t ol
 * \ret sll_string_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_current_working_directory(sll_char_t* o,sll_string_length_t ol);



/**
 * \flags check_output func
 * \name sll_platform_list_directory
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg sll_string_t** o
 * \ret sll_array_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory(const sll_char_t* fp,sll_string_t** o);



/**
 * \flags check_output func
 * \name sll_platform_list_directory_recursive
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg sll_string_t** o
 * \ret sll_array_length_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_array_length_t sll_platform_list_directory_recursive(const sll_char_t* fp,sll_string_t** o);



/**
 * \flags check_output func
 * \name sll_platform_path_copy
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg const sll_char_t* d
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_copy(const sll_char_t* s,const sll_char_t* d);



/**
 * \flags check_output func
 * \name sll_platform_path_delete
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_delete(const sll_char_t* fp);



/**
 * \flags check_output func
 * \name sll_platform_path_exists
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_exists(const sll_char_t* fp);



/**
 * \flags check_output func
 * \name sll_platform_path_is_directory
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_is_directory(const sll_char_t* fp);



/**
 * \flags check_output func
 * \name sll_platform_path_rename
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* s
 * \arg const sll_char_t* d
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_path_rename(const sll_char_t* s,const sll_char_t* d);



/**
 * \flags check_output func
 * \name sll_platform_set_current_working_directory
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* p
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_set_current_working_directory(const sll_char_t* p);



/**
 * \flags func
 * \name sll_platform_set_environment_variable
 * \group platform
 * \subgroup platform-path
 * \desc Docs!
 * \arg const sll_char_t* k
 * \arg const sll_char_t* v
 */
__SLL_EXTERNAL void sll_platform_set_environment_variable(const sll_char_t* k,const sll_char_t* v);



#endif

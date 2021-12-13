// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!
#ifndef __SLL_API__GENERATED__
#define __SLL_API__GENERATED__
#include <sll/common.h>
#include <sll/types.h>



#define __SLL_API_TYPE_sll_api_array_count __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_array_count const sll_array_t* a,const sll_object_t* b
/**
 * \flags check_output func optimizable
 * \name sll_api_array_count
 * \group array-api
 * \desc Docs!
 * \arg const sll_array_t* a -> 
 * \arg const sll_object_t* b -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_array_count_raw
 * \group raw-api
 * \subgroup raw-api-array-api
 * \desc Wrapper function for :sll_api_array_count:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */
/**
 * \flags subgroup
 * \name Array
 * \group raw-api
 * \subgroup raw-api-array-api
 * \desc Docs!
 */



#define __SLL_API_TYPE_sll_api_array_create void
#define __SLL_API_ARGS_sll_api_array_create const sll_integer_t a,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_array_create
 * \group array-api
 * \desc Docs!
 * \arg const sll_integer_t a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_array_create_raw
 * \group raw-api
 * \subgroup raw-api-array-api
 * \desc Wrapper function for :sll_api_array_create:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_array_reverse void
#define __SLL_API_ARGS_sll_api_array_reverse const sll_array_t* a,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_array_reverse
 * \group array-api
 * \desc Docs!
 * \arg const sll_array_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_array_reverse_raw
 * \group raw-api
 * \subgroup raw-api-array-api
 * \desc Wrapper function for :sll_api_array_reverse:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_file_close __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_file_close const sll_handle_data_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_file_close
 * \group file-api
 * \desc Closes a file previously opened by :sll_api_file_open: and invalidates its handle
 * \arg const sll_handle_data_t* a -> Previously allocated file handle
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_file_close_raw
 * \group raw-api
 * \subgroup raw-api-file-api
 * \desc Wrapper function for :sll_api_file_close:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */
/**
 * \flags subgroup
 * \name File
 * \group raw-api
 * \subgroup raw-api-file-api
 * \desc Docs!
 */



#define __SLL_API_TYPE_sll_api_file_open void
#define __SLL_API_ARGS_sll_api_file_open const sll_string_t* a,const sll_string_t* b,sll_handle_data_t* out
/**
 * \flags func optimizable
 * \name sll_api_file_open
 * \group file-api
 * \desc Opens a file with the specified mode
 * \arg const sll_string_t* a -> The path to the file to open
 * \arg const sll_string_t* b -> The optional file mode
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_file_open_raw
 * \group raw-api
 * \subgroup raw-api-file-api
 * \desc Wrapper function for :sll_api_file_open:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_file_read void
#define __SLL_API_ARGS_sll_api_file_read const sll_handle_data_t* a,const sll_integer_t b,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_file_read
 * \group file-api
 * \desc Reads from a file prevoiusly opened by :sll_api_file_open:
 * \arg const sll_handle_data_t* a -> Previously opened file handle
 * \arg const sll_integer_t b -> Maximum number of bytes to read
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_file_read_raw
 * \group raw-api
 * \subgroup raw-api-file-api
 * \desc Wrapper function for :sll_api_file_read:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_file_std_handle void
#define __SLL_API_ARGS_sll_api_file_std_handle const sll_integer_t a,sll_handle_data_t* out
/**
 * \flags func optimizable
 * \name sll_api_file_std_handle
 * \group file-api
 * \desc Docs!
 * \arg const sll_integer_t a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_file_std_handle_raw
 * \group raw-api
 * \subgroup raw-api-file-api
 * \desc Wrapper function for :sll_api_file_std_handle:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_file_write __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_file_write const sll_handle_data_t* a,const sll_object_t*const* b,sll_arg_count_t bc
/**
 * \flags check_output func optimizable
 * \name sll_api_file_write
 * \group file-api
 * \desc Writes to a file prevoiusly opened by :sll_api_file_open:
 * \arg const sll_handle_data_t* a -> Previously opened file handle
 * \arg const sll_object_t*const* b -> The object to write to file
 * \arg sll_arg_count_t bc
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_file_write_raw
 * \group raw-api
 * \subgroup raw-api-file-api
 * \desc Wrapper function for :sll_api_file_write:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_hash_create __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_hash_create const sll_object_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_hash_create
 * \group hash-api
 * \desc Docs!
 * \arg const sll_object_t* a -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_hash_create_raw
 * \group raw-api
 * \subgroup raw-api-hash-api
 * \desc Wrapper function for :sll_api_hash_create:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */
/**
 * \flags subgroup
 * \name Hash
 * \group raw-api
 * \subgroup raw-api-hash-api
 * \desc Docs!
 */



#define __SLL_API_TYPE_sll_api_json_parse __SLL_CHECK_OUTPUT sll_object_t*
#define __SLL_API_ARGS_sll_api_json_parse const sll_string_t* a
/**
 * \flags func optimizable
 * \name sll_api_json_parse
 * \group json-api
 * \desc Converts a [JSON][https://www.json.org/json-en.html] string into Sll types
 * \arg const sll_string_t* a -> The string to parse
 * \ret sll_object_t*
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_json_parse_raw
 * \group raw-api
 * \subgroup raw-api-json-api
 * \desc Wrapper function for :sll_api_json_parse:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */
/**
 * \flags subgroup
 * \name JSON
 * \group raw-api
 * \subgroup raw-api-json-api
 * \desc Docs!
 */



#define __SLL_API_TYPE_sll_api_json_stringify void
#define __SLL_API_ARGS_sll_api_json_stringify const sll_object_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_json_stringify
 * \group json-api
 * \desc Docs!
 * \arg const sll_object_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_json_stringify_raw
 * \group raw-api
 * \subgroup raw-api-json-api
 * \desc Wrapper function for :sll_api_json_stringify:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_json_type void
#define __SLL_API_ARGS_sll_api_json_type const sll_integer_t a,sll_handle_data_t* out
/**
 * \flags func optimizable
 * \name sll_api_json_type
 * \group json-api
 * \desc Docs!
 * \arg const sll_integer_t a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_json_type_raw
 * \group raw-api
 * \subgroup raw-api-json-api
 * \desc Wrapper function for :sll_api_json_type:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_absolute void
#define __SLL_API_ARGS_sll_api_path_absolute const sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_path_absolute
 * \group path-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_absolute_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_absolute:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */
/**
 * \flags subgroup
 * \name Path
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Docs!
 */



#define __SLL_API_TYPE_sll_api_path_exists __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_path_exists const sll_string_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_path_exists
 * \group path-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_exists_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_exists:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_get_cwd void
#define __SLL_API_ARGS_sll_api_path_get_cwd sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_path_get_cwd
 * \group path-api
 * \desc Docs!
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_get_cwd_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_get_cwd:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_is_dir __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_path_is_dir const sll_string_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_path_is_dir
 * \group path-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_is_dir_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_is_dir:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_join void
#define __SLL_API_ARGS_sll_api_path_join const sll_string_t*const* a,sll_arg_count_t ac,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_path_join
 * \group path-api
 * \desc Docs!
 * \arg const sll_string_t*const* a -> 
 * \arg sll_arg_count_t ac
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_join_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_join:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_list_dir void
#define __SLL_API_ARGS_sll_api_path_list_dir const sll_string_t* a,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_path_list_dir
 * \group path-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_list_dir_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_list_dir:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_recursive_list_dir void
#define __SLL_API_ARGS_sll_api_path_recursive_list_dir const sll_string_t* a,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_path_recursive_list_dir
 * \group path-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_recursive_list_dir_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_recursive_list_dir:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_relative void
#define __SLL_API_ARGS_sll_api_path_relative const sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_path_relative
 * \group path-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_relative_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_relative:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_set_cwd __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_path_set_cwd const sll_string_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_path_set_cwd
 * \group json-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_set_cwd_raw
 * \group raw-api
 * \subgroup raw-api-json-api
 * \desc Wrapper function for :sll_api_path_set_cwd:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_size __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_path_size const sll_string_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_path_size
 * \group path-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_size_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_size:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_sort_sort void
#define __SLL_API_ARGS_sll_api_sort_sort const sll_array_t* a,const sll_integer_t b,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_sort_sort
 * \group sort-api
 * \desc Docs!
 * \arg const sll_array_t* a -> 
 * \arg const sll_integer_t b -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_sort_sort_raw
 * \group raw-api
 * \subgroup raw-api-sort-api
 * \desc Wrapper function for :sll_api_sort_sort:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */
/**
 * \flags subgroup
 * \name Sort
 * \group raw-api
 * \subgroup raw-api-sort-api
 * \desc Docs!
 */



#define __SLL_API_TYPE_sll_api_string_convert void
#define __SLL_API_ARGS_sll_api_string_convert const sll_object_t*const* a,sll_arg_count_t ac,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_convert
 * \group string-api
 * \desc Docs!
 * \arg const sll_object_t*const* a -> 
 * \arg sll_arg_count_t ac
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_convert_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_convert:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */
/**
 * \flags subgroup
 * \name String
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Docs!
 */



#define __SLL_API_TYPE_sll_api_string_format void
#define __SLL_API_ARGS_sll_api_string_format const sll_string_t* a,const sll_object_t*const* b,sll_arg_count_t bc,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_format
 * \group string-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 * \arg const sll_object_t*const* b -> 
 * \arg sll_arg_count_t bc
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_format_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_format:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_flip_case void
#define __SLL_API_ARGS_sll_api_string_flip_case const sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_flip_case
 * \group string-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_flip_case_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_flip_case:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_replace void
#define __SLL_API_ARGS_sll_api_string_replace const sll_string_t* a,const sll_object_t* b,const sll_object_t* c,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_replace
 * \group string-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 * \arg const sll_object_t* b -> 
 * \arg const sll_object_t* c -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_replace_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_replace:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_reverse void
#define __SLL_API_ARGS_sll_api_string_reverse const sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_reverse
 * \group string-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_reverse_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_reverse:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_split void
#define __SLL_API_ARGS_sll_api_string_split const sll_string_t* a,const sll_object_t* b,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_split
 * \group string-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 * \arg const sll_object_t* b -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_split_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_split:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_to_lower_case void
#define __SLL_API_ARGS_sll_api_string_to_lower_case const sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_to_lower_case
 * \group string-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_to_lower_case_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_to_lower_case:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_to_title_case void
#define __SLL_API_ARGS_sll_api_string_to_title_case const sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_to_title_case
 * \group string-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_to_title_case_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_to_title_case:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_to_upper_case void
#define __SLL_API_ARGS_sll_api_string_to_upper_case const sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_to_upper_case
 * \group string-api
 * \desc Docs!
 * \arg const sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_to_upper_case_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_to_upper_case:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_sys_arg_get void
#define __SLL_API_ARGS_sll_api_sys_arg_get const sll_integer_t a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_sys_arg_get
 * \group sys-api
 * \desc Docs!
 * \arg const sll_integer_t a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_sys_arg_get_raw
 * \group raw-api
 * \subgroup raw-api-sys-api
 * \desc Wrapper function for :sll_api_sys_arg_get:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */
/**
 * \flags subgroup
 * \name System
 * \group raw-api
 * \subgroup raw-api-sys-api
 * \desc Docs!
 */



#define __SLL_API_TYPE_sll_api_sys_arg_get_count __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_sys_arg_get_count void
/**
 * \flags check_output func optimizable
 * \name sll_api_sys_arg_get_count
 * \group sys-api
 * \desc Docs!
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_sys_arg_get_count_raw
 * \group raw-api
 * \subgroup raw-api-sys-api
 * \desc Wrapper function for :sll_api_sys_arg_get_count:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_sys_get_executable void
#define __SLL_API_ARGS_sll_api_sys_get_executable sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_sys_get_executable
 * \group sys-api
 * \desc Docs!
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_sys_get_executable_raw
 * \group raw-api
 * \subgroup raw-api-sys-api
 * \desc Wrapper function for :sll_api_sys_get_executable:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_sys_get_platform void
#define __SLL_API_ARGS_sll_api_sys_get_platform sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_sys_get_platform
 * \group sys-api
 * \desc Docs!
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_sys_get_platform_raw
 * \group raw-api
 * \subgroup raw-api-sys-api
 * \desc Wrapper function for :sll_api_sys_get_platform:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_time_current __SLL_CHECK_OUTPUT sll_float_t
#define __SLL_API_ARGS_sll_api_time_current void
/**
 * \flags check_output func optimizable
 * \name sll_api_time_current
 * \group time-api
 * \desc Docs!
 * \ret sll_float_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_time_current_raw
 * \group raw-api
 * \subgroup raw-api-time-api
 * \desc Wrapper function for :sll_api_time_current:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */
/**
 * \flags subgroup
 * \name Time
 * \group raw-api
 * \subgroup raw-api-time-api
 * \desc Docs!
 */



#define __SLL_API_TYPE_sll_api_time_current_nanos __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_time_current_nanos void
/**
 * \flags check_output func optimizable
 * \name sll_api_time_current_nanos
 * \group time-api
 * \desc Docs!
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_time_current_nanos_raw
 * \group raw-api
 * \subgroup raw-api-time-api
 * \desc Wrapper function for :sll_api_time_current_nanos:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_time_sleep __SLL_CHECK_OUTPUT sll_float_t
#define __SLL_API_ARGS_sll_api_time_sleep const sll_object_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_time_sleep
 * \group time-api
 * \desc Docs!
 * \arg const sll_object_t* a -> 
 * \ret sll_float_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_time_sleep_raw
 * \group raw-api
 * \subgroup raw-api-time-api
 * \desc Wrapper function for :sll_api_time_sleep:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_time_sleep_nanos __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_time_sleep_nanos const sll_object_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_time_sleep_nanos
 * \group time-api
 * \desc Docs!
 * \arg const sll_object_t* a -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_time_sleep_nanos_raw
 * \group raw-api
 * \subgroup raw-api-time-api
 * \desc Wrapper function for :sll_api_time_sleep_nanos:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_util_instruction_count __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_util_instruction_count void
/**
 * \flags check_output func optimizable
 * \name sll_api_util_instruction_count
 * \group util-api
 * \desc Docs!
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_util_instruction_count_raw
 * \group raw-api
 * \subgroup raw-api-util-api
 * \desc Wrapper function for :sll_api_util_instruction_count:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */
/**
 * \flags subgroup
 * \name Utility
 * \group raw-api
 * \subgroup raw-api-util-api
 * \desc Docs!
 */



#define __SLL_API_TYPE_sll_api_util_ref_count __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_util_ref_count const sll_object_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_util_ref_count
 * \group util-api
 * \desc Docs!
 * \arg const sll_object_t* a -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_util_ref_count_raw
 * \group raw-api
 * \subgroup raw-api-util-api
 * \desc Wrapper function for :sll_api_util_ref_count:
 * \arg const sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#endif

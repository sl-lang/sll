// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!
#ifndef __SLL_API__GENERATED__
#define __SLL_API__GENERATED__
#include <sll/common.h>
#include <sll/types.h>



#define __SLL_API_TYPE_sll_api_array_count __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_array_count sll_array_t* a,sll_object_t* b
/**
 * \flags check_output func optimizable
 * \name sll_api_array_count
 * \group array-api
 * \desc Docs!
 * \arg sll_array_t* a -> 
 * \arg sll_object_t* b -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_array_count_raw
 * \group raw-api
 * \subgroup raw-api-array-api
 * \desc Wrapper function for :sll_api_array_count:
 * \arg sll_object_t*const* al -> Arguments
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
#define __SLL_API_ARGS_sll_api_array_create sll_integer_t a,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_array_create
 * \group array-api
 * \desc Docs!
 * \arg sll_integer_t a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_array_create_raw
 * \group raw-api
 * \subgroup raw-api-array-api
 * \desc Wrapper function for :sll_api_array_create:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_array_replace void
#define __SLL_API_ARGS_sll_api_array_replace sll_array_t* a,sll_object_t* b,sll_object_t* c,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_array_replace
 * \group array-api
 * \desc Docs!
 * \arg sll_array_t* a -> 
 * \arg sll_object_t* b -> 
 * \arg sll_object_t* c -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_array_replace_raw
 * \group raw-api
 * \subgroup raw-api-array-api
 * \desc Wrapper function for :sll_api_array_replace:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_array_reverse void
#define __SLL_API_ARGS_sll_api_array_reverse sll_array_t* a,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_array_reverse
 * \group array-api
 * \desc Docs!
 * \arg sll_array_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_array_reverse_raw
 * \group raw-api
 * \subgroup raw-api-array-api
 * \desc Wrapper function for :sll_api_array_reverse:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_file_close __SLL_CHECK_OUTPUT sll_bool_t
#define __SLL_API_ARGS_sll_api_file_close sll_handle_data_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_file_close
 * \group file-api
 * \desc Closes a file previously opened by :sll_api_file_open: and invalidates its handle
 * \arg sll_handle_data_t* a -> Previously allocated file handle
 * \ret sll_bool_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_file_close_raw
 * \group raw-api
 * \subgroup raw-api-file-api
 * \desc Wrapper function for :sll_api_file_close:
 * \arg sll_object_t*const* al -> Arguments
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



#define __SLL_API_TYPE_sll_api_file_flush void
#define __SLL_API_ARGS_sll_api_file_flush sll_handle_data_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_file_flush
 * \group file-api
 * \desc Docs!
 * \arg sll_handle_data_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_file_flush_raw
 * \group raw-api
 * \subgroup raw-api-file-api
 * \desc Wrapper function for :sll_api_file_flush:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_file_open void
#define __SLL_API_ARGS_sll_api_file_open sll_string_t* a,sll_string_t* b,sll_handle_data_t* out
/**
 * \flags func optimizable
 * \name sll_api_file_open
 * \group file-api
 * \desc Opens a file with the specified mode
 * \arg sll_string_t* a -> The path to the file to open
 * \arg sll_string_t* b -> The optional file mode
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_file_open_raw
 * \group raw-api
 * \subgroup raw-api-file-api
 * \desc Wrapper function for :sll_api_file_open:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_file_read void
#define __SLL_API_ARGS_sll_api_file_read sll_handle_data_t* a,sll_integer_t b,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_file_read
 * \group file-api
 * \desc Reads from a file prevoiusly opened by :sll_api_file_open:
 * \arg sll_handle_data_t* a -> Previously opened file handle
 * \arg sll_integer_t b -> Maximum number of bytes to read
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_file_read_raw
 * \group raw-api
 * \subgroup raw-api-file-api
 * \desc Wrapper function for :sll_api_file_read:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_file_std_handle void
#define __SLL_API_ARGS_sll_api_file_std_handle sll_integer_t a,sll_handle_data_t* out
/**
 * \flags func optimizable
 * \name sll_api_file_std_handle
 * \group file-api
 * \desc Docs!
 * \arg sll_integer_t a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_file_std_handle_raw
 * \group raw-api
 * \subgroup raw-api-file-api
 * \desc Wrapper function for :sll_api_file_std_handle:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_file_write __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_file_write sll_handle_data_t* a,sll_object_t*const* b,sll_arg_count_t bc
/**
 * \flags check_output func optimizable
 * \name sll_api_file_write
 * \group file-api
 * \desc Writes to a file prevoiusly opened by :sll_api_file_open:
 * \arg sll_handle_data_t* a -> Previously opened file handle
 * \arg sll_object_t*const* b -> The object to write to file
 * \arg sll_arg_count_t bc
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_file_write_raw
 * \group raw-api
 * \subgroup raw-api-file-api
 * \desc Wrapper function for :sll_api_file_write:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_hash_create __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_hash_create sll_object_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_hash_create
 * \group hash-api
 * \desc Docs!
 * \arg sll_object_t* a -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_hash_create_raw
 * \group raw-api
 * \subgroup raw-api-hash-api
 * \desc Wrapper function for :sll_api_hash_create:
 * \arg sll_object_t*const* al -> Arguments
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
#define __SLL_API_ARGS_sll_api_json_parse sll_string_t* a
/**
 * \flags func optimizable
 * \name sll_api_json_parse
 * \group json-api
 * \desc Converts a [JSON][https://www.json.org/json-en.html] string into Sll types
 * \arg sll_string_t* a -> The string to parse
 * \ret sll_object_t*
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_json_parse_raw
 * \group raw-api
 * \subgroup raw-api-json-api
 * \desc Wrapper function for :sll_api_json_parse:
 * \arg sll_object_t*const* al -> Arguments
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
#define __SLL_API_ARGS_sll_api_json_stringify sll_object_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_json_stringify
 * \group json-api
 * \desc Docs!
 * \arg sll_object_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_json_stringify_raw
 * \group raw-api
 * \subgroup raw-api-json-api
 * \desc Wrapper function for :sll_api_json_stringify:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_json_type void
#define __SLL_API_ARGS_sll_api_json_type sll_integer_t a,sll_handle_data_t* out
/**
 * \flags func optimizable
 * \name sll_api_json_type
 * \group json-api
 * \desc Docs!
 * \arg sll_integer_t a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_json_type_raw
 * \group raw-api
 * \subgroup raw-api-json-api
 * \desc Wrapper function for :sll_api_json_type:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_log_log void
#define __SLL_API_ARGS_sll_api_log_log sll_object_t*const* a,sll_arg_count_t ac
/**
 * \flags check_output func optimizable
 * \name sll_api_log_log
 * \group log-api
 * \desc Docs!
 * \arg sll_object_t*const* a -> 
 * \arg sll_arg_count_t ac
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_log_log_raw
 * \group raw-api
 * \subgroup raw-api-log-api
 * \desc Wrapper function for :sll_api_log_log:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */
/**
 * \flags subgroup
 * \name Logging
 * \group raw-api
 * \subgroup raw-api-log-api
 * \desc Docs!
 */



#define __SLL_API_TYPE_sll_api_log_set_default void
#define __SLL_API_ARGS_sll_api_log_set_default sll_bool_t a
/**
 * \flags check_output func optimizable
 * \name sll_api_log_set_default
 * \group log-api
 * \desc Docs!
 * \arg sll_bool_t a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_log_set_default_raw
 * \group raw-api
 * \subgroup raw-api-log-api
 * \desc Wrapper function for :sll_api_log_set_default:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_log_set_file void
#define __SLL_API_ARGS_sll_api_log_set_file sll_string_t* a,sll_bool_t b
/**
 * \flags check_output func optimizable
 * \name sll_api_log_set_file
 * \group log-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_bool_t b -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_log_set_file_raw
 * \group raw-api
 * \subgroup raw-api-log-api
 * \desc Wrapper function for :sll_api_log_set_file:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_log_set_function void
#define __SLL_API_ARGS_sll_api_log_set_function sll_string_t* a,sll_string_t* b,sll_bool_t c
/**
 * \flags check_output func optimizable
 * \name sll_api_log_set_function
 * \group log-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_string_t* b -> 
 * \arg sll_bool_t c -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_log_set_function_raw
 * \group raw-api
 * \subgroup raw-api-log-api
 * \desc Wrapper function for :sll_api_log_set_function:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_absolute void
#define __SLL_API_ARGS_sll_api_path_absolute sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_path_absolute
 * \group path-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_absolute_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_absolute:
 * \arg sll_object_t*const* al -> Arguments
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
#define __SLL_API_ARGS_sll_api_path_exists sll_string_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_path_exists
 * \group path-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_exists_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_exists:
 * \arg sll_object_t*const* al -> Arguments
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
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_is_dir __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_path_is_dir sll_string_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_path_is_dir
 * \group path-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_is_dir_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_is_dir:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_join void
#define __SLL_API_ARGS_sll_api_path_join sll_string_t*const* a,sll_arg_count_t ac,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_path_join
 * \group path-api
 * \desc Docs!
 * \arg sll_string_t*const* a -> 
 * \arg sll_arg_count_t ac
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_join_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_join:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_list_dir void
#define __SLL_API_ARGS_sll_api_path_list_dir sll_string_t* a,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_path_list_dir
 * \group path-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_list_dir_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_list_dir:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_recursive_list_dir void
#define __SLL_API_ARGS_sll_api_path_recursive_list_dir sll_string_t* a,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_path_recursive_list_dir
 * \group path-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_recursive_list_dir_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_recursive_list_dir:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_relative void
#define __SLL_API_ARGS_sll_api_path_relative sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_path_relative
 * \group path-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_relative_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_relative:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_set_cwd __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_path_set_cwd sll_string_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_path_set_cwd
 * \group json-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_set_cwd_raw
 * \group raw-api
 * \subgroup raw-api-json-api
 * \desc Wrapper function for :sll_api_path_set_cwd:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_size __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_path_size sll_string_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_path_size
 * \group path-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_size_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_size:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_path_split void
#define __SLL_API_ARGS_sll_api_path_split sll_string_t* a,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_path_split
 * \group path-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_path_split_raw
 * \group raw-api
 * \subgroup raw-api-path-api
 * \desc Wrapper function for :sll_api_path_split:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_sort_sort void
#define __SLL_API_ARGS_sll_api_sort_sort sll_array_t* a,sll_integer_t b,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_sort_sort
 * \group sort-api
 * \desc Docs!
 * \arg sll_array_t* a -> 
 * \arg sll_integer_t b -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_sort_sort_raw
 * \group raw-api
 * \subgroup raw-api-sort-api
 * \desc Wrapper function for :sll_api_sort_sort:
 * \arg sll_object_t*const* al -> Arguments
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
#define __SLL_API_ARGS_sll_api_string_convert sll_object_t*const* a,sll_arg_count_t ac,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_convert
 * \group string-api
 * \desc Docs!
 * \arg sll_object_t*const* a -> 
 * \arg sll_arg_count_t ac
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_convert_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_convert:
 * \arg sll_object_t*const* al -> Arguments
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



#define __SLL_API_TYPE_sll_api_string_count __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_string_count sll_string_t* a,sll_object_t* b
/**
 * \flags check_output func optimizable
 * \name sll_api_string_count
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_object_t* b -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_count_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_count:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_count_left __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_string_count_left sll_string_t* a,sll_char_t b
/**
 * \flags check_output func optimizable
 * \name sll_api_string_count_left
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_char_t b -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_count_left_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_count_left:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_ends __SLL_CHECK_OUTPUT sll_bool_t
#define __SLL_API_ARGS_sll_api_string_ends sll_string_t* a,sll_object_t* b
/**
 * \flags check_output func optimizable
 * \name sll_api_string_ends
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_object_t* b -> 
 * \ret sll_bool_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_ends_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_ends:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_format void
#define __SLL_API_ARGS_sll_api_string_format sll_string_t* a,sll_object_t*const* b,sll_arg_count_t bc,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_format
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_object_t*const* b -> 
 * \arg sll_arg_count_t bc
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_format_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_format:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_flip_case void
#define __SLL_API_ARGS_sll_api_string_flip_case sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_flip_case
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_flip_case_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_flip_case:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_index __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_string_index sll_string_t* a,sll_object_t* b
/**
 * \flags check_output func optimizable
 * \name sll_api_string_index
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_object_t* b -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_index_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_index:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_index_list __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_string_index_list sll_string_t* a,sll_object_t* b
/**
 * \flags check_output func optimizable
 * \name sll_api_string_index_list
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_object_t* b -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_index_list_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_index_list:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_index_reverse __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_string_index_reverse sll_string_t* a,sll_object_t* b
/**
 * \flags check_output func optimizable
 * \name sll_api_string_index_reverse
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_object_t* b -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_index_reverse_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_index_reverse:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_index_reverse_list __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_string_index_reverse_list sll_string_t* a,sll_object_t* b
/**
 * \flags check_output func optimizable
 * \name sll_api_string_index_reverse_list
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_object_t* b -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_index_reverse_list_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_index_reverse_list:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_join void
#define __SLL_API_ARGS_sll_api_string_join sll_object_t* a,sll_array_t* b,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_join
 * \group string-api
 * \desc Docs!
 * \arg sll_object_t* a -> 
 * \arg sll_array_t* b -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_join_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_join:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_pad_left void
#define __SLL_API_ARGS_sll_api_string_pad_left sll_string_t* a,sll_integer_t b,sll_read_char_t c,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_pad_left
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_integer_t b -> 
 * \arg sll_read_char_t c -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_pad_left_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_pad_left:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_pad_right void
#define __SLL_API_ARGS_sll_api_string_pad_right sll_string_t* a,sll_integer_t b,sll_read_char_t c,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_pad_right
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_integer_t b -> 
 * \arg sll_read_char_t c -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_pad_right_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_pad_right:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_replace void
#define __SLL_API_ARGS_sll_api_string_replace sll_string_t* a,sll_object_t* b,sll_object_t* c,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_replace
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_object_t* b -> 
 * \arg sll_object_t* c -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_replace_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_replace:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_reverse void
#define __SLL_API_ARGS_sll_api_string_reverse sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_reverse
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_reverse_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_reverse:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_split void
#define __SLL_API_ARGS_sll_api_string_split sll_string_t* a,sll_object_t* b,sll_array_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_split
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_object_t* b -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_split_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_split:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_starts __SLL_CHECK_OUTPUT sll_bool_t
#define __SLL_API_ARGS_sll_api_string_starts sll_string_t* a,sll_object_t* b
/**
 * \flags check_output func optimizable
 * \name sll_api_string_starts
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \arg sll_object_t* b -> 
 * \ret sll_bool_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_starts_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_starts:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_to_lower_case void
#define __SLL_API_ARGS_sll_api_string_to_lower_case sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_to_lower_case
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_to_lower_case_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_to_lower_case:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_to_title_case void
#define __SLL_API_ARGS_sll_api_string_to_title_case sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_to_title_case
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_to_title_case_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_to_title_case:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_string_to_upper_case void
#define __SLL_API_ARGS_sll_api_string_to_upper_case sll_string_t* a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_string_to_upper_case
 * \group string-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_string_to_upper_case_raw
 * \group raw-api
 * \subgroup raw-api-string-api
 * \desc Wrapper function for :sll_api_string_to_upper_case:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_sys_arg_get void
#define __SLL_API_ARGS_sll_api_sys_arg_get sll_integer_t a,sll_string_t* out
/**
 * \flags func optimizable
 * \name sll_api_sys_arg_get
 * \group sys-api
 * \desc Docs!
 * \arg sll_integer_t a -> 
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_sys_arg_get_raw
 * \group raw-api
 * \subgroup raw-api-sys-api
 * \desc Wrapper function for :sll_api_sys_arg_get:
 * \arg sll_object_t*const* al -> Arguments
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
 * \arg sll_object_t*const* al -> Arguments
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
 * \arg sll_object_t*const* al -> Arguments
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
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_sys_get_version __SLL_CHECK_OUTPUT sll_object_t*
#define __SLL_API_ARGS_sll_api_sys_get_version sll_integer_t a
/**
 * \flags func optimizable
 * \name sll_api_sys_get_version
 * \group sys-api
 * \desc Docs!
 * \arg sll_integer_t a -> 
 * \ret sll_object_t*
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_sys_get_version_raw
 * \group raw-api
 * \subgroup raw-api-sys-api
 * \desc Wrapper function for :sll_api_sys_get_version:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_sys_load_library __SLL_CHECK_OUTPUT sll_bool_t
#define __SLL_API_ARGS_sll_api_sys_load_library sll_string_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_sys_load_library
 * \group sys-api
 * \desc Docs!
 * \arg sll_string_t* a -> 
 * \ret sll_bool_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_sys_load_library_raw
 * \group raw-api
 * \subgroup raw-api-sys-api
 * \desc Wrapper function for :sll_api_sys_load_library:
 * \arg sll_object_t*const* al -> Arguments
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
 * \arg sll_object_t*const* al -> Arguments
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
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_time_sleep __SLL_CHECK_OUTPUT sll_float_t
#define __SLL_API_ARGS_sll_api_time_sleep sll_object_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_time_sleep
 * \group time-api
 * \desc Docs!
 * \arg sll_object_t* a -> 
 * \ret sll_float_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_time_sleep_raw
 * \group raw-api
 * \subgroup raw-api-time-api
 * \desc Wrapper function for :sll_api_time_sleep:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#define __SLL_API_TYPE_sll_api_time_sleep_nanos __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_time_sleep_nanos sll_object_t* a
/**
 * \flags check_output func optimizable
 * \name sll_api_time_sleep_nanos
 * \group time-api
 * \desc Docs!
 * \arg sll_object_t* a -> 
 * \ret sll_integer_t
 */
/**
 * \flags check_output func optimizable
 * \name sll_api_time_sleep_nanos_raw
 * \group raw-api
 * \subgroup raw-api-time-api
 * \desc Wrapper function for :sll_api_time_sleep_nanos:
 * \arg sll_object_t*const* al -> Arguments
 * \arg sll_arg_count_t all -> Argument count
 * \ret sll_object_t* -> The return value of the function
 */



#endif

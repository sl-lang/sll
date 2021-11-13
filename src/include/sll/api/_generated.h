// WARNING: This is an auto-generated file. Any changes made to this file might be lost at any moment. Do Not Edit!
#ifndef __SLL_API__GENERATED__
#define __SLL_API__GENERATED__
#include <sll/common.h>
#include <sll/types.h>



#define __SLL_API_TYPE_sll_api_file_close __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_file_close const sll_handle_data_t* a

#define __SLL_API_TYPE_sll_api_file_open void
#define __SLL_API_ARGS_sll_api_file_open const sll_string_t* a,const sll_string_t* b,sll_handle_data_t* out

#define __SLL_API_TYPE_sll_api_file_write __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_file_write const sll_runtime_object_t* a,const sll_runtime_object_t*const* b,sll_arg_count_t bc

#define __SLL_API_TYPE_sll_api_hash_create __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_hash_create const sll_runtime_object_t* a

#define __SLL_API_TYPE_sll_api_json_parse __SLL_CHECK_OUTPUT sll_runtime_object_t*
#define __SLL_API_ARGS_sll_api_json_parse const sll_string_t* a

#define __SLL_API_TYPE_sll_api_json_stringify void
#define __SLL_API_ARGS_sll_api_json_stringify const sll_runtime_object_t* a,sll_string_t* out

#define __SLL_API_TYPE_sll_api_json_type void
#define __SLL_API_ARGS_sll_api_json_type const sll_integer_t a,sll_handle_data_t* out

#define __SLL_API_TYPE_sll_api_path_absolute void
#define __SLL_API_ARGS_sll_api_path_absolute const sll_string_t* a,sll_string_t* out

#define __SLL_API_TYPE_sll_api_path_exists __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_path_exists const sll_string_t* a

#define __SLL_API_TYPE_sll_api_path_get_cwd void
#define __SLL_API_ARGS_sll_api_path_get_cwd sll_string_t* out

#define __SLL_API_TYPE_sll_api_path_is_dir __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_path_is_dir const sll_string_t* a

#define __SLL_API_TYPE_sll_api_path_list_dir __SLL_CHECK_OUTPUT sll_runtime_object_t*
#define __SLL_API_ARGS_sll_api_path_list_dir const sll_string_t* a

#define __SLL_API_TYPE_sll_api_path_recursive_list_dir __SLL_CHECK_OUTPUT sll_runtime_object_t*
#define __SLL_API_ARGS_sll_api_path_recursive_list_dir const sll_string_t* a

#define __SLL_API_TYPE_sll_api_path_relative void
#define __SLL_API_ARGS_sll_api_path_relative const sll_string_t* a,sll_string_t* out

#define __SLL_API_TYPE_sll_api_path_set_cwd __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_path_set_cwd const sll_string_t* a

#define __SLL_API_TYPE_sll_api_path_size __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_path_size const sll_string_t* a

#define __SLL_API_TYPE_sll_api_sort_sort __SLL_CHECK_OUTPUT sll_runtime_object_t*
#define __SLL_API_ARGS_sll_api_sort_sort const sll_array_t* a,const sll_integer_t b

#define __SLL_API_TYPE_sll_api_string_convert void
#define __SLL_API_ARGS_sll_api_string_convert const sll_runtime_object_t*const* a,sll_arg_count_t ac,sll_string_t* out

#define __SLL_API_TYPE_sll_api_string_length __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_string_length const sll_runtime_object_t*const* a,sll_arg_count_t ac

#define __SLL_API_TYPE_sll_api_string_replace void
#define __SLL_API_ARGS_sll_api_string_replace const sll_string_t* a,const sll_runtime_object_t* b,const sll_runtime_object_t* c,sll_string_t* out

#define __SLL_API_TYPE_sll_api_string_to_lower_case void
#define __SLL_API_ARGS_sll_api_string_to_lower_case const sll_string_t* a,sll_string_t* out

#define __SLL_API_TYPE_sll_api_string_to_title_case void
#define __SLL_API_ARGS_sll_api_string_to_title_case const sll_string_t* a,sll_string_t* out

#define __SLL_API_TYPE_sll_api_string_to_upper_case void
#define __SLL_API_ARGS_sll_api_string_to_upper_case const sll_string_t* a,sll_string_t* out

#define __SLL_API_TYPE_sll_api_sys_arg_get __SLL_CHECK_OUTPUT sll_runtime_object_t*
#define __SLL_API_ARGS_sll_api_sys_arg_get const sll_integer_t a

#define __SLL_API_TYPE_sll_api_sys_arg_get_count __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_sys_arg_get_count void

#define __SLL_API_TYPE_sll_api_sys_get_platform __SLL_CHECK_OUTPUT sll_runtime_object_t*
#define __SLL_API_ARGS_sll_api_sys_get_platform void

#define __SLL_API_TYPE_sll_api_time_current __SLL_CHECK_OUTPUT sll_float_t
#define __SLL_API_ARGS_sll_api_time_current void

#define __SLL_API_TYPE_sll_api_time_current_nanos __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_time_current_nanos void

#define __SLL_API_TYPE_sll_api_time_sleep __SLL_CHECK_OUTPUT sll_float_t
#define __SLL_API_ARGS_sll_api_time_sleep const sll_runtime_object_t* a

#define __SLL_API_TYPE_sll_api_time_sleep_nanos __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_time_sleep_nanos const sll_runtime_object_t* a

#define __SLL_API_TYPE_sll_api_url_execute_request __SLL_CHECK_OUTPUT sll_runtime_object_t*
#define __SLL_API_ARGS_sll_api_url_execute_request const sll_string_t* a,const sll_string_t* b,const sll_string_t* c,const sll_map_t* d,const sll_string_t* e

#define __SLL_API_TYPE_sll_api_util_instruction_count __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_util_instruction_count void

#define __SLL_API_TYPE_sll_api_util_ref_count __SLL_CHECK_OUTPUT sll_integer_t
#define __SLL_API_ARGS_sll_api_util_ref_count const sll_runtime_object_t* a



#endif

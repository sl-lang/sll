#ifndef __SLL_TYPES_H__
#define __SLL_TYPES_H__ 1
#include <sll/_identifier.h>
#include <sll/_object_func.h>
#include <sll/_size_types.h>
#include <sll/common.h>
#include <stdarg.h>
/**
 * \flags group
 * \name Limits
 * \group limits
 * \desc Docs!
 */
/**
 * \flags group
 * \name Types
 * \group types
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_ARG_COUNT
 * \group limits
 * \desc Docs!
 * \type sll_arg_count_t
 */
#define SLL_MAX_ARG_COUNT __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_ARRAY_LENGTH
 * \group limits
 * \desc Docs!
 * \type sll_array_length_t
 */
#define SLL_MAX_ARRAY_LENGTH __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_ASSEMBLY_INSTRUCTION_TYPE
 * \group limits
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_MAX_ASSEMBLY_INSTRUCTION_TYPE __SLL_U8_MAX



/**
 * \flags macro var
 * \name SLL_MAX_BARRIER_INDEX
 * \group limits
 * \desc Docs!
 * \type sll_barrier_index_t
 */
#define SLL_MAX_BARRIER_INDEX __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_CHAR
 * \group limits
 * \desc Docs!
 * \type sll_char_t
 */
#define SLL_MAX_CHAR __SLL_U8_MAX



/**
 * \flags macro var
 * \name SLL_MAX_EXPORT_TABLE_LENGTH
 * \group limits
 * \desc Docs!
 * \type sll_export_table_length_t
 */
#define SLL_MAX_EXPORT_TABLE_LENGTH __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_FILE_OFFSET
 * \group limits
 * \desc Docs!
 * \type sll_file_offset_t
 */
#define SLL_MAX_FILE_OFFSET __SLL_U64_MAX



/**
 * \flags macro var
 * \name SLL_MAX_HANDLE_TYPE
 * \group limits
 * \desc Docs!
 * \type sll_handle_type_t
 */
#define SLL_MAX_HANDLE_TYPE __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_IDENTIFIER_INDEX
 * \group limits
 * \desc Docs!
 * \type sll_identifier_index_t
 */
#define SLL_MAX_IDENTIFIER_INDEX __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_IDENTIFIER_LIST_LENGTH
 * \group limits
 * \desc Docs!
 * \type sll_identifier_list_length_t
 */
#define SLL_MAX_IDENTIFIER_LIST_LENGTH __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_INSTRUCTION_INDEX
 * \group limits
 * \desc Docs!
 * \type sll_instruction_index_t
 */
#define SLL_MAX_INSTRUCTION_INDEX __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_JSON_ARRAY_LENGTH
 * \group limits
 * \desc Docs!
 * \type sll_json_array_length_t
 */
#define SLL_MAX_JSON_ARRAY_LENGTH __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_JSON_MAP_LENGTH
 * \group limits
 * \desc Docs!
 * \type sll_json_map_length_t
 */
#define SLL_MAX_JSON_MAP_LENGTH __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_LOCK_INDEX
 * \group limits
 * \desc Docs!
 * \type sll_lock_index_t
 */
#define SLL_MAX_LOCK_INDEX __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_MAP_LENGTH
 * \group limits
 * \desc Docs!
 * \type sll_map_length_t
 */
#define SLL_MAX_MAP_LENGTH __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_OBJECT_OFFSET
 * \group limits
 * \desc Docs!
 * \type sll_object_offset_t
 */
#define SLL_MAX_OBJECT_OFFSET __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_PAGE_SIZE
 * \group limits
 * \desc Docs!
 * \type sll_page_size_t
 */
#define SLL_MAX_PAGE_SIZE __SLL_U64_MAX



/**
 * \flags macro var
 * \name SLL_MAX_REF_COUNT
 * \group limits
 * \desc Docs!
 * \type sll_ref_count_t
 */
#define SLL_MAX_REF_COUNT __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_SCOPE
 * \group limits
 * \desc Docs!
 * \type sll_scope_t
 */
#define SLL_MAX_SCOPE __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_SEMAPHORE_INDEX
 * \group limits
 * \desc Docs!
 * \type sll_semaphore_index_t
 */
#define SLL_MAX_SEMAPHORE_INDEX __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_SIZE
 * \group limits
 * \desc Docs!
 * \type sll_size_t
 */
#define SLL_MAX_SIZE __SLL_U64_MAX



/**
 * \flags macro var
 * \name SLL_MAX_STACK_OFFSET
 * \group limits
 * \desc Docs!
 * \type sll_stack_offset_t
 */
#define SLL_MAX_STACK_OFFSET __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_STRING_INDEX
 * \group limits
 * \desc Docs!
 * \type sll_string_index_t
 */
#define SLL_MAX_STRING_INDEX __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_STRING_LENGTH
 * \group limits
 * \desc Docs!
 * \type sll_string_length_t
 */
#define SLL_MAX_STRING_LENGTH __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_THREAD_INDEX
 * \group limits
 * \desc Docs!
 * \type sll_thread_index_t
 */
#define SLL_MAX_THREAD_INDEX 0xfffffffe



/**
 * \flags macro var
 * \name SLL_MAX_TIME
 * \group limits
 * \desc Docs!
 * \type sll_time_t
 */
#define SLL_MAX_TIME __SLL_U64_MAX



/**
 * \flags macro var
 * \name SLL_MAX_VARIABLE_INDEX
 * \group limits
 * \desc Docs!
 * \type sll_variable_index_t
 */
#define SLL_MAX_VARIABLE_INDEX __SLL_U32_MAX



struct _SLL_OBJECT;
struct _SLL_JSON_OBJECT;
struct _SLL_JSON_MAP_KEYPAIR;



/**
 * \flags type var
 * \name sll_bool_t
 * \group types
 * \desc Docs!
 * \type __SLL_U1
 */
typedef __SLL_U1 sll_bool_t;



/**
 * \flags type var
 * \name sll_assembly_instruction_type_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_assembly_instruction_type_t;



/**
 * \flags type var
 * \name sll_char_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_char_t;



/**
 * \flags type var
 * \name sll_checksum_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_checksum_t;



/**
 * \flags type var
 * \name sll_compare_result_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_compare_result_t;



/**
 * \flags type var
 * \name sll_day_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_day_t;



/**
 * \flags type var
 * \name sll_flags_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_flags_t;



/**
 * \flags type var
 * \name sll_hour_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_hour_t;



/**
 * \flags type var
 * \name sll_json_object_type_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_json_object_type_t;



/**
 * \flags type var
 * \name sll_minute_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_minute_t;



/**
 * \flags type var
 * \name sll_month_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_month_t;



/**
 * \flags type var
 * \name sll_name_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_name_length_t;



/**
 * \flags type var
 * \name sll_node_type_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_node_type_t;



/**
 * \flags type var
 * \name sll_object_function_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_object_function_index_t;



/**
 * \flags type var
 * \name sll_sandbox_flag_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_sandbox_flag_t;



/**
 * \flags type var
 * \name sll_var_arg_type_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_var_arg_type_t;



/**
 * \flags type var
 * \name sll_call_stack_size_t
 * \group types
 * \desc Docs!
 * \type __SLL_U16
 */
typedef __SLL_U16 sll_call_stack_size_t;



/**
 * \flags type var
 * \name sll_cpu_t
 * \group types
 * \desc Docs!
 * \type __SLL_U16
 */
typedef __SLL_U16 sll_cpu_t;



/**
 * \flags type var
 * \name sll_file_flags_t
 * \group types
 * \desc Docs!
 * \type __SLL_U16
 */
typedef __SLL_U16 sll_file_flags_t;



/**
 * \flags type var
 * \name sll_read_char_t
 * \group types
 * \desc Docs!
 * \type __SLL_U16
 */
typedef __SLL_U16 sll_read_char_t;



/**
 * \flags type var
 * \name sll_year_t
 * \group types
 * \desc Docs!
 * \type __SLL_U16
 */
typedef __SLL_U16 sll_year_t;



/**
 * \flags type var
 * \name sll_relative_instruction_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_S32
 */
typedef __SLL_S32 sll_relative_instruction_index_t;



/**
 * \flags type var
 * \name sll_return_code_t
 * \group types
 * \desc Docs!
 * \type __SLL_S32
 */
typedef __SLL_S32 sll_return_code_t;



/**
 * \flags type var
 * \name sll_arg_count_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_arg_count_t;



/**
 * \flags type var
 * \name sll_array_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_array_length_t;



/**
 * \flags type var
 * \name sll_assembly_file_table_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_assembly_file_table_length_t;



/**
 * \flags type var
 * \name sll_barrier_counter_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_barrier_counter_t;



/**
 * \flags type var
 * \name sll_barrier_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_barrier_index_t;



/**
 * \flags type var
 * \name sll_debug_data_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_debug_data_length_t;



/**
 * \flags type var
 * \name sll_environment_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_environment_length_t;



/**
 * \flags type var
 * \name sll_export_table_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_export_table_length_t;



/**
 * \flags type var
 * \name sll_function_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_function_index_t;



/**
 * \flags type var
 * \name sll_identifier_list_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_identifier_list_length_t;



/**
 * \flags type var
 * \name sll_identifier_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_identifier_index_t;



/**
 * \flags type var
 * \name sll_import_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_import_index_t;



/**
 * \flags type var
 * \name sll_instruction_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_instruction_index_t;



/**
 * \flags type var
 * \name sll_json_array_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_json_array_length_t;



/**
 * \flags type var
 * \name sll_json_map_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_json_map_length_t;



/**
 * \flags type var
 * \name sll_lock_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_lock_index_t;



/**
 * \flags type var
 * \name sll_map_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_map_length_t;



/**
 * \flags type var
 * \name sll_node_offset_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_node_offset_t;



/**
 * \flags type var
 * \name sll_object_type_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_object_type_t;



/**
 * \flags type var
 * \name sll_object_type_table_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_object_type_table_length_t;



/**
 * \flags type var
 * \name sll_pid_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_pid_t;



/**
 * \flags type var
 * \name sll_scope_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_scope_t;



/**
 * \flags type var
 * \name sll_search_path_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_search_path_length_t;



/**
 * \flags type var
 * \name sll_semaphore_counter_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_semaphore_counter_t;



/**
 * \flags type var
 * \name sll_semaphore_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_semaphore_index_t;



/**
 * \flags type var
 * \name sll_source_file_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_source_file_index_t;



/**
 * \flags type var
 * \name sll_stack_offset_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_stack_offset_t;



/**
 * \flags type var
 * \name sll_string_checksum_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_string_checksum_t;



/**
 * \flags type var
 * \name sll_string_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_string_index_t;



/**
 * \flags type var
 * \name sll_string_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_string_length_t;



/**
 * \flags type var
 * \name sll_thread_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_thread_index_t;



/**
 * \flags type var
 * \name sll_tls_object_length_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_tls_object_length_t;



/**
 * \flags type var
 * \name sll_variable_index_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_variable_index_t;



/**
 * \flags type var
 * \name sll_version_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_version_t;



/**
 * \flags type var
 * \name sll_wide_char_t
 * \group types
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_wide_char_t;



/**
 * \flags type var
 * \name sll_integer_t
 * \group types
 * \desc Docs!
 * \type __SLL_S64
 */
typedef __SLL_S64 sll_integer_t;



/**
 * \flags type var
 * \name sll_compressed_integer_t
 * \group types
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_compressed_integer_t;



/**
 * \flags type var
 * \name sll_error_t
 * \group types
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_error_t;



/**
 * \flags type var
 * \name sll_file_offset_t
 * \group types
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_file_offset_t;



/**
 * \flags type var
 * \name sll_reference_count_t
 * \group types
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_reference_count_t;



/**
 * \flags type var
 * \name sll_sandbox_flags_t
 * \group types
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_sandbox_flags_t;



/**
 * \flags type var
 * \name sll_size_t
 * \group types
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_size_t;



/**
 * \flags type var
 * \name sll_time_t
 * \group types
 * \desc Docs!
 * \type __SLL_U64
 */
typedef __SLL_U64 sll_time_t;



/**
 * \flags type var
 * \name sll_float_t
 * \group types
 * \desc Docs!
 * \type __SLL_F64
 */
typedef __SLL_F64 sll_float_t;



/**
 * \flags type var
 * \name sll_second_t
 * \group types
 * \desc Docs!
 * \type __SLL_F64
 */
typedef __SLL_F64 sll_second_t;



/**
 * \flags type var
 * \name sll_arg_state_t
 * \group types
 * \desc Docs!
 * \type void*
 */
typedef void* sll_arg_state_t;



/**
 * \flags type var
 * \name sll_event_handle_t
 * \group types
 * \desc Docs!
 * \type void*
 */
typedef void* sll_event_handle_t;



/**
 * \flags type var
 * \name sll_file_descriptor_t
 * \group types
 * \desc Docs!
 * \type void*
 */
typedef void* sll_file_descriptor_t;



/**
 * \flags type var
 * \name sll_internal_function_pointer_t
 * \group types
 * \desc Docs!
 * \type void*
 */
typedef void* sll_internal_function_pointer_t;



/**
 * \flags type var
 * \name sll_internal_thread_index_t
 * \group types
 * \desc Docs!
 * \type void*
 */
typedef void* sll_internal_thread_index_t;



/**
 * \flags type var
 * \name sll_library_handle_t
 * \group types
 * \desc Docs!
 * \type void*
 */
typedef void* sll_library_handle_t;



/**
 * \flags type var
 * \name sll_lock_handle_t
 * \group types
 * \desc Docs!
 * \type void*
 */
typedef void* sll_lock_handle_t;



/**
 * \flags type var
 * \name sll_process_handle_t
 * \group types
 * \desc Docs!
 * \type void*
 */
typedef void* sll_process_handle_t;



/**
 * \flags type var
 * \name sll_weak_reference_t
 * \group types
 * \desc Docs!
 * \type void*
 */
typedef void* sll_weak_reference_t;



/**
 * \flags type var
 * \name sll_json_parser_state_t
 * \group types
 * \desc Docs!
 * \type const sll_char_t*
 */
typedef const sll_char_t* sll_json_parser_state_t;



/**
 * \flags type
 * \name sll_md5_data_t
 * \group types
 * \desc Docs!
 * \arg __SLL_U32 a
 * \arg __SLL_U32 b
 * \arg __SLL_U32 c
 * \arg __SLL_U32 d
 */
typedef struct _SLL_MD5_DATA{
	__SLL_U32 a;
	__SLL_U32 b;
	__SLL_U32 c;
	__SLL_U32 d;
} sll_md5_data_t;



/**
 * \flags type
 * \name sll_sha1_data_t
 * \group types
 * \desc Docs!
 * \arg __SLL_U32 a
 * \arg __SLL_U32 b
 * \arg __SLL_U32 c
 * \arg __SLL_U32 d
 * \arg __SLL_U32 e
 */
typedef struct _SLL_SHA1_DATA{
	__SLL_U32 a;
	__SLL_U32 b;
	__SLL_U32 c;
	__SLL_U32 d;
	__SLL_U32 e;
} sll_sha1_data_t;



/**
 * \flags type
 * \name sll_sha256_data_t
 * \group types
 * \desc Docs!
 * \arg __SLL_U32 a
 * \arg __SLL_U32 b
 * \arg __SLL_U32 c
 * \arg __SLL_U32 d
 * \arg __SLL_U32 e
 * \arg __SLL_U32 f
 * \arg __SLL_U32 g
 * \arg __SLL_U32 h
 */
typedef struct _SLL_SHA256_DATA{
	__SLL_U32 a;
	__SLL_U32 b;
	__SLL_U32 c;
	__SLL_U32 d;
	__SLL_U32 e;
	__SLL_U32 f;
	__SLL_U32 g;
	__SLL_U32 h;
} sll_sha256_data_t;



/**
 * \flags type
 * \name sll_sha512_data_t
 * \group types
 * \desc Docs!
 * \arg __SLL_U64 a
 * \arg __SLL_U64 b
 * \arg __SLL_U64 c
 * \arg __SLL_U64 d
 * \arg __SLL_U64 e
 * \arg __SLL_U64 f
 * \arg __SLL_U64 g
 * \arg __SLL_U64 h
 */
typedef struct _SLL_SHA512_DATA{
	__SLL_U64 a;
	__SLL_U64 b;
	__SLL_U64 c;
	__SLL_U64 d;
	__SLL_U64 e;
	__SLL_U64 f;
	__SLL_U64 g;
	__SLL_U64 h;
} sll_sha512_data_t;



/**
 * \flags type
 * \name sll_string_t
 * \group types
 * \desc Docs!
 * \arg sll_string_length_t l
 * \arg sll_string_checksum_t c
 * \arg sll_char_t* v
 */
typedef struct _SLL_STRING{
	sll_string_length_t l;
	sll_string_checksum_t c;
	sll_char_t* v;
} sll_string_t;



/**
 * \flags type
 * \name sll_file_data_file_t
 * \group types
 * \desc Docs!
 * \arg const sll_file_descriptor_t fd
 * \arg const sll_string_t nm
 */
typedef struct _SLL_FILE_DATA_FILE{
	const sll_file_descriptor_t fd;
	const sll_string_t nm;
} sll_file_data_file_t;



/**
 * \flags type
 * \name sll_file_data_memory_t
 * \group types
 * \desc Docs!
 * \arg const void* p
 * \arg const sll_size_t sz
 */
typedef struct _SLL_FILE_DATA_MEMORY{
	const void* p;
	const sll_size_t sz;
} sll_file_data_memory_t;



/**
 * \flags type union
 * \name sll_file_data_t
 * \group types
 * \desc Docs!
 * \arg const sll_file_data_file_t fl
 * \arg const sll_file_data_memory_t mm
 */
typedef union _SLL_FILE_DATA{
	const sll_file_data_file_t fl;
	const sll_file_data_memory_t mm;
} sll_file_data_t;



/**
 * \flags type
 * \name sll_file_write_data_buffered_t
 * \group types
 * \desc Docs!
 * \arg sll_char_t* p
 * \arg sll_file_offset_t off
 */
typedef struct _SLL_FILE_WRITE_DATA_BUFFERED{
	sll_char_t* p;
	sll_file_offset_t off;
} sll_file_write_data_buffered_t;



/**
 * \flags type
 * \name sll_file_write_data_dynamic_t
 * \group types
 * \desc Docs!
 * \arg void* b
 * \arg void* t
 * \arg sll_file_offset_t sz
 * \arg sll_file_offset_t off
 */
typedef struct _SLL_FILE_WRITE_DATA_DYNAMIC{
	void* b;
	void* t;
	sll_file_offset_t sz;
	sll_file_offset_t off;
} sll_file_write_data_dynamic_t;



/**
 * \flags type union
 * \name sll_file_write_data_t
 * \group types
 * \desc Docs!
 * \arg sll_file_write_data_buffered_t bf
 * \arg sll_file_write_data_dynamic_t d
 */
typedef union _SLL_FILE_WRITE_DATA{
	sll_file_write_data_buffered_t bf;
	sll_file_write_data_dynamic_t d;
} sll_file_write_data_t;



/**
 * \flags type
 * \name sll_file_hash_t
 * \group types
 * \desc Docs!
 * \arg sll_sha256_data_t h
 * \arg sll_char_t* bf
 * \arg __SLL_U8 bfl
 */
typedef struct _SLL_FILE_HASH{
	sll_sha256_data_t h;
	sll_char_t bf[64];
	__SLL_U8 bfl;
} sll_file_hash_t;



/**
 * \flags type
 * \name sll_file_t
 * \group types
 * \desc Docs!
 * \arg const sll_file_data_t dt
 * \arg const sll_file_flags_t f
 * \arg sll_file_offset_t _l_num
 * \arg sll_file_offset_t _off
 * \arg sll_char_t* _r_bf
 * \arg sll_file_offset_t _r_bf_off
 * \arg sll_file_offset_t _r_bf_sz
 * \arg sll_file_write_data_t _w
 * \arg sll_file_hash_t _h
 * \arg sll_lock_handle_t _lck
 */
typedef struct _SLL_FILE{
	const sll_file_data_t dt;
	const sll_file_flags_t f;
	sll_file_offset_t _l_num;
	sll_file_offset_t _off;
	sll_char_t* _r_bf;
	sll_file_offset_t _r_bf_off;
	sll_file_offset_t _r_bf_sz;
	sll_file_write_data_t _w;
	sll_file_hash_t _h;
	sll_lock_handle_t _lck;
} sll_file_t;



/**
 * \flags type
 * \name sll_complex_t
 * \group types
 * \desc Docs!
 * \arg sll_float_t real
 * \arg sll_float_t imag
 */
typedef struct _SLL_COMPLEX{
	sll_float_t real;
	sll_float_t imag;
} sll_complex_t;



/**
 * \flags type
 * \name sll_function_node_data_t
 * \group types
 * \desc Docs!
 * \arg sll_arg_count_t ac
 * \arg sll_function_index_t id
 * \arg sll_scope_t sc
 */
typedef struct _SLL_FUNCTION_NODE_DATA{
	sll_arg_count_t ac;
	sll_function_index_t id;
	sll_scope_t sc;
} sll_function_node_data_t;



/**
 * \flags type
 * \name sll_loop_node_data_t
 * \group types
 * \desc Docs!
 * \arg sll_arg_count_t ac
 * \arg sll_scope_t sc
 */
typedef struct _SLL_LOOP_NODE_DATA{
	sll_arg_count_t ac;
	sll_scope_t sc;
} sll_loop_node_data_t;



/**
 * \flags type
 * \name sll_decl_node_data_t
 * \group types
 * \desc Docs!
 * \arg sll_arg_count_t ac
 * \arg sll_string_index_t nm
 */
typedef struct _SLL_DECL_NODE_DATA{
	sll_arg_count_t ac;
	sll_string_index_t nm;
} sll_decl_node_data_t;



/**
 * \flags type union
 * \name sll_node_data_t
 * \group types
 * \desc Docs!
 * \arg sll_integer_t i
 * \arg sll_float_t f
 * \arg sll_char_t c
 * \arg sll_complex_t d
 * \arg sll_string_index_t s
 * \arg sll_array_length_t al
 * \arg sll_map_length_t ml
 * \arg sll_identifier_index_t id
 * \arg sll_function_node_data_t fn
 * \arg sll_loop_node_data_t l
 * \arg sll_decl_node_data_t dc
 * \arg sll_arg_count_t ac
 * \arg sll_function_index_t fn_id
 * \arg sll_object_type_t ot
 * \arg void* _p
 */
typedef union _SLL_NODE_DATA{
	sll_integer_t i;
	sll_float_t f;
	sll_char_t c;
	sll_complex_t d;
	sll_string_index_t s;
	sll_array_length_t al;
	sll_map_length_t ml;
	sll_identifier_index_t id;
	sll_function_node_data_t fn;
	sll_loop_node_data_t l;
	sll_decl_node_data_t dc;
	sll_arg_count_t ac;
	sll_function_index_t fn_id;
	sll_object_type_t ot;
	void* _p;
} sll_node_data_t;



/**
 * \flags type
 * \name sll_node_t
 * \group types
 * \desc Docs!
 * \arg sll_node_type_t t
 * \arg sll_node_data_t dt
 */
typedef struct _SLL_NODE{
	sll_node_type_t t;
	sll_node_data_t dt;
} sll_node_t;



/**
 * \flags type
 * \name sll_identifier_t
 * \group types
 * \desc Docs!
 * \arg sll_scope_t sc
 * \arg sll_string_index_t i
 */
typedef struct _SLL_IDENTIFIER{
	sll_scope_t sc;
	sll_string_index_t i;
} sll_identifier_t;



/**
 * \flags type
 * \name sll_identifier_list_t
 * \group types
 * \desc Docs!
 * \arg sll_identifier_t* dt
 * \arg sll_identifier_list_length_t l
 */
typedef struct _SLL_IDENTIFIER_LIST{
	sll_identifier_t* dt;
	sll_identifier_list_length_t l;
} sll_identifier_list_t;



/**
 * \flags type
 * \name sll_identifier_table_t
 * \group types
 * \desc Docs!
 * \arg sll_identifier_list_t* s
 * \arg sll_identifier_t* il
 * \arg sll_identifier_list_length_t ill
 */
typedef struct _SLL_IDENTIFIER_TABLE{
	sll_identifier_list_t s[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
	sll_identifier_t* il;
	sll_identifier_list_length_t ill;
} sll_identifier_table_t;



/**
 * \flags type
 * \name sll_export_table_t
 * \group types
 * \desc Docs!
 * \arg sll_identifier_index_t* dt
 * \arg sll_export_table_length_t l
 */
typedef struct _SLL_EXPORT_TABLE{
	sll_identifier_index_t* dt;
	sll_export_table_length_t l;
} sll_export_table_t;



/**
 * \flags type
 * \name sll_function_t
 * \group types
 * \desc Docs!
 * \arg sll_node_offset_t off
 * \arg sll_string_index_t nm
 * \arg sll_string_index_t desc
 * \arg sll_arg_count_t al
 * \arg sll_identifier_index_t* a
 */
typedef struct _SLL_FUNCTION{
	sll_node_offset_t off;
	sll_string_index_t nm;
	sll_string_index_t desc;
	sll_arg_count_t al;
	sll_identifier_index_t a[];
} sll_function_t;



/**
 * \flags type
 * \name sll_function_table_t
 * \group types
 * \desc Docs!
 * \arg sll_function_t** dt
 * \arg sll_function_index_t l
 */
typedef struct _SLL_FUNCTION_TABLE{
	sll_function_t** dt;
	sll_function_index_t l;
} sll_function_table_t;



/**
 * \flags type
 * \name sll_string_table_t
 * \group types
 * \desc Docs!
 * \arg sll_string_t* dt
 * \arg sll_string_index_t l
 */
typedef struct _SLL_STRING_TABLE{
	sll_string_t* dt;
	sll_string_index_t l;
} sll_string_table_t;



/**
 * \flags type
 * \name sll_object_type_field_t
 * \group types
 * \desc Docs!
 * \arg sll_object_type_t t
 * \arg sll_string_index_t f
 */
typedef struct _SLL_OBJECT_TYPE_FIELD{
	sll_object_type_t t;
	sll_string_index_t f;
} sll_object_type_field_t;



/**
 * \flags type
 * \name sll_import_file_t
 * \group types
 * \desc Docs!
 * \arg sll_source_file_index_t sfi
 * \arg sll_identifier_list_length_t l
 * \arg sll_identifier_index_t* dt
 */
typedef struct _SLL_IMPORT_FILE{
	sll_source_file_index_t sfi;
	sll_identifier_list_length_t l;
	sll_identifier_index_t dt[];
} sll_import_file_t;



/**
 * \flags type
 * \name sll_import_table_t
 * \group types
 * \desc Docs!
 * \arg sll_import_file_t** dt
 * \arg sll_import_index_t l
 */
typedef struct _SLL_IMPORT_TABLE{
	sll_import_file_t** dt;
	sll_import_index_t l;
} sll_import_table_t;



/**
 * \flags type
 * \name sll_node_stack_t
 * \group types
 * \desc Docs!
 * \arg void* s
 * \arg void* e
 * \arg sll_node_t* p
 * \arg sll_node_offset_t off
 * \arg sll_size_t c
 */
typedef struct _SLL_NODE_STACK{
	void* s;
	void* e;
	sll_node_t* p;
	sll_node_offset_t off;
	sll_size_t c;
} sll_node_stack_t;



/**
 * \flags type
 * \name sll_source_file_t
 * \group types
 * \desc Docs!
 * \arg sll_time_t tm
 * \arg sll_file_offset_t sz
 * \arg sll_sha256_data_t h
 * \arg sll_node_t* dt
 * \arg sll_identifier_table_t idt
 * \arg sll_export_table_t et
 * \arg sll_function_table_t ft
 * \arg sll_string_table_t st
 * \arg sll_import_table_t it
 * \arg sll_string_index_t fp_nm
 * \arg sll_node_stack_t _s
 * \arg sll_scope_t _n_sc_id
 */
typedef struct _SLL_SOURCE_FILE{
	sll_time_t tm;
	sll_file_offset_t sz;
	sll_sha256_data_t h;
	sll_node_t* dt;
	sll_identifier_table_t idt;
	sll_export_table_t et;
	sll_function_table_t ft;
	sll_string_table_t st;
	sll_import_table_t it;
	sll_string_index_t fp_nm;
	sll_node_stack_t _s;
	sll_scope_t _n_sc_id;
} sll_source_file_t;



/**
 * \flags type
 * \name sll_compilation_data_t
 * \group types
 * \desc Docs!
 * \arg sll_source_file_t** dt
 * \arg sll_source_file_index_t l
 */
typedef struct _SLL_COMPILATION_DATA{
	sll_source_file_t** dt;
	sll_source_file_index_t l;
} sll_compilation_data_t;



/**
 * \flags type union
 * \name sll_assembly_instruction_data_jump_target_t
 * \group types
 * \desc Docs!
 * \arg sll_instruction_index_t abs
 * \arg sll_relative_instruction_index_t rel
 */
typedef union _SLL_ASSEMBLY_INSTRUCTION_DATA_JUMP_TARGET{
	sll_instruction_index_t abs;
	sll_relative_instruction_index_t rel;
} sll_assembly_instruction_data_jump_target_t;



/**
 * \flags type
 * \name sll_assembly_instruction_data_jump_t
 * \group types
 * \desc Docs!
 * \arg sll_assembly_instruction_data_jump_target_t t
 * \arg void* _p
 */
typedef struct _SLL_ASSEMBLY_INSTRUCTION_DATA_JUMP{
	sll_assembly_instruction_data_jump_target_t t;
	void* _p;
} sll_assembly_instruction_data_jump_t;



/**
 * \flags type
 * \name sll_assembly_instruction_data_var_access_t
 * \group types
 * \desc Docs!
 * \arg sll_variable_index_t v
 * \arg sll_arg_count_t l
 */
typedef struct _SLL_ASSEMBLY_INSTRUCTION_DATA_VAR_ACCESS{
	sll_variable_index_t v;
	sll_arg_count_t l;
} sll_assembly_instruction_data_var_access_t;



/**
 * \flags type union
 * \name sll_assembly_instruction_data_t
 * \group types
 * \desc Docs!
 * \arg sll_integer_t i
 * \arg sll_compressed_integer_t ci
 * \arg sll_float_t f
 * \arg sll_complex_t d
 * \arg sll_char_t c
 * \arg sll_string_index_t s
 * \arg sll_variable_index_t v
 * \arg sll_assembly_instruction_data_jump_t j
 * \arg sll_arg_count_t ac
 * \arg sll_array_length_t al
 * \arg sll_map_length_t ml
 * \arg sll_object_type_t t
 * \arg sll_assembly_instruction_data_var_access_t va
 * \arg sll_stack_offset_t so
 * \arg void* _p
 */
typedef union _SLL_ASSEMBLY_INSTRUCTION_DATA{
	sll_integer_t i;
	sll_compressed_integer_t ci;
	sll_float_t f;
	sll_complex_t d;
	sll_char_t c;
	sll_string_index_t s;
	sll_variable_index_t v;
	sll_assembly_instruction_data_jump_t j;
	sll_arg_count_t ac;
	sll_array_length_t al;
	sll_map_length_t ml;
	sll_object_type_t t;
	sll_assembly_instruction_data_var_access_t va;
	sll_stack_offset_t so;
	void* _p;
} sll_assembly_instruction_data_t;



/**
 * \flags type
 * \name sll_assembly_instruction_t
 * \group types
 * \desc Docs!
 * \arg sll_assembly_instruction_type_t t
 * \arg sll_assembly_instruction_data_t dt
 */
typedef struct _SLL_ASSEMBLY_INSTRUCTION{
	sll_assembly_instruction_type_t t;
	sll_assembly_instruction_data_t dt;
} sll_assembly_instruction_t;



/**
 * \flags type
 * \name sll_assembly_function_t
 * \group types
 * \desc Docs!
 * \arg sll_instruction_index_t i
 * \arg sll_arg_count_t ac
 * \arg sll_string_index_t nm
 */
typedef struct _SLL_ASSEMBLY_FUNCTION{
	sll_instruction_index_t i;
	sll_arg_count_t ac;
	sll_string_index_t nm;
} sll_assembly_function_t;



/**
 * \flags type
 * \name sll_assembly_function_table_t
 * \group types
 * \desc Docs!
 * \arg sll_function_index_t l
 * \arg sll_assembly_function_t* dt
 */
typedef struct _SLL_ASSEMBLY_FUNCTION_TABLE{
	sll_function_index_t l;
	sll_assembly_function_t* dt;
} sll_assembly_function_table_t;



/**
 * \flags type
 * \name sll_debug_line_data_t
 * \group types
 * \desc Docs!
 * \arg sll_instruction_index_t ii
 * \arg sll_file_offset_t ln
 */
typedef struct _SLL_DEBUG_LINE_DATA{
	sll_instruction_index_t ii;
	sll_file_offset_t ln;
} sll_debug_line_data_t;



/**
 * \flags type
 * \name sll_debug_data_t
 * \group types
 * \desc Docs!
 * \arg sll_debug_line_data_t* dt
 * \arg sll_debug_data_length_t l
 */
typedef struct _SLL_DEBUG_DATA{
	sll_debug_line_data_t* dt;
	sll_debug_data_length_t l;
} sll_debug_data_t;



/**
 * \flags type
 * \name sll_assembly_stack_data_t
 * \group types
 * \desc Docs!
 * \arg void* s
 * \arg void* e
 * \arg sll_assembly_instruction_t* p
 * \arg sll_size_t c
 */
typedef struct _SLL_ASSEMBLY_STACK_DATA{
	void* s;
	void* e;
	sll_assembly_instruction_t* p;
	sll_size_t c;
} sll_assembly_stack_data_t;



/**
 * \flags type
 * \name sll_assembly_data_t
 * \group types
 * \desc Docs!
 * \arg sll_time_t tm
 * \arg sll_assembly_instruction_t* h
 * \arg sll_instruction_index_t ic
 * \arg sll_variable_index_t vc
 * \arg sll_variable_index_t tls_vc
 * \arg sll_assembly_function_table_t ft
 * \arg sll_string_table_t st
 * \arg sll_debug_data_t dbg
 * \arg sll_assembly_stack_data_t _s
 */
typedef struct _SLL_ASSEMBLY_DATA{
	sll_time_t tm;
	sll_assembly_instruction_t* h;
	sll_instruction_index_t ic;
	sll_variable_index_t vc;
	sll_variable_index_t tls_vc;
	sll_assembly_function_table_t ft;
	sll_string_table_t st;
	sll_debug_data_t dbg;
	sll_assembly_stack_data_t _s;
} sll_assembly_data_t;



/**
 * \flags type
 * \name sll_array_t
 * \group types
 * \desc Docs!
 * \arg sll_array_length_t l
 * \arg sll_object_t** v
 */
typedef struct _SLL_ARRAY{
	sll_array_length_t l;
	struct _SLL_OBJECT** v;
} sll_array_t;



/**
 * \flags type
 * \name sll_map_t
 * \group types
 * \desc Docs!
 * \arg sll_map_length_t l
 * \arg sll_object_t** v
 */
typedef struct _SLL_MAP{
	sll_map_length_t l;
	struct _SLL_OBJECT** v;
} sll_map_t;



/**
 * \flags type union
 * \name sll_object_field_t
 * \group types
 * \desc Docs!
 * \arg sll_integer_t i
 * \arg sll_float_t f
 * \arg sll_char_t c
 * \arg sll_object_t* o
 */
typedef union _SLL_OBJECT_FIELD{
	sll_integer_t i;
	sll_float_t f;
	sll_char_t c;
	struct _SLL_OBJECT* o;
} sll_object_field_t;



/**
 * \flags type
 * \name sll_object_ptr_t
 * \group types
 * \desc Docs!
 * \arg sll_object_t* p
 * \arg sll_object_t* n
 */
typedef struct _SLL_OBJECT_PTR{
	struct _SLL_OBJECT* p;
	struct _SLL_OBJECT* n;
} sll_object_ptr_t;



/**
 * \flags type union
 * \name sll_object_data_t
 * \group types
 * \desc Docs!
 * \arg sll_integer_t i
 * \arg sll_float_t f
 * \arg sll_char_t c
 * \arg sll_complex_t d
 * \arg sll_string_t s
 * \arg sll_array_t a
 * \arg sll_map_t m
 * \arg sll_object_field_t* p
 * \arg sll_object_ptr_t _ptr
 */
typedef union _SLL_OBJECT_DATA{
	sll_integer_t i;
	sll_float_t f;
	sll_char_t c;
	sll_complex_t d;
	sll_string_t s;
	sll_array_t a;
	sll_map_t m;
	sll_object_field_t* p;
	sll_object_ptr_t _ptr;
} sll_object_data_t;



/**
 * \flags type
 * \name sll_object_t
 * \group types
 * \desc Docs!
 * \arg sll_reference_count_t rc
 * \arg const sll_object_type_t t
 * \arg __SLL_U32 _f
 * \arg sll_object_data_t dt
 */
typedef struct _SLL_OBJECT{
	sll_reference_count_t rc;
	const sll_object_type_t t;
	__SLL_U32 _f;
	sll_object_data_t dt;
} sll_object_t;



/**
 * \flags type
 * \name sll_internal_function_t
 * \group types
 * \desc Docs!
 * \arg sll_string_t nm
 * \arg sll_internal_function_pointer_t p
 * \arg sll_char_t* fmt
 * \arg __SLL_U16 _ret
 * \arg sll_arg_count_t _arg_cnt
 * \arg sll_size_t _arg_sz
 * \arg __SLL_U64* _regs
 */
typedef struct _SLL_INTERNAL_FUNCTION{
	sll_string_t nm;
	sll_internal_function_pointer_t p;
	sll_char_t* fmt;
	__SLL_U16 _ret;
	sll_arg_count_t _arg_cnt;
	sll_size_t _arg_sz;
	__SLL_U64* _regs;
} sll_internal_function_t;



/**
 * \flags type
 * \name sll_internal_function_table_t
 * \group types
 * \desc Docs!
 * \arg const sll_internal_function_t* dt
 * \arg sll_function_index_t l
 */
typedef struct _SLL_INTERNAL_FUNCTION_TABLE{
	const sll_internal_function_t* dt;
	sll_function_index_t l;
} sll_internal_function_table_t;



/**
 * \flags type
 * \name sll_object_type_data_entry_t
 * \group types
 * \desc Docs!
 * \arg sll_object_type_t t
 * \arg sll_bool_t c
 * \arg sll_string_t nm
 */
typedef struct _SLL_OBJECT_TYPE_DATA_ENTRY{
	sll_object_type_t t;
	sll_bool_t c;
	sll_string_t nm;
} sll_object_type_data_entry_t;



/**
 * \flags type
 * \name sll_object_type_data_t
 * \group types
 * \desc Docs!
 * \arg const sll_string_t nm
 * \arg sll_arg_count_t l
 * \arg __SLL_U32 _hash_table_len
 * \arg sll_arg_count_t* _hash_table
 * \arg __SLL_U64 _rng
 * \arg sll_integer_t* fn
 * \arg sll_object_type_data_entry_t* dt
 */
typedef struct _SLL_OBJECT_TYPE_DATA{
	const sll_string_t nm;
	sll_arg_count_t l;
	__SLL_U32 _hash_table_len;
	sll_arg_count_t* _hash_table;
	__SLL_U64 _rng;
	sll_integer_t fn[SLL_MAX_OBJECT_FUNC+1];
	sll_object_type_data_entry_t dt[];
} sll_object_type_data_t;



/**
 * \flags type
 * \name sll_object_type_table_t
 * \group types
 * \desc Docs!
 * \arg const sll_object_type_data_t** dt
 * \arg sll_object_type_table_length_t l
 */
typedef struct _SLL_OBJECT_TYPE_TABLE{
	const sll_object_type_data_t** dt;
	sll_object_type_table_length_t l;
} sll_object_type_table_t;



/**
 * \flags type
 * \name sll_call_stack_frame_t
 * \group types
 * \desc Docs!
 * \arg const sll_char_t* nm
 * \arg sll_instruction_index_t _ii
 * \arg sll_stack_offset_t _s
 * \arg void* _var_mem_off
 */
typedef struct _SLL_CALL_STACK_FRAME{
	const sll_char_t* nm;
	sll_instruction_index_t _ii;
	sll_stack_offset_t _s;
	void* _var_mem_off;
} sll_call_stack_frame_t;



/**
 * \flags type
 * \name sll_call_stack_t
 * \group types
 * \desc Docs!
 * \arg sll_call_stack_frame_t* dt
 * \arg sll_call_stack_size_t l
 */
typedef struct _SLL_CALL_STACK{
	sll_call_stack_frame_t* dt;
	sll_call_stack_size_t l;
} sll_call_stack_t;



/**
 * \flags type
 * \name sll_runtime_data_t
 * \group types
 * \desc Docs!
 * \arg const sll_assembly_data_t* a_dt
 * \arg sll_internal_function_table_t* ift
 * \arg sll_object_type_table_t* tt
 */
typedef struct _SLL_RUNTIME_DATA{
	const sll_assembly_data_t* a_dt;
	sll_internal_function_table_t* ift;
	sll_object_type_table_t* tt;
} sll_runtime_data_t;



/**
 * \flags type
 * \name sll_vm_config_t
 * \group types
 * \desc Docs!
 * \arg sll_size_t s_sz
 * \arg sll_size_t c_st_sz
 * \arg sll_internal_function_table_t* ift
 * \arg sll_file_t* in
 * \arg sll_file_t* out
 * \arg sll_file_t* err
 */
typedef struct _SLL_VM_CONFIG{
	sll_size_t s_sz;
	sll_size_t c_st_sz;
	sll_internal_function_table_t* ift;
	sll_file_t* in;
	sll_file_t* out;
	sll_file_t* err;
} sll_vm_config_t;



/**
 * \flags func type
 * \name sll_import_resolver_t
 * \group types
 * \desc Docs!
 * \arg const sll_string_t* path
 * \arg sll_compilation_data_t* out
 * \ret sll_bool_t
 */
typedef sll_bool_t (*sll_import_resolver_t)(const sll_string_t* path,sll_compilation_data_t* out);



typedef struct _SLL_JSON_ARRAY{
	sll_json_array_length_t l;
	struct _SLL_JSON_OBJECT* dt;
} sll_json_array_t;



typedef struct _SLL_JSON_MAP{
	sll_json_map_length_t l;
	struct _SLL_JSON_MAP_KEYPAIR* dt;
} sll_json_map_t;



typedef union _SLL_JSON_OBJECT_DATA{
	sll_integer_t i;
	sll_float_t f;
	sll_string_t s;
	sll_json_array_t a;
	sll_json_map_t m;
} sll_json_object_data_t;



typedef struct _SLL_JSON_OBJECT{
	sll_json_object_type_t t;
	sll_json_object_data_t dt;
} sll_json_object_t;



typedef struct _SLL_JSON_MAP_KEYPAIR{
	sll_string_t k;
	sll_json_object_t v;
} sll_json_map_keypair_t;



/**
 * \flags func type
 * \name sll_cleanup_function_t
 * \group types
 * \desc Docs!
 */
typedef void (*sll_cleanup_function_t)(void);



/**
 * \flags func type
 * \name sll_unary_operator_t
 * \group types
 * \desc Docs!
 * \arg sll_object_t* a
 * \ret sll_object_t*
 */
typedef sll_object_t* (*sll_unary_operator_t)(sll_object_t* a);



/**
 * \flags func type
 * \name sll_binary_operator_t
 * \group types
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \ret sll_object_t*
 */
typedef sll_object_t* (*sll_binary_operator_t)(sll_object_t* a,sll_object_t* b);



/**
 * \flags func type
 * \name sll_ternary_operator_t
 * \group types
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \arg sll_object_t* c
 * \ret sll_object_t*
 */
typedef sll_object_t* (*sll_ternary_operator_t)(sll_object_t* a,sll_object_t* b,sll_object_t* c);



/**
 * \flags func type
 * \name sll_quaternary_operator_t
 * \group types
 * \desc Docs!
 * \arg sll_object_t* a
 * \arg sll_object_t* b
 * \arg sll_object_t* c
 * \arg sll_object_t* d
 * \ret sll_object_t*
 */
typedef sll_object_t* (*sll_quaternary_operator_t)(sll_object_t* a,sll_object_t* b,sll_object_t* c,sll_object_t* d);



typedef struct _SLL_VAR_ARG_LIST_DATA_SLL{
	sll_object_t*const* p;
	sll_arg_count_t l;
} sll_var_arg_list_data_sll_t;



typedef struct _SLL_VAR_ARG_LIST_DATA_STRUCT{
	const void* ptr;
	sll_size_t* off;
	sll_arg_count_t l;
	void** fn;
	sll_arg_count_t fnl;
} sll_var_arg_list_data_struct_t;



typedef union _SLL_VAR_ARG_LIST_DATA{
	va_list* c;
	sll_var_arg_list_data_sll_t sll;
	sll_var_arg_list_data_struct_t s;
} sll_var_arg_list_data_t;



typedef struct _SLL_VAR_ARG_LIST{
	sll_var_arg_type_t t;
	sll_var_arg_list_data_t dt;
} sll_var_arg_list_t;



typedef struct _SLL_TIME_ZONE{
	sll_char_t nm[32];
	sll_time_t off;
} sll_time_zone_t;



typedef struct _SLL_DATE{
	sll_year_t y;
	sll_month_t m;
	sll_day_t d;
	sll_day_t wd;
	sll_hour_t h;
	sll_minute_t mn;
	sll_second_t s;
	sll_time_zone_t tz;
} sll_date_t;



typedef struct _SLL_FACTOR{
	sll_integer_t n;
	sll_size_t pw;
} sll_factor_t;



typedef struct _SLL_ENVIRONMENT_VARIABLE{
	const sll_string_t k;
	const sll_string_t v;
} sll_environment_variable_t;



typedef struct _SLL_ENVIRONMENT{
	const sll_environment_variable_t*const* dt;
	const sll_environment_length_t l;
} sll_environment_t;



typedef struct _SLL_MODULE_LOADER_DATA{
	sll_file_offset_t sz;
	sll_sha256_data_t h;
} sll_module_loader_data_t;



/**
 * \flags func type
 * \name sll_internal_thread_function_t
 * \group types
 * \desc Docs!
 * \arg void* arg
 */
typedef void (*sll_internal_thread_function_t)(void* arg);



typedef struct _SLL_BUNDLE_SOURCE_FILE{
	sll_string_t nm;
	sll_source_file_t dt;
} sll_bundle_source_file_t;



typedef struct _SLL_BUNDLE{
	sll_time_t tm;
	sll_string_t nm;
	sll_bundle_source_file_t** dt;
	sll_source_file_index_t l;
} sll_bundle_t;



typedef union _SLL_INT_FLOAT_DATA{
	sll_integer_t i;
	sll_float_t f;
} sll_int_float_data_t;



typedef struct _SLL_INT_FLOAT{
	sll_bool_t t;
	sll_int_float_data_t dt;
} sll_int_float_t;



typedef union _SLL_CHAR_STRING_DATA{
	sll_char_t c;
	const sll_string_t* s;
} sll_char_string_data_t;



typedef struct _SLL_CHAR_STRING{
	sll_bool_t t;
	sll_char_string_data_t dt;
} sll_char_string_t;



typedef struct _SLL_TLS_VALUE{
	sll_thread_index_t t;
	sll_object_t* v;
} sll_tls_value_t;



typedef struct _SLL_TLS_OBJECT{
	sll_tls_object_length_t sz;
	sll_tls_value_t* dt;
} sll_tls_object_t;



/**
 * \flags func type
 * \name sll_weak_ref_destructor_t
 * \group types
 * \desc Docs!
 * \arg sll_weak_reference_t wr
 * \arg sll_object_t* o
 * \arg void* arg
 */
typedef void (*sll_weak_ref_destructor_t)(sll_weak_reference_t wr,sll_object_t* o,void* arg);



/**
 * \flags func type
 * \name sll_audit_callback_t
 * \group types
 * \desc Docs!
 * \arg const sll_string_t* name
 * \arg const sll_array_t* args
 */
typedef void (*sll_audit_callback_t)(const sll_string_t* name,const sll_array_t* args);



typedef struct _SLL_SEARCH_PATH{
	sll_string_t* dt;
	sll_search_path_length_t l;
} sll_search_path_t;



typedef struct _SLL_INTERNAL_FUNCTION_DESCRIPTOR{
	const sll_char_t* nm;
	const sll_internal_function_pointer_t f;
	const sll_char_t* fmt;
} sll_internal_function_descriptor_t;



#endif

#ifndef __SLL_TYPES_H__
#define __SLL_TYPES_H__ 1
#include <sll/_identifier.h>
#include <sll/_object_func.h>
#include <sll/_size_types.h>
#include <sll/common.h>
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
 * \name SLL_MAX_EXPORT_TABLE_LENGTH
 * \group limits
 * \desc Docs!
 * \type sll_export_table_length_t
 */
#define SLL_MAX_EXPORT_TABLE_LENGTH __SLL_U32_MAX



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
 * \flags type var
 * \name sll_bool_t
 * \group types
 * \desc Docs!
 * \type __SLL_U1
 */
typedef __SLL_U1 sll_bool_t;



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
 * \name sll_flags_t
 * \group types
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_flags_t;



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



#endif

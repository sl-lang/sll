#ifndef __SLL_TYPES_H__
#define __SLL_TYPES_H__ 1
#include <sll/_size_types.h>
#include <sll/common.h>
#include <stdarg.h>



#define SLL_MAX_ARG_COUNT __SLL_U32_MAX
#define SLL_MAX_ARRAY_LENGTH __SLL_U32_MAX
#define SLL_MAX_ASSEMBLY_INSTRUCTION_TYPE __SLL_U8_MAX
#define SLL_MAX_CHAR __SLL_U8_MAX
#define SLL_MAX_EXPORT_TABLE_LENGTH __SLL_U32_MAX
#define SLL_MAX_FILE_OFFSET __SLL_U64_MAX
#define SLL_MAX_HANDLE_TYPE __SLL_U32_MAX
#define SLL_MAX_IDENTIFIER_INDEX __SLL_U32_MAX
#define SLL_MAX_IDENTIFIER_LIST_LENGTH __SLL_U32_MAX
#define SLL_MAX_INSTRUCTION_INDEX __SLL_U32_MAX
#define SLL_MAX_JSON_ARRAY_LENGTH __SLL_U32_MAX
#define SLL_MAX_JSON_MAP_LENGTH __SLL_U32_MAX
#define SLL_MAX_MAP_LENGTH __SLL_U32_MAX
#define SLL_MAX_OBJECT_OFFSET __SLL_U32_MAX
#define SLL_MAX_PAGE_SIZE __SLL_U64_MAX
#define SLL_MAX_REF_COUNT __SLL_U32_MAX
#define SLL_MAX_SCOPE __SLL_U32_MAX
#define SLL_MAX_SIZE __SLL_U64_MAX
#define SLL_MAX_STACK_OFFSET __SLL_U32_MAX
#define SLL_MAX_STRING_INDEX __SLL_U32_MAX
#define SLL_MAX_STRING_LENGTH __SLL_U32_MAX
#define SLL_MAX_TIME __SLL_U64_MAX
#define SLL_MAX_VARIABLE_INDEX __SLL_U32_MAX



struct __SLL_OBJECT;
struct __SLL_JSON_OBJECT;
struct __SLL_JSON_MAP_KEYPAIR;



typedef _Bool sll_bool_t;



typedef __SLL_U8 sll_assembly_instruction_type_t;



typedef __SLL_U8 sll_char_t;



typedef __SLL_U8 sll_checksum_t;



typedef __SLL_U8 sll_compare_result_t;



typedef __SLL_U8 sll_flags_t;



typedef __SLL_U8 sll_hour_t;



typedef __SLL_U8 sll_json_object_type_t;



typedef __SLL_U8 sll_minute_t;



typedef __SLL_U8 sll_month_t;



typedef __SLL_U8 sll_name_length_t;



typedef __SLL_U8 sll_node_type_t;



typedef __SLL_U8 sll_sandbox_flags_t;



typedef __SLL_U8 sll_day_t;



typedef __SLL_U16 sll_call_stack_size_t;



typedef __SLL_U16 sll_file_flags_t;



typedef __SLL_U16 sll_read_char_t;



typedef __SLL_U16 sll_year_t;



typedef __SLL_S32 sll_relative_instruction_index_t;



typedef __SLL_S32 sll_return_code_t;



typedef __SLL_U32 sll_arg_count_t;



typedef __SLL_U32 sll_array_length_t;



typedef __SLL_U32 sll_export_table_length_t;



typedef __SLL_U32 sll_function_index_t;



typedef __SLL_U32 sll_identifier_list_length_t;



typedef __SLL_U32 sll_identifier_index_t;



typedef __SLL_U32 sll_instruction_index_t;



typedef __SLL_U32 sll_json_array_length_t;



typedef __SLL_U32 sll_json_map_length_t;



typedef __SLL_U32 sll_map_length_t;



typedef __SLL_U32 sll_node_offset_t;



typedef __SLL_U32 sll_object_type_t;



typedef __SLL_U32 sll_pid_t;



typedef __SLL_U32 sll_ref_count_t;



typedef __SLL_U32 sll_scope_t;



typedef __SLL_U32 sll_stack_offset_t;



typedef __SLL_U32 sll_string_checksum_t;



typedef __SLL_U32 sll_string_index_t;



typedef __SLL_U32 sll_string_length_t;



typedef __SLL_U32 sll_variable_index_t;



typedef __SLL_U32 sll_version_t;



typedef __SLL_S64 sll_integer_t;



typedef __SLL_U64 sll_file_offset_t;



typedef __SLL_U64 sll_size_t;



typedef __SLL_U64 sll_size_t;



typedef __SLL_U64 sll_time_t;



typedef __SLL_F64 sll_float_t;



typedef sll_float_t sll_second_t;



typedef void* sll_file_descriptor_t;



typedef void* sll_library_handle_t;



typedef void* sll_process_handle_t;



typedef const sll_char_t* sll_json_parser_state_t;



typedef struct __SLL_STRING{
	sll_string_length_t l;
	sll_string_checksum_t c;
	sll_char_t* v;
} sll_string_t;



typedef struct __SLL_FILE_DATA_FILE{
	const sll_file_descriptor_t fd;
	const sll_string_t nm;
} sll_file_data_file_t;



typedef struct __SLL_FILE_DATA_MEMORY{
	const void* p;
	const sll_size_t sz;
} sll_file_data_memory_t;



typedef union __SLL_FILE_DATA{
	const sll_file_data_file_t fl;
	const sll_file_data_memory_t mm;
} sll_file_data_t;



typedef struct __SLL_FILE_WRITE_DATA_BUFFERED{
	sll_char_t* p;
	sll_file_offset_t off;
} sll_file_write_data_buffered_t;



typedef struct __SLL_FILE_WRITE_DATA_DYNAMIC{
	void* b;
	void* t;
	sll_file_offset_t sz;
	sll_file_offset_t off;
} sll_file_write_data_dynamic_t;



typedef union __SLL_FILE_WRITE_DATA{
	sll_file_write_data_buffered_t bf;
	sll_file_write_data_dynamic_t d;
} sll_file_write_data_t;



typedef struct __SLL_FILE{
	const sll_file_data_t dt;
	const sll_file_flags_t f;
	sll_file_offset_t _l_num;
	sll_file_offset_t _l_off;
	sll_file_offset_t _off;
	sll_char_t* _r_bf;
	sll_file_offset_t _r_bf_off;
	sll_file_offset_t _r_bf_sz;
	sll_file_write_data_t _w;
} sll_file_t;



typedef struct __SLL_FUNCTION_NODE_DATA{
	sll_arg_count_t ac;
	sll_function_index_t id;
	sll_scope_t sc;
} sll_function_node_data_t;



typedef struct __SLL_LOOP_NODE_DATA{
	sll_arg_count_t ac;
	sll_scope_t sc;
} sll_loop_node_data_t;



typedef struct __SLL_DECL_NODE_DATA{
	sll_arg_count_t ac;
	sll_string_index_t nm;
} sll_decl_node_data_t;



typedef struct __SLL_DECL_COPY_NODE_DATA{
	sll_object_type_t t;
	sll_string_index_t nm;
} sll_decl_copy_node_data_t;



typedef union __SLL_NODE_DATA{
	sll_char_t c;
	sll_integer_t i;
	sll_float_t f;
	sll_string_index_t s;
	sll_array_length_t al;
	sll_map_length_t ml;
	sll_identifier_index_t id;
	sll_function_node_data_t fn;
	sll_loop_node_data_t l;
	sll_decl_node_data_t d;
	sll_decl_copy_node_data_t dc;
	sll_arg_count_t ac;
	sll_function_index_t fn_id;
	sll_object_type_t ot;
	void* _p;
} sll_node_data_t;



typedef struct __SLL_NODE{
	sll_node_type_t t;
	sll_node_data_t dt;
} sll_node_t;



typedef struct __SLL_IDENTIFIER{
	sll_scope_t sc;
	sll_string_index_t i;
} sll_identifier_t;



typedef struct __SLL_IDENTIFIER_LIST{
	sll_identifier_t* dt;
	sll_identifier_list_length_t l;
} sll_identifier_list_t;



typedef struct __SLL_IDENTIFIER_TABLE{
	sll_identifier_list_t s[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
	sll_identifier_t* il;
	sll_identifier_list_length_t ill;
} sll_identifier_table_t;



typedef struct __SLL_EXPORT_TABLE{
	sll_identifier_index_t* dt;
	sll_export_table_length_t l;
} sll_export_table_t;



typedef struct __SLL_FUNCTION{
	sll_node_offset_t off;
	sll_string_index_t nm;
	sll_arg_count_t al;
	sll_identifier_index_t a[];
} sll_function_t;



typedef struct __SLL_FUNCTION_TABLE{
	sll_function_t** dt;
	sll_function_index_t l;
} sll_function_table_t;



typedef struct __SLL_STRING_TABLE{
	sll_string_t* dt;
	sll_string_index_t l;
} sll_string_table_t;



typedef struct __SLL_OBJECT_TYPE_FIELD{
	sll_object_type_t t;
	sll_string_index_t f;
} sll_object_type_field_t;



typedef struct __SLL_FILE_PATH_TABEL{
	sll_string_index_t* dt;
	sll_string_index_t l;
} sll_file_path_table_t;



typedef struct __SLL_COMPILATION_STACK_DATA{
	void* s;
	void* e;
	sll_node_t* p;
	sll_node_offset_t off;
	sll_size_t c;
} sll_compilation_stack_data_t;



typedef struct __SLL_COMPILATION_DATA{
	sll_time_t tm;
	sll_node_t* h;
	sll_identifier_table_t idt;
	sll_export_table_t et;
	sll_function_table_t ft;
	sll_string_table_t st;
	sll_file_path_table_t fpt;
	sll_compilation_stack_data_t _s;
	sll_scope_t _n_sc_id;
} sll_compilation_data_t;



typedef struct __SLL_ASSEMBLY_INSTRUCTION_DATA_VAR_ACCESS{
	sll_variable_index_t v;
	sll_arg_count_t l;
} sll_assembly_instruction_data_var_access_t;



typedef union __SLL_ASSEMBLY_INSTRUCTION_DATA{
	sll_char_t c;
	sll_integer_t i;
	sll_float_t f;
	sll_string_index_t s;
	sll_variable_index_t v;
	sll_instruction_index_t j;
	sll_relative_instruction_index_t rj;
	sll_arg_count_t ac;
	sll_array_length_t al;
	sll_map_length_t ml;
	sll_object_type_t t;
	sll_assembly_instruction_data_var_access_t va;
	void* _p;
} sll_assembly_instruction_data_t;



typedef struct __SLL_ASSEMBLY_INSTRUCTION{
	sll_assembly_instruction_type_t t;
	sll_assembly_instruction_data_t dt;
} sll_assembly_instruction_t;



typedef struct __SLL_ASSEMBLY_FUNCTION{
	sll_instruction_index_t i;
	sll_arg_count_t ac;
	sll_string_index_t nm;
} sll_assembly_function_t;



typedef struct __SLL_ASSEMBLY_FUNCTION_TABLE{
	sll_function_index_t l;
	sll_assembly_function_t* dt;
} sll_assembly_function_table_t;



typedef struct __SLL_DEBUG_LINE_DATA{
	sll_instruction_index_t ii;
	sll_file_offset_t ln;
} sll_debug_line_data_t;



typedef struct __SLL_DEBUG_DATA{
	sll_debug_line_data_t* dt;
	sll_instruction_index_t l;
} sll_debug_data_t;



typedef struct __SLL_ASSEMBLY_STACK_DATA{
	void* s;
	void* e;
	sll_assembly_instruction_t* p;
	sll_size_t c;
} sll_assembly_stack_data_t;



typedef struct __SLL_ASSEMBLY_DATA{
	sll_time_t tm;
	sll_assembly_instruction_t* h;
	sll_instruction_index_t ic;
	sll_variable_index_t vc;
	sll_assembly_function_table_t ft;
	sll_string_table_t st;
	sll_debug_data_t dbg;
	sll_assembly_stack_data_t _s;
} sll_assembly_data_t;



typedef struct __SLL_ARRAY{
	sll_array_length_t l;
	struct __SLL_OBJECT** v;
} sll_array_t;



typedef struct __SLL_MAP{
	sll_map_length_t l;
	struct __SLL_OBJECT** v;
} sll_map_t;



typedef struct __SLL_OBJECT_FIELD{
	sll_integer_t i;
	sll_float_t f;
	sll_char_t c;
	struct __SLL_OBJECT* o;
} sll_object_field_t;



typedef union __SLL_OBJECT_DATA{
	sll_integer_t i;
	sll_float_t f;
	sll_char_t c;
	sll_string_t s;
	sll_array_t a;
	sll_map_t m;
	sll_object_field_t* p;
} sll_object_data_t;



typedef struct __SLL_OBJECT{
	sll_ref_count_t rc;
	sll_object_type_t t;
	void* _dbg;
	sll_object_data_t dt;
} sll_object_t;



typedef sll_object_t* (*sll_internal_function_pointer_t)(sll_object_t*const* al,sll_arg_count_t all);



typedef struct __SLL_INTERNAL_FUNCTION{
	const sll_string_t nm;
	const sll_internal_function_pointer_t p;
} sll_internal_function_t;



typedef struct __SLL_INTERNAL_FUNCTION_TABLE{
	const sll_internal_function_t*const* dt;
	sll_function_index_t l;
} sll_internal_function_table_t;



typedef struct __SLL_OBJECT_TYPE_DATA_FUNCTIONS{
	sll_integer_t copy;
	sll_integer_t del;
	sll_integer_t init;
	sll_integer_t str;
} sll_object_type_data_functions_t;



typedef struct __SLL_OBJECT_TYPE_DATA_ENTRY{
	sll_object_type_t t;
	sll_string_t nm;
} sll_object_type_data_entry_t;



typedef struct __SLL_OBJECT_TYPE_DATA{
	const sll_string_t nm;
	sll_arg_count_t l;
	sll_object_type_data_functions_t fn;
	sll_object_type_data_entry_t dt[];
} sll_object_type_data_t;



typedef struct __SLL_OBJECT_TYPE_TABLE{
	const sll_object_type_data_t** dt;
	sll_object_type_t l;
} sll_object_type_table_t;



typedef struct __SLL_CALL_STACK_FRAME{
	const sll_char_t* nm;
	sll_instruction_index_t _ii;
	sll_stack_offset_t _s;
} sll_call_stack_frame_t;



typedef struct __SLL_CALL_STACK{
	sll_call_stack_frame_t* dt;
	sll_call_stack_size_t l;
} sll_call_stack_t;



typedef struct __SLL_RUNTIME_DATA{
	const sll_assembly_data_t* a_dt;
	sll_internal_function_table_t* ift;
	sll_object_type_table_t* tt;
	sll_call_stack_t c_st;
} sll_runtime_data_t;



typedef struct __SLL_VM_CONFIG{
	sll_size_t s_sz;
	sll_size_t c_st_sz;
	sll_internal_function_table_t* ift;
	sll_file_t* in;
	sll_file_t* out;
	sll_file_t* err;
} sll_vm_config_t;



typedef sll_bool_t (*sll_import_resolver_t)(const sll_string_t* s,sll_compilation_data_t* o);



typedef struct __SLL_JSON_ARRAY{
	sll_json_array_length_t l;
	struct __SLL_JSON_OBJECT* dt;
} sll_json_array_t;



typedef struct __SLL_JSON_MAP{
	sll_json_map_length_t l;
	struct __SLL_JSON_MAP_KEYPAIR* dt;
} sll_json_map_t;



typedef union __SLL_JSON_OBJECT_DATA{
	sll_integer_t i;
	sll_float_t f;
	sll_string_t s;
	sll_json_array_t a;
	sll_json_map_t m;
} sll_json_object_data_t;



typedef struct __SLL_JSON_OBJECT{
	sll_json_object_type_t t;
	sll_json_object_data_t dt;
} sll_json_object_t;



typedef struct __SLL_JSON_MAP_KEYPAIR{
	sll_string_t k;
	sll_json_object_t v;
} sll_json_map_keypair_t;



typedef void (*sll_cleanup_function_t)(void);



typedef sll_object_t* (*sll_unary_operator_t)(sll_object_t* a);



typedef sll_object_t* (*sll_binary_operator_t)(sll_object_t* a,sll_object_t* b);



typedef sll_object_t* (*sll_ternary_operator_t)(sll_object_t* a,sll_object_t* b,sll_object_t* c);



typedef sll_object_t* (*sll_quaternary_operator_t)(sll_object_t* a,sll_object_t* b,sll_object_t* c,sll_object_t* d);



typedef struct __SLL_VAR_ARG_LIST_DATA_SLL{
	const sll_object_t*const* p;
	sll_arg_count_t l;
} sll_var_arg_list_data_sll_t;



typedef union __SLL_VAR_ARG_LIST_DATA{
	va_list* c;
	sll_var_arg_list_data_sll_t sll;
} sll_var_arg_list_data_t;



typedef struct __SLL_VAR_ARG_LIST{
	sll_bool_t t;
	sll_var_arg_list_data_t dt;
} sll_var_arg_list_t;



typedef struct __SLL_DATE{
	sll_year_t y;
	sll_month_t m;
	sll_day_t d;
	sll_day_t wd;
	sll_hour_t h;
	sll_minute_t mn;
	sll_second_t s;
} sll_date_t;



typedef struct __SLL_FACTOR{
	sll_size_t n;
	sll_size_t pw;
} sll_factor_t;



typedef struct __SLL_ENVIRONMENT_VARIABLE{
	const sll_string_t k;
	const sll_string_t v;
} sll_environment_variable_t;



typedef struct __SLL_ENVIRONMENT{
	const sll_environment_variable_t*const* dt;
	const sll_array_length_t l;
} sll_environment_t;



#endif

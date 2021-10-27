#ifndef __SLL_TYPES_H__
#define __SLL_TYPES_H__ 1
#include <stdint.h>



#define SLL_MAX_IDENTIFIER_INDEX UINT32_MAX



typedef uint32_t sll_identifier_index_t;



#include <sll/identifier.h>



#define SLL_MAX_ASSEMBLY_INSTRUCTION_TYPE UINT8_MAX
#define SLL_MAX_BOOL UINT8_MAX
#define SLL_MAX_CHAR UINT8_MAX
#define SLL_MAX_CLEANUP_TYPE UINT8_MAX
#define SLL_MAX_COMPARE_RESULT UINT8_MAX
#define SLL_MAX_ERROR_TYPE UINT8_MAX
#define SLL_MAX_JSON_OBJECT_TYPE UINT8_MAX
#define SLL_MAX_INTERNAL_FUNCTION_TYPE UINT8_MAX
#define SLL_MAX_OBJECT_TYPE UINT8_MAX
#define SLL_MAX_RETURN UINT8_MAX
#define SLL_MAX_RUNTIME_OBJECT_TYPE UINT8_MAX
#define SLL_MAX_STRING_CHECKSUM UINT8_MAX
#define SLL_MAX_FILE_PATH_INDEX UINT16_MAX
#define SLL_MAX_READ_CHAR UINT16_MAX
#define SLL_MAX_ALLOCATED_BLOCK_LENGTH UINT32_MAX
#define SLL_MAX_ARG_COUNT UINT32_MAX
#define SLL_MAX_ARRAY_LENGTH UINT32_MAX
#define SLL_MAX_COLUMN_NUMBER UINT32_MAX
#define SLL_MAX_EXPORT_TABLE_LENGTH UINT32_MAX
#define SLL_MAX_FILE_OFFSET UINT32_MAX
#define SLL_MAX_FUNCTION_INDEX UINT32_MAX
#define SLL_MAX_HANDLE_TYPE UINT32_MAX
#define SLL_MAX_HEADER_COUNT UINT32_MAX
#define SLL_MAX_IDENTIFIER_LIST_LENGTH UINT32_MAX
#define SLL_MAX_INSTRUCTION_INDEX UINT32_MAX
#define SLL_MAX_JSON_ARRAY_LENGTH UINT32_MAX
#define SLL_MAX_JSON_MAP_LENGTH UINT32_MAX
#define SLL_MAX_LINE_NUMBER UINT32_MAX
#define SLL_MAX_MAP_LENGTH UINT32_MAX
#define SLL_MAX_OBJECT_OFFSET UINT32_MAX
#define SLL_MAX_REF_COUNT UINT32_MAX
#define SLL_MAX_SCOPE UINT32_MAX
#define SLL_MAX_STACK_OFFSET UINT32_MAX
#define SLL_MAX_STRING_INDEX UINT32_MAX
#define SLL_MAX_STRING_LENGTH UINT32_MAX
#define SLL_MAX_VARIABLE_INDEX UINT32_MAX
#define SLL_MAX_VERSION UINT32_MAX
#define SLL_MAX_PAGE_SIZE UINT64_MAX
#define SLL_MAX_TIME UINT64_MAX



struct __SLL_RUNTIME_OBJECT;
struct __SLL_JSON_OBJECT;
struct __SLL_JSON_MAP_KEYPAIR;



typedef uint8_t sll_assembly_instruction_type_t;



typedef _Bool sll_bool_t;



typedef uint8_t sll_char_t;



typedef uint8_t sll_checksum_t;



typedef uint8_t sll_cleanup_type_t;



typedef uint8_t sll_compare_result_t;



typedef uint8_t sll_constant_type_t;



typedef uint8_t sll_error_type_t;



typedef uint8_t sll_json_object_type_t;



typedef uint8_t sll_internal_function_type_t;



typedef uint8_t sll_name_length_t;



typedef uint8_t sll_object_type_t;



typedef uint8_t sll_return_t;



typedef uint8_t sll_runtime_object_type_t;



typedef uint16_t sll_file_path_index_t;



typedef uint16_t sll_read_char_t;



typedef int32_t sll_relative_instruction_index_t;



typedef int32_t sll_return_code_t;



typedef uint32_t sll_allocated_block_length_t;



typedef uint32_t sll_arg_count_t;



typedef uint32_t sll_array_length_t;



typedef uint32_t sll_column_number_t;



typedef uint32_t sll_export_table_length_t;



typedef uint32_t sll_file_offset_t;



typedef uint32_t sll_function_index_t;



typedef uint32_t sll_handle_type_t;



typedef uint32_t sll_header_count_t;



typedef uint32_t sll_identifier_list_length_t;



typedef uint32_t sll_instruction_index_t;



typedef uint32_t sll_json_array_length_t;



typedef uint32_t sll_json_map_length_t;



typedef uint32_t sll_line_number_t;



typedef uint32_t sll_map_length_t;



typedef uint32_t sll_object_offset_t;



typedef uint32_t sll_ref_count_t;



typedef uint32_t sll_scope_t;



typedef uint32_t sll_stack_offset_t;



typedef uint32_t sll_string_checksum_t;



typedef uint32_t sll_string_index_t;



typedef uint32_t sll_string_length_t;



typedef uint32_t sll_variable_index_t;



typedef uint32_t sll_version_t;



typedef int64_t sll_integer_t;



typedef uint64_t sll_handle_t;



typedef uint64_t sll_page_size_t;



typedef uint64_t sll_time_t;



typedef double sll_float_t;



typedef const sll_char_t* sll_json_parser_state_t;



typedef struct __SLL_INPUT_DATA_STREAM{
	void* ctx;
	sll_read_char_t (*rf)(struct __SLL_INPUT_DATA_STREAM* is);
	sll_read_char_t (*rbf)(struct __SLL_INPUT_DATA_STREAM* is,sll_char_t* bf,sll_string_length_t l);
	void (*rlf)(struct __SLL_INPUT_DATA_STREAM* is,sll_file_offset_t lp);
	sll_line_number_t _lc;
	sll_file_offset_t _off;
	sll_file_offset_t _loff;
} sll_input_data_stream_t;



typedef struct __SLL_OUTPUT_DATA_STREAM{
	void* ctx;
	void (*wcf)(struct __SLL_OUTPUT_DATA_STREAM* os,sll_char_t c);
	void (*wf)(struct __SLL_OUTPUT_DATA_STREAM* os,const sll_char_t* bf,sll_string_length_t l);
} sll_output_data_stream_t;



typedef struct __SLL_INPUT_BUFFER{
	const sll_char_t* bf;
	sll_string_length_t sz;
} sll_input_buffer_t;



typedef struct __SLL_FUNCTION_OBJECT_DATA{
	sll_arg_count_t ac;
	sll_function_index_t id;
	sll_scope_t sc;
} sll_function_object_data_t;



typedef struct __SLL_LOOP_DATA{
	sll_arg_count_t ac;
	sll_scope_t sc;
} sll_loop_data_t;



typedef struct __SLL_DEBUG_OBJECT_DATA{
	sll_string_index_t fpi;
	sll_line_number_t ln;
	sll_column_number_t cn;
	sll_file_offset_t ln_off;
} sll_debug_object_data_t;



typedef union __SLL_OBJECT_DATA{
	sll_char_t c;
	sll_integer_t i;
	sll_float_t f;
	sll_string_index_t s;
	sll_array_length_t al;
	sll_map_length_t ml;
	sll_identifier_index_t id;
	sll_function_object_data_t fn;
	sll_loop_data_t l;
	sll_arg_count_t ac;
	sll_debug_object_data_t dbg;
	sll_function_index_t fn_id;
	void* _p;
} sll_object_data_t;



typedef struct __SLL_OBJECT{
	sll_object_type_t t;
	sll_object_data_t dt;
} sll_object_t;



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
	sll_object_offset_t off;
	sll_arg_count_t al;
	sll_identifier_index_t a[];
} sll_function_t;



typedef struct __SLL_FUNCTION_TABLE{
	sll_function_t** dt;
	sll_function_index_t l;
} sll_function_table_t;



typedef struct __SLL_STRING{
	sll_string_length_t l;
	sll_string_checksum_t c;
	sll_char_t* v;
} sll_string_t;



typedef struct __SLL_STRING_TABLE{
	sll_string_index_t l;
	sll_string_t* dt;
} sll_string_table_t;



typedef struct __SLL_COMPILATION_STACK_DATA{
	void* s;
	void* e;
	unsigned int c;
	sll_object_t* p;
	sll_object_offset_t off;
} sll_compilation_stack_data_t;



typedef struct __SLL_COMPILATION_DATA{
	sll_input_data_stream_t* is;
	sll_time_t tm;
	sll_object_t* h;
	sll_identifier_table_t idt;
	sll_export_table_t et;
	sll_function_table_t ft;
	sll_string_table_t st;
	sll_compilation_stack_data_t _s;
	sll_scope_t _n_sc_id;
} sll_compilation_data_t;



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
	sll_stack_offset_t st;
	sll_constant_type_t t;
	void* _p;
} sll_assembly_instruction_data_t;



typedef struct __SLL_ASSEMBLY_INSTRUCTION{
	sll_assembly_instruction_type_t t;
	sll_assembly_instruction_data_t dt;
} sll_assembly_instruction_t;



typedef struct __SLL_ASSEMBLY_FUNCTION_TABLE{
	sll_function_index_t l;
	sll_instruction_index_t* dt;
} sll_assembly_function_table_t;



typedef struct __SLL_ASSEMBLY_STACK_DATA{
	void* s;
	void* e;
	unsigned int c;
	sll_assembly_instruction_t* p;
} sll_assembly_stack_data_t;



typedef struct __SLL_ASSEMBLY_DATA{
	sll_time_t tm;
	sll_assembly_instruction_t* h;
	sll_instruction_index_t ic;
	sll_variable_index_t vc;
	sll_assembly_function_table_t ft;
	sll_string_table_t st;
	sll_assembly_stack_data_t _s;
} sll_assembly_data_t;



typedef struct __SLL_ARRAY{
	sll_array_length_t l;
	struct __SLL_RUNTIME_OBJECT** v;
} sll_array_t;



typedef struct __SLL_MAP{
	sll_map_length_t l;
	struct __SLL_RUNTIME_OBJECT** v;
} sll_map_t;



typedef sll_string_length_t (*sll_handle_stringify_t)(sll_handle_t h,sll_string_length_t i,sll_string_t* o);



typedef void (*sll_handle_destructor_t)(sll_handle_t h);



typedef struct __SLL_HANDLE_DESCRIPTOR{
	sll_char_t nm[256];
	sll_name_length_t nml;
	sll_checksum_t c;
	sll_handle_stringify_t sf;
	sll_handle_destructor_t df;
} sll_handle_descriptor_t;



typedef struct __SLL_HANDLE_LIST{
	sll_handle_descriptor_t** dt;
	sll_handle_type_t dtl;
} sll_handle_list_t;



typedef struct __SLL_HANDLE_DATA{
	sll_handle_type_t t;
	sll_handle_t h;
} sll_handle_data_t;



typedef union __SLL_RUNTIME_OBJECT_DATA{
	sll_char_t c;
	sll_integer_t i;
	sll_float_t f;
	sll_string_t s;
	sll_array_t a;
	sll_handle_data_t h;
	sll_map_t m;
} sll_runtime_object_data_t;



typedef struct __SLL_RUNTIME_OBJECT{
	sll_ref_count_t rc;
	sll_runtime_object_type_t t;
	uint8_t _dbg0;
	uint16_t _dbg1;
	sll_runtime_object_data_t dt;
} sll_runtime_object_t;



typedef struct __SLL_ERROR_DATA_RANGE{
	sll_file_offset_t off;
	sll_file_offset_t sz;
} sll_error_data_range_t;



typedef union __SLL_ERROR_DATA{
	sll_char_t str[256];
	sll_error_data_range_t r;
	sll_assembly_instruction_type_t it;
} sll_error_data_t;



typedef struct __SLL_ERROR{
	sll_error_type_t t;
	sll_error_data_t dt;
} sll_error_t;



typedef sll_runtime_object_t* (*sll_internal_function_pointer_t)(const sll_runtime_object_t*const* al,sll_arg_count_t all);



typedef struct __SLL_INTERNAL_FUNCTION{
	sll_char_t nm[256];
	sll_name_length_t nml;
	sll_checksum_t c;
	sll_internal_function_pointer_t p;
	sll_internal_function_type_t t;
} sll_internal_function_t;



typedef struct __SLL_INTERNAL_FUNCTION_TABLE{
	sll_internal_function_t** dt;
	sll_function_index_t l;
} sll_internal_function_table_t;



typedef struct __SLL_RUNTIME_DATA{
	sll_internal_function_table_t* ift;
	sll_handle_list_t* hl;
	sll_input_data_stream_t* is;
	sll_output_data_stream_t* os;
} sll_runtime_data_t;



typedef sll_return_t (*sll_import_loader_t)(const sll_string_t* s,sll_compilation_data_t* o,sll_error_t* e);



typedef struct __SLL_HEADER{
	sll_string_t k;
	sll_string_t v;
} sll_header_t;



typedef struct __SLL_HEADER_LIST{
	sll_header_t** dt;
	sll_header_count_t l;
} sll_header_list_t;



typedef struct __SLL_HTTP_RESPONSE{
	sll_string_t* rc;
	sll_header_list_t* hl;
	sll_string_t* dt;
} sll_http_response_t;



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



typedef struct __SLL_STACK_DATA{
	sll_char_t* ptr;
	sll_stack_offset_t off;
	sll_stack_offset_t sz;
} sll_stack_data_t;



typedef sll_runtime_object_t* (*sll_unary_operator_t)(sll_runtime_object_t* a);



typedef sll_runtime_object_t* (*sll_binary_operator_t)(sll_runtime_object_t* a,sll_runtime_object_t* b);



typedef sll_runtime_object_t* (*sll_ternary_operator_t)(sll_runtime_object_t* a,sll_runtime_object_t* b,sll_runtime_object_t* c);



typedef sll_runtime_object_t* (*sll_quaternary_operator_t)(sll_runtime_object_t* a,sll_runtime_object_t* b,sll_runtime_object_t* c,sll_runtime_object_t* d);



typedef struct __SLL_binary_heap{
	sll_integer_t* v;
	sll_array_length_t l;
} sll_binary_heap_t;



#endif

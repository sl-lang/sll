#ifndef __LLL_TYPES_H__
#define __LLL_TYPES_H__ 1
#include <lll/constants.h>
#include <stdint.h>



typedef uint8_t lll_arg_count_t;



typedef uint8_t lll_assembly_instruction_type_t;



typedef uint8_t lll_bool_t;



typedef uint8_t lll_char_t;



typedef uint8_t lll_compare_result_t;



typedef uint8_t lll_error_type_t;



typedef uint8_t lll_object_type_t;



typedef uint8_t lll_return_t;



typedef uint8_t lll_runtime_object_type_t;



typedef uint16_t lll_file_path_index_t;



typedef uint16_t lll_heap_data_t;



typedef uint16_t lll_read_char_t;



typedef uint16_t lll_statement_count_t;



typedef uint16_t lll_version_t;



typedef int32_t lll_relative_instruction_index_t;



typedef int32_t lll_return_code_t;



typedef uint32_t lll_sys_arg_count_t;



typedef uint32_t lll_buffer_size_t;



typedef uint32_t lll_column_number_t;



typedef uint32_t lll_export_table_length_t;



typedef uint32_t lll_file_offset_t;



typedef uint32_t lll_function_index_t;



typedef uint32_t lll_identifier_index_t;



typedef uint32_t lll_identifier_list_length_t;



typedef uint32_t lll_instruction_index_t;



typedef uint32_t lll_line_number_t;



typedef uint32_t lll_object_offset_t;



typedef uint32_t lll_ref_count_t;



typedef uint32_t lll_scope_t;



typedef uint32_t lll_stack_offset_t;



typedef uint32_t lll_string_checksum_t;



typedef uint32_t lll_string_index_t;



typedef uint32_t lll_string_length_t;



typedef uint32_t lll_variable_index_t;



typedef int64_t lll_integer_t;



typedef uint64_t lll_page_size_t;



typedef uint64_t lll_time_t;



typedef double lll_float_t;



typedef uint8_t* lll_buffer_t;



typedef const uint8_t* lll_const_buffer_t;



typedef struct __LLL_INPUT_DATA_SOURCE{
	void* ctx;
	lll_read_char_t (*rf)(struct __LLL_INPUT_DATA_SOURCE* is);
	lll_read_char_t (*rbf)(struct __LLL_INPUT_DATA_SOURCE* is,lll_buffer_t bf,lll_buffer_size_t sz);
	void (*rlf)(struct __LLL_INPUT_DATA_SOURCE* is,lll_file_offset_t lp);
	lll_line_number_t _lc;
	lll_file_offset_t _off;
	lll_file_offset_t _loff;
} lll_input_data_stream_t;



typedef struct __LLL_OUTPUT_DATA_STREAM{
	void* ctx;
	void (*wcf)(struct __LLL_OUTPUT_DATA_STREAM* os,char c);
	void (*wsf)(struct __LLL_OUTPUT_DATA_STREAM* os,const char* s);
	void (*wf)(struct __LLL_OUTPUT_DATA_STREAM* os,lll_const_buffer_t bf,lll_buffer_size_t sz);
} lll_output_data_stream_t;



typedef struct __LLL_INPUT_BUFFER{
	lll_const_buffer_t bf;
	lll_buffer_size_t sz;
} lll_input_buffer_t;



typedef struct __LLL_FUNCTION_OBJECT_DATA{
	lll_arg_count_t ac;
	lll_function_index_t id;
} lll_function_object_data_t;



typedef struct __LLL_DEBUG_OBJECT_DATA{
	lll_string_index_t fpi;
	lll_line_number_t ln;
	lll_column_number_t cn;
	lll_file_offset_t ln_off;
} lll_debug_object_data_t;



typedef union __LLL_OBJECT_DATA{
	lll_char_t c;
	lll_integer_t i;
	lll_float_t f;
	lll_string_index_t s;
	lll_identifier_index_t id;
	lll_function_object_data_t fn;
	lll_arg_count_t ac;
	lll_statement_count_t sc;
	lll_debug_object_data_t dbg;
} lll_object_data_t;



typedef struct __LLL_OBJECT{
	lll_object_type_t t;
	lll_object_data_t dt;
} lll_object_t;



typedef struct __LLL_IDENTIFIER{
	lll_scope_t sc;
	lll_string_index_t i;
} lll_identifier_t;



typedef struct __LLL_IDENTIFIER_LIST{
	lll_identifier_t* dt;
	lll_identifier_list_length_t l;
} lll_identifier_list_t;



typedef struct __LLL_IDENTIFIER_DATA{
	lll_identifier_list_t s[LLL_MAX_SHORT_IDENTIFIER_LENGTH];
	lll_identifier_t* il;
	lll_identifier_list_length_t ill;
} lll_identifier_table_t;



typedef struct __LLL_EXPORT_TABLE{
	lll_identifier_index_t* dt;
	lll_export_table_length_t l;
} lll_export_table_t;



typedef struct __LLL_FUNCTION{
	lll_object_offset_t off;
	lll_arg_count_t al;
	lll_identifier_index_t a[];
} lll_function_t;



typedef struct __LLL_FUNCTION_DATA{
	lll_function_t** dt;
	lll_function_index_t l;
} lll_function_table_t;



typedef struct __LLL_STRING{
	const lll_string_length_t l;
	lll_ref_count_t rc;
	const lll_heap_data_t h;
	lll_string_checksum_t c;
	lll_char_t v[];
} lll_string_t;



typedef struct __LLL_STRING_TABLE{
	lll_string_index_t l;
	lll_string_t** dt;
} lll_string_table_t;



typedef struct __LLL_STACK_CONTEXT{
	lll_buffer_t ptr;
	lll_stack_offset_t off;
	lll_stack_offset_t sz;
} lll_stack_data_t;



typedef struct __LLL_COMPILATION_DATA{
	lll_input_data_stream_t* is;
	lll_time_t tm;
	lll_object_t* h;
	lll_identifier_table_t idt;
	lll_export_table_t et;
	lll_function_table_t ft;
	lll_string_table_t st;
	lll_stack_data_t _s;
	lll_scope_t _n_sc_id;
} lll_compilation_data_t;



typedef union __LLL_ASSEMBLY_INSTRUCTION_DATA{
	lll_char_t c;
	lll_integer_t i;
	lll_float_t f;
	lll_string_index_t s;
	lll_variable_index_t v;
	lll_instruction_index_t j;
	lll_relative_instruction_index_t rj;
	lll_arg_count_t ac;
} lll_assembly_instruction_data_t;



typedef struct __LLL_ASSEMBLY_INSTRUCTION{
	lll_assembly_instruction_type_t t;
	lll_assembly_instruction_data_t dt;
} lll_assembly_instruction_t;



typedef struct __LLL_ASSEMBLY_FUNCTION_TABLE{
	lll_function_index_t l;
	lll_instruction_index_t* dt;
} lll_assembly_function_table_t;



typedef struct __LLL_ASSEMBLY_DATA{
	lll_time_t tm;
	lll_assembly_instruction_t* h;
	lll_instruction_index_t ic;
	lll_variable_index_t vc;
	lll_assembly_function_table_t ft;
	lll_string_table_t st;
	lll_stack_data_t _s;
} lll_assembly_data_t;



typedef union __LLL_RUNTIME_OBJECT_DATA{
	lll_char_t c;
	lll_integer_t i;
	lll_float_t f;
	lll_string_t* s;
	lll_instruction_index_t ii;
} lll_runtime_object_data_t;



typedef struct __LLL_RUNTIME_OBJECT{
	lll_runtime_object_type_t t;
	lll_runtime_object_data_t dt;
} lll_runtime_object_t;



typedef struct __LLL_ERROR_DATA_RANGE{
	lll_file_offset_t off;
	lll_file_offset_t sz;
} lll_error_data_range_t;



typedef union __LLL_ERROR_DATA{
	char str[256];
	lll_error_data_range_t r;
	lll_assembly_instruction_type_t it;
} lll_error_data_t;



typedef struct __LLL_ERROR{
	lll_error_type_t t;
	lll_error_data_t dt;
} lll_error_t;



typedef lll_return_t (*lll_import_loader_t)(const lll_string_t* s,lll_compilation_data_t* o,lll_error_t* e);



typedef void (*lll_internal_function_pointer_t)(lll_runtime_object_t* o,lll_arg_count_t ac,lll_runtime_object_t* a);



typedef struct __LLL_INTERNAL_FUNCTION{
	char nm[256];
	uint8_t nml;
	lll_string_checksum_t c;
	lll_internal_function_pointer_t p;
} lll_internal_function_t;



typedef struct __LLL_INTERNAL_FUNCTION_TABLE{
	lll_internal_function_t** dt;
	lll_function_index_t l;
} lll_internal_function_table_t;



#endif

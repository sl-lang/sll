#ifndef __LLL_LIB_H__
#define __LLL_LIB_H__ 1
#include <stdint.h>
#include <stdio.h>
#include <version.h>



#ifdef _MSC_VER
#ifdef __LLL_LIB_COMPILATION__
#define __LLL_IMPORT_EXPORT __declspec(dllexport)
#else
#define __LLL_IMPORT_EXPORT __declspec(dllimport)
#endif
#define __LLL_CHECK_OUTPUT _Check_return_
#define __LLL_OBJECT_ALIGNMENT __declspec(align(8))
#else
#ifdef __LLL_LIB_COMPILATION__
#define __LLL_IMPORT_EXPORT __attribute__((visibility("default")))
#else
#define __LLL_IMPORT_EXPORT
#endif
#define __LLL_CHECK_OUTPUT __attribute__((warn_unused_result))
#define __LLL_OBJECT_ALIGNMENT __attribute__((aligned(8)))
#endif
#define __LLL_RETURN __LLL_CHECK_OUTPUT lll_return_t
#define __LLL_RETURN_CODE __LLL_CHECK_OUTPUT lll_return_code_t
#define __LLL_RETURN_FUNCTION_INDEX __LLL_CHECK_OUTPUT lll_function_index_t
#define __LLL_RETURN_SIZE __LLL_CHECK_OUTPUT lll_object_offset_t
#define __LLL_RETURN_STRING_INDEX __LLL_CHECK_OUTPUT lll_string_index_t

#define LLL_ERROR_UNKNOWN 0
#define LLL_ERROR_INTERNAL_STACK_OVERFLOW 1
#define LLL_ERROR_UNMATCHED_OPEN_PARENTHESES 2
#define LLL_ERROR_UNMATCHED_CLOSE_PARENTHESES 3
#define LLL_ERROR_UNMATCHED_OPEN_QUOTE 4
#define LLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS 5
#define LLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS 6
#define LLL_ERROR_UNMATCHED_QUOTES 7
#define LLL_ERROR_EMPTY_CHAR_STRING 8
#define LLL_ERROR_UNTERMINATED_CHAR_STRING 9
#define LLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE 10
#define LLL_ERROR_UNKNOWN_ESCAPE_CHARACTER 11
#define LLL_ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE 12
#define LLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER 13
#define LLL_ERROR_UNKNOWN_DECIMAL_CHARCTER 14
#define LLL_ERROR_UNKNOWN_OCTAL_CHARCTER 15
#define LLL_ERROR_UNKNOWN_BINARY_CHARCTER 16
#define LLL_ERROR_UNKNOWN_SYMBOL 17
#define LLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER 18
#define LLL_ERROR_UNEXPECTED_CHARACTER 19
#define LLL_ERROR_SYMBOL_TOO_LONG 20
#define LLL_ERROR_NO_SYMBOL 21
#define LLL_ERROR_TOO_MANY_ARGUMENTS 22
#define LLL_ERROR_TOO_MANY_STATEMENTS 23
#define LLL_ERROR_UNKNOWN_IDENTIFIER 24
#define LLL_ERROR_INTERNAL_FUNCTION_NAME_TOO_LONG 25
#define LLL_ERROR_STRING_REQUIRED 26
#define LLL_ERROR_UNKNOWN_INTERNAL_FUNCTION 27
#define LLL_ERROR_NO_STACK 28
#define LLL_ERROR_DIVISION_BY_ZERO 29
#define LLL_ERROR_INVALID_FILE_FORMAT 30
#define LLL_ERROR_INVALID_IMPORT_INDEX 31
#define LLL_ERROR_INVALID_INSTRUCTION 32
#define LLL_ERROR_STACK_CORRUPTED 33
#define LLL_ERROR_INVALID_INSTRUCTION_INDEX 34
#define LLL_ERROR_ASSERTION 255
#define LLL_MAX_COMPILATION_ERROR LLL_ERROR_STRING_REQUIRED

#define LLL_RETURN_ERROR 0
#define LLL_RETURN_NO_ERROR 1

#define LLL_OBJECT_TYPE_UNKNOWN 0
#define LLL_OBJECT_TYPE_CHAR 1
#define LLL_OBJECT_TYPE_INT 2
#define LLL_OBJECT_TYPE_FLOAT 3
#define LLL_OBJECT_TYPE_STRING 4
#define LLL_OBJECT_TYPE_IDENTIFIER 5
#define LLL_OBJECT_TYPE_PRINT 6
#define LLL_OBJECT_TYPE_INPUT 7
#define LLL_OBJECT_TYPE_AND 8
#define LLL_OBJECT_TYPE_OR 9
#define LLL_OBJECT_TYPE_NOT 10
#define LLL_OBJECT_TYPE_ASSIGN 11
#define LLL_OBJECT_TYPE_FUNC 12
#define LLL_OBJECT_TYPE_INTERNAL_FUNC 13
#define LLL_OBJECT_TYPE_CALL 14
#define LLL_OBJECT_TYPE_IF 15
#define LLL_OBJECT_TYPE_FOR 16
#define LLL_OBJECT_TYPE_WHILE 17
#define LLL_OBJECT_TYPE_LOOP 18
#define LLL_OBJECT_TYPE_ADD 19
#define LLL_OBJECT_TYPE_SUB 20
#define LLL_OBJECT_TYPE_MULT 21
#define LLL_OBJECT_TYPE_DIV 22
#define LLL_OBJECT_TYPE_FLOOR_DIV 23
#define LLL_OBJECT_TYPE_MOD 24
#define LLL_OBJECT_TYPE_BIT_AND 25
#define LLL_OBJECT_TYPE_BIT_OR 26
#define LLL_OBJECT_TYPE_BIT_XOR 27
#define LLL_OBJECT_TYPE_BIT_NOT 28
#define LLL_OBJECT_TYPE_LESS 29
#define LLL_OBJECT_TYPE_LESS_EQUAL 30
#define LLL_OBJECT_TYPE_EQUAL 31
#define LLL_OBJECT_TYPE_NOT_EQUAL 32
#define LLL_OBJECT_TYPE_MORE 33
#define LLL_OBJECT_TYPE_MORE_EQUAL 34
#define LLL_OBJECT_TYPE_RETURN 35
#define LLL_OBJECT_TYPE_EXIT 36
#define LLL_OBJECT_TYPE_IMPORT 37
#define LLL_OBJECT_TYPE_OPERATION_LIST 38
#define LLL_OBJECT_TYPE_DEBUG_DATA 39
#define LLL_OBJECT_TYPE_RESERVED0 254
#define LLL_OBJECT_TYPE_NOP 255
#define LLL_IS_OBJECT_TYPE_NOT_TYPE(o) ((o)->t>LLL_OBJECT_TYPE_IDENTIFIER)

#define LLL_MAX_SHORT_IDENTIFIER_LENGTH 15
#define LLL_IDENTIFIER_GET_ARRAY_ID(i) ((i)&0xf)
#define LLL_IDENTIFIER_GET_ARRAY_INDEX(i) ((i)>>4)
#define LLL_IDENTIFIER_ADD_INDEX(i,j) ((i)+((j)<<4))
#define LLL_CREATE_IDENTIFIER(i,j) (((i)<<4)|(j))

#define LLL_ASSEMBLY_INSTRUCTION_TYPE_NOP 0
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_POP 1
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO 2
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_ROT 3
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP 4
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_DUP 5
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT 6
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE 7
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO 8
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE 9
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO 10
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE 11
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR 12
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT 13
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR 14
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD 15
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS 16
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE 17
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP 18
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE 19
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO 20
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE 21
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO 22
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE 23
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR 24
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP 25
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_JB 26
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE 27
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_JA 28
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE 29
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_JE 30
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE 31
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_JZ 32
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ 33
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_NOT 34
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_INC 35
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_DEC 36
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_ADD 37
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_SUB 38
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_MULT 39
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_DIV 40
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV 41
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_MOD 42
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_AND 43
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_OR 44
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_XOR 45
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_INV 46
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT 47
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR 48
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR 49
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR 50
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_CALL 51
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_RET 52
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT 53
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO 54
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT 55
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR 56
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR 57
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR 58
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_END 59
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_END_ZERO 60
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_END_ONE 61
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED0 126
#define LLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED1 127
#define LLL_ASSEMBLY_INSTRUCTION_INPLACE 128
#define LLL_ASSEMBLY_INSTRUCTION_RELATIVE 128
#define LLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai) ((ai)->t&0x7f)
#define LLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai) ((ai)->t>>7)
#define LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai) ((ai)->t>>7)

#define LLL_RUNTIME_OBJECT_TYPE_INT 0
#define LLL_RUNTIME_OBJECT_TYPE_FLOAT 1
#define LLL_RUNTIME_OBJECT_TYPE_CHAR 2
#define LLL_RUNTIME_OBJECT_TYPE_STRING 3
#define LLL_RUNTIME_OBJECT_RESERVED0 128
#define LLL_RUNTIME_OBJECT_GET_TYPE(r) ((r)->t&0x7f)

#define LLL_END_OF_DATA (-1)
#define LLL_READ_FROM_INPUT_DATA_STREAM(is) ((is)->rf((is)))
#define LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,bf,sz) ((is)->rbf((is),(bf),(sz)))
#define LLL_INPUT_DATA_STREAM_RESTART_LINE(is,lp) ((is)->rlf((is),(lp)))
#define LLL_GET_INPUT_DATA_STREAM_OFFSET(is) ((is)->_off)
#define LLL_GET_INPUT_DATA_STREAM_LINE_NUMBER(is) ((is)->_lc)
#define LLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is) ((is)->_loff)

#define LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,c) ((os)->wcf((os),(c)))
#define LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,s) ((os)->wsf((os),(s)))
#define LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,bf,sz) ((os)->wf((os),(bf),(sz)))

#define LLL_MAX_FUNCTION_INDEX UINT32_MAX
#define LLL_MAX_IMPORT_INDEX UINT32_MAX
#define LLL_MAX_OBJECT_OFFSET UINT32_MAX
#define LLL_MAX_SCOPE UINT32_MAX
#define LLL_MAX_STRING_INDEX UINT32_MAX
#define LLL_MAX_VARIABLE_INDEX UINT32_MAX



typedef uint8_t lll_arg_count_t;



typedef uint8_t lll_assembly_instruction_type_t;



typedef uint8_t lll_char_t;



typedef uint8_t lll_error_type_t;



typedef uint8_t lll_object_type_t;



typedef uint8_t lll_return_t;



typedef uint8_t lll_runtime_object_type_t;



typedef uint8_t lll_string_checksum_t;



typedef uint16_t lll_file_path_index_t;



typedef uint16_t lll_statement_count_t;



typedef uint16_t lll_version_t;



typedef int32_t lll_relative_instruction_index_t;



typedef int32_t lll_return_code_t;



typedef uint32_t lll_column_number_t;



typedef uint32_t lll_export_table_length_t;



typedef uint32_t lll_file_offset_t;



typedef uint32_t lll_function_index_t;



typedef uint32_t lll_identifier_index_t;



typedef uint32_t lll_identifier_list_length_t;



typedef uint32_t lll_import_index_t;



typedef uint32_t lll_instruction_index_t;



typedef uint32_t lll_line_number_t;



typedef uint32_t lll_object_offset_t;



typedef uint32_t lll_scope_t;



typedef uint32_t lll_stack_offset_t;



typedef uint32_t lll_string_index_t;



typedef uint32_t lll_string_length_t;



typedef uint32_t lll_variable_index_t;



typedef int64_t lll_integer_t;



typedef uint64_t lll_time_t;



typedef double lll_float_t;



typedef struct __LLL_INPUT_DATA_SOURCE{
	void* ctx;
	int (*rf)(struct __LLL_INPUT_DATA_SOURCE* is);
	uint8_t (*rbf)(struct __LLL_INPUT_DATA_SOURCE* is,uint8_t* bf,uint32_t sz);
	void (*rlf)(struct __LLL_INPUT_DATA_SOURCE* is,lll_file_offset_t lp);
	lll_line_number_t _lc;
	lll_file_offset_t _off;
	lll_file_offset_t _loff;
} lll_input_data_stream_t;



typedef struct __LLL_OUTPUT_DATA_STREAM{
	void* ctx;
	void (*wcf)(struct __LLL_OUTPUT_DATA_STREAM* os,char c);
	void (*wsf)(struct __LLL_OUTPUT_DATA_STREAM* os,const char* s);
	void (*wf)(struct __LLL_OUTPUT_DATA_STREAM* os,const uint8_t* bf,size_t sz);
} lll_output_data_stream_t;



typedef struct __LLL_FUNCTION_OBJECT_DATA{
	lll_arg_count_t ac;
	lll_function_index_t id;
} lll_function_object_data_t;



typedef struct __LLL_IMPORT_OBJECT_DATA{
	lll_import_index_t ii;
	lll_scope_t sc;
} lll_import_object_data_t;



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
	lll_import_object_data_t im;
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



typedef struct __LLL_IMPORT_DATA{
	lll_string_index_t* dt;
	lll_import_index_t l;
} lll_import_table_t;



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
	lll_string_length_t l;
	lll_string_checksum_t c;
	lll_char_t v[];
} lll_string_t;



typedef struct __LLL_STRING_TABLE{
	lll_string_index_t l;
	lll_string_t** dt;
} lll_string_table_t;



typedef struct __LLL_STACK_CONTEXT{
	uint8_t* ptr;
	lll_stack_offset_t off;
	lll_stack_offset_t sz;
} lll_stack_data_t;



typedef struct __LLL_COMPILATION_DATA{
	lll_input_data_stream_t* is;
	lll_time_t tm;
	lll_object_t* h;
	lll_identifier_table_t idt;
	lll_import_table_t it;
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
	lll_import_index_t im_i;
	lll_assembly_instruction_type_t it;
} lll_error_data_t;



typedef struct __LLL_ERROR{
	lll_error_type_t t;
	lll_error_data_t dt;
} lll_error_t;



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



__LLL_IMPORT_EXPORT void lll_create_internal_function_table(lll_internal_function_table_t* o);



__LLL_IMPORT_EXPORT void lll_create_input_data_stream(FILE* f,lll_input_data_stream_t* o);



__LLL_IMPORT_EXPORT void lll_create_output_data_stream(FILE* f,lll_output_data_stream_t* o);



__LLL_IMPORT_EXPORT __LLL_RETURN_STRING_INDEX lll_create_string(lll_string_table_t* st,const lll_char_t* dt,lll_string_length_t l);



__LLL_IMPORT_EXPORT __LLL_RETURN_CODE lll_execute_assembly(const lll_assembly_data_t* a_dt,const lll_stack_data_t* st,lll_internal_function_table_t* i_ft,lll_input_data_stream_t* in,lll_output_data_stream_t* out,lll_error_t* e);



__LLL_IMPORT_EXPORT void lll_free_assembly_data(lll_assembly_data_t* a_dt);



__LLL_IMPORT_EXPORT void lll_free_assembly_function_table(lll_assembly_function_table_t* ft);



__LLL_IMPORT_EXPORT void lll_free_compilation_data(lll_compilation_data_t* c_dt);



__LLL_IMPORT_EXPORT void lll_free_export_table(lll_export_table_t* et);



__LLL_IMPORT_EXPORT void lll_free_function_table(lll_function_table_t* ft);



__LLL_IMPORT_EXPORT void lll_free_identifier_table(lll_identifier_table_t* idt);



__LLL_IMPORT_EXPORT void lll_free_import_table(lll_import_table_t* it);



__LLL_IMPORT_EXPORT void lll_free_internal_function_table(lll_internal_function_table_t* ift);



__LLL_IMPORT_EXPORT void lll_free_string_table(lll_string_table_t* st);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_generate_assembly(const lll_compilation_data_t* c_dt,lll_assembly_data_t* o,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN_SIZE lll_get_object_size(const lll_object_t* o);



__LLL_IMPORT_EXPORT void lll_init_compilation_data(const char* fp,lll_input_data_stream_t* is,lll_compilation_data_t* o);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_insert_debug_object(lll_compilation_data_t* c_dt,lll_input_data_stream_t* is,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_load_assembly(lll_input_data_stream_t* is,lll_assembly_data_t* a_dt,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_load_compiled_object(lll_input_data_stream_t* is,lll_compilation_data_t* c_dt,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_load_object(lll_compilation_data_t* c_dt,lll_input_data_stream_t* is,lll_object_t** o,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN_FUNCTION_INDEX lll_lookup_internal_function(const lll_internal_function_table_t* i_ft,const char* nm);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_merge_import(lll_compilation_data_t* c_dt,lll_import_index_t im_i,lll_compilation_data_t* im,lll_error_t* e);



__LLL_IMPORT_EXPORT void lll_optimize_object(lll_compilation_data_t* c_dt,lll_object_t* o);



__LLL_IMPORT_EXPORT void lll_optimize_metadata(lll_compilation_data_t* c_dt);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_parse_all_objects(lll_compilation_data_t* c_dt,lll_internal_function_table_t* i_ft,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_parse_object(lll_compilation_data_t* c_dt,lll_internal_function_table_t* i_ft,lll_error_t* e,lll_object_t** o);



__LLL_IMPORT_EXPORT void lll_print_assembly(const lll_assembly_data_t* a_dt,FILE* f);



__LLL_IMPORT_EXPORT void lll_print_error(lll_input_data_stream_t* is,const lll_error_t* e);



__LLL_IMPORT_EXPORT void lll_print_object(const lll_compilation_data_t* c_dt,const lll_object_t* o,FILE* f);



__LLL_IMPORT_EXPORT lll_function_index_t lll_register_internal_function(lll_internal_function_table_t* i_ft,const char* nm,lll_internal_function_pointer_t f);



__LLL_IMPORT_EXPORT void lll_register_standard_internal_functions(lll_internal_function_table_t* i_ft);



__LLL_IMPORT_EXPORT void lll_remove_object_debug_data(lll_object_t* o);



__LLL_IMPORT_EXPORT void lll_remove_object_padding(lll_compilation_data_t* c_dt,lll_object_t* o);



__LLL_IMPORT_EXPORT void lll_set_assembly_data_stack(lll_assembly_data_t* a_dt,uint8_t* bf,lll_stack_offset_t sz);



__LLL_IMPORT_EXPORT void lll_set_compilation_data_stack(lll_compilation_data_t* c_dt,uint8_t* bf,lll_stack_offset_t sz);



__LLL_IMPORT_EXPORT void lll_setup_stack(lll_stack_data_t* o,uint8_t* bf,lll_stack_offset_t sz);



__LLL_IMPORT_EXPORT void lll_write_assembly(lll_output_data_stream_t* os,const lll_assembly_data_t* o);



__LLL_IMPORT_EXPORT void lll_write_compiled_object(lll_output_data_stream_t* os,const lll_compilation_data_t* c_dt);



__LLL_IMPORT_EXPORT void lll_write_object(lll_output_data_stream_t* os,const lll_object_t* o);



#endif

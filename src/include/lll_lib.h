#ifndef __LLL_H__
#define __LLL_H__ 1
#include <stdint.h>
#include <stdio.h>
#include <version.h>



#ifdef _MSC_VER
#ifdef __LLL_LIB_COMPILATION__
#define __LLL_IMPORT_EXPORT __declspec(dllexport)
#else
#define __LLL_IMPORT_EXPORT __declspec(dllimport)
#endif
#define __LLL_RETURN _Check_return_ lll_return_t
#define __LLL_RETURN_CODE _Check_return_ lll_return_code_t
#define __LLL_OBJECT_ALIGNMENT __declspec(align(8))
#else
#ifdef __LLL_LIB_COMPILATION__
#define __LLL_IMPORT_EXPORT __attribute__((visibility("default")))
#else
#define __LLL_IMPORT_EXPORT
#endif
#define __LLL_RETURN __attribute__((warn_unused_result)) lll_return_t
#define __LLL_RETURN_CODE __attribute__((warn_unused_result)) lll_return_code_t
#define __LLL_OBJECT_ALIGNMENT __attribute__((aligned(8)))
#endif

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
#define LLL_ERROR_MATH_OP_NOT_ENOUGH_ARGUMENTS 24
#define LLL_ERROR_MATH_OP_TOO_MANY_ARGUMENTS 25
#define LLL_ERROR_FOR_NOT_ENOUGH_ARGUMENTS 26
#define LLL_ERROR_SET_NOT_ENOUGH_ARGUMENTS 27
#define LLL_ERROR_SET_NO_INDENTIFIER 28
#define LLL_ERROR_UNKNOWN_IDENTIFIER 29
#define LLL_ERROR_STRING_REQUIRED 30
#define LLL_ERROR_NO_STACK 31
#define LLL_ERROR_DIVISION_BY_ZERO 32
#define LLL_ERROR_INVALID_FILE_FORMAT 33
#define LLL_ERROR_INVALID_IMPORT_INDEX 34
#define LLL_ERROR_ASSERTION 255
#define LLL_MAX_COMPILATION_ERROR LLL_ERROR_STRING_REQUIRED

#define LLL_RETURN_ERROR 0
#define LLL_RETURN_NO_ERROR 1

#define LLL_OBJECT_TYPE_UNKNOWN 0
#define LLL_OBJECT_TYPE_CHAR 1
#define LLL_OBJECT_TYPE_INT 2
#define LLL_OBJECT_TYPE_FLOAT 3
#define LLL_OBJECT_TYPE_NIL 4
#define LLL_OBJECT_TYPE_TRUE 5
#define LLL_OBJECT_TYPE_FALSE 6
#define LLL_OBJECT_TYPE_STRING 7
#define LLL_OBJECT_TYPE_IDENTIFIER 8
#define LLL_OBJECT_TYPE_PRINT 9
#define LLL_OBJECT_TYPE_INPUT 10
#define LLL_OBJECT_TYPE_AND 11
#define LLL_OBJECT_TYPE_OR 12
#define LLL_OBJECT_TYPE_NOT 13
#define LLL_OBJECT_TYPE_SET 14
#define LLL_OBJECT_TYPE_FUNC 15
#define LLL_OBJECT_TYPE_CALL 16
#define LLL_OBJECT_TYPE_IF 17
#define LLL_OBJECT_TYPE_FOR 18
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
#define LLL_OBJECT_TYPE_IMPORT 61
#define LLL_OBJECT_TYPE_OPERATION_LIST 62
#define LLL_OBJECT_TYPE_DEBUG_DATA 63
#define LLL_OBJECT_TYPE_NOP 0xff
#define LLL_OBJECT_TYPE_CONST 0x80
#define LLL_OBJECT_TYPE_MAX_INTEGRAL_TYPE LLL_OBJECT_TYPE_FALSE
#define LLL_OBJECT_TYPE_MAX_TYPE LLL_OBJECT_TYPE_IDENTIFIER
#define LLL_OBJECT_TYPE_MAX_FLOW LLL_OBJECT_TYPE_FOR
#define LLL_OBJECT_TYPE_MAX_MATH LLL_OBJECT_TYPE_BIT_NOT
#define LLL_OBJECT_TYPE_MAX_COMPARE LLL_OBJECT_TYPE_MORE_EQUAL
#define LLL_OBJECT_TYPE_MIN_EXTRA LLL_OBJECT_TYPE_IMPORT
#define LLL_IS_OBJECT_TYPE_NOT_INTEGRAL(o) (LLL_GET_OBJECT_TYPE(o)>LLL_OBJECT_TYPE_MAX_INTEGRAL_TYPE)
#define LLL_IS_OBJECT_TYPE_TYPE(o) (LLL_GET_OBJECT_TYPE(o)<=LLL_OBJECT_TYPE_MAX_TYPE)
#define LLL_IS_OBJECT_TYPE_NOT_TYPE(o) (LLL_GET_OBJECT_TYPE(o)>LLL_OBJECT_TYPE_MAX_TYPE)
#define LLL_IS_OBJECT_TYPE_MATH(o) (LLL_GET_OBJECT_TYPE(o)>LLL_OBJECT_TYPE_MAX_FLOW&&LLL_GET_OBJECT_TYPE(o)<=LLL_OBJECT_TYPE_MAX_MATH)
#define LLL_IS_OBJECT_TYPE_COMPARE(o) (LLL_GET_OBJECT_TYPE(o)>LLL_OBJECT_TYPE_MAX_MATH&&LLL_GET_OBJECT_TYPE(o)<=LLL_OBJECT_TYPE_MAX_COMPARE)
#define LLL_IS_OBJECT_UNKNOWN(o) (!((o)->t&0x3f))
#define LLL_IS_OBJECT_CONST(o) ((o)->t>>7)
#define LLL_GET_OBJECT_TYPE(o) ((o)->t&0x3f)
#define LLL_GET_DEBUG_OBJECT_CHILD(o) ((lll_object_t*)(void*)(((uint64_t)(void*)(o))+sizeof(lll_debug_object_t)))
#define LLL_GET_OBJECT_AFTER_NOP(o) ((lll_object_t*)(void*)(((uint64_t)(void*)(o))+sizeof(lll_object_type_t)))
#define LLL_GET_OBJECT_ARGUMENT(o,i) ((lll_object_t*)(void*)(((uint64_t)(void*)(o))+(i)))
#define LLL_GET_OBJECT_STATEMENT(o,i) ((lll_object_t*)(void*)(((uint64_t)(void*)(o))+(i)))
#define LLL_SET_OBJECT_NOP(o,i) ((*((lll_object_type_t*)(void*)((uint64_t)(void*)(o)+(i))))=LLL_OBJECT_TYPE_NOP)

#define LLL_MAX_SHORT_IDENTIFIER_LENGTH 15
#define LLL_IDENTIFIER_GET_ARRAY_ID(i) ((i)&0xf)
#define LLL_IDENTIFIER_GET_ARRAY_INDEX(i) ((i)>>4)
#define LLL_IDENTIFIER_ADD_INDEX(i,j) ((i)+((j)<<4))
#define LLL_CREATE_IDENTIFIER(i,j) (((i)<<4)|(j))

#define LLL_WRITE_MODE_ASSEMBLY 0
#define LLL_WRITE_MODE_CODE 1
#define LLL_WRITE_MODE_RAW 2

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

#define LLL_MAX_SCOPE UINT32_MAX
#define LLL_MAX_STACK_OFFSET UINT32_MAX
#define LLL_MAX_STRING_INDEX UINT32_MAX



typedef char lll_char_t;



typedef uint8_t lll_arg_count_t;



typedef uint8_t lll_error_type_t;



typedef uint8_t lll_object_type_t;



typedef uint8_t lll_return_t;



typedef uint8_t lll_string_checksum_t;



typedef uint16_t lll_file_path_index_t;



typedef uint16_t lll_function_index_t;



typedef uint16_t lll_statement_count_t;



typedef uint16_t lll_version_t;



typedef int32_t lll_return_code_t;



typedef uint32_t lll_column_number_t;



typedef uint32_t lll_file_offset_t;



typedef uint32_t lll_identifier_index_t;



typedef uint32_t lll_identifier_list_length_t;



typedef uint32_t lll_import_index_t;



typedef uint32_t lll_line_number_t;



typedef uint32_t lll_scope_t;



typedef uint32_t lll_stack_offset_t;



typedef uint32_t lll_string_index_t;



typedef uint32_t lll_string_length_t;



typedef int64_t lll_integer_t;



typedef uint64_t lll_time_t;



typedef double lll_float_t;



typedef struct __LLL_STACK_CONTEXT{
	uint8_t* ptr;
	lll_stack_offset_t off;
	lll_stack_offset_t sz;
} lll_stack_context_t;



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



typedef struct __LLL_OBJECT_ALIGNMENT __LLL_OBJECT{
	lll_object_type_t t;
} lll_object_t;



typedef struct __LLL_OBJECT_ALIGNMENT __LLL_CHAR_OBJECT{
	lll_object_type_t t;
	lll_char_t v;
} lll_char_object_t;



typedef struct __LLL_OBJECT_ALIGNMENT __LLL_INTEGER_OBJECT{
	lll_object_type_t t;
	lll_integer_t v;
} lll_integer_object_t;



typedef struct __LLL_OBJECT_ALIGNMENT __LLL_FLOAT_OBJECT{
	lll_object_type_t t;
	lll_float_t v;
} lll_float_object_t;



typedef struct __LLL_OBJECT_ALIGNMENT __LLL_STRING_OBJECT{
	lll_object_type_t t;
	lll_string_index_t i;
} lll_string_object_t;



typedef struct __LLL_OBJECT_ALIGNMENT __LLL_IDENTIFIER_OBJECT{
	lll_object_type_t t;
	lll_identifier_index_t idx;
} lll_identifier_object_t;



typedef struct __LLL_OBJECT_ALIGNMENT __LLL_FUNCTION_OBJECT{
	lll_object_type_t t;
	lll_arg_count_t ac;
	lll_function_index_t id;
} lll_function_object_t;



typedef struct __LLL_OBJECT_ALIGNMENT __LLL_OPERATOR_OBJECT{
	lll_object_type_t t;
	lll_arg_count_t ac;
} lll_operator_object_t;



typedef struct __LLL_OBJECT_ALIGNMENT __LLL_IMPORT_OBJECT{
	lll_object_type_t t;
	lll_arg_count_t ac;
	lll_import_index_t idx[];
} lll_import_object_t;



typedef struct __LLL_OBJECT_ALIGNMENT __LLL_OPERATION_LIST_OBJECT{
	lll_object_type_t t;
	lll_statement_count_t sc;
} lll_operation_list_object_t;



typedef struct __LLL_OBJECT_ALIGNMENT __LLL_DEBUG_OBJECT{
	lll_object_type_t t;
	lll_string_index_t fpi;
	lll_line_number_t ln;
	lll_column_number_t cn;
	lll_file_offset_t ln_off;
} lll_debug_object_t;



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
} lll_identifier_data_t;



typedef struct __LLL_IMPORT_DATA{
	lll_string_index_t* dt;
	lll_import_index_t l;
} lll_import_data_t;



typedef struct __LLL_FUNCTION{
	lll_stack_offset_t off;
	lll_arg_count_t al;
	lll_identifier_index_t a[];
} lll_function_t;



typedef struct __LLL_FUNCTION_DATA{
	lll_function_t** dt;
	lll_function_index_t l;
} lll_function_data_t;



typedef struct __LLL_STRING{
	lll_string_length_t l;
	lll_string_checksum_t c;
	lll_char_t v[];
} lll_string_t;



typedef struct __LLL_STRING_TABLE{
	lll_string_index_t l;
	lll_string_t** dt;
} lll_string_table_t;



typedef struct __LLL_COMPILATION_DATA{
	lll_input_data_stream_t* is;
	lll_time_t tm;
	lll_object_t* h;
	lll_identifier_data_t i_dt;
	lll_import_data_t im;
	lll_function_data_t f_dt;
	lll_string_table_t st;
	lll_scope_t _n_sc_id;
} lll_compilation_data_t;



typedef struct __LLL_ERROR_DATA_RANGE{
	lll_file_offset_t off;
	lll_file_offset_t sz;
} lll_error_data_range_t;



typedef union __LLL_ERROR_DATA{
	char str[256];
	lll_error_data_range_t r;
	lll_import_index_t im_i;
} lll_error_data_t;



typedef struct __LLL_ERROR{
	lll_error_type_t t;
	lll_error_data_t dt;
} lll_error_t;



__LLL_IMPORT_EXPORT void lll_create_input_data_stream(FILE* f,lll_input_data_stream_t* o);



__LLL_IMPORT_EXPORT void lll_create_output_data_stream(FILE* f,lll_output_data_stream_t* o);



__LLL_IMPORT_EXPORT void lll_free_compilation_data(lll_compilation_data_t* c_dt);



__LLL_IMPORT_EXPORT void lll_free_function_data(lll_function_data_t* f_dt);



__LLL_IMPORT_EXPORT void lll_free_identifier_data(lll_identifier_data_t* i_dt);



__LLL_IMPORT_EXPORT void lll_free_import_data(lll_import_data_t* im);



__LLL_IMPORT_EXPORT void lll_free_string_table(lll_string_table_t* st);



__LLL_IMPORT_EXPORT void lll_init_compilation_data(const char* fp,lll_input_data_stream_t* is,lll_compilation_data_t* o);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_insert_debug_object(lll_input_data_stream_t* is,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_load_compiled_object(lll_input_data_stream_t* is,lll_compilation_data_t* c_dt,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_load_object(lll_input_data_stream_t* is,lll_object_t** o,lll_error_t* e);



__LLL_IMPORT_EXPORT void lll_load_stack_context(const lll_stack_context_t* ctx);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_merge_import(lll_compilation_data_t* c_dt,lll_import_index_t im_i,lll_compilation_data_t* im,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_optimize_object(lll_object_t* o,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_optimize_metadata(lll_compilation_data_t* c_dt,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_parse_all_objects(lll_compilation_data_t* c_dt,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_parse_object(lll_compilation_data_t* c_dt,lll_error_t* e,lll_object_t** o);



__LLL_IMPORT_EXPORT void lll_print_error(lll_input_data_stream_t* is,const lll_error_t* e);



__LLL_IMPORT_EXPORT void lll_print_object(lll_compilation_data_t* c_dt,const lll_object_t* o,FILE* f);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_remove_object_debug_data(lll_object_t* o,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_remove_object_padding(lll_compilation_data_t* c_dt,lll_object_t* o,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN_CODE lll_run_compiled_object(const lll_compilation_data_t* c_dt,lll_input_data_stream_t* in,lll_output_data_stream_t* out);



__LLL_IMPORT_EXPORT void lll_save_stack_context(lll_stack_context_t* ctx);



__LLL_IMPORT_EXPORT void lll_set_internal_stack(uint8_t* bf,lll_stack_offset_t sz);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_write_compiled_object(lll_output_data_stream_t* os,const lll_compilation_data_t* c_dt,uint8_t m,lll_error_t* e);



__LLL_IMPORT_EXPORT __LLL_RETURN lll_write_object(lll_output_data_stream_t* os,const lll_object_t* o,lll_error_t* e);



#endif

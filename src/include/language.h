#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__ 1
#include <stdint.h>
#include <stdio.h>



#ifndef NULL
#define NULL ((void*)0)
#endif
#ifdef IS_ERROR
#undef IS_ERROR
#endif

#define ERROR_UNKNOWN 0
#define ERROR_INTERNAL_STACK_OVERFLOW 1
#define ERROR_UNMATCHED_OPEN_PARENTHESES 2
#define ERROR_UNMATCHED_CLOSE_PARENTHESES 3
#define ERROR_UNMATCHED_OPEN_QUOTE 4
#define ERROR_UNMATCHED_CURLY_OPEN_BRACKETS 5
#define ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS 6
#define ERROR_UNMATCHED_QUOTES 7
#define ERROR_EMPTY_CHAR_STRING 8
#define ERROR_UNTERMINATED_CHAR_STRING 9
#define ERROR_UNTERMINATED_ESCAPE_SEQUENCE 10
#define ERROR_UNKNOWN_ESCAPE_CHARACTER 11
#define ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE 12
#define ERROR_UNKNOWN_HEXADECIMAL_CHARCTER 13
#define ERROR_UNKNOWN_DECIMAL_CHARCTER 14
#define ERROR_UNKNOWN_OCTAL_CHARCTER 15
#define ERROR_UNKNOWN_BINARY_CHARCTER 16
#define ERROR_UNKNOWN_SYMBOL 17
#define ERROR_UNKNOWN_IDENTIFIER_CHARACTER 18
#define ERROR_UNEXPECTED_CHARACTER 19
#define ERROR_SYMBOL_TOO_LONG 20
#define ERROR_NO_SYMBOL 21
#define ERROR_TOO_MANY_ARGUMENTS 22
#define ERROR_MATH_OP_NOT_ENOUGH_ARGUMENTS 23
#define ERROR_MATH_OP_TOO_MANY_ARGUMENTS 24
#define IS_ERROR(o) (((uint64_t)(void*)(o))>>63)
#define GET_ERROR(o) ((error_t)(((uint64_t)(void*)(o))&0x7fffffffffffffffull))
#define GET_ERROR_TYPE(e) ((e)&0x1f)
#define GET_ERROR_FILE_OFFSET(e) (((e)>>5)&0xffffffff)
#define GET_ERROR_SIZE(e) ((e)>>37)
#define GET_ERROR_CHAR(e) ((char)((e)>>5))
#define GET_ERROR_STRING(e) ((char*)(_bf+((uint32_t)((e)>>5))))
#define GET_ERROR_OBJECT(e) ((object_t*)(_bf+((uint32_t)((e)>>5))))
#define RETURN_ERROR(e) ((object_t*)((e)|0x8000000000000000ull))
#define CREATE_ERROR_FILE_OFFSET(e,off,sz) ((((uint64_t)(sz))<<37)|(((uint64_t)(off))<<5)|(e))
#define CREATE_ERROR_CHAR(e,c) ((((uint16_t)(c))<<5)|(e))
#define CREATE_ERROR_STRING(e,s) (((((uint64_t)(void*)(s))-((uint64_t)(void*)_bf))<<5)|(e))
#define CREATE_ERROR_OBJECT(e,o) (((((uint64_t)(void*)(o))-((uint64_t)(void*)_bf))<<5)|(e))
#define CREATE_ERROR_SINGLE_CHAR(e,in) CREATE_ERROR_FILE_OFFSET(e,GET_INPUT_DATA_STREAM_OFFSET(in)-2,1)

#define OBJECT_TYPE_UNKNOWN 0
#define OBJECT_TYPE_CHAR 1
#define OBJECT_TYPE_STRING 2
#define OBJECT_TYPE_INT 3
#define OBJECT_TYPE_FLOAT 4
#define OBJECT_TYPE_IDENTIFIER 5
#define OBJECT_TYPE_NIL 6
#define OBJECT_TYPE_TRUE 7
#define OBJECT_TYPE_FALSE 8
#define OBJECT_TYPE_CAST_CHAR 9
#define OBJECT_TYPE_CAST_STRING 10
#define OBJECT_TYPE_CAST_INT 11
#define OBJECT_TYPE_CAST_INT64 12
#define OBJECT_TYPE_CAST_FLOAT 13
#define OBJECT_TYPE_CAST_FLOAT64 14
#define OBJECT_TYPE_CAST_BOOL 15
#define OBJECT_TYPE_FUNC_PRINT 16
#define OBJECT_TYPE_FUNC_PTR 17
#define OBJECT_TYPE_FUNC_TYPE 18
#define OBJECT_TYPE_AND 19
#define OBJECT_TYPE_OR 20
#define OBJECT_TYPE_NOT 21
#define OBJECT_TYPE_SET 22
#define OBJECT_TYPE_FUNC 23
#define OBJECT_TYPE_IF 24
#define OBJECT_TYPE_FOR 25
#define OBJECT_TYPE_ADD 26
#define OBJECT_TYPE_SUB 27
#define OBJECT_TYPE_MULT 28
#define OBJECT_TYPE_DIV 29
#define OBJECT_TYPE_FLOOR_DIV 30
#define OBJECT_TYPE_MOD 31
#define OBJECT_TYPE_BIT_AND 32
#define OBJECT_TYPE_BIT_OR 33
#define OBJECT_TYPE_BIT_XOR 34
#define OBJECT_TYPE_BIT_NOT 35
#define OBJECT_TYPE_DIV_MOD 36
#define OBJECT_TYPE_POW 37
#define OBJECT_TYPE_ROOT 38
#define OBJECT_TYPE_FLOOR_ROOT 39
#define OBJECT_TYPE_LOG 40
#define OBJECT_TYPE_FLOOR_LOG 41
#define OBJECT_TYPE_LESS 42
#define OBJECT_TYPE_LESS_EQUAL 43
#define OBJECT_TYPE_EQUAL 44
#define OBJECT_TYPE_NOT_EQUAL 45
#define OBJECT_TYPE_MORE 46
#define OBJECT_TYPE_MORE_EQUAL 47
#define OBJECT_TYPE_OPERATION_LIST 48
#define OBJECT_TYPE_INT64_FLAG 0x40
#define OBJECT_TYPE_FLOAT64_FLAG 0x40
#define OBJECT_TYPE_REF_FLAG 0x80
#define OBJECT_TYPE_MAX_TYPE OBJECT_TYPE_FALSE
#define OBJECT_TYPE_MAX_FUNC OBJECT_TYPE_PTR
#define OBJECT_TYPE_MAX_FLOW OBJECT_TYPE_FOR
#define OBJECT_TYPE_MAX_MATH OBJECT_TYPE_FLOOR_LOG
#define OBJECT_TYPE_MAX_MATH_CHAIN OBJECT_TYPE_BIT_NOT
#define OBJECT_TYPE_MAX_COMPARE OBJECT_TYPE_MORE_EQUAL
#define IS_OBJECT_INT64(o) ((o)->t&OBJECT_TYPE_INT64_FLAG)
#define IS_OBJECT_FLOAT64(o) ((o)->t&OBJECT_TYPE_FLOAT64_FLAG)
#define IS_OBJECT_REF(o) ((o)->t>>7)
#define IS_OBJECT_TYPE_NOT_TYPE(o) (GET_OBJECT_TYPE(o)>OBJECT_TYPE_MAX_TYPE)
#define IS_OBJECT_TYPE_MATH_CHAIN_OPERATION(o) (GET_OBJECT_TYPE(o)>OBJECT_TYPE_MAX_FLOW&&GET_OBJECT_TYPE(o)<=OBJECT_TYPE_MAX_MATH_CHAIN)
#define IS_OBJECT_TYPE_MATH_NO_CHAIN_OPERATION(o) (GET_OBJECT_TYPE(o)>OBJECT_TYPE_MAX_MATH_CHAIN&&GET_OBJECT_TYPE(o)<=OBJECT_TYPE_MAX_MATH)
#define GET_OBJECT_TYPE(o) ((o)->t&0x3f)
#define GET_OBJECT_REF(o) GET_OBJECT_FROM_STACK_OFFSET(*((uint32_t*)(((uint8_t*)(o))+sizeof(object_t))))
#define GET_OBJECT_ARGUMENT_COUNT(o) (*((arg_count_t*)(((uint8_t*)(o))+sizeof(object_t))))
#define GET_OBJECT_ARGUMENT(o,i) ((object_t*)(((uint8_t*)(o))+(i)))
#define GET_OBJECT_AS_CHAR(o) (*((char*)(((uint8_t*)(o))+sizeof(object_t))))
#define GET_OBJECT_AS_INT32(o) (*((int32_t*)(((uint8_t*)(o))+sizeof(object_t))))
#define GET_OBJECT_AS_INT64(o) (*((int64_t*)(((uint8_t*)(o))+sizeof(object_t))))
#define GET_OBJECT_AS_FLOAT32(o) (*((float*)(((uint8_t*)(o))+sizeof(object_t))))
#define GET_OBJECT_AS_FLOAT64(o) (*((double*)(((uint8_t*)(o))+sizeof(object_t))))
#define GET_OBJECT_STRING_LENGTH(o) (*((string_length_t*)(((uint8_t*)(o))+sizeof(object_t))))
#define GET_OBJECT_AS_STRING(o) ((char*)(((uint8_t*)(o))+sizeof(object_t)+sizeof(string_length_t)))
#define RESET_OBJECT_ARGUMENT_COUNT(o) ((*((arg_count_t*)(((uint8_t*)(o))+sizeof(object_t))))=0)
#define INCREASE_OBJECT_ARGUMENT_COUNT(o,sp,ep) \
	do{ \
		arg_count_t* __ac=((arg_count_t*)(((uint8_t*)(o))+sizeof(object_t))); \
		if (*__ac==UINT8_MAX){ \
			return RETURN_ERROR(CREATE_ERROR_FILE_OFFSET(ERROR_TOO_MANY_ARGUMENTS,sp,ep-sp)); \
		} \
		(*__ac)++; \
	} while (0)
#define SET_OBJECT_AS_INT32(o,i) ((*((int32_t*)(((uint8_t*)(o))+sizeof(object_t))))=(int32_t)(i))
#define SET_OBJECT_AS_INT64(o,i) ((*((int64_t*)(((uint8_t*)(o))+sizeof(object_t))))=(int64_t)(i))
#define SET_OBJECT_AS_FLOAT32(o,f) ((*((float*)(((uint8_t*)(o))+sizeof(object_t))))=(f))
#define SET_OBJECT_AS_FLOAT64(o,f) ((*((double*)(((uint8_t*)(o))+sizeof(object_t))))=(f))
#define SET_OBJECT_STRING_LENGTH(o,sz) ((*((string_length_t*)(((uint8_t*)(o))+sizeof(object_t))))=(sz))

#ifndef INTERNAL_STACK_SIZE
#define INTERNAL_STACK_SIZE 65536
#endif
#if INTERNAL_STACK_SIZE>=67108864
#error Internal Stack Too Big!
#endif
#define GET_OBJECT_STACK_OFFSET(o) ((uint32_t)(((uint64_t)(o))-((uint64_t)_bf)))
#define GET_OBJECT_FROM_STACK_OFFSET(i) ((object_t*)(_bf+(i)))

#define END_OF_DATA -1
#define READ_FROM_INPUT_DATA_STREAM(in) ((in)->rf((in)))
#define GET_INPUT_DATA_STREAM_OFFSET(in) ((in)->_off)
#define INPUT_DATA_STREAM_RESTART_LINE(in,lp) ((in)->rlf((in),(lp)))



struct __INPUT_DATA_SOURCE;



typedef uint8_t arg_count_t;



typedef uint32_t string_length_t;



typedef uint64_t error_t;



typedef int (*input_data_source_read_t)(struct __INPUT_DATA_SOURCE* in);



typedef void (*input_data_source_restart_line_t)(struct __INPUT_DATA_SOURCE* in,uint32_t lp);



typedef struct __INPUT_DATA_SOURCE{
	void* ctx;
	input_data_source_read_t rf;
	input_data_source_restart_line_t rlf;
	uint32_t _lc;
	uint32_t _off;
} input_data_source_t;



typedef struct __OBJECT{
	uint8_t t;
} object_t;



void create_input_data_source(FILE* f,input_data_source_t* o);



void print_error(input_data_source_t* in,error_t e);



void print_object(object_t* o,FILE* f);



object_t* read_object(input_data_source_t* in);



#endif

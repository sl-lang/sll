#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__ 1
#include <stdint.h>
#include <stdio.h>



#define ERROR_UNKNOWN 0
#define ERROR_INTERNAL_STACK_OVERFLOW 1
#define ERROR_UNMATCHED_OPEN_PARENTHESES 2
#define ERROR_UNMATCHED_CLOSE_PARENTHESES 3
#define ERROR_EMPTY_PARENTHESES 4
#define ERROR_UNMATCHED_QUOTES 5
#define ERROR_EMPTY_CHAR_STRING 6
#define ERROR_UNTERMINATED_CHAR_STRING 7
#define ERROR_UNTERMINATED_ESCAPE_SEQUENCE 8
#define ERROR_UNKNOWN_ESCAPE_CHARACTER 9
#define ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE 10
#define ERROR_UNKNOWN_HEXADECIMAL_CHARCTER 11
#define ERROR_UNKNOWN_DECIMAL_CHARCTER 12
#define ERROR_UNKNOWN_OCTAL_CHARCTER 13
#define ERROR_UNKNOWN_BINARY_CHARCTER 14
#define ERROR_UNKNOWN_SYMBOL 15
#define ERROR_UNKNOWN_IDENTIFIER_CHARACTER 16
#define ERROR_UNEXPECTED_CHARACTER 17
#define ERROR_NO_SYMBOL 18
#define ERROR_MATH_OP_NOT_ENOUGH_ARGUMENTS 19
#define ERROR_MATH_OP_TOO_MANY_ARGUMENTS 20
#define IS_ERROR(o) (((uint64_t)(void*)(o))>>63)
#define GET_ERROR(o) ((error_t)(((uint64_t)(void*)(o))&0x7fffffffffffffffull))
#define GET_ERROR_TYPE(e) ((e)&0x1f)
#define GET_ERROR_CHAR(e) ((char)((e)>>5))
#define GET_ERROR_STRING(e) ((char*)(_bf+((uint32_t)((e)>>5))))
#define GET_ERROR_OBJECT(e) ((object_t*)(_bf+((uint32_t)((e)>>5))))
#define RETURN_ERROR(e) ((object_t*)((e)|0x8000000000000000ull))
#define CREATE_ERROR_CHAR(e,c) ((((uint16_t)(c))<<5)|(e))
#define CREATE_ERROR_STRING(e,s) (((((uint64_t)(void*)(s))-((uint64_t)(void*)_bf))<<5)|(e))
#define CREATE_ERROR_OBJECT(e,o) (((((uint64_t)(void*)(o))-((uint64_t)(void*)_bf))<<5)|(e))

#define OBJECT_TYPE_UNKNOWN 0
#define OBJECT_TYPE_CHAR 1
#define OBJECT_TYPE_STRING 2
#define OBJECT_TYPE_INT 3
#define OBJECT_TYPE_FLOAT 4
#define OBJECT_TYPE_IDENTIFIER 5
#define OBJECT_TYPE_NIL 6
#define OBJECT_TYPE_TRUE 7
#define OBJECT_TYPE_FALSE 8
#define OBJECT_TYPE_PRINT 9
#define OBJECT_TYPE_PTR 10
#define OBJECT_TYPE_OPERATION_LIST 11
#define OBJECT_TYPE_AND 12
#define OBJECT_TYPE_OR 13
#define OBJECT_TYPE_SET 14
#define OBJECT_TYPE_FUNC 15
#define OBJECT_TYPE_IF 16
#define OBJECT_TYPE_ADD 17
#define OBJECT_TYPE_SUB 18
#define OBJECT_TYPE_MULT 19
#define OBJECT_TYPE_DIV 20
#define OBJECT_TYPE_FLOOR_DIV 21
#define OBJECT_TYPE_MOD 22
#define OBJECT_TYPE_BIT_AND 23
#define OBJECT_TYPE_BIT_OR 24
#define OBJECT_TYPE_BIT_XOR 25
#define OBJECT_TYPE_BIT_NOT 26
#define OBJECT_TYPE_DIV_MOD 27
#define OBJECT_TYPE_POW 28
#define OBJECT_TYPE_ROOT 29
#define OBJECT_TYPE_FLOOR_ROOT 30
#define OBJECT_TYPE_LOG 31
#define OBJECT_TYPE_FLOOR_LOG 32
#define OBJECT_TYPE_LESS 33
#define OBJECT_TYPE_LESS_EQUAL 34
#define OBJECT_TYPE_EQUAL 35
#define OBJECT_TYPE_NOT_EQUAL 36
#define OBJECT_TYPE_MORE 37
#define OBJECT_TYPE_MORE_EQUAL 38
#define OBJECT_TYPE_INT64_FLAG 0x40
#define OBJECT_TYPE_FLOAT64_FLAG 0x40
#define OBJECT_TYPE_REF_FLAG 0x80
#define OBJECT_TYPE_MAX_TYPE OBJECT_TYPE_FALSE
#define OBJECT_TYPE_MAX_FUNC OBJECT_TYPE_OPERATION_LIST
#define OBJECT_TYPE_MAX_FLOW OBJECT_TYPE_IF
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
#define INCREASE_OBJECT_ARGUMENT_COUNT(o) ((*((arg_count_t*)(((uint8_t*)(o))+sizeof(object_t))))++)
#define SET_OBJECT_AS_INT32(o,i) ((*((int32_t*)(((uint8_t*)(o))+sizeof(object_t))))=(int32_t)(i))
#define SET_OBJECT_AS_INT64(o,i) ((*((int64_t*)(((uint8_t*)(o))+sizeof(object_t))))=(int64_t)(i))
#define SET_OBJECT_AS_FLOAT32(o,f) ((*((float*)(((uint8_t*)(o))+sizeof(object_t))))=(f))
#define SET_OBJECT_AS_FLOAT64(o,f) ((*((double*)(((uint8_t*)(o))+sizeof(object_t))))=(f))
#define SET_OBJECT_STRING_LENGTH(o,sz) ((*((string_length_t*)(((uint8_t*)(o))+sizeof(object_t))))=(sz))

#define FEATURE_EMPTY_EXPRESSION 1
#define FEATURE_OPERATION_LIST 2

#define INTERNAL_STACK_SIZE 65536
#define GET_OBJECT_STACK_OFFSET(o) ((uint32_t)(((uint64_t)(o))-((uint64_t)_bf)))
#define GET_OBJECT_FROM_STACK_OFFSET(i) ((object_t*)(_bf+(i)))



typedef uint8_t arg_count_t;



typedef uint32_t string_length_t;



typedef uint64_t error_t;



typedef struct __OBJECT{
	uint8_t t;
} object_t;



uint8_t get_feature(uint8_t f);



void set_feature(uint8_t f,uint8_t st);



void print_error(error_t e);



void print_object(object_t* o,FILE* f);



object_t* read_object(FILE* f);



#endif

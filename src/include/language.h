#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__ 1
#include <stdint.h>
#include <stdio.h>



#define ERROR_INTERNAL_STACK_OVERFLOW 1
#define ERROR_UNMATCHED_OPEN_PARENTHESES 2
#define ERROR_EMPTY_CHAR_STRING 3
#define ERROR_UNTERMINATED_CHAR_STRING 4
#define ERROR_UNMATCHED_QUOTES 5
#define ERROR_UNKNOWN_HEXADECIMAL_CHARCTER 6
#define ERROR_UNKNOWN_DECIMAL_CHARCTER 7
#define ERROR_UNKNOWN_OCTAL_CHARCTER 8
#define ERROR_UNKNOWN_BINARY_CHARCTER 9
#define ERROR_UNEXPECTED_CHARACTER 10
#define ERROR_UNTERMINATED_ESCAPE_SEQUENCE 11
#define ERROR_UNMATCHED_CLOSE_PARENTHESES 12
#define ERROR_EMPTY_PARENTHESES 13
#define ERROR_UNKNOWN_SYMBOL 14
#define ERROR_NO_SYMBOL 15
#define ERROR_UNKNOWN_IDENTIFIER_CHARACTER 16
#define ERROR_UNKNOWN_ESCAPE_CHARACTER 17
#define ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE 18
#define ERROR_MATH_OP_NOT_ENOUGH_ARGUMENTS 19
#define ERROR_MATH_OP_TOO_MANY_ARGUMENTS 20
#define IS_ERROR(o) (((uint64_t)(void*)(o))>>63)
#define GET_ERROR(o) ((error_t)(((uint64_t)(void*)(o))&0x7fffffffffffffffull))
#define GET_ERROR_TYPE(e) ((e)&0xf)
#define GET_ERROR_CHAR(e) ((char)((e)>>5))
#define RETURN_ERROR(e) ((object_t*)((e)|0x8000000000000000ull))
#define CREATE_ERROR_CHAR(e,c) ((((uint16_t)(c))<<5)|(e))
#define CREATE_ERROR_STRING(e,s) ((((s)-_bf)<<5)|(e))

#define OBJECT_TYPE_UNKNOWN 0
#define OBJECT_TYPE_CHAR 1
#define OBJECT_TYPE_STRING 2
#define OBJECT_TYPE_INT 3
#define OBJECT_TYPE_FLOAT 4
#define OBJECT_TYPE_IDENTIFIER 5
#define OBJECT_TYPE_NIL 6
#define OBJECT_TYPE_TRUE 7
#define OBJECT_TYPE_FALSE 8
#define OBJECT_TYPE_PAIR 9/***/
#define OBJECT_TYPE_LIST 10/***/
#define OBJECT_TYPE_MAP 11/***/
#define OBJECT_TYPE_PRINT 12
#define OBJECT_TYPE_PTR 13
#define OBJECT_TYPE_AND 14
#define OBJECT_TYPE_OR 15
#define OBJECT_TYPE_LET 16
#define OBJECT_TYPE_SET 17
#define OBJECT_TYPE_FUNC 18
#define OBJECT_TYPE_IF 19
#define OBJECT_TYPE_WHILE 20
#define OBJECT_TYPE_LABEL 21
#define OBJECT_TYPE_GOTO 22
#define OBJECT_TYPE_ADD 23
#define OBJECT_TYPE_SUB 24
#define OBJECT_TYPE_MULT 25
#define OBJECT_TYPE_DIV 26
#define OBJECT_TYPE_FLOOR_DIV 27
#define OBJECT_TYPE_MOD 28
#define OBJECT_TYPE_BIT_AND 29
#define OBJECT_TYPE_BIT_OR 30
#define OBJECT_TYPE_BIT_XOR 31
#define OBJECT_TYPE_BIT_NOT 32
#define OBJECT_TYPE_DIV_MOD 33
#define OBJECT_TYPE_POW 34
#define OBJECT_TYPE_ROOT 35
#define OBJECT_TYPE_FLOOR_ROOT 36
#define OBJECT_TYPE_LOG 37
#define OBJECT_TYPE_LESS 38
#define OBJECT_TYPE_LESS_EQUAL 39
#define OBJECT_TYPE_EQUAL 40
#define OBJECT_TYPE_NOT_EQUAL 41
#define OBJECT_TYPE_MORE 42
#define OBJECT_TYPE_MORE_EQUAL 43
#define OBJECT_TYPE_MAX_TYPE OBJECT_TYPE_MAP
#define OBJECT_TYPE_MAX_FUNC OBJECT_TYPE_PTR
#define OBJECT_TYPE_MAX_FLOW OBJECT_TYPE_GOTO
#define OBJECT_TYPE_MAX_MATH OBJECT_TYPE_LOG
#define OBJECT_TYPE_MAX_MATH_CHAIN OBJECT_TYPE_BIT_NOT
#define OBJECT_TYPE_IS_MATH_CHAIN_OPERATION(o) ((o)->t>OBJECT_TYPE_MAX_FLOW&&(o)->t<=OBJECT_TYPE_MAX_MATH_CHAIN)
#define OBJECT_TYPE_IS_MATH_NO_CHAIN_OPERATION(o) ((o)->t>OBJECT_TYPE_MAX_MATH_CHAIN&&(o)->t<=OBJECT_TYPE_MAX_MATH)
#define GET_OBJECT_ARGUMENT_COUNT(o) (*((arg_count_t*)(((uint8_t*)(o))+sizeof(object_t))))
#define GET_OBJECT_ARGUMENT(o,i) ((object_t*)(((uint8_t*)(o))+(i)))
#define GET_OBJECT_AS_CHAR(o) ((char)(*((char*)(((uint8_t*)(o))+sizeof(object_t)))))
#define GET_OBJECT_AS_INT(o) ((int64_t)(*((int64_t*)(((uint8_t*)(o))+sizeof(object_t)))))
#define GET_OBJECT_AS_FLOAT(o) ((double)(*((double*)(((uint8_t*)(o))+sizeof(object_t)))))
#define GET_OBJECT_STRING_LENGTH(o) (*((string_length_t*)(((uint8_t*)(o))+sizeof(object_t))))
#define GET_OBJECT_AS_STRING(o) ((char*)(((uint8_t*)(o))+sizeof(object_t)+sizeof(string_length_t)))
#define RESET_OBJECT_ARGUMENT_COUNT(o) ((*((arg_count_t*)(((uint8_t*)(o))+sizeof(object_t))))=0)
#define INCREASE_OBJECT_ARGUMENT_COUNT(o) ((*((arg_count_t*)(((uint8_t*)(o))+sizeof(object_t))))++)
#define SET_OBJECT_STRING_LENGTH(o,sz) ((*((string_length_t*)(((uint8_t*)(o))+sizeof(object_t))))=(sz))

#define INTERNAL_STACK_SIZE 65536
#define GET_OBJECT_STACK_OFFSET(o) ((uint32_t)(((uint64_t)(o))-((uint64_t)_bf)))



typedef uint8_t arg_count_t;



typedef uint32_t string_length_t;



typedef uint64_t error_t;



typedef struct __OBJECT{
	uint8_t t;
} object_t;



void print_error(error_t e);



void print_object(object_t* o,FILE* f);



object_t* read_object(FILE* f);



#endif

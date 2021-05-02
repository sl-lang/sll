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
#define IS_ERROR(o) (((uint64_t)(void*)(o))>>63)
#define GET_ERROR(o) ((error_t)(((uint64_t)(void*)(o))&0x7fffffffffffffffull))
#define GET_ERROR_TYPE(e) ((e)&0xf)
#define GET_ERROR_CHAR(e) ((char)((e)>>8/***/))
#define RETURN_ERROR(e) ((object_t*)((e)|0x8000000000000000ull))
#define CREATE_ERROR_CHAR(e,c) (((c)<<8/***/)|(e))
#define CREATE_ERROR_STRING(e,s) ((((s)-_bf)<<8/***/)|(e))

#define OBJECT_TYPE_PRINT 0
#define OBJECT_TYPE_CHAR 1
#define OBJECT_TYPE_STRING 2
#define OBJECT_TYPE_INT 3
#define OBJECT_TYPE_FLOAT 4
#define OBJECT_TYPE_IDENTIFIER 5
#define OBJECT_TYPE_NIL 6
#define OBJECT_TYPE_TRUE 7
#define OBJECT_TYPE_FALSE 8
#define OBJECT_TYPE_ADD 9
#define OBJECT_TYPE_SUB 10
#define OBJECT_TYPE_MULT 11
#define OBJECT_TYPE_DIV 12
#define OBJECT_TYPE_FLOOR_DIV 13
#define OBJECT_TYPE_MOD 14
#define OBJECT_TYPE_DIVMOD 15
#define OBJECT_TYPE_UNKNOWN 0xff
#define OBJECT_TYPE_IS_MATH_OPERATION(o) ((o)->t>=OBJECT_TYPE_ADD)
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

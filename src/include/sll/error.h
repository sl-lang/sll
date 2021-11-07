#ifndef __SLL_ERROR_H__
#define __SLL_ERROR_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_ERROR_UNKNOWN 0
#define SLL_ERROR_INTERNAL_STACK_OVERFLOW 1
#define SLL_ERROR_UNMATCHED_OPEN_PARENTHESES 2
#define SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES 3
#define SLL_ERROR_UNMATCHED_OPEN_QUOTE 4
#define SLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS 5
#define SLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS 6
#define SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS 7
#define SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS 8
#define SLL_ERROR_UNMATCHED_MAP_OPEN_BRACKETS 9
#define SLL_ERROR_UNMATCHED_MAP_CLOSE_BRACKETS 10
#define SLL_ERROR_UNMATCHED_QUOTES 11
#define SLL_ERROR_EMPTY_CHAR_STRING 12
#define SLL_ERROR_UNTERMINATED_CHAR_STRING 13
#define SLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE 14
#define SLL_ERROR_UNKNOWN_ESCAPE_CHARACTER 15
#define SLL_ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE 16
#define SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER 17
#define SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER 18
#define SLL_ERROR_UNKNOWN_OCTAL_CHARCTER 19
#define SLL_ERROR_UNKNOWN_BINARY_CHARCTER 20
#define SLL_ERROR_UNKNOWN_SYMBOL 21
#define SLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER 22
#define SLL_ERROR_UNEXPECTED_CHARACTER 23
#define SLL_ERROR_SYMBOL_TOO_LONG 24
#define SLL_ERROR_NO_SYMBOL 25
#define SLL_ERROR_TOO_MANY_ARGUMENTS 26
#define SLL_ERROR_ARRAY_TOO_LONG 27
#define SLL_ERROR_MAP_TOO_LONG 28
#define SLL_ERROR_UNKNOWN_IDENTIFIER 29
#define SLL_ERROR_INTERNAL_FUNCTION_NAME_TOO_LONG 30
#define SLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII 31
#define SLL_ERROR_UNKNOWN_INTERNAL_FUNCTION 32
#define SLL_ERROR_NO_STACK 33
#define SLL_ERROR_DIVISION_BY_ZERO 34
#define SLL_ERROR_INVALID_FILE_FORMAT 35
#define SLL_ERROR_INVALID_INSTRUCTION 36
#define SLL_ERROR_STACK_CORRUPTED 37
#define SLL_ERROR_INVALID_INSTRUCTION_INDEX 38
#define SLL_ERROR_INVALID_STACK_INDEX 39

#define SLL_MAX_COMPILATION_ERROR SLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII
#define SLL_MAX_ERROR SLL_ERROR_INVALID_STACK_INDEX



__SLL_FUNC void sll_print_error(sll_input_data_stream_t* is,const sll_error_t* e);



#endif

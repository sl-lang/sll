#ifndef __SLL_CONSTANTS_H__
#define __SLL_CONSTANTS_H__ 1
#include <stdint.h>



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
#define SLL_ERROR_TOO_MANY_STATEMENTS 27
#define SLL_ERROR_ARRAY_TOO_LONG 28
#define SLL_ERROR_MAP_TOO_LONG 29
#define SLL_ERROR_UNKNOWN_IDENTIFIER 30
#define SLL_ERROR_INTERNAL_FUNCTION_NAME_TOO_LONG 31
#define SLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII 32
#define SLL_ERROR_UNKNOWN_INTERNAL_FUNCTION 33
#define SLL_ERROR_NO_STACK 34
#define SLL_ERROR_DIVISION_BY_ZERO 35
#define SLL_ERROR_INVALID_FILE_FORMAT 36
#define SLL_ERROR_INVALID_INSTRUCTION 37
#define SLL_ERROR_STACK_CORRUPTED 38
#define SLL_ERROR_INVALID_INSTRUCTION_INDEX 39
#define SLL_ERROR_INVALID_STACK_INDEX 40
#define SLL_ERROR_UNRELEASED_OBJECTS 41
#define SLL_ERROR_SLL_ASSERTION 255
#define SLL_MAX_COMPILATION_ERROR SLL_ERROR_INTERNAL_FUNCTION_NAME_NOT_ASCII
#define SLL_MAX_ERROR SLL_ERROR_SLL_ASSERTION

#define SLL_RETURN_ERROR 0
#define SLL_RETURN_NO_ERROR 1

#define SLL_OBJECT_TYPE_UNKNOWN 0
#define SLL_OBJECT_TYPE_CHAR 1
#define SLL_OBJECT_TYPE_INT 2
#define SLL_OBJECT_TYPE_FLOAT 3
#define SLL_OBJECT_TYPE_STRING 4
#define SLL_OBJECT_TYPE_ARRAY 5
#define SLL_OBJECT_TYPE_MAP 6
#define SLL_OBJECT_TYPE_IDENTIFIER 7
#define SLL_OBJECT_TYPE_FUNCTION_ID 8
#define SLL_OBJECT_TYPE_PRINT 9
#define SLL_OBJECT_TYPE_INPUT 10
#define SLL_OBJECT_TYPE_AND 11
#define SLL_OBJECT_TYPE_OR 12
#define SLL_OBJECT_TYPE_NOT 13
#define SLL_OBJECT_TYPE_ASSIGN 14
#define SLL_OBJECT_TYPE_FUNC 15
#define SLL_OBJECT_TYPE_INTERNAL_FUNC 16
#define SLL_OBJECT_TYPE_INLINE_FUNC 17
#define SLL_OBJECT_TYPE_CALL 18
#define SLL_OBJECT_TYPE_IF 19
#define SLL_OBJECT_TYPE_FOR 20
#define SLL_OBJECT_TYPE_WHILE 21
#define SLL_OBJECT_TYPE_LOOP 22
#define SLL_OBJECT_TYPE_ADD 23
#define SLL_OBJECT_TYPE_SUB 24
#define SLL_OBJECT_TYPE_MULT 25
#define SLL_OBJECT_TYPE_DIV 26
#define SLL_OBJECT_TYPE_FLOOR_DIV 27
#define SLL_OBJECT_TYPE_MOD 28
#define SLL_OBJECT_TYPE_BIT_AND 29
#define SLL_OBJECT_TYPE_BIT_OR 30
#define SLL_OBJECT_TYPE_BIT_XOR 31
#define SLL_OBJECT_TYPE_BIT_NOT 32
#define SLL_OBJECT_TYPE_BIT_RSHIFT 33
#define SLL_OBJECT_TYPE_BIT_LSHIFT 34
#define SLL_OBJECT_TYPE_LESS 35
#define SLL_OBJECT_TYPE_LESS_EQUAL 36
#define SLL_OBJECT_TYPE_EQUAL 37
#define SLL_OBJECT_TYPE_NOT_EQUAL 38
#define SLL_OBJECT_TYPE_MORE 39
#define SLL_OBJECT_TYPE_MORE_EQUAL 40
#define SLL_OBJECT_TYPE_LENGTH 41
#define SLL_OBJECT_TYPE_ACCESS 42
#define SLL_OBJECT_TYPE_CAST 43
#define SLL_OBJECT_TYPE_BREAK 44
#define SLL_OBJECT_TYPE_CONTINUE 45
#define SLL_OBJECT_TYPE_RETURN 46
#define SLL_OBJECT_TYPE_EXIT 47
#define SLL_OBJECT_TYPE_COMMA 48
#define SLL_OBJECT_TYPE_OPERATION_LIST 49
#define SLL_OBJECT_TYPE_DEBUG_DATA 50
#define SLL_OBJECT_TYPE_NOP 255
#define SLL_IS_OBJECT_TYPE_NOT_TYPE(o) ((o)->t>SLL_OBJECT_TYPE_IDENTIFIER)

#define SLL_MAX_SHORT_IDENTIFIER_LENGTH 15
#define SLL_IDENTIFIER_GET_ARRAY_ID(i) ((i)&0xf)
#define SLL_IDENTIFIER_GET_ARRAY_INDEX(i) ((i)>>4)
#define SLL_IDENTIFIER_ADD_INDEX(i,j) ((i)+((j)<<4))
#define SLL_CREATE_IDENTIFIER(i,j) (((i)<<4)|(j))

#define SLL_ASSEMBLY_INSTRUCTION_TYPE_NOP 0
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_POP 1
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO 2
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT 3
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP 4
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP 5
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT 6
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE 7
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO 8
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE 9
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO 10
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE 11
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR 12
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT 13
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR 14
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD 15
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS 16
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK 17
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO 18
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE 19
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP 20
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO 21
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE 22
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP 23
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE 24
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO 25
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE 26
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO 27
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE 28
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR 29
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP 30
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JB 31
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE 32
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JA 33
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE 34
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JE 35
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE 36
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ 37
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ 38
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT 39
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_INC 40
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC 41
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD 42
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB 43
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT 44
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV 45
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV 46
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD 47
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_AND 48
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_OR 49
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR 50
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_INV 51
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_SHR 52
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_SHL 53
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH 54
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY 55
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS 56
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO 57
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE 58
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST 59
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE 60
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT 61
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR 62
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR 63
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR 64
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL 65
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP 66
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO 67
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE 68
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET 69
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT 70
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO 71
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT 72
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR 73
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR 74
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR 75
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_END 76
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ZERO 77
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_END_ONE 78
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL 79
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL 80
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED0 126
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED1 127
#define SLL_ASSEMBLY_INSTRUCTION_INPLACE 128
#define SLL_ASSEMBLY_INSTRUCTION_RELATIVE 128
#define SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai) ((ai)->t&0x7f)
#define SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai) ((ai)->t>>7)
#define SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai) ((ai)->t>>7)

#define SLL_RUNTIME_OBJECT_TYPE_INT 0
#define SLL_RUNTIME_OBJECT_TYPE_FLOAT 1
#define SLL_RUNTIME_OBJECT_TYPE_CHAR 2
#define SLL_RUNTIME_OBJECT_TYPE_STRING 3
#define SLL_RUNTIME_OBJECT_TYPE_ARRAY 4
#define SLL_RUNTIME_OBJECT_TYPE_HANDLE 5
#define SLL_RUNTIME_OBJECT_TYPE_MAP 6
#define SLL_RUNTIME_OBJECT_TYPE_RESERVED0 62
#define SLL_RUNTIME_OBJECT_TYPE_RESERVED1 63
#define SLL_RUNTIME_OBJECT_FLAG_RESERVED0 64
#define SLL_RUNTIME_OBJECT_FLAG_RESERVED1 128
#define SLL_RUNTIME_OBJECT_GET_TYPE(r) ((r)->t&0x3f)

#define SLL_COMPARE_RESULT_BELOW 0
#define SLL_COMPARE_RESULT_EQUAL 1
#define SLL_COMPARE_RESULT_ABOVE 2

#define SLL_END_OF_DATA UINT16_MAX
#define SLL_READ_FROM_INPUT_DATA_STREAM(is) ((is)->rf((is)))
#define SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,bf,sz) ((is)->rbf((is),(bf),(sz)))
#define SLL_INPUT_DATA_STREAM_RESTART_LINE(is,lp) ((is)->rlf((is),(lp)))
#define SLL_GET_INPUT_DATA_STREAM_OFFSET(is) ((is)->_off)
#define SLL_GET_INPUT_DATA_STREAM_LINE_NUMBER(is) ((is)->_lc)
#define SLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is) ((is)->_loff)

#define SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,c) ((os)->wcf((os),(c)))
#define SLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,s) ((os)->wsf((os),(s)))
#define SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,bf,sz) ((os)->wf((os),(bf),(sz)))

#define SLL_UNKNOWN_HANDLE_TYPE 0
#define SLL_HANDLE_FREE UINT64_MAX

#define SLL_JSON_OBJECT_TYPE_ARRAY 0
#define SLL_JSON_OBJECT_TYPE_FALSE 1
#define SLL_JSON_OBJECT_TYPE_FLOAT 2
#define SLL_JSON_OBJECT_TYPE_INTEGER 3
#define SLL_JSON_OBJECT_TYPE_MAP 4
#define SLL_JSON_OBJECT_TYPE_NULL 5
#define SLL_JSON_OBJECT_TYPE_STRING 6
#define SLL_JSON_OBJECT_TYPE_TRUE 7

#define SLL_CLEANUP_ORDER_NORMAL 0
#define SLL_CLEANUP_ORDER_RESERVED0 1

#define SLL_CONSTANT_TYPE_INT 1
#define SLL_CONSTANT_TYPE_FLOAT 2
#define SLL_CONSTANT_TYPE_CHAR 3
#define SLL_CONSTANT_TYPE_STRING 4
#define SLL_CONSTANT_TYPE_ARRAY 5
#define SLL_CONSTANT_TYPE_HANDLE 6
#define SLL_CONSTANT_TYPE_MAP 7
#define SLL_CONSTANT_TYPE_MAP_KEY 8
#define SLL_CONSTANT_TYPE_MAP_VALUE 9
#define SLL_MAX_CONSTANT_TYPE SLL_CONSTANT_TYPE_MAP_VALUE

#define SLL_INTERNAL_FUNCTION_TYPE_DEFAULT 0
#define SLL_INTERNAL_FUNCTION_TYPE_REQUIRED 1

#define SLL_MAX_ASSEMBLY_INSTRUCTION_TYPE UINT8_MAX
#define SLL_MAX_BOOL UINT8_MAX
#define SLL_MAX_CHAR UINT8_MAX
#define SLL_MAX_COMPARE_RESULT UINT8_MAX
#define SLL_MAX_ERROR_TYPE UINT8_MAX
#define SLL_MAX_OBJECT_TYPE UINT8_MAX
#define SLL_MAX_RETURN UINT8_MAX
#define SLL_MAX_RUNTIME_OBJECT_TYPE UINT8_MAX
#define SLL_MAX_FILE_PATH_INDEX UINT16_MAX
#define SLL_MAX_READ_CHAR UINT16_MAX
#define SLL_MAX_VERSION UINT16_MAX
#define SLL_MAX_ALLOCATED_BLOCK_LENGTH UINT32_MAX
#define SLL_MAX_ARG_COUNT UINT32_MAX
#define SLL_MAX_BUFFER_SIZE UINT32_MAX
#define SLL_MAX_COLUMN_NUMBER UINT32_MAX
#define SLL_MAX_EXPORT_TABLE_LENGTH UINT32_MAX
#define SLL_MAX_FILE_OFFSET UINT32_MAX
#define SLL_MAX_FUNCTION_INDEX UINT32_MAX
#define SLL_MAX_HANDLE_TYPE UINT32_MAX
#define SLL_MAX_IDENTIFIER_INDEX UINT32_MAX
#define SLL_MAX_IDENTIFIER_LIST_LENGTH UINT32_MAX
#define SLL_MAX_INSTRUCTION_INDEX UINT32_MAX
#define SLL_MAX_LINE_NUMBER UINT32_MAX
#define SLL_MAX_OBJECT_OFFSET UINT32_MAX
#define SLL_MAX_REF_COUNT UINT32_MAX
#define SLL_MAX_SCOPE UINT32_MAX
#define SLL_MAX_STACK_OFFSET UINT32_MAX
#define SLL_MAX_STATEMENT_COUNT UINT32_MAX
#define SLL_MAX_STRING_CHECKSUM UINT32_MAX
#define SLL_MAX_STRING_INDEX UINT32_MAX
#define SLL_MAX_STRING_LENGTH UINT32_MAX
#define SLL_MAX_VARIABLE_INDEX UINT32_MAX
#define SLL_MAX_ARRAY_LENGTH UINT32_MAX
#define SLL_MAX_ALLOCATION_SIZE UINT64_MAX
#define SLL_MAX_HANDLE (UINT64_MAX-1)
#define SLL_MAX_PAGE_SIZE UINT64_MAX
#define SLL_MAX_TIME UINT64_MAX



#endif

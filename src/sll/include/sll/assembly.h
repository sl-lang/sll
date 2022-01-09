#ifndef __SLL_ASSEMBLY_H__
#define __SLL_ASSEMBLY_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Assembly
 * \group assembly
 * \desc Docs!
 */



#define SLL_ASSEMBLY_INSTRUCTION_TYPE_POP 0
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO 1
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT 2
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP 3
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP 4
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT 5
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE 6
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO 7
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE 8
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO 9
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE 10
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR 11
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT 12
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR 13
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL 14
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
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP 30
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR 31
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP 32
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JB 33
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE 34
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JA 35
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE 36
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JE 37
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE 38
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ 39
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ 40
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE 41
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE 42
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JI 43
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI 44
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JT 45
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT 46
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL 47
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_INC 48
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC 49
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD 50
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB 51
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT 52
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV 53
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV 54
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD 55
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_AND 56
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_OR 57
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR 58
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_INV 59
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_SHR 60
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_SHL 61
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH 62
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY 63
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS 64
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO 65
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE 66
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR 67
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN 68
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO 69
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE 70
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS 71
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST 72
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE 73
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF 74
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL 75
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO 76
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW 77
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL 78
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT 79
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR 80
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR 81
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR 82
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL 83
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP 84
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO 85
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE 86
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_REF 87
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET 88
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT 89
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO 90
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT 91
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR 92
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR 93
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR 94
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL 95
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL 96
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED0 122
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED1 123
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED2 124
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED3 125
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED4 126
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED5 127
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_ANONYMOUS 128
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_INPLACE 128
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_RELATIVE 128

#define SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai) ((ai)->t&0x7f)
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai) ((ai)->t>>7)
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai) ((ai)->t>>7)
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai) ((ai)->t>>7)

#define SLL_INIT_ASSEMBLY_DATA_STRUCT {0,NULL,0,0,{0,NULL},{NULL,0},{NULL,0},{NULL,NULL,0,NULL}}

#define SLL_DEBUG_LINE_DATA_GET_DATA(dbg) ((dbg)->ln&0x3fffffffffffffffull)
#define SLL_DEBUG_LINE_DATA_FLAG_FILE 0x4000000000000000ull
#define SLL_DEBUG_LINE_DATA_FLAG_FUNC 0x8000000000000000ull



/**
 * \flags func
 * \name sll_free_assembly_data
 * \group assembly
 * \desc Docs!
 * \arg sll_assembly_data_t* a_dt
 */
__SLL_EXTERNAL void sll_free_assembly_data(sll_assembly_data_t* a_dt);



/**
 * \flags check_output func
 * \name sll_generate_assembly
 * \group assembly
 * \desc Docs!
 * \arg const sll_compilation_data_t* c_dt
 * \arg sll_assembly_data_t* o
 */
__SLL_EXTERNAL void sll_generate_assembly(const sll_compilation_data_t* c_dt,sll_assembly_data_t* o);



/**
 * \flags func
 * \name sll_print_assembly
 * \group assembly
 * \desc Docs!
 * \arg const sll_assembly_data_t* a_dt
 * \arg sll_file_t* wf
 */
__SLL_EXTERNAL void sll_print_assembly(const sll_assembly_data_t* a_dt,sll_file_t* wf);



#endif

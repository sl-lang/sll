#ifndef __SLL_ASSEMBLY_H__
#define __SLL_ASSEMBLY_H__ 1
#include <sll/_size_types.h>
#include <sll/api/time.h>
#include <sll/common.h>
#include <sll/compression.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/node.h>
#include <sll/string_table.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Assembly
 * \group assembly
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Instructions
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_MAX_ASSEMBLY_INSTRUCTION_TYPE
 * \group assembly
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_MAX_ASSEMBLY_INSTRUCTION_TYPE __SLL_U8_MAX



/**
 * \flags macro var
 * \name SLL_MAX_INSTRUCTION_INDEX
 * \group assembly
 * \desc Docs!
 * \type sll_instruction_index_t
 */
#define SLL_MAX_INSTRUCTION_INDEX __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_MAX_VARIABLE_INDEX
 * \group assembly
 * \desc Docs!
 * \type sll_variable_index_t
 */
#define SLL_MAX_VARIABLE_INDEX __SLL_U32_MAX



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_POP
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_POP 0



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_POP_TWO 1



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT 2



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ROT_POP 3



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DUP 4



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT 5



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED 6



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE 7



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO 8



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE 9



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO 10



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE 11



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR 12



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT 13



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_COMPLEX
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_COMPLEX 14



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR 15



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL 16



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK 17



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD 18



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS 19



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK 20



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO 21



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE 22



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP 23



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO 24



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE 25



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP 26



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE 27



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO 28



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE 29



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO 30



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE 31



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR 32



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP 33



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR 34



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP 35



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JB
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JB 36



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE 37



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JA
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JA 38



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE 39



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JE 40



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE 41



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ 42



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ 43



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE 44



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE 45



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JI
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JI 46



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI 47



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JT 48



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT 49



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL 50



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_INC
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_INC 51



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC 52



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD 53



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB 54



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT 55



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV 56



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV 57



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD 58



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_AND
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_AND 59



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_OR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_OR 60



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR 61



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_INV
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_INV 62



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_SHR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_SHR 63



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_SHL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_SHL 64



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH 65



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY 66



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_DEEP_COPY
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DEEP_COPY 67



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS 68



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO 69



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE 70



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR 71



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN 72



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO 73



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE 74



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS 75



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST 76



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE 77



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF 78



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_NAMEOF
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_NAMEOF 79



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_NAMEOF_TYPE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_NAMEOF_TYPE 80



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL 81



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO 82



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW 83



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL 84



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT 85



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR 86



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR 87



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR 88



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL 89



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP 90



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO 91



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE 92



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ARRAY
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ARRAY 93



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_REF
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_REF 94



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET 95



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT 96



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO 97



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT 98



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR 99



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR 100



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR 101



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL 102



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL 103



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_ID
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_ID 104



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_WAIT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_WAIT 105



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_LOCK
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_LOCK 106



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_SEMAPHORE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_SEMAPHORE 107



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_EQ
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_EQ 108



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_GEQ
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_BARRIER_GEQ 109



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_EXIT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_EXIT 110



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING 111



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING_CHAR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_READ_BLOCKING_CHAR 112



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK 113



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED0
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Internally used as `ASSEMBLY_INSTRUCTION_TYPE_DBG`
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED0 123



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED1
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Internally used as `ASSEMBLY_INSTRUCTION_TYPE_DBG_FUNC`
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED1 124



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED2
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Internally used as `ASSEMBLY_INSTRUCTION_TYPE_FUNC_START`
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED2 125



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED3
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Internally used as `ASSEMBLY_INSTRUCTION_TYPE_LABEL_TARGET`
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED3 126



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED4
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Internally used as `ASSEMBLY_INSTRUCTION_TYPE_NOP`
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED4 127



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_FLAG_ANONYMOUS
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_ANONYMOUS 128



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_FLAG_INPLACE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_INPLACE 128



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_FLAG_RELATIVE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_RELATIVE 128



/**
 * \flags func macro
 * \name SLL_ASSEMBLY_INSTRUCTION_GET_TYPE
 * \group assembly
 * \desc Docs!
 * \arg sll_assembly_instruction_type_t ai
 * \ret sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai) ((ai)->type&0x7f)



/**
 * \flags func macro
 * \name SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS
 * \group assembly
 * \desc Docs!
 * \arg sll_assembly_instruction_type_t ai
 * \ret sll_bool_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai) ((ai)->type>>7)



/**
 * \flags func macro
 * \name SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE
 * \group assembly
 * \desc Docs!
 * \arg sll_assembly_instruction_type_t ai
 * \ret sll_bool_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai) ((ai)->type>>7)



/**
 * \flags func macro
 * \name SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE
 * \group assembly
 * \desc Docs!
 * \arg sll_assembly_instruction_type_t ai
 * \ret sll_bool_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai) ((ai)->type>>7)



/**
 * \flags macro var
 * \name SLL_INIT_ASSEMBLY_DATA_STRUCT
 * \group assembly
 * \desc Docs!
 * \type sll_assembly_data_t
 */
#define SLL_INIT_ASSEMBLY_DATA_STRUCT {0,NULL,0,0,0,{NULL,0},{NULL,0},{NULL,0},{NULL,NULL,NULL,0}}



/**
 * \flags func macro
 * \name SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT
 * \group assembly
 * \desc Docs!
 * \arg const sll_assembly_function_t* f
 * \ret sll_arg_count_t
 */
#define SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(f) ((f)->arg_count>>1)



/**
 * \flags func macro
 * \name SLL_ASSEMBLY_FUNCTION_IS_VAR_ARG
 * \group assembly
 * \desc Docs!
 * \arg const sll_assembly_function_t* f
 * \ret sll_bool_t
 */
#define SLL_ASSEMBLY_FUNCTION_IS_VAR_ARG(f) ((f)->arg_count&1)



/**
 * \flags func macro
 * \name SLL_ASSEMBLY_VARIABLE_GET_INDEX
 * \group assembly
 * \desc Docs!
 * \arg sll_variable_index_t v
 * \ret sll_variable_index_t
 */
#define SLL_ASSEMBLY_VARIABLE_GET_INDEX(v) ((v)>>1)



/**
 * \flags func macro
 * \name SLL_ASSEMBLY_VARIABLE_IS_TLS
 * \group assembly
 * \desc Docs!
 * \arg sll_variable_index_t v
 * \ret sll_bool_t
 */
#define SLL_ASSEMBLY_VARIABLE_IS_TLS(v) ((v)&1)



/**
 * \flags type var
 * \name sll_assembly_instruction_type_t
 * \group assembly
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_assembly_instruction_type_t;



/**
 * \flags type var
 * \name sll_relative_instruction_index_t
 * \group assembly
 * \desc Docs!
 * \type __SLL_S32
 */
typedef __SLL_S32 sll_relative_instruction_index_t;



/**
 * \flags type var
 * \name sll_debug_data_length_t
 * \group assembly
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_debug_data_length_t;



/**
 * \flags type var
 * \name sll_instruction_index_t
 * \group assembly
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_instruction_index_t;



/**
 * \flags type var
 * \name sll_variable_index_t
 * \group assembly
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_variable_index_t;



/**
 * \flags type union
 * \name sll_assembly_instruction_data_jump_target_t
 * \group assembly
 * \desc Docs!
 * \arg sll_instruction_index_t abs
 * \arg sll_relative_instruction_index_t rel
 */
typedef union _SLL_ASSEMBLY_INSTRUCTION_DATA_JUMP_TARGET{
	sll_instruction_index_t abs;
	sll_relative_instruction_index_t rel;
} sll_assembly_instruction_data_jump_target_t;



/**
 * \flags type
 * \name sll_assembly_instruction_data_jump_t
 * \group assembly
 * \desc Docs!
 * \arg sll_assembly_instruction_data_jump_target_t target
 * \arg void* _instruction
 */
typedef struct _SLL_ASSEMBLY_INSTRUCTION_DATA_JUMP{
	sll_assembly_instruction_data_jump_target_t target;
	void* _instruction;
} sll_assembly_instruction_data_jump_t;



/**
 * \flags type
 * \name sll_assembly_instruction_data_var_access_t
 * \group assembly
 * \desc Docs!
 * \arg sll_variable_index_t variable
 * \arg sll_arg_count_t arg_count
 */
typedef struct _SLL_ASSEMBLY_INSTRUCTION_DATA_VAR_ACCESS{
	sll_variable_index_t variable;
	sll_arg_count_t arg_count;
} sll_assembly_instruction_data_var_access_t;



/**
 * \flags type union
 * \name sll_assembly_instruction_data_t
 * \group assembly
 * \desc Docs!
 * \arg sll_integer_t int_
 * \arg sll_compressed_integer_t compressed_int
 * \arg sll_float_t float_
 * \arg sll_char_t char_
 * \arg sll_complex_t complex_
 * \arg sll_string_index_t string_index
 * \arg sll_variable_index_t variable
 * \arg sll_assembly_instruction_data_jump_t jump
 * \arg sll_arg_count_t arg_count
 * \arg sll_array_length_t array_length
 * \arg sll_map_length_t map_length
 * \arg sll_object_type_t type
 * \arg sll_assembly_instruction_data_var_access_t variable_access
 * \arg sll_stack_offset_t stack_offset
 * \arg void* _next_instruction
 */
typedef union _SLL_ASSEMBLY_INSTRUCTION_DATA{
	sll_integer_t int_;
	sll_compressed_integer_t compressed_int;
	sll_float_t float_;
	sll_char_t char_;
	sll_complex_t complex_;
	sll_string_index_t string_index;
	sll_variable_index_t variable;
	sll_assembly_instruction_data_jump_t jump;
	sll_arg_count_t arg_count;
	sll_array_length_t array_length;
	sll_map_length_t map_length;
	sll_object_type_t type;
	sll_assembly_instruction_data_var_access_t variable_access;
	sll_stack_offset_t stack_offset;
	void* _next_instruction;
} sll_assembly_instruction_data_t;



/**
 * \flags type
 * \name sll_assembly_instruction_t
 * \group assembly
 * \desc Docs!
 * \arg sll_assembly_instruction_type_t type
 * \arg sll_assembly_instruction_data_t data
 */
typedef struct _SLL_ASSEMBLY_INSTRUCTION{
	sll_assembly_instruction_type_t type;
	sll_assembly_instruction_data_t data;
} sll_assembly_instruction_t;



/**
 * \flags type
 * \name sll_assembly_function_t
 * \group assembly
 * \desc Docs!
 * \arg sll_instruction_index_t instruction_index
 * \arg sll_arg_count_t arg_count
 * \arg sll_string_index_t name_string_index
 */
typedef struct _SLL_ASSEMBLY_FUNCTION{
	sll_instruction_index_t instruction_index;
	sll_arg_count_t arg_count;
	sll_string_index_t name_string_index;
} sll_assembly_function_t;



/**
 * \flags type
 * \name sll_assembly_function_table_t
 * \group assembly
 * \desc Docs!
 * \arg sll_assembly_function_t* data
 * \arg sll_function_index_t length
 */
typedef struct _SLL_ASSEMBLY_FUNCTION_TABLE{
	sll_assembly_function_t* data;
	sll_function_index_t length;
} sll_assembly_function_table_t;



/**
 * \flags type
 * \name sll_debug_line_data_t
 * \group assembly
 * \desc Docs!
 * \arg sll_instruction_index_t delta_instruction_index
 * \arg sll_file_offset_t line
 */
typedef struct _SLL_DEBUG_LINE_DATA{
	sll_instruction_index_t delta_instruction_index;
	sll_file_offset_t line;
} sll_debug_line_data_t;



/**
 * \flags type
 * \name sll_debug_data_t
 * \group assembly
 * \desc Docs!
 * \arg sll_debug_line_data_t* data
 * \arg sll_debug_data_length_t length
 */
typedef struct _SLL_DEBUG_DATA{
	sll_debug_line_data_t* data;
	sll_debug_data_length_t length;
} sll_debug_data_t;



/**
 * \flags type
 * \name sll_assembly_stack_data_t
 * \group assembly
 * \desc Docs!
 * \arg void* start
 * \arg void* end
 * \arg sll_assembly_instruction_t* next_instruction
 * \arg sll_size_t count
 */
typedef struct _SLL_ASSEMBLY_STACK_DATA{
	void* start;
	void* end;
	sll_assembly_instruction_t* next_instruction;
	sll_size_t count;
} sll_assembly_stack_data_t;



/**
 * \flags type
 * \name sll_assembly_data_t
 * \group assembly
 * \desc Docs!
 * \arg sll_time_t time
 * \arg sll_assembly_instruction_t* first_instruction
 * \arg sll_instruction_index_t instruction_count
 * \arg sll_variable_index_t variable_count
 * \arg sll_variable_index_t tls_variable_count
 * \arg sll_assembly_function_table_t function_table
 * \arg sll_string_table_t string_table
 * \arg sll_debug_data_t debug_data
 * \arg sll_assembly_stack_data_t _instruction_stack
 */
typedef struct _SLL_ASSEMBLY_DATA{
	sll_time_t time;
	sll_assembly_instruction_t* first_instruction;
	sll_instruction_index_t instruction_count;
	sll_variable_index_t variable_count;
	sll_variable_index_t tls_variable_count;
	sll_assembly_function_table_t function_table;
	sll_string_table_t string_table;
	sll_debug_data_t debug_data;
	sll_assembly_stack_data_t _instruction_stack;
} sll_assembly_data_t;



/**
 * \flags func
 * \name sll_free_assembly_data
 * \group assembly
 * \desc Docs!
 * \arg sll_assembly_data_t* assembly_data
 */
__SLL_EXTERNAL void sll_free_assembly_data(sll_assembly_data_t* assembly_data);



/**
 * \flags func
 * \name sll_generate_assembly
 * \group assembly
 * \desc Docs!
 * \arg const sll_source_file_t* source_file
 * \arg sll_assembly_data_t* out
 */
__SLL_EXTERNAL void sll_generate_assembly(const sll_source_file_t* source_file,sll_assembly_data_t* out);



/**
 * \flags func
 * \name sll_print_assembly
 * \group assembly
 * \desc Docs!
 * \arg const sll_assembly_data_t* assembly_data
 * \arg sll_file_t* out
 */
__SLL_EXTERNAL void sll_print_assembly(const sll_assembly_data_t* assembly_data,sll_file_t* out);



#endif

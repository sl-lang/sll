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
/**
 * \flags subgroup
 * \name Instructions
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 */



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
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_MINUS_ONE 6



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ZERO 7



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_ONE 8



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_TWO 9



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_THREE 10



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FOUR 11



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT 12



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR 13



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL 14



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK 15



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD 16



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS 17



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK 18



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ZERO 19



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK_ONE 20



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP 21



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP_ZERO 22



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE 23



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP 24



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE 25



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO 26



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE 27



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO 28



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE 29



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR 30



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP 31



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR 32



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP 33



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JB
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JB 34



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE 35



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JA
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JA 36



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE 37



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JE 38



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE 39



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ 40



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ 41



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE 42



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE 43



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JI
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JI 44



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI 45



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_JT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_JT 46



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT 47



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_BOOL 48



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_INC
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_INC 49



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC 50



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD 51



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB 52



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT 53



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV 54



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV 55



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD 56



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_AND
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_AND 57



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_OR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_OR 58



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR 59



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_INV
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_INV 60



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_SHR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_SHR 61



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_SHL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_SHL 62



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LENGTH 63



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_COPY 64



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS 65



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_TWO 66



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_THREE 67



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR 68



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN 69



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO 70



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE 71



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS 72



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST 73



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE 74



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_TYPEOF 75



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL 76



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL_ZERO 77



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW 78



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW_DECL 79



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT 80



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR 81



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR 82



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR 83



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL 84



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP 85



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO 86



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE 87



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ARRAY
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ARRAY 88



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_REF
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_REF 89



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET 90



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT 91



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_ZERO 92



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT 93



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR 94



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR 95



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR 96



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL 97



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL 98



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_WAIT
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_WAIT 99



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_ID
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_ID 100



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_LOCK
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_THREAD_LOCK 101



/**
 * \flags macro var
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK
 * \group assembly
 * \subgroup assembly-instruction
 * \desc Docs!
 * \type sll_assembly_instruction_type_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK 102



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
 * \name SLL_ASSEMBLY_INSTRUCTION_TYPE_RESERVED5
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
#define SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai) ((ai)->t&0x7f)



/**
 * \flags func macro
 * \name SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS
 * \group assembly
 * \desc Docs!
 * \arg sll_assembly_instruction_type_t ai
 * \ret sll_bool_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_ANONYMOUS(ai) ((ai)->t>>7)



/**
 * \flags func macro
 * \name SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE
 * \group assembly
 * \desc Docs!
 * \arg sll_assembly_instruction_type_t ai
 * \ret sll_bool_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai) ((ai)->t>>7)



/**
 * \flags func macro
 * \name SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE
 * \group assembly
 * \desc Docs!
 * \arg sll_assembly_instruction_type_t ai
 * \ret sll_bool_t
 */
#define SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai) ((ai)->t>>7)



/**
 * \flags macro var
 * \name SLL_INIT_ASSEMBLY_DATA_STRUCT
 * \group assembly
 * \desc Docs!
 * \type sll_assembly_data_t
 */
#define SLL_INIT_ASSEMBLY_DATA_STRUCT {0,NULL,0,0,{0,NULL},{NULL,0},{NULL,0},{NULL,NULL,NULL,0}}



/**
 * \flags func macro
 * \name SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT
 * \group assembly
 * \desc Docs!
 * \arg const sll_assembly_function_t* f
 * \ret sll_arg_count_t
 */
#define SLL_ASSEMBLY_FUNCTION_GET_ARGUMENT_COUNT(f) ((f)->ac>>1)



/**
 * \flags func macro
 * \name SLL_ASSEMBLY_FUNCTION_IS_VAR_ARG
 * \group assembly
 * \desc Docs!
 * \arg const sll_assembly_function_t* f
 * \ret sll_bool_t
 */
#define SLL_ASSEMBLY_FUNCTION_IS_VAR_ARG(f) ((f)->ac&1)



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
 * \arg const sll_source_file_t* sf
 * \arg sll_assembly_data_t* o
 */
__SLL_EXTERNAL void sll_generate_assembly(const sll_source_file_t* sf,sll_assembly_data_t* o);



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

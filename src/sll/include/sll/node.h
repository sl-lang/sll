#ifndef __SLL_NODE_H__
#define __SLL_NODE_H__ 1
#include <sll/api/hash.h>
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Node
 * \group node
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Types
 * \group node
 * \subgroup node-type
 * \desc Docs!
 */
/**
 * \flags subgroup
 * \name Utilites
 * \group node
 * \subgroup node-util
 * \desc Docs!
 */



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_INT
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_INT 0



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_FLOAT
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_FLOAT 1



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_CHAR
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_CHAR 2



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_COMPLEX
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_COMPLEX 3



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_STRING
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_STRING 4



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_ARRAY
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_ARRAY 5



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_MAP
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_MAP 6



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_IDENTIFIER
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_IDENTIFIER 7



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_FIELD
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_FIELD 8



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_FUNCTION_ID
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_FUNCTION_ID 9



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_PRINT
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_PRINT 10



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_AND
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_AND 11



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_OR
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_OR 12



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_NOT
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_NOT 13



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_BOOL
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_BOOL 14



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_ASSIGN
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_ASSIGN 15



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_FUNC
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_FUNC 16



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_INTERNAL_FUNC
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_INTERNAL_FUNC 17



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_INTERNAL_FUNC_LOAD
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_INTERNAL_FUNC_LOAD 18



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_INLINE_FUNC
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_INLINE_FUNC 19



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_CALL
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_CALL 20



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_CALL_ARRAY
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_CALL_ARRAY 21



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_IF
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_IF 22



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_INLINE_IF
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_INLINE_IF 23



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_SWITCH
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_SWITCH 24



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_FOR
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_FOR 25



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_WHILE
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_WHILE 26



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_LOOP
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_LOOP 27



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_INC
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_INC 28



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_DEC
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_DEC 29



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_ADD
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_ADD 30



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_SUB
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_SUB 31



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_MULT
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_MULT 32



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_DIV
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_DIV 33



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_FLOOR_DIV
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_FLOOR_DIV 34



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_MOD
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_MOD 35



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_BIT_AND
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_BIT_AND 36



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_BIT_OR
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_BIT_OR 37



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_BIT_XOR
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_BIT_XOR 38



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_BIT_NOT
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_BIT_NOT 39



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_BIT_RSHIFT
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_BIT_RSHIFT 40



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_BIT_LSHIFT
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_BIT_LSHIFT 41



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_LESS
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_LESS 42



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_LESS_EQUAL
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_LESS_EQUAL 43



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_EQUAL
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_EQUAL 44



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_NOT_EQUAL
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_NOT_EQUAL 45



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_MORE
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_MORE 46



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_MORE_EQUAL
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_MORE_EQUAL 47



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_STRICT_EQUAL
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_STRICT_EQUAL 48



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_STRICT_NOT_EQUAL
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_STRICT_NOT_EQUAL 49



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_LENGTH
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_LENGTH 50



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_ACCESS
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_ACCESS 51



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_DEEP_COPY
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_DEEP_COPY 52



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_VAR_ACCESS
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_VAR_ACCESS 53



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_HAS
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_HAS 54



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_CAST
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_CAST 55



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_TYPEOF
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_TYPEOF 56



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_NAMEOF
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_NAMEOF 57



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_NAMEOF_TYPE
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_NAMEOF_TYPE 58



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_DECL
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_DECL 59



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_NEW
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_NEW 60



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_FOR_ARRAY
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_FOR_ARRAY 61



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_WHILE_ARRAY
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_WHILE_ARRAY 62



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_FOR_MAP
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_FOR_MAP 63



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_WHILE_MAP
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_WHILE_MAP 64



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_BREAK
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_BREAK 65



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_CONTINUE
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_CONTINUE 66



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_REF
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_REF 67



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_RETURN
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_RETURN 68



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_COMMA
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_COMMA 69



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_OPERATION_LIST
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_OPERATION_LIST 70



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_THREAD_ID
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_THREAD_ID 71



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_THREAD_WAIT
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_THREAD_WAIT 72



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_THREAD_LOCK
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_THREAD_LOCK 73



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_THREAD_SEMAPHORE
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_THREAD_SEMAPHORE 74



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_THREAD_BARRIER_EQ
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_THREAD_BARRIER_EQ 75



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_THREAD_BARRIER_GEQ
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_THREAD_BARRIER_GEQ 76



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_READ_BLOCKING
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_READ_BLOCKING 77



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_READ_BLOCKING_CHAR
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_READ_BLOCKING_CHAR 78



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_DBG
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_DBG 79



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_CHANGE_STACK
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_CHANGE_STACK 80



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_RESERVED0
 * \group node
 * \subgroup node-type
 * \desc Internally used as `NODE_TYPE_UNKNOWN`
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_RESERVED0 254



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_NOP
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_NOP 255



/**
 * \flags func macro
 * \name SLL_IS_OBJECT_TYPE_NOT_TYPE
 * \group node
 * \desc Docs!
 * \arg const sll_node_t* o
 * \ret sll_bool_t
 */
#define SLL_IS_OBJECT_TYPE_NOT_TYPE(o) ((o)->t>SLL_NODE_TYPE_FUNCTION_ID)



/**
 * \flags func macro
 * \name SLL_IS_OBJECT_TYPE_IF
 * \group node
 * \desc Docs!
 * \arg const sll_node_t* o
 * \ret sll_bool_t
 */
#define SLL_IS_OBJECT_TYPE_IF(o) ((o)->t>=SLL_NODE_TYPE_IF&&(o)->t<=SLL_NODE_TYPE_SWITCH)



/**
 * \flags macro var
 * \name SLL_INIT_COMPILATION_DATA_STRUCT
 * \group node
 * \desc Docs!
 * \type sll_compilation_data_t
 */
#define SLL_INIT_COMPILATION_DATA_STRUCT {NULL,0}



/**
 * \flags func macro
 * \name SLL_FUNCTION_GET_ARGUMENT_COUNT
 * \group node
 * \desc Docs!
 * \arg const sll_function_t* f
 * \ret sll_arg_count_t
 */
#define SLL_FUNCTION_GET_ARGUMENT_COUNT(f) ((f)->al>>1)



/**
 * \flags func macro
 * \name SLL_FUNCTION_GET_ARGUMENT_COUNT_RAW
 * \group node
 * \desc Docs!
 * \arg sll_arg_count_t al
 * \ret sll_arg_count_t
 */
#define SLL_FUNCTION_GET_ARGUMENT_COUNT_RAW(al) ((al)>>1)



/**
 * \flags func macro
 * \name SLL_FUNCTION_IS_VAR_ARG
 * \group node
 * \desc Docs!
 * \arg const sll_function_t* f
 * \ret sll_bool_t
 */
#define SLL_FUNCTION_IS_VAR_ARG(f) ((f)->al&1)



/**
 * \flags func
 * \name sll_free_compilation_data
 * \group node
 * \desc Docs!
 * \arg sll_compilation_data_t* c_dt
 */
__SLL_EXTERNAL void sll_free_compilation_data(sll_compilation_data_t* c_dt);



/**
 * \flags func
 * \name sll_free_source_file
 * \group node
 * \desc Docs!
 * \arg sll_source_file_t* sf
 */
__SLL_EXTERNAL void sll_free_source_file(sll_source_file_t* sf);



/**
 * \flags check_output func
 * \name sll_get_node_size
 * \group node
 * \subgroup node-util
 * \desc Docs!
 * \arg const sll_node_t* o
 * \ret sll_node_offset_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_node_offset_t sll_get_node_size(const sll_node_t* o);



/**
 * \flags func
 * \name sll_init_compilation_data
 * \group node
 * \desc Docs!
 * \arg const sll_char_t* fp
 * \arg sll_compilation_data_t* o
 */
__SLL_EXTERNAL void sll_init_compilation_data(const sll_char_t* fp,sll_compilation_data_t* o);



/**
 * \flags func
 * \name sll_optimize_metadata
 * \group node
 * \desc Docs!
 * \arg sll_compilation_data_t* c_dt
 */
__SLL_EXTERNAL void sll_optimize_metadata(sll_compilation_data_t* c_dt);



/**
 * \flags check_output func
 * \name sll_parse_nodes
 * \group node
 * \desc Docs!
 * \arg sll_file_t* rf
 * \arg sll_compilation_data_t* c_dt
 * \arg sll_internal_function_table_t* i_ft
 * \arg sll_import_resolver_t il
 */
__SLL_EXTERNAL void sll_parse_nodes(sll_file_t* rf,sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_resolver_t il);



/**
 * \flags func
 * \name sll_print_node
 * \group node
 * \subgroup node-util
 * \desc Docs!
 * \arg const sll_source_file_t* sf
 * \arg const sll_internal_function_table_t* i_ft
 * \arg const sll_node_t* o
 * \arg sll_file_t* wf
 */
__SLL_EXTERNAL void sll_print_node(const sll_source_file_t* sf,const sll_internal_function_table_t* i_ft,const sll_node_t* o,sll_file_t* wf);



/**
 * \flags func
 * \name sll_remove_debug_data
 * \group node
 * \desc Docs!
 * \arg sll_compilation_data_t* c_dt
 */
__SLL_EXTERNAL void sll_remove_debug_data(sll_compilation_data_t* c_dt);



/**
 * \flags func
 * \name sll_remove_debug_names
 * \group node
 * \desc Docs!
 * \arg sll_compilation_data_t* c_dt
 */
__SLL_EXTERNAL void sll_remove_debug_names(sll_compilation_data_t* c_dt);



/**
 * \flags func
 * \name sll_remove_node_padding
 * \group node
 * \desc Docs!
 * \arg sll_compilation_data_t* c_dt
 */
__SLL_EXTERNAL void sll_remove_node_padding(sll_compilation_data_t* c_dt);



/**
 * \flags check_output func
 * \name sll_skip_node
 * \group node
 * \subgroup node-util
 * \desc Docs!
 * \arg sll_node_t* o
 * \ret sll_node_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_node_t* sll_skip_node(sll_node_t* o);



/**
 * \flags check_output func
 * \name sll_skip_node_const
 * \group node
 * \subgroup node-util
 * \desc Docs!
 * \arg const sll_node_t* o
 * \ret const sll_node_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_node_t* sll_skip_node_const(const sll_node_t* o);



/**
 * \flags func
 * \name sll_unify_compilation_data
 * \group node
 * \desc Docs!
 * \arg const sll_compilation_data_t* c_dt
 * \arg sll_source_file_t* o
 */
__SLL_EXTERNAL void sll_unify_compilation_data(const sll_compilation_data_t* c_dt,sll_source_file_t* o);



#endif

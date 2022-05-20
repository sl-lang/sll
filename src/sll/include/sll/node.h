#ifndef __SLL_NODE_H__
#define __SLL_NODE_H__ 1
#include <sll/_size_types.h>
#include <sll/api/hash.h>
#include <sll/api/time.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/identifier.h>
#include <sll/ift.h>
#include <sll/string.h>
#include <sll/string_table.h>
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
 * \name SLL_NODE_TYPE_THREAD_EXIT
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_THREAD_EXIT 77



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_READ_BLOCKING
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_READ_BLOCKING 78



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_READ_BLOCKING_CHAR
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_READ_BLOCKING_CHAR 79



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_DBG
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_DBG 80



/**
 * \flags macro var
 * \name SLL_NODE_TYPE_CHANGE_STACK
 * \group node
 * \subgroup node-type
 * \desc Docs!
 * \type sll_node_type_t
 */
#define SLL_NODE_TYPE_CHANGE_STACK 81



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
 * \flags type var
 * \name sll_node_type_t
 * \group node
 * \desc Docs!
 * \type __SLL_U8
 */
typedef __SLL_U8 sll_node_type_t;



/**
 * \flags type var
 * \name sll_export_table_length_t
 * \group node
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_export_table_length_t;



/**
 * \flags type var
 * \name sll_import_index_t
 * \group node
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_import_index_t;



/**
 * \flags type var
 * \name sll_node_offset_t
 * \group node
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_node_offset_t;



/**
 * \flags type var
 * \name sll_source_file_index_t
 * \group node
 * \desc Docs!
 * \type __SLL_U32
 */
typedef __SLL_U32 sll_source_file_index_t;



/**
 * \flags type
 * \name sll_function_node_data_t
 * \group node
 * \desc Docs!
 * \arg sll_arg_count_t ac
 * \arg sll_function_index_t id
 * \arg sll_scope_t sc
 */
typedef struct _SLL_FUNCTION_NODE_DATA{
	sll_arg_count_t ac;
	sll_function_index_t id;
	sll_scope_t sc;
} sll_function_node_data_t;



/**
 * \flags type
 * \name sll_loop_node_data_t
 * \group node
 * \desc Docs!
 * \arg sll_arg_count_t ac
 * \arg sll_scope_t sc
 */
typedef struct _SLL_LOOP_NODE_DATA{
	sll_arg_count_t ac;
	sll_scope_t sc;
} sll_loop_node_data_t;



/**
 * \flags type
 * \name sll_decl_node_data_t
 * \group node
 * \desc Docs!
 * \arg sll_arg_count_t ac
 * \arg sll_string_index_t nm
 */
typedef struct _SLL_DECL_NODE_DATA{
	sll_arg_count_t ac;
	sll_string_index_t nm;
} sll_decl_node_data_t;



/**
 * \flags type union
 * \name sll_node_data_t
 * \group node
 * \desc Docs!
 * \arg sll_integer_t i
 * \arg sll_float_t f
 * \arg sll_char_t c
 * \arg sll_complex_t d
 * \arg sll_string_index_t s
 * \arg sll_array_length_t al
 * \arg sll_map_length_t ml
 * \arg sll_identifier_index_t id
 * \arg sll_function_node_data_t fn
 * \arg sll_loop_node_data_t l
 * \arg sll_decl_node_data_t dc
 * \arg sll_arg_count_t ac
 * \arg sll_function_index_t fn_id
 * \arg void* _p
 */
typedef union _SLL_NODE_DATA{
	sll_integer_t i;
	sll_float_t f;
	sll_char_t c;
	sll_complex_t d;
	sll_string_index_t s;
	sll_array_length_t al;
	sll_map_length_t ml;
	sll_identifier_index_t id;
	sll_function_node_data_t fn;
	sll_loop_node_data_t l;
	sll_decl_node_data_t dc;
	sll_arg_count_t ac;
	sll_function_index_t fn_id;
	void* _p;
} sll_node_data_t;



/**
 * \flags type
 * \name sll_node_t
 * \group node
 * \desc Docs!
 * \arg sll_node_type_t t
 * \arg sll_node_data_t dt
 */
typedef struct _SLL_NODE{
	sll_node_type_t t;
	sll_node_data_t dt;
} sll_node_t;



/**
 * \flags type
 * \name sll_export_table_t
 * \group node
 * \desc Docs!
 * \arg sll_identifier_index_t* dt
 * \arg sll_export_table_length_t l
 */
typedef struct _SLL_EXPORT_TABLE{
	sll_identifier_index_t* dt;
	sll_export_table_length_t l;
} sll_export_table_t;



/**
 * \flags type
 * \name sll_function_t
 * \group node
 * \desc Docs!
 * \arg sll_node_offset_t off
 * \arg sll_string_index_t nm
 * \arg sll_string_index_t desc
 * \arg sll_arg_count_t al
 * \arg sll_identifier_index_t[] a
 */
typedef struct _SLL_FUNCTION{
	sll_node_offset_t off;
	sll_string_index_t nm;
	sll_string_index_t desc;
	sll_arg_count_t al;
	sll_identifier_index_t a[];
} sll_function_t;



/**
 * \flags type
 * \name sll_function_table_t
 * \group node
 * \desc Docs!
 * \arg sll_function_t** dt
 * \arg sll_function_index_t l
 */
typedef struct _SLL_FUNCTION_TABLE{
	sll_function_t** dt;
	sll_function_index_t l;
} sll_function_table_t;



/**
 * \flags type
 * \name sll_import_file_t
 * \group node
 * \desc Docs!
 * \arg sll_source_file_index_t sfi
 * \arg sll_identifier_list_length_t l
 * \arg sll_identifier_index_t[] dt
 */
typedef struct _SLL_IMPORT_FILE{
	sll_source_file_index_t sfi;
	sll_identifier_list_length_t l;
	sll_identifier_index_t dt[];
} sll_import_file_t;



/**
 * \flags type
 * \name sll_import_table_t
 * \group node
 * \desc Docs!
 * \arg sll_import_file_t** dt
 * \arg sll_import_index_t l
 */
typedef struct _SLL_IMPORT_TABLE{
	sll_import_file_t** dt;
	sll_import_index_t l;
} sll_import_table_t;



/**
 * \flags type
 * \name sll_node_stack_t
 * \group node
 * \desc Docs!
 * \arg void* s
 * \arg void* e
 * \arg sll_node_t* p
 * \arg sll_node_offset_t off
 * \arg sll_size_t c
 */
typedef struct _SLL_NODE_STACK{
	void* s;
	void* e;
	sll_node_t* p;
	sll_node_offset_t off;
	sll_size_t c;
} sll_node_stack_t;



/**
 * \flags type
 * \name sll_source_file_t
 * \group node
 * \desc Docs!
 * \arg sll_time_t tm
 * \arg sll_file_offset_t sz
 * \arg sll_sha256_data_t h
 * \arg sll_node_t* dt
 * \arg sll_identifier_table_t idt
 * \arg sll_export_table_t et
 * \arg sll_function_table_t ft
 * \arg sll_string_table_t st
 * \arg sll_import_table_t it
 * \arg sll_string_index_t fp_nm
 * \arg sll_node_stack_t _s
 * \arg sll_scope_t _n_sc_id
 */
typedef struct _SLL_SOURCE_FILE{
	sll_time_t tm;
	sll_file_offset_t sz;
	sll_sha256_data_t h;
	sll_node_t* dt;
	sll_identifier_table_t idt;
	sll_export_table_t et;
	sll_function_table_t ft;
	sll_string_table_t st;
	sll_import_table_t it;
	sll_string_index_t fp_nm;
	sll_node_stack_t _s;
	sll_scope_t _n_sc_id;
} sll_source_file_t;



/**
 * \flags type
 * \name sll_compilation_data_t
 * \group node
 * \desc Docs!
 * \arg sll_source_file_t** dt
 * \arg sll_source_file_index_t l
 */
typedef struct _SLL_COMPILATION_DATA{
	sll_source_file_t** dt;
	sll_source_file_index_t l;
} sll_compilation_data_t;



/**
 * \flags func type
 * \name sll_import_resolver_t
 * \group node
 * \desc Docs!
 * \arg const sll_string_t* path
 * \arg sll_compilation_data_t* out
 * \ret sll_bool_t
 */
typedef sll_bool_t (*sll_import_resolver_t)(const sll_string_t* path,sll_compilation_data_t* out);



/**
 * \flags func
 * \name sll_compilation_data_from_source_file
 * \group node
 * \desc Docs!
 * \arg const sll_source_file_t* sf
 * \arg sll_compilation_data_t* out
 */
__SLL_EXTERNAL void sll_compilation_data_from_source_file(const sll_source_file_t* sf,sll_compilation_data_t* out);



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

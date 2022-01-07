#ifndef __SLL_NODE_H__
#define __SLL_NODE_H__ 1
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
 * \name Utilites
 * \group node
 * \subgroup node-util
 * \desc Docs!
 */



#define SLL_NODE_TYPE_CHAR 0
#define SLL_NODE_TYPE_INT 1
#define SLL_NODE_TYPE_FLOAT 2
#define SLL_NODE_TYPE_STRING 3
#define SLL_NODE_TYPE_ARRAY 4
#define SLL_NODE_TYPE_MAP 5
#define SLL_NODE_TYPE_IDENTIFIER 6
#define SLL_NODE_TYPE_FIELD 7
#define SLL_NODE_TYPE_FUNCTION_ID 8
#define SLL_NODE_TYPE_PRINT 9
#define SLL_NODE_TYPE_AND 10
#define SLL_NODE_TYPE_OR 11
#define SLL_NODE_TYPE_NOT 12
#define SLL_NODE_TYPE_BOOL 13
#define SLL_NODE_TYPE_ASSIGN 14
#define SLL_NODE_TYPE_FUNC 15
#define SLL_NODE_TYPE_INTERNAL_FUNC 16
#define SLL_NODE_TYPE_INTERNAL_FUNC_LOAD 17
#define SLL_NODE_TYPE_INLINE_FUNC 18
#define SLL_NODE_TYPE_CALL 19
#define SLL_NODE_TYPE_IF 20
#define SLL_NODE_TYPE_INLINE_IF 21
#define SLL_NODE_TYPE_SWITCH 22
#define SLL_NODE_TYPE_FOR 23
#define SLL_NODE_TYPE_WHILE 24
#define SLL_NODE_TYPE_LOOP 25
#define SLL_NODE_TYPE_INC 26
#define SLL_NODE_TYPE_DEC 27
#define SLL_NODE_TYPE_ADD 28
#define SLL_NODE_TYPE_SUB 29
#define SLL_NODE_TYPE_MULT 30
#define SLL_NODE_TYPE_DIV 31
#define SLL_NODE_TYPE_FLOOR_DIV 32
#define SLL_NODE_TYPE_MOD 33
#define SLL_NODE_TYPE_BIT_AND 34
#define SLL_NODE_TYPE_BIT_OR 35
#define SLL_NODE_TYPE_BIT_XOR 36
#define SLL_NODE_TYPE_BIT_NOT 37
#define SLL_NODE_TYPE_BIT_RSHIFT 38
#define SLL_NODE_TYPE_BIT_LSHIFT 39
#define SLL_NODE_TYPE_LESS 40
#define SLL_NODE_TYPE_LESS_EQUAL 41
#define SLL_NODE_TYPE_EQUAL 42
#define SLL_NODE_TYPE_NOT_EQUAL 43
#define SLL_NODE_TYPE_MORE 44
#define SLL_NODE_TYPE_MORE_EQUAL 45
#define SLL_NODE_TYPE_STRICT_EQUAL 46
#define SLL_NODE_TYPE_STRICT_NOT_EQUAL 47
#define SLL_NODE_TYPE_LENGTH 48
#define SLL_NODE_TYPE_ACCESS 49
#define SLL_NODE_TYPE_VAR_ACCESS 50
#define SLL_NODE_TYPE_HAS 51
#define SLL_NODE_TYPE_CAST 52
#define SLL_NODE_TYPE_TYPEOF 53
#define SLL_NODE_TYPE_DECL 54
#define SLL_NODE_TYPE_DECL_COPY 55
#define SLL_NODE_TYPE_NEW 56
#define SLL_NODE_TYPE_FOR_ARRAY 57
#define SLL_NODE_TYPE_WHILE_ARRAY 58
#define SLL_NODE_TYPE_FOR_MAP 59
#define SLL_NODE_TYPE_WHILE_MAP 60
#define SLL_NODE_TYPE_BREAK 61
#define SLL_NODE_TYPE_CONTINUE 62
#define SLL_NODE_TYPE_REF 63
#define SLL_NODE_TYPE_RETURN 64
#define SLL_NODE_TYPE_COMMA 65
#define SLL_NODE_TYPE_OPERATION_LIST 66
#define SLL_NODE_TYPE_DBG 67
#define SLL_NODE_TYPE_RESERVED0 253
#define SLL_NODE_TYPE_RESERVED1 254
#define SLL_NODE_TYPE_NOP 255

#define SLL_IS_OBJECT_TYPE_NOT_TYPE(o) ((o)->t>SLL_NODE_TYPE_FUNCTION_ID)
#define SLL_IS_OBJECT_TYPE_IF(o) ((o)->t>=SLL_NODE_TYPE_IF&&(o)->t<=SLL_NODE_TYPE_SWITCH)

#define SLL_INIT_COMPILATION_DATA_STRUCT {NULL,0,NULL,{.il=NULL,.ill=0},{NULL,0},{NULL,0},{NULL,0},{NULL,0},{NULL,0},{NULL,NULL,0,NULL,0},0}



/**
 * \flags func
 * \name sll_free_compilation_data
 * \group node
 * \desc Docs!
 * \arg sll_compilation_data_t* c_dt
 */
__SLL_EXTERNAL void sll_free_compilation_data(sll_compilation_data_t* c_dt);



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
 * \arg sll_file_t* rf
 * \arg sll_compilation_data_t* o
 */
__SLL_EXTERNAL void sll_init_compilation_data(const sll_char_t* fp,sll_file_t* rf,sll_compilation_data_t* o);



/**
 * \flags func
 * \name sll_optimize_metadata
 * \group node
 * \desc Docs!
 * \arg sll_compilation_data_t* c_dt
 */
__SLL_EXTERNAL void sll_optimize_metadata(sll_compilation_data_t* c_dt);



/**
 * \flags func
 * \name sll_optimize_node
 * \group node
 * \desc Docs!
 * \arg sll_compilation_data_t* c_dt
 * \arg sll_internal_function_table_t* i_ft
 */
__SLL_EXTERNAL void sll_optimize_node(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft);



/**
 * \flags check_output func
 * \name sll_parse_all_nodes
 * \group node
 * \desc Docs!
 * \arg sll_compilation_data_t* c_dt
 * \arg sll_internal_function_table_t* i_ft
 * \arg sll_import_resolver_t il
 */
__SLL_EXTERNAL void sll_parse_all_nodes(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_resolver_t il);



/**
 * \flags func
 * \name sll_print_node
 * \group node
 * \subgroup node-util
 * \desc Docs!
 * \arg const sll_compilation_data_t* c_dt
 * \arg const sll_internal_function_table_t* i_ft
 * \arg const sll_node_t* o
 * \arg sll_file_t* wf
 */
__SLL_EXTERNAL void sll_print_node(const sll_compilation_data_t* c_dt,const sll_internal_function_table_t* i_ft,const sll_node_t* o,sll_file_t* wf);



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
 * \name sll_remove_node_padding
 * \group node
 * \desc Docs!
 * \arg sll_compilation_data_t* c_dt
 * \arg sll_node_t* o
 */
__SLL_EXTERNAL void sll_remove_node_padding(sll_compilation_data_t* c_dt,sll_node_t* o);



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



#endif

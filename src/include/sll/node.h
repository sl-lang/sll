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



#define SLL_NODE_TYPE_UNKNOWN 0
#define SLL_NODE_TYPE_CHAR 1
#define SLL_NODE_TYPE_INT 2
#define SLL_NODE_TYPE_FLOAT 3
#define SLL_NODE_TYPE_STRING 4
#define SLL_NODE_TYPE_ARRAY 5
#define SLL_NODE_TYPE_MAP 6
#define SLL_NODE_TYPE_IDENTIFIER 7
#define SLL_NODE_TYPE_FUNCTION_ID 8
#define SLL_NODE_TYPE_PRINT 9
#define SLL_NODE_TYPE_AND 10
#define SLL_NODE_TYPE_OR 11
#define SLL_NODE_TYPE_NOT 12
#define SLL_NODE_TYPE_BOOL 13
#define SLL_NODE_TYPE_ASSIGN 14
#define SLL_NODE_TYPE_FUNC 15
#define SLL_NODE_TYPE_INTERNAL_FUNC 16
#define SLL_NODE_TYPE_INLINE_FUNC 17
#define SLL_NODE_TYPE_CALL 18
#define SLL_NODE_TYPE_IF 19
#define SLL_NODE_TYPE_INLINE_IF 20
#define SLL_NODE_TYPE_SWITCH 21
#define SLL_NODE_TYPE_FOR 22
#define SLL_NODE_TYPE_WHILE 23
#define SLL_NODE_TYPE_LOOP 24
#define SLL_NODE_TYPE_ADD 25
#define SLL_NODE_TYPE_SUB 26
#define SLL_NODE_TYPE_MULT 27
#define SLL_NODE_TYPE_DIV 28
#define SLL_NODE_TYPE_FLOOR_DIV 29
#define SLL_NODE_TYPE_MOD 30
#define SLL_NODE_TYPE_BIT_AND 31
#define SLL_NODE_TYPE_BIT_OR 32
#define SLL_NODE_TYPE_BIT_XOR 33
#define SLL_NODE_TYPE_BIT_NOT 34
#define SLL_NODE_TYPE_BIT_RSHIFT 35
#define SLL_NODE_TYPE_BIT_LSHIFT 36
#define SLL_NODE_TYPE_LESS 37
#define SLL_NODE_TYPE_LESS_EQUAL 38
#define SLL_NODE_TYPE_EQUAL 39
#define SLL_NODE_TYPE_NOT_EQUAL 40
#define SLL_NODE_TYPE_MORE 41
#define SLL_NODE_TYPE_MORE_EQUAL 42
#define SLL_NODE_TYPE_STRICT_EQUAL 43
#define SLL_NODE_TYPE_STRICT_NOT_EQUAL 44
#define SLL_NODE_TYPE_LENGTH 45
#define SLL_NODE_TYPE_ACCESS 46
#define SLL_NODE_TYPE_CAST 47
#define SLL_NODE_TYPE_TYPEOF 48
#define SLL_NODE_TYPE_BREAK 49
#define SLL_NODE_TYPE_CONTINUE 50
#define SLL_NODE_TYPE_REF 51
#define SLL_NODE_TYPE_RETURN 52
#define SLL_NODE_TYPE_EXIT 53
#define SLL_NODE_TYPE_COMMA 54
#define SLL_NODE_TYPE_OPERATION_LIST 55
#define SLL_NODE_TYPE_DEBUG_DATA 56
#define SLL_NODE_TYPE_RESERVED0 254
#define SLL_NODE_TYPE_NOP 255

#define SLL_IS_OBJECT_TYPE_NOT_TYPE(o) ((o)->t>SLL_NODE_TYPE_FUNCTION_ID)
#define SLL_IS_OBJECT_TYPE_IF(o) ((o)->t>=SLL_NODE_TYPE_IF&&(o)->t<=SLL_NODE_TYPE_SWITCH)

#define SLL_INIT_COMPILATION_DATA_STRUCT {NULL,0,NULL,{.il=NULL,.ill=0},{NULL,0},{NULL,0},{NULL,0},{NULL,NULL,0,NULL,0},0}



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
 * \name sll_insert_debug_node
 * \group node
 * \desc Docs!
 * \arg sll_compilation_data_t* c_dt
 * \arg sll_file_t* rf
 */
__SLL_EXTERNAL void sll_insert_debug_node(sll_compilation_data_t* c_dt,sll_file_t* rf);



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
 * \arg sll_import_loader_t il
 * \arg sll_error_t* e
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_parse_all_nodes(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e);



/**
 * \flags check_output func
 * \name sll_parse_node
 * \group node
 * \desc Docs!
 * \arg sll_compilation_data_t* c_dt
 * \arg sll_internal_function_table_t* i_ft
 * \arg sll_import_loader_t il
 * \arg sll_error_t* e
 * \arg sll_node_t** o
 * \ret sll_bool_t
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_parse_node(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e,sll_node_t** o);



/**
 * \flags func
 * \name sll_print_node
 * \group node
 * \desc Docs!
 * \arg const sll_compilation_data_t* c_dt
 * \arg const sll_internal_function_table_t* i_ft
 * \arg const sll_node_t* o
 * \arg sll_file_t* wf
 */
__SLL_EXTERNAL void sll_print_node(const sll_compilation_data_t* c_dt,const sll_internal_function_table_t* i_ft,const sll_node_t* o,sll_file_t* wf);



/**
 * \flags func
 * \name sll_remove_node_debug_data
 * \group node
 * \desc Docs!
 * \arg sll_node_t* o
 */
__SLL_EXTERNAL void sll_remove_node_debug_data(sll_node_t* o);



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
 * \desc Docs!
 * \arg sll_node_t* o
 * \ret sll_node_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_node_t* sll_skip_node(sll_node_t* o);



/**
 * \flags check_output func
 * \name sll_skip_node_const
 * \group node
 * \desc Docs!
 * \arg const sll_node_t* o
 * \ret const sll_node_t*
 */
__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_node_t* sll_skip_node_const(const sll_node_t* o);



#endif

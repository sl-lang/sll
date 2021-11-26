#ifndef __SLL_DEBUG_H__
#define __SLL_DEBUG_H__ 1
#include <sll/common.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Debug
 * \group debug
 * \desc Docs!
 */




/**
 * \flags check_output func
 * \name sll_debug_print_assembly
 * \group debug
 * \desc Docs!
 * \arg const sll_assembly_data_t* a_dt
 */
__SLL_EXTERNAL void sll_debug_print_assembly(const sll_assembly_data_t* a_dt);




/**
 * \flags check_output func
 * \name sll_debug_print_node
 * \group debug
 * \desc Docs!
 * \arg const sll_node_t* o
 * \arg const sll_compilation_data_t* c_dt
 */
__SLL_EXTERNAL void sll_debug_print_node(const sll_node_t* o,const sll_compilation_data_t* c_dt);




/**
 * \flags check_output func
 * \name sll_debug_print_object
 * \group debug
 * \desc Docs!
 * \arg const sll_object_t* v
 */
__SLL_EXTERNAL void sll_debug_print_object(const sll_object_t* v);



#endif

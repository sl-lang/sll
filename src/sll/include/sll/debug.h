#ifndef __SLL_DEBUG_H__
#define __SLL_DEBUG_H__ 1
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/gc.h>
#include <sll/node.h>
#include <sll/types.h>
/**
 * \flags group
 * \name Debug
 * \group debug
 * \desc Docs!
 */



/**
 * \flags func
 * \name sll_debug_print_assembly
 * \group debug
 * \desc Docs!
 * \arg const sll_assembly_data_t* assembly_data
 */
__SLL_EXTERNAL void sll_debug_print_assembly(const sll_assembly_data_t* assembly_data);



/**
 * \flags func
 * \name sll_debug_print_node
 * \group debug
 * \desc Docs!
 * \arg sll_node_t node
 * \arg const sll_source_file_t* source_file
 */
__SLL_EXTERNAL void sll_debug_print_node(sll_node_t node,const sll_source_file_t* source_file);



/**
 * \flags func
 * \name sll_debug_print_object
 * \group debug
 * \desc Docs!
 * \arg sll_object_t object
 */
__SLL_EXTERNAL void sll_debug_print_object(sll_object_t object);



#endif

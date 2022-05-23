#ifndef __SLL_GENERATED_OPTIMIZER_H__
#define __SLL_GENERATED_OPTIMIZER_H__ 1
#include <sll/_internal/common.h>
#include <sll/node.h>
#include <../optimizer/merge_print_strings.h>
#include <../optimizer/merge_operation_list.h>



static __SLL_FORCE_INLINE void _execute_optimizer(sll_source_file_t* source_file,sll_node_t* node,sll_arg_count_t arg_count,sll_node_t*const* children,sll_node_t* parent){
	__optimizer_merge_print_strings(source_file,node,arg_count,children,parent);
	__optimizer_merge_operation_list(source_file,node,arg_count,children,parent);
}



#endif

#ifndef __SLL_GENERATED_OPTIMIZER_H__
#define __SLL_GENERATED_OPTIMIZER_H__ 1
#include <sll/_internal/common.h>
#include <sll/node.h>
#include <../optimizer/optimize_print.h>
#include <../optimizer/optimize_assign.h>
#include <../optimizer/optimize_loops.h>
#include <../optimizer/optimize_generator_loops.h>
#include <../optimizer/optimize_function.h>
#include <../optimizer/optimize_comma.h>
#include <../optimizer/optimize_operation_list.h>



static __SLL_FORCE_INLINE void _deinit_optimizer(sll_source_file_t* source_file){
}



static __SLL_FORCE_INLINE void _init_optimizer(sll_source_file_t* source_file){
}



static __SLL_FORCE_INLINE void _pre_visit_optimizer(sll_source_file_t* source_file,sll_node_t* node,sll_node_t* parent){
}



static __SLL_FORCE_INLINE void _visit_optimizer(sll_source_file_t* source_file,sll_node_t* node,sll_node_t*const* children,sll_node_t* parent){
	node=__optimizer_optimize_print(source_file,node,children,parent);
	node=__optimizer_optimize_assign(source_file,node,children,parent);
	node=__optimizer_optimize_loops(source_file,node,children,parent);
	node=__optimizer_optimize_generator_loops(source_file,node,children,parent);
	node=__optimizer_optimize_function(source_file,node,children,parent);
	node=__optimizer_optimize_comma(source_file,node,children,parent);
	__optimizer_optimize_operation_list(source_file,node,children,parent);
}



#endif
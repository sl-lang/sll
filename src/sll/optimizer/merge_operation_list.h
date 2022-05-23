#include <sll/_internal/optimizer.h>
#include <sll/node.h>
#include <sll/types.h>



OPTIMIZER_FUNTION(merge_operation_list){
	if (node->type!=SLL_NODE_TYPE_OPERATION_LIST){
		return;
	}
	for (sll_arg_count_t i=0;i<arg_count;i++){
		sll_node_t* child=*(children+i);
		if (child->type==SLL_NODE_TYPE_OPERATION_LIST){
			child->type=SLL_NODE_TYPE_NOP;
			node->data.arg_count+=child->data.arg_count;
		}
	}
}

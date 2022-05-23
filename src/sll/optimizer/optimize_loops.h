#include <sll/_internal/optimizer.h>
#include <sll/node.h>
#include <sll/types.h>



OPTIMIZER_FUNTION(optimize_loops){
	if (node->type!=SLL_NODE_TYPE_FOR&&node->type!=SLL_NODE_TYPE_WHILE&&node->type!=SLL_NODE_TYPE_LOOP){
		return node;
	}
	sll_arg_count_t ac=node->data.arg_count;
	for (sll_arg_count_t i=(node->type==SLL_NODE_TYPE_LOOP?0:2);i<ac;i++){
		_unneeded_result(&(node->data.arg_count),*(children+i));
	}
	if (node->type!=SLL_NODE_TYPE_LOOP){
		if (!node->data.arg_count){
			node->type=SLL_NODE_TYPE_INT;
			node->data.int_=0;
		}
		else if (node->data.arg_count==1){
			node->type=SLL_NODE_TYPE_OPERATION_LIST;
		}
	}
	return node;
}

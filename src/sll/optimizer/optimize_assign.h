#include <sll/_internal/optimizer.h>
#include <sll/node.h>
#include <sll/types.h>



OPTIMIZER_FUNTION(optimize_assign){
	if (node->type!=SLL_NODE_TYPE_ASSIGN){
		return node;
	}
	sll_arg_count_t ac=node->data.arg_count;
	for (sll_arg_count_t i=2;i<ac;i++){
		_unneeded_result(&(node->data.arg_count),*(children+i));
	}
	if (!node->data.arg_count){
		node->type=SLL_NODE_TYPE_INT;
		node->data.int_=0;
	}
	else if (node->data.arg_count==1){
		node->type=SLL_NODE_TYPE_NOP;
		node=*children;
	}
	else{
		sll_node_t* var=*children;
		if (var->type!=SLL_NODE_TYPE_IDENTIFIER&&var->type!=SLL_NODE_TYPE_ACCESS&&var->type!=SLL_NODE_TYPE_VAR_ACCESS){
			SLL_UNIMPLEMENTED();
		}
	}
	return node;
}

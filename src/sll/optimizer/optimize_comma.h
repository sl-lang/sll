#include <sll/_internal/optimizer.h>
#include <sll/node.h>
#include <sll/types.h>



OPTIMIZER_FUNTION(optimize_comma){
	if (node->type!=SLL_NODE_TYPE_COMMA){
		return node;
	}
	sll_arg_count_t ac=node->data.arg_count;
	sll_node_t* last=*(children+ac-1);
	for (sll_arg_count_t i=0;i<ac-1;i++){
		_unneeded_result(&(node->data.arg_count),*(children+i));
	}
	if (!node->data.arg_count){
		node->type=SLL_NODE_TYPE_INT;
		node->data.int_=0;
	}
	else if (node->data.arg_count==1){
		node->type=SLL_NODE_TYPE_NOP;
		node=last;
	}
	return node;
}

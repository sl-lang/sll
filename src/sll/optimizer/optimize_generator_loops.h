#include <sll/_internal/optimizer.h>
#include <sll/node.h>
#include <sll/types.h>



OPTIMIZER_FUNTION(optimize_generator_loops){
	if (node->type!=SLL_NODE_TYPE_FOR_ARRAY&&node->type!=SLL_NODE_TYPE_WHILE_ARRAY&&node->type!=SLL_NODE_TYPE_FOR_MAP&&node->type!=SLL_NODE_TYPE_WHILE_MAP){
		return node;
	}
	if (!node->data.arg_count){
		if (node->type==SLL_NODE_TYPE_FOR_ARRAY||node->type==SLL_NODE_TYPE_WHILE_ARRAY){
			node->type=SLL_NODE_TYPE_ARRAY;
			node->data.array_length=0;
		}
		else{
			node->type=SLL_NODE_TYPE_MAP;
			node->data.map_length=0;
		}
	}
	else if (node->data.arg_count==1){
		SLL_UNIMPLEMENTED();
	}
	else if (node->data.arg_count==2){
		SLL_UNIMPLEMENTED();
	}
	else{
		sll_arg_count_t ac=node->data.arg_count-(node->type==SLL_NODE_TYPE_FOR_ARRAY||node->type==SLL_NODE_TYPE_WHILE_ARRAY?1:2);
		for (sll_arg_count_t i=2;i<ac;i++){
			_unneeded_result(&(node->data.arg_count),*(children+i));
		}
	}
	return node;
}

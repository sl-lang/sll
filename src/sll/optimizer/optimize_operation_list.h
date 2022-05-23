#include <sll/_internal/optimizer.h>
#include <sll/node.h>
#include <sll/types.h>



OPTIMIZER_FUNTION(optimize_operation_list){
	if (node->type!=SLL_NODE_TYPE_OPERATION_LIST){
		return node;
	}
	sll_arg_count_t ac=node->data.arg_count;
	for (sll_arg_count_t i=0;i<ac;i++){
		sll_node_t* child=*(children+i);
		switch (child->type){
			case SLL_NODE_TYPE_INT:
			case SLL_NODE_TYPE_FLOAT:
			case SLL_NODE_TYPE_CHAR:
			case SLL_NODE_TYPE_COMPLEX:
			case SLL_NODE_TYPE_STRING:
			case SLL_NODE_TYPE_IDENTIFIER:
			case SLL_NODE_TYPE_FIELD:
			case SLL_NODE_TYPE_FUNCTION_ID:
				child->type=SLL_NODE_TYPE_NOP;
				node->data.arg_count--;
				break;
			case SLL_NODE_TYPE_ARRAY:
			case SLL_NODE_TYPE_MAP:
			case SLL_NODE_TYPE_COMMA:
			case SLL_NODE_TYPE_OPERATION_LIST:
				child->type=SLL_NODE_TYPE_NOP;
				node->data.arg_count+=child->data.arg_count;
				break;
		}
	}
	if (!node->data.arg_count){
		node->type=SLL_NODE_TYPE_INT;
		node->data.int_=0;
	}
	return node;
}

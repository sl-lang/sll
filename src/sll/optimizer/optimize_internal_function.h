#include <sll/_internal/optimizer.h>
#include <sll/node.h>
#include <sll/types.h>



OPTIMIZER_FUNTION(optimize_internal_function){
	if (node->type!=SLL_NODE_TYPE_INTERNAL_FUNC){
		return node;
	}
	sll_arg_count_t ac=node->data.arg_count;
	if (ac&&(*children)->type!=SLL_NODE_TYPE_STRING){
		node->type=SLL_NODE_TYPE_OPERATION_LIST;
		return node;
	}
	for (sll_arg_count_t i=1;i<ac;i++){
		_unneeded_result(&(node->data.arg_count),*(children+i));
	}
	return node;
}

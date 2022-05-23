#include <sll/_internal/optimizer.h>
#include <sll/node.h>
#include <sll/types.h>



OPTIMIZER_FUNTION(optimize_function){
	if (node->type!=SLL_NODE_TYPE_FUNC){
		return node;
	}
	sll_arg_count_t ac=node->data.arg_count;
	for (sll_arg_count_t i=0;i<ac;i++){
		_unneeded_result(&(node->data.arg_count),*(children+i));
	}
	return node;
}

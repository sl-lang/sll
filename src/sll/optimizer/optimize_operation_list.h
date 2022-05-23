#include <sll/_internal/optimizer.h>
#include <sll/node.h>
#include <sll/types.h>



OPTIMIZER_FUNTION(optimize_operation_list){
	if (node->type!=SLL_NODE_TYPE_OPERATION_LIST){
		return node;
	}
	sll_arg_count_t ac=node->data.arg_count;
	for (sll_arg_count_t i=0;i<ac;i++){
		_unneeded_result(&(node->data.arg_count),*(children+i));
	}
	if (!node->data.arg_count){
		node->type=SLL_NODE_TYPE_INT;
		node->data.int_=0;
	}
	return node;
}

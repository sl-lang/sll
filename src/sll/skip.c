#include <sll/common.h>
#include <sll/node.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_node_t sll_skip_node(sll_node_t node){
	while (node->type==SLL_NODE_TYPE_NOP||node->type==SLL_NODE_TYPE_DBG||node->type==SLL_NODE_TYPE_CHANGE_STACK){
		node=(node->type==SLL_NODE_TYPE_CHANGE_STACK?node->data._next_node:node+1);
	}
	if (!SLL_NODE_HAS_CHILDREN(node)){
		return node+1;
	}
	sll_arg_count_t arg_count=node->data.arg_count;
	node++;
	while (arg_count){
		arg_count--;
		node=sll_skip_node(node);
	}
	return node;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_node_t sll_skip_node_const(sll_node_t node){
	return sll_skip_node((sll_node_t)node);
}

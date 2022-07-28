#include <sll/common.h>
#include <sll/node.h>
#include <sll/types.h>



static const sll_node_t* _get_node_size(const sll_node_t* node,sll_node_offset_t* size){
	while (node->type==SLL_NODE_TYPE_NOP||node->type==SLL_NODE_TYPE_DBG||node->type==SLL_NODE_TYPE_CHANGE_STACK){
		if (node->type==SLL_NODE_TYPE_CHANGE_STACK){
			node=node->data._next_node;
		}
		else{
			(*size)++;
			node++;
		}
	}
	(*size)++;
	if (!SLL_NODE_HAS_CHILDREN(node)){
		return node+1;
	}
	sll_arg_count_t arg_count=node->data.arg_count;
	node++;
	while (arg_count){
		arg_count--;
		node=_get_node_size(node,size);
	}
	return node;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_node_offset_t sll_get_node_size(const sll_node_t* node){
	sll_node_offset_t out=0;
	_get_node_size(node,&out);
	return out;
}

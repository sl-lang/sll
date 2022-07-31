#include <sll/common.h>
#include <sll/node.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_node_offset_t sll_get_node_size(sll_node_t node){
	sll_node_offset_t out=0;
	sll_node_offset_t stack=1;
	while (stack){
		stack--;
		if (node->type==SLL_NODE_TYPE_CHANGE_STACK){
			node=node->data._next_node;
		}
		if (SLL_NODE_HAS_CHILDREN(node)&&node->type<SLL_NODE_TYPE_DBG){
			stack+=node->data.arg_count;
		}
		else if (node->type==SLL_NODE_TYPE_DBG){
			stack++;
		}
		out++;
		node++;
	}
	return out;
}

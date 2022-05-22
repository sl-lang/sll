#include <sll/common.h>
#include <sll/node.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_node_t* sll_skip_node(sll_node_t* node){
	while (node->type==SLL_NODE_TYPE_NOP||node->type==SLL_NODE_TYPE_DBG||node->type==SLL_NODE_TYPE_CHANGE_STACK){
		node=(node->type==SLL_NODE_TYPE_CHANGE_STACK?node->data._next_node:node+1);
	}
	switch (node->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return node+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=node->data.array_length;
				node++;
				while (l){
					l--;
					node=sll_skip_node(node);
				}
				return node;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=node->data.map_length;
				node++;
				while (l){
					l--;
					node=sll_skip_node(node);
				}
				return node;
			}
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=node->data.function.arg_count;
				node++;
				while (l){
					l--;
					node=sll_skip_node(node);
				}
				return node;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				sll_arg_count_t l=node->data.loop.arg_count;
				node++;
				while (l){
					l--;
					node=sll_skip_node(node);
				}
				return node;
			}
	}
	sll_arg_count_t l=node->data.arg_count;
	node++;
	while (l){
		l--;
		node=sll_skip_node(node);
	}
	return node;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_node_t* sll_skip_node_const(const sll_node_t* node){
	return sll_skip_node((sll_node_t*)node);
}

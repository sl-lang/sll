#ifndef __SLL__INTERNAL_OPTIMIZER_H__
#define __SLL__INTERNAL_OPTIMIZER_H__ 1
#include <sll/_internal/common.h>
#include <sll/_size_types.h>
#include <sll/node.h>
#include <sll/types.h>



#define SKIP_NODE_NOP(node) \
	while ((node)->type==SLL_NODE_TYPE_NOP||(node)->type==SLL_NODE_TYPE_DBG||(node)->type==SLL_NODE_TYPE_CHANGE_STACK){ \
		(node)=((node)->type==SLL_NODE_TYPE_CHANGE_STACK?(node)->data._next_node:(node)+1); \
	}



typedef __SLL_U8 child_level_count_t;



typedef __SLL_U32 child_count_t;



typedef struct __OPTIMIZER_NODE_CHILDREN_DATA{
	sll_node_t* node;
	struct __OPTIMIZER_NODE_CHILDREN_DATA* children;
	child_count_t child_count;
	__SLL_U32 child_type_bitmap[3];
} optimizer_node_children_data_t;



static __SLL_FORCE_INLINE child_count_t* _get_child_count(sll_node_t* node){
	switch (node->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return NULL;
		case SLL_NODE_TYPE_ARRAY:
			return &(node->data.array_length);
		case SLL_NODE_TYPE_MAP:
			return &(node->data.map_length);
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			return &(node->data.function.arg_count);
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			return &(node->data.loop.arg_count);
		default:
			return &(node->data.arg_count);
	}
}



void _delete_node(optimizer_node_children_data_t* data,sll_node_t* parent);



void _expand_node(optimizer_node_children_data_t* data,sll_node_t* parent);



void _set_nop(optimizer_node_children_data_t* data);



#endif

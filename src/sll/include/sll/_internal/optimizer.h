#ifndef __SLL__INTERNAL_OPTIMIZER_H__
#define __SLL__INTERNAL_OPTIMIZER_H__ 1
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



void _delete_node(optimizer_node_children_data_t* data,sll_node_t* parent);



void _expand_node(optimizer_node_children_data_t* data,sll_node_t* parent);



#endif

#include <sll/_internal/optimizer.h>
#include <sll/common.h>
#include <sll/generated/optimizer.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/optimizer.h>
#include <sll/types.h>



static void _delete_deep_children(optimizer_node_children_data_t* children,child_count_t child_count,child_level_count_t levels){
	if (levels){
		for (child_count_t i=0;i<child_count;i++){
			_delete_deep_children((children+i)->children,(children+i)->child_count,levels-1);
		}
		return;
	}
	for (child_count_t i=0;i<child_count;i++){
		sll_deallocate((children+i)->children);
		(children+i)->children=NULL;
		(children+i)->child_count=0;
	}
}



static sll_node_t* _visit_node(sll_source_file_t* source_file,sll_node_t* node,optimizer_node_children_data_t* parent,child_level_count_t child_level_count_required){
	parent->node=node;
	child_count_t child_count;
	switch (node->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			parent->children=NULL;
			parent->child_count=0;
			return node+1;
		case SLL_NODE_TYPE_ARRAY:
			child_count=node->data.array_length;
			break;
		case SLL_NODE_TYPE_MAP:
			child_count=node->data.map_length;
			break;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			child_count=node->data.function.arg_count;
			break;
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			child_count=node->data.loop.arg_count;
			break;
		default:
			child_count=node->data.arg_count;
			break;
	}
	optimizer_node_children_data_t* children=sll_allocate(child_count*sizeof(optimizer_node_children_data_t));
	parent->children=children;
	parent->child_count=child_count;
	sll_node_t* child=node+1;
	child_level_count_required=(child_level_count_required?child_level_count_required-1:0);
	for (child_count_t i=0;i<child_count;i++){
		SKIP_NODE_NOP(child);
		child_level_count_t child_level_count=_optimizer_required_child_levels[(node->type<<OPTIMIZER_DATA_NODE_TYPE_SHIFT)|child->type];
		child=_visit_node(source_file,child,children+i,(child_level_count>child_level_count_required?child_level_count:child_level_count_required));
	}
	_optimizer_execute(parent);
	_delete_deep_children(children,child_count,child_level_count_required);
	return child;
}



__SLL_EXTERNAL void sll_optimize_source_file(sll_source_file_t* source_file,sll_optimization_round_count_t round_count){
	if (!source_file->first_node){
		return;
	}
	optimizer_node_children_data_t tmp;
	while (round_count){
		round_count--;
		_visit_node(source_file,source_file->first_node,&tmp,0);
		sll_deallocate(tmp.children);
	}
}

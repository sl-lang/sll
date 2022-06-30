#include <sll/_internal/optimizer.h>
#include <sll/common.h>
#include <sll/generated/optimizer.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/optimizer.h>
#include <sll/types.h>



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
	child_count_t* child_count_ptr=_get_child_count(node);
	if (!child_count_ptr){
		return node+1;
	}
	child_count_t child_count=*child_count_ptr;
	optimizer_node_children_data_t* children=sll_zero_allocate(child_count*sizeof(optimizer_node_children_data_t));
	parent->children=children;
	parent->child_count=child_count;
	sll_node_t* child=node+1;
	child_level_count_required=(child_level_count_required?child_level_count_required-1:0);
	for (child_count_t i=0;i<child_count;i++){
		SKIP_NODE_NOP(child);
		(children+i)->child_type_bitmap[child->type>>5]|=1u<<(child->type&31);
		child_level_count_t child_level_count=_optimizer_required_child_levels[(node->type<<OPTIMIZER_DATA_NODE_TYPE_SHIFT)|child->type];
		child=_visit_node(source_file,child,children+i,(child_level_count>child_level_count_required?child_level_count:child_level_count_required));
	}
	_optimizer_execute(source_file,parent);
	_delete_deep_children(children,child_count,child_level_count_required);
	return child;
}



static sll_node_t* _delete_node_raw(sll_node_t* node){
	SKIP_NODE_NOP(node);
	child_count_t* child_count_ptr=_get_child_count(node);
	node->type=SLL_NODE_TYPE_NOP;
	node++;
	if (!child_count_ptr){
		return node;
	}
	child_count_t child_count=*child_count_ptr;
	while (child_count){
		child_count--;
		node=_delete_node_raw(node);
	}
	return node;
}



static void _delete_recursive_children(optimizer_node_children_data_t* data){
	if (data->child_count){
		data->node->type=SLL_NODE_TYPE_NOP;
		for (child_count_t i=0;i<data->child_count;i++){
			_delete_recursive_children(data->children+i);
		}
		sll_deallocate(data->children);
	}
	else{
		_delete_node_raw(data->node);
	}
}



static void _delete_recursive_children_optimizer_objects(optimizer_node_children_data_t* data){
	for (child_count_t i=0;i<data->child_count;i++){
		_delete_recursive_children_optimizer_objects(data->children+i);
	}
	sll_deallocate(data->children);
}



void _delete_node(optimizer_node_children_data_t* data,sll_node_t* parent){
	(*_get_child_count(parent))--;
	_delete_recursive_children(data);
	sll_zero_memory(data,sizeof(optimizer_node_children_data_t));
}



void _delete_node_children(optimizer_node_children_data_t* data,child_count_t start,child_count_t end){
	SLL_UNIMPLEMENTED();
}



void _expand_node(optimizer_node_children_data_t* data,sll_node_t* parent){
	child_count_t* child_count_ptr=_get_child_count(data->node);
	if (!child_count_ptr){
		_delete_node(data,parent);
		return;
	}
	data->node->type=SLL_NODE_TYPE_NOP;
	_delete_recursive_children_optimizer_objects(data);
	sll_zero_memory(data,sizeof(optimizer_node_children_data_t));
	(*_get_child_count(parent))+=(*child_count_ptr)-1;
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

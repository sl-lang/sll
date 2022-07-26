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
	if (!SLL_NODE_HAS_CHILDREN(node)){
		return node+1;
	}
	child_count_t child_count=node->data.arg_count;
	optimizer_node_children_data_t* children=sll_zero_allocate(child_count*sizeof(optimizer_node_children_data_t));
	parent->children=children;
	parent->child_count=child_count;
	sll_node_t* child=node+1;
	child_level_count_required=(child_level_count_required?child_level_count_required-1:0);
	for (child_count_t i=0;i<child_count;i++){
		SKIP_NODE_NOP(child);
		parent->child_type_bitmap[child->type>>5]|=1u<<(child->type&31);
		child_level_count_t child_level_count=_optimizer_required_child_levels[(node->type<<OPTIMIZER_DATA_NODE_TYPE_SHIFT)|child->type];
		child=_visit_node(source_file,child,children+i,(child_level_count>child_level_count_required?child_level_count:child_level_count_required));
	}
	_optimizer_execute(source_file,parent);
	_delete_deep_children(children,child_count,child_level_count_required);
	return child;
}



static sll_node_t* _delete_node_raw(sll_node_t* node){
	SKIP_NODE_NOP(node);
	sll_bool_t has_children=SLL_NODE_HAS_CHILDREN(node);
	node->type=SLL_NODE_TYPE_NOP;
	if (!has_children){
		return node+1;
	}
	child_count_t child_count=node->data.arg_count;
	node++;
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
	parent->data.arg_count--;
	_delete_recursive_children(data);
	sll_zero_memory(data,sizeof(optimizer_node_children_data_t));
}



void _expand_node(optimizer_node_children_data_t* data,sll_node_t* parent){
	if (!SLL_NODE_HAS_CHILDREN(data->node)){
		_delete_node(data,parent);
		return;
	}
	parent->data.arg_count+=data->node->data.arg_count-1;
	_set_nop(data);
}



void _set_nop(optimizer_node_children_data_t* data){
	data->node->type=SLL_NODE_TYPE_NOP;
	_delete_recursive_children_optimizer_objects(data);
	sll_zero_memory(data,sizeof(optimizer_node_children_data_t));
}



__SLL_EXTERNAL void sll_optimize_source_file(sll_source_file_t* source_file,sll_optimization_round_count_t round_count){
	if (!source_file->first_node){
		return;
	}
	optimizer_node_children_data_t tmp;
	while (round_count){
		round_count--;
		_visit_node(source_file,source_file->first_node,&tmp,0);
		_delete_recursive_children_optimizer_objects(&tmp);
	}
}

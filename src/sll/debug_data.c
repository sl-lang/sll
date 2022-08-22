#include <sll/common.h>
#include <sll/node.h>
#include <sll/string_table.h>
#include <sll/types.h>



static sll_node_t _remove_debug_data(sll_node_t node){
	while (node->type==SLL_NODE_TYPE_NOP||node->type==SLL_NODE_TYPE_DBG||node->type==SLL_NODE_TYPE_CHANGE_STACK){
		if (node->type==SLL_NODE_TYPE_DBG){
			node->type=SLL_NODE_TYPE_NOP;
		}
		node=(node->type==SLL_NODE_TYPE_CHANGE_STACK?node->data._next_node:node+1);
	}
	if (!SLL_NODE_HAS_CHILDREN(node)){
		return node+1;
	}
	sll_arg_count_t arg_count=node->data.arg_count;
	node++;
	while (arg_count){
		arg_count--;
		node=_remove_debug_data(node);
	}
	return node;
}



static sll_node_t _remove_type_names(sll_node_t node){
	while (node->type==SLL_NODE_TYPE_NOP||node->type==SLL_NODE_TYPE_DBG||node->type==SLL_NODE_TYPE_CHANGE_STACK){
		node=(node->type==SLL_NODE_TYPE_CHANGE_STACK?node->data._next_node:node+1);
	}
	if (!SLL_NODE_HAS_CHILDREN(node)){
		return node+1;
	}
	if (node->type==SLL_NODE_TYPE_DECL){
		node->data.declaration.name_string_index=SLL_MAX_STRING_INDEX;
		node->data.declaration.description_string_index=SLL_MAX_STRING_INDEX;
	}
	sll_arg_count_t arg_count=node->data.arg_count;
	node++;
	while (arg_count){
		arg_count--;
		node=_remove_type_names(node);
	}
	return node;
}



__SLL_EXTERNAL void sll_remove_debug_data(sll_compilation_data_t* compilation_data){
	for (sll_source_file_index_t i=0;i<compilation_data->length;i++){
		sll_node_t node=(*(compilation_data->data+i))->first_node;
		if (node){
			_remove_debug_data(node);
		}
	}
}



__SLL_EXTERNAL void sll_remove_debug_names(sll_compilation_data_t* compilation_data){
	for (sll_source_file_index_t i=0;i<compilation_data->length;i++){
		sll_source_file_t* source_file=*(compilation_data->data+i);
		for (sll_function_index_t j=0;j<source_file->function_table.length;j++){
			(*(source_file->function_table.data+j))->name_string_index=SLL_MAX_STRING_INDEX;
			(*(source_file->function_table.data+j))->description_string_index=SLL_MAX_STRING_INDEX;
		}
		if (source_file->first_node){
			_remove_type_names(source_file->first_node);
		}
	}
}

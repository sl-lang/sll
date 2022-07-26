#include <sll/common.h>
#include <sll/node.h>
#include <sll/string_table.h>
#include <sll/types.h>



static sll_node_t* _remove_debug_data(sll_node_t* node){
	while (node->type==SLL_NODE_TYPE_NOP||node->type==SLL_NODE_TYPE_DBG||node->type==SLL_NODE_TYPE_CHANGE_STACK){
		if (node->type==SLL_NODE_TYPE_DBG){
			node->type=SLL_NODE_TYPE_NOP;
		}
		node=(node->type==SLL_NODE_TYPE_CHANGE_STACK?node->data._next_node:node+1);
	}
	sll_arg_count_t arg_count;
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
			arg_count=node->data.array_length;
			break;
		case SLL_NODE_TYPE_MAP:
			arg_count=node->data.map_length;
			break;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			arg_count=node->data.function.arg_count;
			break;
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			arg_count=node->data.loop.arg_count;
			break;
		default:
			arg_count=node->data.arg_count;
			break;
	}
	node++;
	while (arg_count){
		arg_count--;
		node=_remove_debug_data(node);
	}
	return node;
}



static sll_node_t* _remove_type_names(sll_node_t* node){
	while (node->type==SLL_NODE_TYPE_NOP||node->type==SLL_NODE_TYPE_DBG||node->type==SLL_NODE_TYPE_CHANGE_STACK){
		node=(node->type==SLL_NODE_TYPE_CHANGE_STACK?node->data._next_node:node+1);
	}
	sll_arg_count_t arg_count;
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
			arg_count=node->data.array_length;
			break;
		case SLL_NODE_TYPE_MAP:
			arg_count=node->data.map_length;
			break;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			arg_count=node->data.function.arg_count;
			break;
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			arg_count=node->data.loop.arg_count;
			break;
		case SLL_NODE_TYPE_DECL:
			node->data.declaration.name_string_index=SLL_MAX_STRING_INDEX;
		default:
			arg_count=node->data.arg_count;
			break;
	}
	node++;
	while (arg_count){
		arg_count--;
		node=_remove_type_names(node);
	}
	return node;
}



__SLL_EXTERNAL void sll_remove_debug_data(sll_compilation_data_t* compilation_data){
	for (sll_source_file_index_t i=0;i<compilation_data->length;i++){
		sll_node_t* node=(*(compilation_data->data+i))->first_node;
		if (node){
			_remove_debug_data(node);
		}
	}
}



__SLL_EXTERNAL void sll_remove_debug_names(sll_compilation_data_t* compilation_data){
	for (sll_source_file_index_t i=0;i<compilation_data->length;i++){
		sll_source_file_t* sf=*(compilation_data->data+i);
		for (sll_function_index_t j=0;j<sf->function_table.length;j++){
			(*(sf->function_table.data+j))->name_string_index=SLL_MAX_STRING_INDEX;
			(*(sf->function_table.data+j))->description_string_index=SLL_MAX_STRING_INDEX;
		}
		if (sf->first_node){
			_remove_type_names(sf->first_node);
		}
	}
}

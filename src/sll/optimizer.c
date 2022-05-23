#include <sll/_internal/common.h>
#include <sll/_internal/optimizer.h>
#include <sll/common.h>
#include <sll/generated/optimizer.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/types.h>



static sll_node_t* _visit_node(sll_source_file_t* source_file,sll_node_t* node,sll_node_t* parent){
	SKIP_NODE_NOP(node);
	sll_arg_count_t arg_count=0;
	switch (node->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			break;
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
	_pre_visit_optimizer(source_file,node,parent);
	sll_node_t* current_node=node;
	node++;
	sll_node_t** children=sll_allocate_stack(arg_count*sizeof(sll_node_t*));
	for (sll_arg_count_t i=0;i<arg_count;i++){
		SKIP_NODE_NOP(node);
		*(children+i)=node;
		node=_visit_node(source_file,node,current_node);
	}
	_visit_optimizer(source_file,current_node,children,parent);
	sll_deallocate(children);
	return node;
}



void _unneeded_result(sll_arg_count_t* arg_count,sll_node_t* node){
	switch (node->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			node->type=SLL_NODE_TYPE_NOP;
			(*arg_count)--;
			break;
		case SLL_NODE_TYPE_ARRAY:
		case SLL_NODE_TYPE_MAP:
		case SLL_NODE_TYPE_COMMA:
		case SLL_NODE_TYPE_OPERATION_LIST:
			node->type=SLL_NODE_TYPE_NOP;
			(*arg_count)+=node->data.arg_count;
			break;
	}
}



__SLL_EXTERNAL void sll_optimize_source_file(sll_source_file_t* source_file){
	if (!source_file->first_node){
		return;
	}
	_init_optimizer(source_file);
	_visit_node(source_file,source_file->first_node,NULL);
	_deinit_optimizer(source_file);
}

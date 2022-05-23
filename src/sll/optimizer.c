#include <sll/_internal/common.h>
#include <sll/_internal/optimizer.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/types.h>



#define SKIP_NOP \
	while (node->type==SLL_NODE_TYPE_NOP||node->type==SLL_NODE_TYPE_DBG||node->type==SLL_NODE_TYPE_CHANGE_STACK){ \
		node=(node->type==SLL_NODE_TYPE_CHANGE_STACK?node->data._next_node:node+1); \
	} \



static sll_node_t* _visit_node(sll_source_file_t* source_file,sll_node_t* node,sll_node_t* parent){
	SKIP_NOP;
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
	sll_node_t* current_node=node;
	node++;
	sll_node_t** children=sll_allocate_stack(arg_count*sizeof(sll_node_t*));
	for (sll_arg_count_t i=0;i<arg_count;i++){
		SKIP_NOP;
		*(children+i)=node;
		node=_visit_node(source_file,node,current_node);
	}
	if (_merge_print_strings(source_file,current_node,arg_count,children,parent)){
		sll_arg_count_t new=0;
		while (arg_count){
			arg_count--;
			new+=((*(children+arg_count))->type!=SLL_NODE_TYPE_NOP);
		}
		switch (current_node->type){
			case SLL_NODE_TYPE_ARRAY:
				current_node->data.array_length=new;
				break;
			case SLL_NODE_TYPE_MAP:
				current_node->data.map_length=new;
				break;
			case SLL_NODE_TYPE_FUNC:
			case SLL_NODE_TYPE_INTERNAL_FUNC:
				current_node->data.function.arg_count=new;
				break;
			case SLL_NODE_TYPE_FOR:
			case SLL_NODE_TYPE_WHILE:
			case SLL_NODE_TYPE_LOOP:
			case SLL_NODE_TYPE_FOR_ARRAY:
			case SLL_NODE_TYPE_WHILE_ARRAY:
			case SLL_NODE_TYPE_FOR_MAP:
			case SLL_NODE_TYPE_WHILE_MAP:
				current_node->data.loop.arg_count=new;
				break;
			default:
				current_node->data.arg_count=new;
				break;
		}
	}
	sll_deallocate(children);
	return node;
}



__SLL_EXTERNAL void sll_optimize_source_file(sll_source_file_t* source_file){
	if (!source_file->first_node){
		return;
	}
	_visit_node(source_file,source_file->first_node,NULL);
}

#include <sll/_internal/common.h>
#include <sll/_internal/optimizer.h>
#include <sll/common.h>
#include <sll/generated/optimizer.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/optimizer.h>
#include <sll/types.h>



static sll_node_t* _visit_node(sll_source_file_t* source_file,sll_node_t* node,sll_node_t* parent){
	SKIP_NODE_NOP(node);
	sll_arg_count_t* arg_count=NULL;
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
			arg_count=&(node->data.array_length);
			break;
		case SLL_NODE_TYPE_MAP:
			arg_count=&(node->data.map_length);
			break;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			arg_count=&(node->data.function.arg_count);
			break;
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			arg_count=&(node->data.loop.arg_count);
			break;
		default:
			arg_count=&(node->data.arg_count);
			break;
	}
	_pre_visit_optimizer(source_file,node,parent);
	sll_node_t* current_node=node;
	node++;
	sll_node_t** children=sll_allocate_stack(1);
	if (arg_count){
		sll_arg_count_t i=0;
		while (i<*arg_count){
			i++;
			children=sll_reallocate(children,i*sizeof(sll_node_t*));
			*(children+i-1)=node;
			sll_arg_count_t old_ac=*arg_count;
			node=_visit_node(source_file,node,current_node);
			if (*arg_count<old_ac){
				i--;
			}
			else{
				SKIP_NODE_NOP(*(children+i-1));
			}
		}
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
		case SLL_NODE_TYPE_NOT:
		case SLL_NODE_TYPE_BOOL:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC_LOAD:
		case SLL_NODE_TYPE_ADD:
		case SLL_NODE_TYPE_SUB:
		case SLL_NODE_TYPE_MULT:
		case SLL_NODE_TYPE_DIV:
		case SLL_NODE_TYPE_FLOOR_DIV:
		case SLL_NODE_TYPE_MOD:
		case SLL_NODE_TYPE_BIT_AND:
		case SLL_NODE_TYPE_BIT_OR:
		case SLL_NODE_TYPE_BIT_XOR:
		case SLL_NODE_TYPE_BIT_NOT:
		case SLL_NODE_TYPE_BIT_RSHIFT:
		case SLL_NODE_TYPE_BIT_LSHIFT:
		case SLL_NODE_TYPE_LENGTH:
		case SLL_NODE_TYPE_ACCESS:
		case SLL_NODE_TYPE_DEEP_COPY:
		case SLL_NODE_TYPE_VAR_ACCESS:
		case SLL_NODE_TYPE_HAS:
		case SLL_NODE_TYPE_CAST:
		case SLL_NODE_TYPE_TYPEOF:
		case SLL_NODE_TYPE_NAMEOF:
		case SLL_NODE_TYPE_NAMEOF_TYPE:
		case SLL_NODE_TYPE_DECL:
		case SLL_NODE_TYPE_NEW:
		case SLL_NODE_TYPE_REF:
		case SLL_NODE_TYPE_COMMA:
		case SLL_NODE_TYPE_OPERATION_LIST:
			node->type=SLL_NODE_TYPE_NOP;
			(*arg_count)+=node->data.arg_count-1;
			break;
		case SLL_NODE_TYPE_INLINE_IF:
			node->type=SLL_NODE_TYPE_IF;
			break;
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
			node->type=SLL_NODE_TYPE_FOR;
			break;
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_WHILE_MAP:
			node->type=SLL_NODE_TYPE_WHILE;
			break;
	}
}



__SLL_EXTERNAL void sll_optimize_source_file(sll_source_file_t* source_file,sll_optimization_round_count_t round_count){
	if (!source_file->first_node){
		return;
	}
	while (round_count){
		round_count--;
		_init_optimizer(source_file);
		_visit_node(source_file,source_file->first_node,NULL);
		_deinit_optimizer(source_file);
	}
}

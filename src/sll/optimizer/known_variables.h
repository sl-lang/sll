#include <sll/_internal/common.h>
#include <sll/_internal/optimizer.h>
#include <sll/_internal/stack.h>
#include <sll/_internal/static_object.h>
#include <sll/_internal/util.h>
#include <sll/_size_types.h>
#include <sll/gc.h>
#include <sll/identifier.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



#define GET_VARIABLE_DATA(identifier_index) (_variable_data[SLL_IDENTIFIER_GET_ARRAY_ID((identifier_index))]+SLL_IDENTIFIER_GET_ARRAY_INDEX((identifier_index)))
#define LOCK_IDENTIFIER(identifier_index) \
	do{ \
		variable_data_t* __var=GET_VARIABLE_DATA(identifier_index); \
		if (!__var->locked){ \
			__var->locked=1; \
			if (__var->value){ \
				SLL_RELEASE(__var->value); \
				__var->value=NULL; \
			} \
			__var->next=_loop_closure_data.data[_loop_closure_data.length]; \
			_loop_closure_data.data[_loop_closure_data.length]=__var; \
		} \
	} while (0)



typedef struct _VARIABLE_DATA{
	sll_object_t* value;
	struct _VARIABLE_DATA* next;
	sll_bool_t locked;
} variable_data_t;



typedef struct _LOOP_CLOSURE_DATA{
	variable_data_t** data;
	sll_array_length_t length;
} loop_closure_data_t;



static variable_data_t* _variable_data[SLL_MAX_SHORT_IDENTIFIER_LENGTH+1];
static loop_closure_data_t _loop_closure_data={
	NULL,
	0
};



static void _release_var_data(variable_data_t* data,sll_identifier_list_length_t length){
	while (length){
		length--;
		if ((data+length)->value){
			SLL_RELEASE((data+length)->value);
		}
		SLL_ASSERT(!(data+length)->locked);
	}
	sll_deallocate(data);
}



static const sll_node_t* _mark_loop_change(const sll_node_t* node){
	SKIP_NODE_NOP(node);
	if (node->type==SLL_NODE_TYPE_ASSIGN||node->type==SLL_NODE_TYPE_INC||node->type==SLL_NODE_TYPE_DEC){
		const sll_node_t* var=node+1;
		SKIP_NODE_NOP(var);
		if (var->type==SLL_NODE_TYPE_IDENTIFIER){
			LOCK_IDENTIFIER(var->data.identifier_index);
		}
		else{
			SLL_UNIMPLEMENTED();
		}
	}
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
	node++;
	for (sll_arg_count_t i=0;i<arg_count;i++){
		node=_mark_loop_change(node);
	}
	return node;
}



OPTIMIZER_FUNTION_INIT(known_variables){
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		_variable_data[i]=sll_zero_allocate(source_file->identifier_table.short_[i].length*sizeof(variable_data_t));
	}
	_variable_data[SLL_MAX_SHORT_IDENTIFIER_LENGTH]=sll_zero_allocate(source_file->identifier_table.long_data_length*sizeof(variable_data_t));
}



OPTIMIZER_FUNTION_DEINIT(known_variables){
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		_release_var_data(_variable_data[i],source_file->identifier_table.short_[i].length);
	}
	_release_var_data(_variable_data[SLL_MAX_SHORT_IDENTIFIER_LENGTH],source_file->identifier_table.long_data_length);
	SLL_ASSERT(!_loop_closure_data.length);
}



OPTIMIZER_FUNTION_PRE_VISIT(known_variables){
	if (node->type!=SLL_NODE_TYPE_FOR&&node->type!=SLL_NODE_TYPE_WHILE&&node->type!=SLL_NODE_TYPE_LOOP&&node->type!=SLL_NODE_TYPE_FOR_ARRAY&&node->type!=SLL_NODE_TYPE_WHILE_ARRAY&&node->type!=SLL_NODE_TYPE_FOR_MAP&&node->type!=SLL_NODE_TYPE_WHILE_MAP){
		return node;
	}
	_loop_closure_data.data=sll_reallocate(_loop_closure_data.data,(_loop_closure_data.length+1)*sizeof(variable_data_t*));
	_loop_closure_data.data[_loop_closure_data.length]=NULL;
	_mark_loop_change(node);
	_loop_closure_data.length++;
	return node;
}



OPTIMIZER_FUNTION(known_variables){
	if (node->type==SLL_NODE_TYPE_FOR||node->type==SLL_NODE_TYPE_WHILE||node->type==SLL_NODE_TYPE_LOOP||node->type==SLL_NODE_TYPE_FOR_ARRAY||node->type==SLL_NODE_TYPE_WHILE_ARRAY||node->type==SLL_NODE_TYPE_FOR_MAP||node->type==SLL_NODE_TYPE_WHILE_MAP){
		_loop_closure_data.length--;
		variable_data_t* var=_loop_closure_data.data[_loop_closure_data.length];
		while (var){
			var->locked=0;
			var=var->next;
		}
	}
	else if (node->type==SLL_NODE_TYPE_IDENTIFIER&&(!parent||(parent->type!=SLL_NODE_TYPE_ASSIGN&&parent->type!=SLL_NODE_TYPE_ACCESS&&parent->type!=SLL_NODE_TYPE_VAR_ACCESS&&parent->type!=SLL_NODE_TYPE_INC&&parent->type!=SLL_NODE_TYPE_DEC&&parent->type!=SLL_NODE_TYPE_CALL))){
		variable_data_t* var_data=GET_VARIABLE_DATA(node->data.identifier_index);
		if (var_data->value){
			node=_build_value(source_file,var_data->value,node);
			REWIND_NODE(node);
			return node;
		}
	}
	else if (node->type==SLL_NODE_TYPE_ASSIGN){
		SLL_ASSERT(node->type>=2);
		sll_node_t* variable=*children;
		if (variable->type==SLL_NODE_TYPE_IDENTIFIER){
			variable_data_t* var_data=GET_VARIABLE_DATA(variable->data.identifier_index);
			if (!var_data->locked){
				if (var_data->value){
					SLL_RELEASE(var_data->value);
					var_data->value=NULL;
				}
				_parse_value(source_file,*(children+1),&(var_data->value));
			}
		}
		else{
			SLL_UNIMPLEMENTED();
		}
	}
	return node;
}

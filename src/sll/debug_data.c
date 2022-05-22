#include <sll/common.h>
#include <sll/node.h>
#include <sll/string_table.h>
#include <sll/types.h>



static sll_node_t* _remove_debug_data(sll_node_t* o){
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		if (o->type==SLL_NODE_TYPE_DBG){
			o->type=SLL_NODE_TYPE_NOP;
		}
		o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
	}
	switch (o->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->data.array_length;
				o++;
				while (l){
					l--;
					o=_remove_debug_data(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->data.map_length;
				o++;
				while (l){
					l--;
					o=_remove_debug_data(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->data.function.arg_count;
				o++;
				while (l){
					l--;
					o=_remove_debug_data(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				sll_arg_count_t l=o->data.loop.arg_count;
				o++;
				while (l){
					l--;
					o=_remove_debug_data(o);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->data.arg_count;
	o++;
	while (l){
		l--;
		o=_remove_debug_data(o);
	}
	return o;
}



static sll_node_t* _remove_type_names(sll_node_t* o){
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		o=(o->type==SLL_NODE_TYPE_CHANGE_STACK?o->data._next_node:o+1);
	}
	switch (o->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->data.array_length;
				o++;
				while (l){
					l--;
					o=_remove_type_names(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->data.map_length;
				o++;
				while (l){
					l--;
					o=_remove_type_names(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->data.function.arg_count;
				o++;
				while (l){
					l--;
					o=_remove_type_names(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				sll_arg_count_t l=o->data.loop.arg_count;
				o++;
				while (l){
					l--;
					o=_remove_type_names(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			o->data.declaration.name_string_index=SLL_MAX_STRING_INDEX;
			break;
	}
	sll_arg_count_t l=o->data.arg_count;
	o++;
	while (l){
		l--;
		o=_remove_type_names(o);
	}
	return o;
}



__SLL_EXTERNAL void sll_remove_debug_data(sll_compilation_data_t* compilation_data){
	for (sll_source_file_index_t i=0;i<compilation_data->length;i++){
		sll_node_t* p=(*(compilation_data->data+i))->first_node;
		if (p){
			_remove_debug_data(p);
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

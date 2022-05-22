#include <sll/common.h>
#include <sll/node.h>
#include <sll/types.h>



static sll_node_t* _remove_padding_internal(sll_node_t* o,sll_source_file_t* sf,sll_node_t** d,sll_node_offset_t* rm){
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		if (o->type==SLL_NODE_TYPE_CHANGE_STACK){
			o=o->data._next_node;
		}
		else{
			o++;
			(*rm)++;
		}
	}
	**d=*o;
	(*d)++;
	if ((*d)->type==SLL_NODE_TYPE_CHANGE_STACK){
		(*d)=(*d)->data._next_node;
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
					o=_remove_padding_internal(o,sf,d,rm);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->data.map_length;
				o++;
				while (l){
					l--;
					o=_remove_padding_internal(o,sf,d,rm);
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
			(*(sf->function_table.data+o->data.function.function_index))->offset-=*rm;
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->data.function.arg_count;
				o++;
				while (l){
					l--;
					o=_remove_padding_internal(o,sf,d,rm);
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
					o=_remove_padding_internal(o,sf,d,rm);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				sll_arg_count_t l=o->data.declaration.arg_count;
				o++;
				while (l){
					l--;
					o=_remove_padding_internal(o,sf,d,rm);
				}
				return o;
			}
		case SLL_NODE_TYPE_DBG:
			return _remove_padding_internal(o+1,sf,d,rm);
	}
	sll_arg_count_t l=o->data.arg_count;
	o++;
	while (l){
		l--;
		o=_remove_padding_internal(o,sf,d,rm);
	}
	return o;
}



__SLL_EXTERNAL void sll_remove_node_padding(sll_compilation_data_t* c_dt){
	for (sll_source_file_index_t i=0;i<c_dt->length;i++){
		sll_source_file_t* sf=*(c_dt->data+i);
		sll_node_t* d=sf->first_node;
		if (!d){
			continue;
		}
		sll_node_offset_t rm=0;
		_remove_padding_internal(d,sf,&d,&rm);
	}
}

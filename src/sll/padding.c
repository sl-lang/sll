#include <sll/common.h>
#include <sll/node.h>
#include <sll/types.h>



static sll_node_t* _remove_padding_internal(sll_node_t* o,sll_source_file_t* sf,sll_node_t** d,sll_node_offset_t* rm){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		if (o->t==SLL_NODE_TYPE_CHANGE_STACK){
			o=o->dt._p;
		}
		else{
			o++;
			(*rm)++;
		}
	}
	**d=*o;
	(*d)++;
	if ((*d)->t==SLL_NODE_TYPE_CHANGE_STACK){
		(*d)=(*d)->dt._p;
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=_remove_padding_internal(o,sf,d,rm);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_remove_padding_internal(o,sf,d,rm);
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
			(*(sf->ft.dt+o->dt.fn.id))->off-=*rm;
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
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
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_remove_padding_internal(o,sf,d,rm);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				sll_arg_count_t l=o->dt.d.ac;
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
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_remove_padding_internal(o,sf,d,rm);
	}
	return o;
}



__SLL_EXTERNAL void sll_remove_node_padding(sll_compilation_data_t* c_dt){
	for (sll_source_file_index_t i=0;i<c_dt->l;i++){
		sll_source_file_t* sf=*(c_dt->dt+i);
		sll_node_t* d=sf->dt;
		sll_node_offset_t rm=0;
		_remove_padding_internal(d,sf,&d,&rm);
	}
}

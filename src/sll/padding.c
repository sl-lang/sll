#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/node.h>
#include <sll/types.h>



static sll_node_t* _remove_padding_internal(sll_node_t* o,sll_compilation_data_t* c_dt,sll_node_t** d,sll_node_offset_t* rm){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==NODE_TYPE_CHANGE_STACK){
		if (o->t==NODE_TYPE_CHANGE_STACK){
			o=o->dt._p;
		}
		else{
			o++;
			(*rm)++;
		}
	}
	**d=*o;
	(*d)++;
	if ((*d)->t==NODE_TYPE_CHANGE_STACK){
		(*d)=(*d)->dt._p;
	}
	switch (o->t){
		case SLL_NODE_TYPE_UNKNOWN:
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
					o=_remove_padding_internal(o,c_dt,d,rm);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_remove_padding_internal(o,c_dt,d,rm);
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
			(*(c_dt->ft.dt+o->dt.fn.id))->off-=*rm;
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_remove_padding_internal(o,c_dt,d,rm);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_remove_padding_internal(o,c_dt,d,rm);
				}
				return o;
			}
		case SLL_NODE_TYPE_DEBUG_DATA:
			return _remove_padding_internal(o+1,c_dt,d,rm);
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_remove_padding_internal(o,c_dt,d,rm);
	}
	return o;
}



__SLL_EXTERNAL void sll_remove_node_padding(sll_compilation_data_t* c_dt,sll_node_t* o){
	sll_node_t* d=o;
	sll_node_offset_t rm=0;
	_remove_padding_internal(o,c_dt,&d,&rm);
}

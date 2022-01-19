#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/node.h>
#include <sll/types.h>



static const sll_node_t* _get_node_size(const sll_node_t* o,sll_node_offset_t* sz){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
		if (o->t==NODE_TYPE_CHANGE_STACK){
			o=o->dt._p;
		}
		else{
			(*sz)++;
			o++;
		}
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			(*sz)++;
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=o->dt.al;
				(*sz)++;
				o++;
				while (l){
					l--;
					o=_get_node_size(o,sz);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				(*sz)++;
				o++;
				while (l){
					l--;
					o=_get_node_size(o,sz);
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				(*sz)++;
				o++;
				while (l){
					l--;
					o=_get_node_size(o,sz);
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
				(*sz)++;
				o++;
				while (l){
					l--;
					o=_get_node_size(o,sz);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	(*sz)++;
	o++;
	while (l){
		l--;
		o=_get_node_size(o,sz);
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_node_offset_t sll_get_node_size(const sll_node_t* o){
	sll_node_offset_t sz=0;
	_get_node_size(o,&sz);
	return sz;
}

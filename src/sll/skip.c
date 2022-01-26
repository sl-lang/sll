#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/node.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_node_t* sll_skip_node(sll_node_t* o){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		o=(o->t==SLL_NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
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
					o=sll_skip_node(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=sll_skip_node(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=sll_skip_node(o);
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
					o=sll_skip_node(o);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=sll_skip_node(o);
	}
	return o;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT const sll_node_t* sll_skip_node_const(const sll_node_t* o){
	return sll_skip_node((sll_node_t*)o);
}

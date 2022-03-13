#include <sll/common.h>
#include <sll/node.h>
#include <sll/types.h>



static sll_node_t* _remove_debug_data(sll_node_t* o){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		if (o->t==SLL_NODE_TYPE_DBG){
			o->t=SLL_NODE_TYPE_NOP;
		}
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
					o=_remove_debug_data(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
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
				sll_arg_count_t l=o->dt.fn.ac;
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
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_remove_debug_data(o);
				}
				return o;
			}
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_remove_debug_data(o);
	}
	return o;
}



static sll_node_t* _remove_type_names(sll_node_t* o){
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
					o=_remove_type_names(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=o->dt.ml;
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
				sll_arg_count_t l=o->dt.fn.ac;
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
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_remove_type_names(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			o->dt.d.nm=SLL_MAX_STRING_INDEX;
			break;
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_remove_type_names(o);
	}
	return o;
}



__SLL_EXTERNAL void sll_remove_debug_data(sll_compilation_data_t* c_dt){
	for (sll_source_file_index_t i=0;i<c_dt->l;i++){
		_remove_debug_data((*(c_dt->dt+i))->dt);
	}
}



__SLL_EXTERNAL void sll_remove_debug_names(sll_compilation_data_t* c_dt){
	for (sll_source_file_index_t i=0;i<c_dt->l;i++){
		sll_source_file_t* sf=*(c_dt->dt+i);
		for (sll_function_index_t j=0;j<sf->ft.l;j++){
			(*(sf->ft.dt+j))->nm=SLL_MAX_STRING_INDEX;
		}
		_remove_type_names(sf->dt);
	}
}

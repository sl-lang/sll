#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/node.h>
#include <sll/types.h>



static sll_node_t* _remove_debug_data_internal(sll_node_t* o){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==NODE_TYPE_CHANGE_STACK){
		o=(o->t==NODE_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_NODE_TYPE_UNKNOWN:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_IDENTIFIER:
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_FIELD:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_remove_debug_data_internal(o);
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
					o=_remove_debug_data_internal(o);
				}
				return o;
			}
		case SLL_NODE_TYPE_DEBUG_DATA:
			o->t=SLL_NODE_TYPE_NOP;
			return _remove_debug_data_internal(o+1);
	}
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_remove_debug_data_internal(o);
	}
	return o;
}



__SLL_EXTERNAL void sll_insert_debug_node(sll_compilation_data_t* c_dt,sll_file_t* rf){
	sll_node_t* dbg=_acquire_next_node(c_dt);
	dbg->t=SLL_NODE_TYPE_DEBUG_DATA;
	dbg->dt.dbg.fpi=0;
	dbg->dt.dbg.ln=SLL_FILE_GET_LINE_NUMBER(rf);
	dbg->dt.dbg.cn=SLL_FILE_GET_OFFSET(rf)-SLL_FILE_GET_LINE_OFFSET(rf)-1;
	dbg->dt.dbg.ln_off=SLL_FILE_GET_LINE_OFFSET(rf);
}



__SLL_EXTERNAL void sll_remove_node_debug_data(sll_node_t* o){
	_remove_debug_data_internal(o);
}

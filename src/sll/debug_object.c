#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/object.h>
#include <sll/types.h>



static sll_object_t* _remove_debug_data_internal(sll_object_t* o){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==OBJECT_TYPE_CHANGE_STACK){
		o=(o->t==OBJECT_TYPE_CHANGE_STACK?o->dt._p:o+1);
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_IDENTIFIER:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			return o+1;
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_remove_debug_data_internal(o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_WHILE:
		case SLL_OBJECT_TYPE_LOOP:
			{
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_remove_debug_data_internal(o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			o->t=SLL_OBJECT_TYPE_NOP;
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



__SLL_EXTERNAL void sll_insert_debug_object(sll_compilation_data_t* c_dt,sll_file_t* rf){
	sll_object_t* dbg=_acquire_next_object(c_dt);
	dbg->t=SLL_OBJECT_TYPE_DEBUG_DATA;
	dbg->dt.dbg.fpi=0;
	dbg->dt.dbg.ln=SLL_FILE_GET_LINE_NUMBER(rf);
	dbg->dt.dbg.cn=SLL_FILE_GET_OFFSET(rf)-SLL_FILE_GET_LINE_OFFSET(rf)-1;
	dbg->dt.dbg.ln_off=SLL_FILE_GET_LINE_OFFSET(rf);
}



__SLL_EXTERNAL void sll_remove_object_debug_data(sll_object_t* o){
	_remove_debug_data_internal(o);
}

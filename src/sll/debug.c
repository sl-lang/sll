#include <sll/common.h>
#include <sll/constants.h>
#include <sll/types.h>



static sll_object_offset_t _remove_debug_data_internal(sll_object_t* o){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP){
		eoff++;
		o++;
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_STRING:
		case SLL_OBJECT_TYPE_IDENTIFIER:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
			return eoff+1;
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_remove_debug_data_internal(o+off);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_COMMA:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=1;
				sll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_remove_debug_data_internal(o+off);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			o->t=SLL_OBJECT_TYPE_NOP;
			return eoff+_remove_debug_data_internal(o+1)+1;
	}
	sll_object_offset_t off=1;
	sll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_remove_debug_data_internal(o+off);
	}
	return off+eoff;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_insert_debug_object(sll_compilation_data_t* c_dt,sll_input_data_stream_t* is,sll_error_t* e){
	if (!c_dt->_s.ptr){
		e->t=SLL_ERROR_NO_STACK;
		return SLL_RETURN_ERROR;
	}
	sll_object_t* dbg=(sll_object_t*)(c_dt->_s.ptr+c_dt->_s.off);
	c_dt->_s.off+=sizeof(sll_object_t);
	if (c_dt->_s.off>=c_dt->_s.sz){
		e->t=SLL_ERROR_INTERNAL_STACK_OVERFLOW;
		e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
		e->dt.r.sz=1;
		return SLL_RETURN_ERROR;
	}
	dbg->t=SLL_OBJECT_TYPE_DEBUG_DATA;
	dbg->dt.dbg.fpi=0;
	dbg->dt.dbg.ln=SLL_GET_INPUT_DATA_STREAM_LINE_NUMBER(is);
	dbg->dt.dbg.cn=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-SLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is)-1;
	dbg->dt.dbg.ln_off=SLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is);
	return SLL_RETURN_NO_ERROR;
}



__SLL_FUNC void sll_remove_object_debug_data(sll_object_t* o){
	_remove_debug_data_internal(o);
}

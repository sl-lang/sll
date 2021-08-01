#include <lll/common.h>
#include <lll/core.h>
#include <lll/types.h>



lll_object_offset_t _remove_debug_data_internal(lll_object_t* o){
	lll_object_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff++;
		o++;
	}
	switch (o->t){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_CHAR:
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_IDENTIFIER:
		case LLL_OBJECT_TYPE_INT:
		case LLL_OBJECT_TYPE_FLOAT:
			return eoff+1;
		case LLL_OBJECT_TYPE_FUNC:
		case LLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				lll_object_offset_t off=1;
				lll_arg_count_t l=o->dt.fn.ac;
				while (l){
					l--;
					off+=_remove_debug_data_internal(o+off);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_object_offset_t off=1;
				lll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_remove_debug_data_internal(o+off);
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			o->t=LLL_OBJECT_TYPE_NOP;
			return eoff+_remove_debug_data_internal(o+1)+1;
	}
	lll_object_offset_t off=1;
	lll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_remove_debug_data_internal(o+off);
	}
	return off+eoff;
}



__LLL_FUNC __LLL_RETURN lll_insert_debug_object(lll_compilation_data_t* c_dt,lll_input_data_stream_t* is,lll_error_t* e){
	if (!c_dt->_s.ptr){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	lll_object_t* dbg=(lll_object_t*)(c_dt->_s.ptr+c_dt->_s.off);
	c_dt->_s.off+=sizeof(lll_object_t);
	if (c_dt->_s.off>=c_dt->_s.sz){
		e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
		e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
		e->dt.r.sz=1;
		return LLL_RETURN_ERROR;
	}
	dbg->t=LLL_OBJECT_TYPE_DEBUG_DATA;
	dbg->dt.dbg.fpi=0;
	dbg->dt.dbg.ln=LLL_GET_INPUT_DATA_STREAM_LINE_NUMBER(is);
	dbg->dt.dbg.cn=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-LLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is)-1;
	dbg->dt.dbg.ln_off=LLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is);
	return LLL_RETURN_NO_ERROR;
}



__LLL_FUNC void lll_remove_object_debug_data(lll_object_t* o){
	_remove_debug_data_internal(o);
}

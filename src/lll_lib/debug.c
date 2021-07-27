#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdint.h>
#include <stdio.h>



lll_stack_offset_t _remove_debug_data_internal(lll_object_t* o){
	lll_stack_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_TRUE:
		case LLL_OBJECT_TYPE_FALSE:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			return sizeof(lll_char_object_t)+eoff;
		case LLL_OBJECT_TYPE_STRING:
			return sizeof(lll_string_object_t)+eoff;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_identifier_object_t)+eoff;
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_integer_object_t)+eoff;
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_float_object_t)+eoff;
		case LLL_OBJECT_TYPE_IMPORT:
			return sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac+eoff;
		case LLL_OBJECT_TYPE_FUNC:
			{
				lll_stack_offset_t off=sizeof(lll_function_object_t);
				lll_arg_count_t l=((lll_function_object_t*)o)->ac;
				while (l){
					l--;
					off+=_remove_debug_data_internal(LLL_GET_OBJECT_ARGUMENT(o,off));
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_stack_offset_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				while (l){
					l--;
					off+=_remove_debug_data_internal(LLL_GET_OBJECT_STATEMENT(o,off));
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			for (lll_stack_offset_t i=0;i<sizeof(lll_debug_object_t);i+=sizeof(lll_object_type_t)){
				LLL_SET_OBJECT_NOP(o,i);
			}
			return sizeof(lll_debug_object_t)+eoff+_remove_debug_data_internal(LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o));
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	while (l){
		l--;
		off+=_remove_debug_data_internal(LLL_GET_OBJECT_ARGUMENT(o,off));
	}
	return off+eoff;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_insert_debug_object(lll_compilation_data_t* c_dt,lll_input_data_stream_t* is,lll_error_t* e){
	if (!c_dt->_s.ptr){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	lll_debug_object_t* dbg=(lll_debug_object_t*)(c_dt->_s.ptr+c_dt->_s.off);
	c_dt->_s.off+=sizeof(lll_debug_object_t);
	if (c_dt->_s.off>=c_dt->_s.sz){
		e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
		e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
		e->dt.r.sz=1;
		return LLL_RETURN_ERROR;
	}
	dbg->t=LLL_OBJECT_TYPE_DEBUG_DATA;
	dbg->fpi=0;
	dbg->ln=LLL_GET_INPUT_DATA_STREAM_LINE_NUMBER(is);
	dbg->cn=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-LLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is)-1;
	dbg->ln_off=LLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is);
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT void lll_remove_object_debug_data(lll_object_t* o){
	_remove_debug_data_internal(o);
}

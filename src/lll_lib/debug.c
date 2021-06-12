#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>
#include <stdio.h>



uint32_t _remove_debug_data_internal(lll_object_t* o){
	uint32_t eoff=0;
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
			return sizeof(lll_object_t)+eoff+sizeof(char);
		case LLL_OBJECT_TYPE_STRING:
			return sizeof(lll_object_t)+eoff+sizeof(lll_string_length_t)+LLL_GET_OBJECT_STRING_LENGTH(o);
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_object_t)+eoff+sizeof(lll_identifier_index_t);
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_object_t)+eoff+LLL_GET_OBJECT_INTEGER_WIDTH(o);
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_object_t)+eoff+(LLL_IS_OBJECT_FLOAT64(o)?sizeof(double):sizeof(float));
		case LLL_OBJECT_TYPE_IMPORT:
			return sizeof(lll_object_t)+sizeof(lll_arg_count_t)+sizeof(lll_import_index_t)*(*LLL_GET_OBJECT_ARGUMENT_COUNT(o))+eoff;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
				lll_statement_count_t l=*LLL_GET_OBJECT_STATEMENT_COUNT(o);
				while (l){
					l--;
					off+=_remove_debug_data_internal(LLL_GET_OBJECT_STATEMENT(o,off));
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_debug_object_t* dbg=(lll_debug_object_t*)o;
				uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
				for (uint32_t i=0;i<sz;i++){
					*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,i))=LLL_OBJECT_TYPE_NOP;
				}
				return sz+eoff+_remove_debug_data_internal(LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz));
			}
	}
	uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
	lll_arg_count_t l=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
	while (l){
		l--;
		off+=_remove_debug_data_internal(LLL_GET_OBJECT_ARGUMENT(o,off));
	}
	return off+eoff;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_insert_debug_object(lll_input_data_stream_t* is,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	lll_debug_object_t* dbg=(lll_debug_object_t*)(_bf+_bf_off);
	dbg->t=LLL_OBJECT_TYPE_DEBUG_DATA;
	dbg->f=0;
	uint32_t ln=LLL_GET_INPUT_DATA_STREAM_LINE_NUMBER(is);
	uint32_t i=sizeof(lll_debug_object_t);
	if (ln>UINT16_MAX){
		dbg->f|=LLL_DEBUG_OBJECT_LINE_NUMBER_INT32;
		LLL_SET_DEBUG_OBJECT_DATA_UINT32(dbg,i,ln);
		i+=sizeof(uint32_t);
	}
	else if (ln>UINT8_MAX){
		dbg->f|=LLL_DEBUG_OBJECT_LINE_NUMBER_INT16;
		LLL_SET_DEBUG_OBJECT_DATA_UINT16(dbg,i,ln);
		i+=sizeof(uint16_t);
	}
	else{
		dbg->f|=LLL_DEBUG_OBJECT_LINE_NUMBER_INT8;
		LLL_SET_DEBUG_OBJECT_DATA_UINT8(dbg,i,ln);
		i+=sizeof(uint8_t);
	}
	uint32_t cl=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-LLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is)-1;
	if (cl>UINT16_MAX){
		dbg->f|=LLL_DEBUG_OBJECT_COLUMN_NUMBER_INT32;
		LLL_SET_DEBUG_OBJECT_DATA_UINT32(dbg,i,cl);
		i+=sizeof(uint32_t);
	}
	else if (cl>UINT8_MAX){
		dbg->f|=LLL_DEBUG_OBJECT_COLUMN_NUMBER_INT16;
		LLL_SET_DEBUG_OBJECT_DATA_UINT16(dbg,i,cl);
		i+=sizeof(uint16_t);
	}
	else{
		dbg->f|=LLL_DEBUG_OBJECT_COLUMN_NUMBER_INT8;
		LLL_SET_DEBUG_OBJECT_DATA_UINT8(dbg,i,cl);
		i+=sizeof(uint8_t);
	}
	uint32_t ln_off=LLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is);
	if (ln_off>UINT16_MAX){
		dbg->f|=LLL_DEBUG_OBJECT_FILE_OFFSET_INT32;
		LLL_SET_DEBUG_OBJECT_DATA_UINT32(dbg,i,ln_off);
		i+=sizeof(uint32_t);
	}
	else if (ln_off>UINT8_MAX){
		dbg->f|=LLL_DEBUG_OBJECT_FILE_OFFSET_INT16;
		LLL_SET_DEBUG_OBJECT_DATA_UINT16(dbg,i,ln_off);
		i+=sizeof(uint16_t);
	}
	else{
		dbg->f|=LLL_DEBUG_OBJECT_FILE_OFFSET_INT8;
		LLL_SET_DEBUG_OBJECT_DATA_UINT8(dbg,i,ln_off);
		i+=sizeof(uint8_t);
	}
	_bf_off+=i;
	if (_bf_off>=_bf_sz){
		e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
		e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
		e->dt.r.sz=1;
		return LLL_RETURN_ERROR;
	}
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_remove_object_debug_data(lll_object_t* o,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	_remove_debug_data_internal(o);
	return LLL_RETURN_NO_ERROR;
}

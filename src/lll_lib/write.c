#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>



uint32_t _get_object_size(lll_object_t* o){
	uint32_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	if (LLL_IS_OBJECT_TYPE_NOT_INTEGRAL(o)){
		while (LLL_IS_OBJECT_REF(o)){
			o=READ_REF_FROM_STACK(o);
		}
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
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_object_t)+eoff+LLL_GET_OBJECT_STRING_LENGTH(o)+sizeof(lll_string_length_t);
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_object_t)+eoff+LLL_GET_OBJECT_INTEGER_WIDTH(o);
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_object_t)+eoff+(LLL_IS_OBJECT_FLOAT64(o)?sizeof(double):sizeof(float));
		case LLL_OBJECT_TYPE_OPERATION_LIST:;
			uint32_t off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
			lll_statement_count_t l=*LLL_GET_OBJECT_STATEMENT_COUNT(o);
			while (l){
				l--;
				off+=_get_object_size(LLL_GET_OBJECT_STATEMENT(o,off));
			}
			return off+eoff;
		case LLL_OBJECT_TYPE_DEBUG_DATA:;
			lll_debug_object_t* dbg=(lll_debug_object_t*)o;
			uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
			return sz+eoff+_get_object_size(LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz));
	}
	uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
	lll_arg_count_t l=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
	while (l){
		l--;
		off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
	}
	return off+eoff;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_write_object(lll_output_data_stream_t* os,lll_object_t* o,uint8_t f,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	if (f==LLL_WRITE_MODE_C){
		ASSERT(!"Unimplemented",e,LLL_RETURN_ERROR);
	}
	else{
		if (LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)o,_get_object_size(o))){
			return LLL_RETURN_NO_ERROR;
		}
	}
	e->t=LLL_ERROR_FAILED_FILE_WRITE;
	return LLL_RETURN_ERROR;
}

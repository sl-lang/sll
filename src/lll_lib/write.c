#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>



uint32_t _get_object_size(lll_object_t* o){
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
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				uint32_t off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
				lll_statement_count_t l=*LLL_GET_OBJECT_STATEMENT_COUNT(o);
				while (l){
					l--;
					off+=_get_object_size(LLL_GET_OBJECT_STATEMENT(o,off));
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_debug_object_t* dbg=(lll_debug_object_t*)o;
				uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
				return sz+eoff+_get_object_size(LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz));
			}
	}
	uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
	lll_arg_count_t l=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
	while (l){
		l--;
		off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
	}
	return off+eoff;
}



uint32_t _write_object_as_assembly(lll_output_data_stream_t* os,lll_object_t* o,lll_error_t* e){
	uint32_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			ASSERT(!"Character Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_INT:
			ASSERT(!"Integer Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FLOAT:
			ASSERT(!"Float Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_NIL:
			ASSERT(!"NIL Value Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_TRUE:
			ASSERT(!"TRUE Value Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FALSE:
			ASSERT(!"FALSE Value Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_STRING:
			ASSERT(!"String Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_IDENTIFIER:
			ASSERT(!"Identifier Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_CHAR:
			ASSERT(!"'char' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_STRING:
			ASSERT(!"'str' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_INT:
			ASSERT(!"'int' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_INT64:
			ASSERT(!"'int64' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_FLOAT:
			ASSERT(!"'float' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_FLOAT64:
			ASSERT(!"'float64' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_CAST_BOOL:
			ASSERT(!"'bool' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FUNC_PRINT:
			ASSERT(!"'print' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FUNC_PTR:
			ASSERT(!"'ptr' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FUNC_TYPEOF:
			ASSERT(!"'typeof' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_AND:
			ASSERT(!"'&&' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_OR:
			ASSERT(!"'||' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_NOT:
			ASSERT(!"'!' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_SET:
			ASSERT(!"'=' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FUNC:
			ASSERT(!"'func' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_IF:
			ASSERT(!"'if' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FOR:
			ASSERT(!"'for' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_ADD:
			ASSERT(!"'+' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_SUB:
			ASSERT(!"'-' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_MULT:
			ASSERT(!"'*' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_DIV:
			ASSERT(!"'/' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FLOOR_DIV:
			ASSERT(!"'//' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_MOD:
			ASSERT(!"'%' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_BIT_AND:
			ASSERT(!"'&' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_BIT_OR:
			ASSERT(!"'|' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_BIT_XOR:
			ASSERT(!"'^' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_BIT_NOT:
			ASSERT(!"'~' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_DIV_MOD:
			ASSERT(!"'/%' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_POW:
			ASSERT(!"'**' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_ROOT:
			ASSERT(!"'*/' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FLOOR_ROOT:
			ASSERT(!"'*//' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_LOG:
			ASSERT(!"'_/' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_FLOOR_LOG:
			ASSERT(!"'_//' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_LESS:
			ASSERT(!"'<' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			ASSERT(!"'<=' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_EQUAL:
			ASSERT(!"'==' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			ASSERT(!"'!=' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_MORE:
			ASSERT(!"'>' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			ASSERT(!"'>=' Operator Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			ASSERT(!"Operation List Assembly Code Generation Not Implemented yet",e,UINT32_MAX);
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				lll_debug_object_t* dbg=(lll_debug_object_t*)o;
				uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
				uint32_t off=_write_object_as_assembly(os,LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz),e);
				if (off==UINT32_MAX){
					return off;
				}
				return sz+eoff+off;
			}
		default:
			UNREACHABLE();
	}
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_write_object(lll_output_data_stream_t* os,lll_object_t* o,uint8_t f,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	if (f==LLL_WRITE_MODE_ASSEMBLY){
		LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"bits 64\ndefault rel\nsection .text\nglobal main\nextern _CRT_INIT\nextern ExitProcess\nextern printf\nmain:\n\tpush rbp\n\tmov rbp,rsp\n\tsub rsp,32\n\tcall _CRT_INIT\n");
		if (_write_object_as_assembly(os,o,e)==UINT32_MAX){
			return LLL_RETURN_ERROR;
		}
		LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,"\txor rax,rax\n\tjmp ExitProcess\n");
		return LLL_RETURN_NO_ERROR;
	}
	else{
		if (LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)o,_get_object_size(o))){
			return LLL_RETURN_NO_ERROR;
		}
	}
	e->t=LLL_ERROR_FAILED_FILE_WRITE;
	return LLL_RETURN_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_write_compiled_object(lll_output_data_stream_t* os,lll_compilation_data_t* c_dt,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	uint32_t sz=_get_object_size(c_dt->h);
	compiled_object_file_t dt={
		COMPLIED_OBJECT_FILE_MAGIC_NUMBER,
		sz,
		c_dt->tm,
		c_dt->fpl,
		.ill=c_dt->i_dt.ill
	};
	for (uint32_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		dt.sil[i]=c_dt->i_dt.s[i].l;
	}
	if (!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&dt),sizeof(compiled_object_file_t))||!LLL_WRITE_STRING_TO_OUTPUT_DATA_STREAM(os,c_dt->fp)){
		e->t=LLL_ERROR_FAILED_FILE_WRITE;
		return LLL_RETURN_ERROR;
	}
	for (uint32_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		lll_identifier_list_t* l=c_dt->i_dt.s+i;
		for (uint32_t j=0;j<l->l;j++){
			if (!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&((l->dt+j)->sc)),sizeof(uint32_t))||!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)((l->dt+j)->v),(i+1)*sizeof(char))){
				e->t=LLL_ERROR_FAILED_FILE_WRITE;
				return LLL_RETURN_ERROR;
			}
		}
	}
	for (uint32_t i=0;i<c_dt->i_dt.ill;i++){
		lll_identifier_t* k=*(c_dt->i_dt.il+i);
		if (!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&(k->sz)),sizeof(uint32_t))||!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&(k->sc)),sizeof(uint32_t))||!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(k->v),k->sz*sizeof(char))){
			e->t=LLL_ERROR_FAILED_FILE_WRITE;
			return LLL_RETURN_ERROR;
		}
	}
	if (!LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(c_dt->h),sz)){
		e->t=LLL_ERROR_FAILED_FILE_WRITE;
		return LLL_RETURN_ERROR;
	}
	return LLL_RETURN_NO_ERROR;
}

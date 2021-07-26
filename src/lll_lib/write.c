#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdint.h>
#include <stdlib.h>



#define WRITE_FIELD(o,t,f,os) LLL_WRITE_TO_OUTPUT_DATA_STREAM((os),(uint8_t*)(&(((t*)(o))->f)),sizeof(((t*)(o))->f))
#define WRITE_SIGNED_INTEGER(os,n) _write_integer((os),((n)<0?((~(n))<<1)|1:(n)<<1))



void _write_integer(lll_output_data_stream_t* os,uint64_t v){
	while (v>0x7f){
		LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,(uint8_t)((v&0x7f)|0x80));
		v>>=7;
	}
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,(uint8_t)v);
}



lll_stack_offset_t _write_object(lll_output_data_stream_t* os,const lll_object_t* o){
	lll_stack_offset_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	WRITE_FIELD(o,lll_object_t,t,os);
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_TRUE:
		case LLL_OBJECT_TYPE_FALSE:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			WRITE_FIELD(o,lll_char_object_t,v,os);
			return sizeof(lll_char_object_t)+eoff;
		case LLL_OBJECT_TYPE_INT:
			WRITE_SIGNED_INTEGER(os,((lll_integer_object_t*)o)->v);
			return sizeof(lll_integer_object_t)+eoff;
		case LLL_OBJECT_TYPE_FLOAT:
			WRITE_FIELD(o,lll_float_object_t,v,os);
			return sizeof(lll_float_object_t)+eoff;
		case LLL_OBJECT_TYPE_STRING:
			_write_integer(os,((lll_string_object_t*)o)->i);
			return sizeof(lll_string_object_t)+eoff;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			_write_integer(os,((lll_identifier_object_t*)o)->idx);
			return sizeof(lll_identifier_object_t)+eoff;
		case LLL_OBJECT_TYPE_FUNC:
			{
				lll_stack_offset_t off=sizeof(lll_function_object_t);
				_write_integer(os,((lll_function_object_t*)o)->id);
				lll_arg_count_t l=((lll_function_object_t*)o)->ac;
				LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,l);
				while (l){
					l--;
					off+=_write_object(os,LLL_GET_OBJECT_ARGUMENT(o,off));
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_IMPORT:
			LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,((lll_import_object_t*)o)->ac);
			for (lll_arg_count_t i=0;i<((lll_import_object_t*)o)->ac;i++){
				_write_integer(os,((lll_import_object_t*)o)->idx[i]);
			}
			return sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac+eoff;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				lll_stack_offset_t off=sizeof(lll_operation_list_object_t);
				lll_statement_count_t l=((lll_operation_list_object_t*)o)->sc;
				_write_integer(os,l);
				while (l){
					l--;
					off+=_write_object(os,LLL_GET_OBJECT_STATEMENT(o,off));
				}
				return off+eoff;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				_write_integer(os,((lll_debug_object_t*)o)->fpi);
				_write_integer(os,((lll_debug_object_t*)o)->ln);
				_write_integer(os,((lll_debug_object_t*)o)->cn);
				_write_integer(os,((lll_debug_object_t*)o)->ln_off);
				return sizeof(lll_debug_object_t)+eoff+_write_object(os,LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)o));
			}
	}
	lll_stack_offset_t off=sizeof(lll_operator_object_t);
	lll_arg_count_t l=((lll_operator_object_t*)o)->ac;
	LLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,l);
	while (l){
		l--;
		off+=_write_object(os,LLL_GET_OBJECT_ARGUMENT(o,off));
	}
	return off+eoff;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_write_object(lll_output_data_stream_t* os,const lll_object_t* o,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	_write_object(os,o);
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_write_compiled_object(lll_output_data_stream_t* os,const lll_compilation_data_t* c_dt,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	uint32_t n=COMPLIED_OBJECT_FILE_MAGIC_NUMBER;
	LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&n),sizeof(uint32_t));
	lll_version_t v=LLL_VERSION;
	LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&v),sizeof(lll_version_t));
	_write_integer(os,c_dt->tm);
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		const lll_identifier_list_t* l=c_dt->i_dt.s+i;
		_write_integer(os,l->l);
		for (lll_identifier_list_length_t j=0;j<l->l;j++){
			_write_integer(os,(l->dt+j)->sc);
			_write_integer(os,(l->dt+j)->i);
		}
	}
	_write_integer(os,c_dt->i_dt.ill);
	for (lll_identifier_list_length_t i=0;i<c_dt->i_dt.ill;i++){
		_write_integer(os,(c_dt->i_dt.il+i)->sc);
		_write_integer(os,(c_dt->i_dt.il+i)->i);
	}
	_write_integer(os,c_dt->im.l);
	for (lll_import_index_t i=0;i<c_dt->im.l;i++){
		_write_integer(os,*(c_dt->im.dt+i));
	}
	_write_integer(os,c_dt->f_dt.l);
	for (lll_function_index_t i=0;i<c_dt->f_dt.l;i++){
		const lll_function_t* k=*(c_dt->f_dt.dt+i);
		_write_integer(os,k->off);
		_write_integer(os,k->al);
		for (lll_arg_count_t i=0;i<k->al;i++){
			_write_integer(os,k->a[i]);
		}
	}
	_write_integer(os,c_dt->st.l);
	for (lll_string_index_t i=0;i<c_dt->st.l;i++){
		const lll_string_t* s=*(c_dt->st.dt+i);
		_write_integer(os,s->l);
		LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)s->v,s->l*sizeof(lll_char_t));
	}
	_write_object(os,c_dt->h);
	return LLL_RETURN_NO_ERROR;
}

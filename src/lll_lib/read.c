#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdint.h>
#include <stdlib.h>



#define CHECK_ERROR(is,o,ot,e) \
	do{ \
		int64_t __v=_read_integer(is); \
		if (__v==READ_INTEGER_ERROR){ \
			e->t=LLL_ERROR_INVALID_FILE_FORMAT; \
			return LLL_RETURN_ERROR; \
		} \
		(o)=(ot)__v; \
	} while (0)
#define CHECK_ERROR2(is,o,t,f,ft) \
	do{ \
		int64_t __v=_read_integer(is); \
		if (__v==READ_INTEGER_ERROR){ \
			return 0; \
		} \
		((t*)(o))->f=(ft)__v; \
	} while (0)
#define READ_FIELD(o,t,f,is) \
	do{ \
		if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM((is),(uint8_t*)(&(((t*)(o))->f)),sizeof(((t*)(o))->f))){ \
			return 0; \
		} \
	} while(0)



int64_t _read_integer(lll_input_data_stream_t* is){
	int c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
	if (c==LLL_END_OF_DATA){
		return READ_INTEGER_ERROR;
	}
	uint64_t v=0;
	uint8_t s=0;
	while (c&0x80){
		v|=(c&0x7f)<<s;
		s+=7;
		c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
		if (c==LLL_END_OF_DATA){
			return READ_INTEGER_ERROR;
		}
	}
	v|=c<<s;
	return (v>>1)^(-(v&1));
}



uint8_t _read_object(lll_input_data_stream_t* is){
	lll_object_t* o=(lll_object_t*)(_bf+_bf_off);
	READ_FIELD(o,lll_object_t,t,is);
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_TRUE:
		case LLL_OBJECT_TYPE_FALSE:
			_bf_off+=sizeof(lll_object_t);
			return 1;
		case LLL_OBJECT_TYPE_CHAR:
			READ_FIELD(o,lll_char_object_t,v,is);
			_bf_off+=sizeof(lll_char_object_t);
			return 1;
		case LLL_OBJECT_TYPE_INT:
			CHECK_ERROR2(is,o,lll_integer_object_t,v,int64_t);
			_bf_off+=sizeof(lll_integer_object_t);
			return 1;
		case LLL_OBJECT_TYPE_FLOAT:
			READ_FIELD(o,lll_float_object_t,v,is);
			_bf_off+=sizeof(lll_float_object_t);
			return 1;
		case LLL_OBJECT_TYPE_STRING:
			CHECK_ERROR2(is,o,lll_string_object_t,ln,lll_string_length_t);
			if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(((lll_string_object_t*)o)->v),((lll_string_object_t*)o)->ln*sizeof(char))){
				return 0;
			}
			_bf_off+=sizeof(lll_string_object_t)+((lll_string_object_t*)o)->ln*sizeof(char);
			return 1;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			CHECK_ERROR2(is,o,lll_identifier_object_t,idx,lll_identifier_index_t);
			_bf_off+=sizeof(lll_identifier_object_t);
			return 1;
		case LLL_OBJECT_TYPE_FUNC:
			CHECK_ERROR2(is,o,lll_function_object_t,id,lll_function_index_t);
			CHECK_ERROR2(is,o,lll_function_object_t,ac,lll_arg_count_t);
			_bf_off+=sizeof(lll_function_object_t);
			for (lll_arg_count_t i=0;i<((lll_function_object_t*)o)->ac;i++){
				if (!_read_object(is)){
					return 0;
				}
			}
			return 1;
		case LLL_OBJECT_TYPE_IMPORT:
			CHECK_ERROR2(is,o,lll_import_object_t,ac,lll_arg_count_t);
			for (lll_arg_count_t i=0;i<((lll_import_object_t*)o)->ac;i++){
				int64_t v=_read_integer(is);
				if (v==READ_INTEGER_ERROR){
					return 0;
				}
				((lll_import_object_t*)o)->idx[i]=(lll_import_index_t)v;
			}
			_bf_off+=sizeof(lll_import_object_t)+sizeof(lll_import_index_t)*((lll_import_object_t*)o)->ac;
			return 1;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			{
				CHECK_ERROR2(is,o,lll_operation_list_object_t,sc,lll_statement_count_t);
				_bf_off+=sizeof(lll_operation_list_object_t);
				for (lll_statement_count_t i=0;i<((lll_operation_list_object_t*)o)->sc;i++){
					if (!_read_object(is)){
						return 0;
					}
				}
				return 1;
			}
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			{
				CHECK_ERROR2(is,o,lll_debug_object_t,fpi,uint16_t);
				CHECK_ERROR2(is,o,lll_debug_object_t,ln,uint32_t);
				CHECK_ERROR2(is,o,lll_debug_object_t,cn,uint32_t);
				CHECK_ERROR2(is,o,lll_debug_object_t,ln_off,uint32_t);
				_bf_off+=sizeof(lll_debug_object_t);
				return _read_object(is);
			}
	}
	CHECK_ERROR2(is,o,lll_operator_object_t,ac,lll_arg_count_t);
	_bf_off+=sizeof(lll_operator_object_t);
	for (lll_arg_count_t i=0;i<((lll_function_object_t*)o)->ac;i++){
		if (!_read_object(is)){
			return 0;
		}
	}
	return 1;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_load_compiled_object(lll_input_data_stream_t* is,lll_compilation_data_t* c_dt,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	uint32_t n;
	if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(&n),sizeof(n))||n!=COMPLIED_OBJECT_FILE_MAGIC_NUMBER){
		e->t=LLL_ERROR_INVALID_FILE_FORMAT;
		return LLL_RETURN_ERROR;
	}
	c_dt->is=NULL;
	CHECK_ERROR(is,c_dt->tm,uint64_t,e);
	for (uint32_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		CHECK_ERROR(is,c_dt->i_dt.s[i].l,uint32_t,e);
	}
	CHECK_ERROR(is,c_dt->i_dt.ill,uint32_t,e);
	CHECK_ERROR(is,c_dt->im.l,lll_import_index_t,e);
	CHECK_ERROR(is,c_dt->fp_dt.l,uint16_t,e);
	CHECK_ERROR(is,c_dt->f_dt.l,lll_function_index_t,e);
	c_dt->fp_dt.dt=malloc(c_dt->fp_dt.l*sizeof(lll_file_path_t));
	for (uint32_t i=0;i<c_dt->fp_dt.l;i++){
		lll_file_path_t* ifp=c_dt->fp_dt.dt+i;
		CHECK_ERROR(is,ifp->l,uint32_t,e);
		if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(ifp->fp),ifp->l*sizeof(char))){
			e->t=LLL_ERROR_INVALID_FILE_FORMAT;
			return LLL_RETURN_ERROR;
		}
		ifp->fp[ifp->l]=0;
	}
	for (uint32_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		c_dt->i_dt.s[i].dt=malloc(c_dt->i_dt.s[i].l*sizeof(lll_small_identifier_t));
		for (uint32_t j=0;j<c_dt->i_dt.s[i].l;j++){
			lll_small_identifier_t* si=c_dt->i_dt.s[i].dt+j;
			si->v=malloc((i+1)*sizeof(char));
			CHECK_ERROR(is,si->sc,uint32_t,e);
			if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(si->v),(i+1)*sizeof(char))){
				e->t=LLL_ERROR_INVALID_FILE_FORMAT;
				return LLL_RETURN_ERROR;
			}
		}
	}
	c_dt->i_dt.il=malloc(c_dt->i_dt.ill*sizeof(lll_identifier_t*));
	for (uint32_t i=0;i<c_dt->i_dt.ill;i++){
		uint32_t sz;
		CHECK_ERROR(is,sz,uint32_t,e);
		lll_identifier_t* k=malloc(sizeof(lll_identifier_t)+sz*sizeof(char));
		k->sz=sz;
		CHECK_ERROR(is,k->sc,uint32_t,e);
		if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(k->v),sz*sizeof(char))){
			e->t=LLL_ERROR_INVALID_FILE_FORMAT;
			return LLL_RETURN_ERROR;
		}
		*(c_dt->i_dt.il+i)=k;
	}
	c_dt->im.dt=malloc(c_dt->im.l*sizeof(lll_import_data_path_t));
	for (uint32_t i=0;i<c_dt->im.l;i++){
		CHECK_ERROR(is,(c_dt->im.dt+i)->sz,uint32_t,e);
		if ((c_dt->im.dt+i)->sz==UINT32_MAX){
			(c_dt->im.dt+i)->nm=NULL;
		}
		else{
			(c_dt->im.dt+i)->nm=malloc((c_dt->im.dt+i)->sz*sizeof(char));
			if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)((c_dt->im.dt+i)->nm),(c_dt->im.dt+i)->sz*sizeof(char))){
				e->t=LLL_ERROR_INVALID_FILE_FORMAT;
				return LLL_RETURN_ERROR;
			}
		}
	}
	c_dt->f_dt.dt=malloc(c_dt->f_dt.l*sizeof(lll_function_t*));
	for (uint16_t i=0;i<c_dt->f_dt.l;i++){
		uint32_t off;
		CHECK_ERROR(is,off,uint32_t,e);
		lll_arg_count_t al;
		CHECK_ERROR(is,al,lll_arg_count_t,e);
		lll_function_t* k=malloc(sizeof(lll_function_t)+al*sizeof(lll_identifier_index_t));
		k->off=off;
		k->al=al;
		for (lll_arg_count_t i=0;i<al;i++){
			CHECK_ERROR(is,k->a[i],lll_identifier_index_t,e);
		}
		*(c_dt->f_dt.dt+i)=k;
	}
	c_dt->h=(lll_object_t*)(_bf+_bf_off);
	if (!_read_object(is)){
		e->t=LLL_ERROR_INVALID_FILE_FORMAT;
		return LLL_RETURN_ERROR;
	}
	return LLL_RETURN_NO_ERROR;
}

#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdint.h>
#include <stdlib.h>



#define CHECK_ERROR(is,o,ot,e) \
	do{ \
		uint8_t __e=0; \
		(o)=(ot)_read_integer(is,&__e); \
		if (__e){ \
			e->t=LLL_ERROR_INVALID_FILE_FORMAT; \
			return LLL_RETURN_ERROR; \
		} \
	} while (0)
#define CHECK_ERROR2(is,f,ft) \
	do{ \
		uint8_t __e=0; \
		(f)=(ft)_read_integer(is,&__e); \
		if (__e){ \
			return 0; \
		} \
	} while (0)
#define READ_FIELD(f,is) \
	do{ \
		if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM((is),(uint8_t*)(&(f)),sizeof((f)))){ \
			return 0; \
		} \
	} while(0)



uint64_t _read_integer(lll_input_data_stream_t* is,uint8_t* e){
	int c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
	if (c==LLL_END_OF_DATA){
		*e=1;
		return 0;
	}
	uint64_t v=0;
	uint8_t s=0;
	while (c&0x80){
		v|=((uint64_t)(c&0x7f))<<s;
		s+=7;
		c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
		if (c==LLL_END_OF_DATA){
			*e=1;
			return 0;
		}
	}
	return v|(((uint64_t)c)<<s);
}



int64_t _read_signed_integer(lll_input_data_stream_t* is,uint8_t* e){
	uint64_t v=_read_integer(is,e);
	if (*e){
		return 0;
	}
	return (v>>1)^(-((int64_t)(v&1)));
}



uint8_t _read_object(lll_compilation_data_t* c_dt,lll_input_data_stream_t* is){
	lll_object_t* o=(lll_object_t*)(c_dt->_s.ptr+c_dt->_s.off);
	c_dt->_s.off+=sizeof(lll_object_t);
	READ_FIELD(o->t,is);
	while (o->t==LLL_OBJECT_TYPE_NOP){
		o++;
		c_dt->_s.off+=sizeof(lll_object_t);
		READ_FIELD(o->t,is);
	}
	switch (o->t){
		case LLL_OBJECT_TYPE_UNKNOWN:
			return 1;
		case LLL_OBJECT_TYPE_CHAR:
			READ_FIELD(o->dt.c,is);
			return 1;
		case LLL_OBJECT_TYPE_INT:
			{
				uint8_t e=0;
				o->dt.i=(lll_integer_t)_read_signed_integer(is,&e);
				if (e){
					return 0;
				}
				return 1;
			}
		case LLL_OBJECT_TYPE_FLOAT:
			READ_FIELD(o->dt.f,is);
			return 1;
		case LLL_OBJECT_TYPE_STRING:
			CHECK_ERROR2(is,o->dt.s,lll_string_index_t);
			return 1;
		case LLL_OBJECT_TYPE_IDENTIFIER:
			CHECK_ERROR2(is,o->dt.id,lll_identifier_index_t);
			return 1;
		case LLL_OBJECT_TYPE_FUNC:
		case LLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				CHECK_ERROR2(is,o->dt.fn.id,lll_function_index_t);
				int c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c==LLL_END_OF_DATA){
					return 0;
				}
				o->dt.fn.ac=(lll_arg_count_t)c;
				for (lll_arg_count_t i=0;i<o->dt.fn.ac;i++){
					if (!_read_object(c_dt,is)){
						return 0;
					}
				}
				return 1;
			}
		case LLL_OBJECT_TYPE_IMPORT:
			CHECK_ERROR2(is,o->dt.ii,lll_import_index_t);
			return 1;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			CHECK_ERROR2(is,o->dt.sc,lll_statement_count_t);
			for (lll_statement_count_t i=0;i<o->dt.sc;i++){
				if (!_read_object(c_dt,is)){
					return 0;
				}
			}
			return 1;
		case LLL_OBJECT_TYPE_DEBUG_DATA:
			CHECK_ERROR2(is,o->dt.dbg.fpi,lll_file_path_index_t);
			CHECK_ERROR2(is,o->dt.dbg.ln,lll_line_number_t);
			CHECK_ERROR2(is,o->dt.dbg.cn,lll_column_number_t);
			CHECK_ERROR2(is,o->dt.dbg.ln_off,lll_file_offset_t);
			return _read_object(c_dt,is);
	}
	int c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
	if (c==LLL_END_OF_DATA){
		return 0;
	}
	o->dt.ac=(lll_arg_count_t)c;
	for (lll_arg_count_t i=0;i<o->dt.fn.ac;i++){
		if (!_read_object(c_dt,is)){
			return 0;
		}
	}
	return 1;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_load_assembly(lll_input_data_stream_t* is,lll_assembly_data_t* a_dt,lll_error_t* e){
	if (!a_dt->_s.ptr){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	uint32_t n;
	lll_version_t v;
	if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(&n),sizeof(uint32_t))||n!=ASSEMBLY_FILE_MAGIC_NUMBER||!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(&v),sizeof(lll_version_t))||v!=LLL_VERSION){
		e->t=LLL_ERROR_INVALID_FILE_FORMAT;
		return LLL_RETURN_ERROR;
	}
	CHECK_ERROR(is,a_dt->tm,lll_time_t,e);
	CHECK_ERROR(is,a_dt->ic,lll_instruction_index_t,e);
	CHECK_ERROR(is,a_dt->vc,lll_variable_index_t,e);
	CHECK_ERROR(is,a_dt->ft.l,lll_function_index_t,e);
	a_dt->ft.dt=malloc(a_dt->ft.l*sizeof(lll_instruction_index_t));
	for (lll_function_index_t i=0;i<a_dt->ft.l;i++){
		CHECK_ERROR(is,*(a_dt->ft.dt+i),lll_instruction_index_t,e);
	}
	CHECK_ERROR(is,a_dt->st.l,lll_string_index_t,e);
	a_dt->st.dt=malloc(a_dt->st.l*sizeof(lll_string_t*));
	for (lll_string_index_t i=0;i<a_dt->st.l;i++){
		lll_string_length_t l;
		CHECK_ERROR(is,l,lll_string_length_t,e);
		lll_string_t* s=malloc(sizeof(lll_string_t)+(l+1)*sizeof(lll_char_t));
		s->l=l;
		s->c=0;
		if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)s->v,s->l*sizeof(lll_char_t))){
			e->t=LLL_ERROR_INVALID_FILE_FORMAT;
			return LLL_RETURN_ERROR;
		}
		s->v[s->l]=0;
		for (lll_string_length_t j=0;j<s->l;j++){
			s->c^=(lll_string_checksum_t)(s->v[j]);
		}
		*(a_dt->st.dt+i)=s;
	}
	a_dt->h=(lll_assembly_instruction_t*)(a_dt->_s.ptr+a_dt->_s.off);
	lll_assembly_instruction_t* ai=a_dt->h;
	for (lll_instruction_index_t i=0;i<a_dt->ic;i++){
		int c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
		if (c==LLL_END_OF_DATA){
			e->t=LLL_ERROR_INVALID_FILE_FORMAT;
			return LLL_RETURN_ERROR;
		}
		ai->t=(lll_assembly_instruction_type_t)c;
		switch (LLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				{
					uint8_t re=0;
					ai->dt.i=(lll_integer_t)_read_signed_integer(is,&re);
					if (re){
						return 0;
					}
					break;
				}
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(&(ai->dt.f)),sizeof(lll_float_t))){
					e->t=LLL_ERROR_INVALID_FILE_FORMAT;
					return LLL_RETURN_ERROR;
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c==LLL_END_OF_DATA){
					e->t=LLL_ERROR_INVALID_FILE_FORMAT;
					return LLL_RETURN_ERROR;
				}
				ai->dt.c=(lll_char_t)c;
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_INC:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_DEC:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
				CHECK_ERROR(is,ai->dt.v,lll_variable_index_t,e);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				CHECK_ERROR(is,ai->dt.s,lll_string_index_t,e);
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					uint8_t re=0;
					ai->dt.i=(lll_relative_instruction_index_t)_read_signed_integer(is,&re);
					if (re){
						return 0;
					}
				}
				else{
					CHECK_ERROR(is,ai->dt.rj,lll_instruction_index_t,e);
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_ADD:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_SUB:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_MULT:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_DIV:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_MOD:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_AND:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_OR:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_XOR:
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_INV:
				if (LLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					CHECK_ERROR(is,ai->dt.v,lll_variable_index_t,e);
				}
				break;
			case LLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
				c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c==LLL_END_OF_DATA){
					e->t=LLL_ERROR_INVALID_FILE_FORMAT;
					return LLL_RETURN_ERROR;
				}
				ai->dt.ac=(lll_arg_count_t)c;
				break;
		}
		ai++;
	}
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_load_compiled_object(lll_input_data_stream_t* is,lll_compilation_data_t* c_dt,lll_error_t* e){
	if (!c_dt->_s.ptr){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	uint32_t n;
	lll_version_t v;
	if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(&n),sizeof(uint32_t))||n!=COMPLIED_OBJECT_FILE_MAGIC_NUMBER||!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(&v),sizeof(lll_version_t))||v!=LLL_VERSION){
		e->t=LLL_ERROR_INVALID_FILE_FORMAT;
		return LLL_RETURN_ERROR;
	}
	c_dt->is=NULL;
	CHECK_ERROR(is,c_dt->tm,lll_time_t,e);
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		CHECK_ERROR(is,c_dt->i_dt.s[i].l,lll_identifier_list_length_t,e);
		c_dt->i_dt.s[i].dt=malloc(c_dt->i_dt.s[i].l*sizeof(lll_identifier_t));
		for (lll_identifier_list_length_t j=0;j<c_dt->i_dt.s[i].l;j++){
			CHECK_ERROR(is,(c_dt->i_dt.s[i].dt+j)->sc,lll_scope_t,e);
			CHECK_ERROR(is,(c_dt->i_dt.s[i].dt+j)->i,lll_string_index_t,e);
		}
	}
	CHECK_ERROR(is,c_dt->i_dt.ill,lll_identifier_list_length_t,e);
	c_dt->i_dt.il=malloc(c_dt->i_dt.ill*sizeof(lll_identifier_t));
	for (lll_identifier_list_length_t i=0;i<c_dt->i_dt.ill;i++){
		CHECK_ERROR(is,(c_dt->i_dt.il+i)->sc,lll_scope_t,e);
		CHECK_ERROR(is,(c_dt->i_dt.il+i)->i,lll_string_index_t,e);
	}
	CHECK_ERROR(is,c_dt->im.l,lll_import_index_t,e);
	c_dt->im.dt=malloc(c_dt->im.l*sizeof(lll_string_index_t));
	for (lll_import_index_t i=0;i<c_dt->im.l;i++){
		CHECK_ERROR(is,*(c_dt->im.dt+i),lll_string_index_t,e);
	}
	CHECK_ERROR(is,c_dt->f_dt.l,lll_function_index_t,e);
	c_dt->f_dt.dt=malloc(c_dt->f_dt.l*sizeof(lll_function_t*));
	for (lll_function_index_t i=0;i<c_dt->f_dt.l;i++){
		lll_stack_offset_t off;
		CHECK_ERROR(is,off,lll_stack_offset_t,e);
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
	CHECK_ERROR(is,c_dt->st.l,lll_string_index_t,e);
	c_dt->st.dt=malloc(c_dt->st.l*sizeof(lll_string_t*));
	for (lll_string_index_t i=0;i<c_dt->st.l;i++){
		lll_string_length_t l;
		CHECK_ERROR(is,l,lll_string_length_t,e);
		lll_string_t* s=malloc(sizeof(lll_string_t)+(l+1)*sizeof(lll_char_t));
		s->l=l;
		s->c=0;
		if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)s->v,s->l*sizeof(lll_char_t))){
			e->t=LLL_ERROR_INVALID_FILE_FORMAT;
			return LLL_RETURN_ERROR;
		}
		s->v[s->l]=0;
		for (lll_string_length_t j=0;j<s->l;j++){
			s->c^=(lll_string_checksum_t)(s->v[j]);
		}
		*(c_dt->st.dt+i)=s;
	}
	CHECK_ERROR(is,c_dt->_n_sc_id,lll_scope_t,e);
	c_dt->h=(lll_object_t*)(c_dt->_s.ptr+c_dt->_s.off);
	if (!_read_object(c_dt,is)){
		e->t=LLL_ERROR_INVALID_FILE_FORMAT;
		return LLL_RETURN_ERROR;
	}
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_load_object(lll_compilation_data_t* c_dt,lll_input_data_stream_t* is,lll_object_t** o,lll_error_t* e){
	if (!c_dt->_s.ptr){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	*o=(lll_object_t*)(c_dt->_s.ptr+c_dt->_s.off);
	if (!_read_object(c_dt,is)){
		e->t=LLL_ERROR_INVALID_FILE_FORMAT;
		return LLL_RETURN_ERROR;
	}
	return LLL_RETURN_NO_ERROR;
}

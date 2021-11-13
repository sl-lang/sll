#include <sll/_sll_internal.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/stream.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/version.h>
#include <stdint.h>



#define CHECK_ERROR(is,o,ot,e) \
	do{ \
		uint8_t __e=0; \
		(o)=(ot)_read_integer(is,&__e); \
		if (__e){ \
			e->t=SLL_ERROR_INVALID_FILE_FORMAT; \
			return SLL_RETURN_ERROR; \
		} \
	} while (0)
#define CHECK_ERROR2(is,f,ft) \
	do{ \
		uint8_t __e=0; \
		(f)=(ft)_read_integer(is,&__e); \
		if (__e){ \
			return SLL_RETURN_ERROR; \
		} \
	} while (0)
#define READ_FIELD(f,is) \
	do{ \
		if (SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM((is),SLL_CHAR(&(f)),sizeof((f)))==SLL_END_OF_DATA){ \
			return SLL_RETURN_ERROR; \
		} \
	} while(0)



static uint64_t _read_integer(sll_input_data_stream_t* is,uint8_t* e){
	sll_read_char_t c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
	if (c==SLL_END_OF_DATA){
		*e=1;
		return 0;
	}
	uint64_t v=0;
	uint8_t s=0;
	while (c&0x80){
		v|=((uint64_t)(c&0x7f))<<s;
		s+=7;
		c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
		if (c==SLL_END_OF_DATA){
			*e=1;
			return 0;
		}
	}
	return v|(((uint64_t)c)<<s);
}



static int64_t _read_signed_integer(sll_input_data_stream_t* is,uint8_t* e){
	uint64_t v=_read_integer(is,e);
	if (*e){
		return 0;
	}
	return (v>>1)^(-((int64_t)(v&1)));
}



static uint8_t _read_object(sll_compilation_data_t* c_dt,sll_input_data_stream_t* is){
	sll_object_t* o=_acquire_next_object(c_dt);
	READ_FIELD(o->t,is);
	while (o->t==SLL_OBJECT_TYPE_NOP){
		o=_acquire_next_object(c_dt);
		READ_FIELD(o->t,is);
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
			return 1;
		case SLL_OBJECT_TYPE_CHAR:
			READ_FIELD(o->dt.c,is);
			return 1;
		case SLL_OBJECT_TYPE_INT:
			{
				uint8_t e=0;
				o->dt.i=(sll_integer_t)_read_signed_integer(is,&e);
				if (e){
					return 0;
				}
				return 1;
			}
		case SLL_OBJECT_TYPE_FLOAT:
			READ_FIELD(o->dt.f,is);
			return 1;
		case SLL_OBJECT_TYPE_STRING:
			CHECK_ERROR2(is,o->dt.s,sll_string_index_t);
			return 1;
		case SLL_OBJECT_TYPE_ARRAY:
			CHECK_ERROR2(is,o->dt.al,sll_array_length_t);
			for (sll_array_length_t i=0;i<o->dt.al;i++){
				if (!_read_object(c_dt,is)){
					return 0;
				}
			}
			return 1;
		case SLL_OBJECT_TYPE_MAP:
			CHECK_ERROR2(is,o->dt.ml,sll_map_length_t);
			for (sll_map_length_t i=0;i<o->dt.ml;i++){
				if (!_read_object(c_dt,is)){
					return 0;
				}
			}
			return 1;
		case SLL_OBJECT_TYPE_IDENTIFIER:
			CHECK_ERROR2(is,o->dt.id,sll_identifier_index_t);
			return 1;
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			CHECK_ERROR2(is,o->dt.fn_id,sll_function_index_t);
			return 1;
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				CHECK_ERROR2(is,o->dt.fn.ac,sll_arg_count_t);
				CHECK_ERROR2(is,o->dt.fn.id,sll_function_index_t);
				if (o->t==SLL_OBJECT_TYPE_FUNC){
					CHECK_ERROR2(is,o->dt.fn.sc,sll_scope_t);
				}
				for (sll_arg_count_t i=0;i<o->dt.fn.ac;i++){
					if (!_read_object(c_dt,is)){
						return 0;
					}
				}
				return 1;
			}
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_WHILE:
		case SLL_OBJECT_TYPE_LOOP:
			CHECK_ERROR2(is,o->dt.l.ac,sll_arg_count_t);
			CHECK_ERROR2(is,o->dt.l.sc,sll_scope_t);
			for (sll_arg_count_t i=0;i<o->dt.l.ac;i++){
				if (!_read_object(c_dt,is)){
					return 0;
				}
			}
			return 1;
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			CHECK_ERROR2(is,o->dt.dbg.fpi,sll_file_path_index_t);
			CHECK_ERROR2(is,o->dt.dbg.ln,sll_line_number_t);
			CHECK_ERROR2(is,o->dt.dbg.cn,sll_column_number_t);
			CHECK_ERROR2(is,o->dt.dbg.ln_off,sll_file_offset_t);
			return _read_object(c_dt,is);
	}
	CHECK_ERROR2(is,o->dt.ac,sll_arg_count_t);
	for (sll_arg_count_t i=0;i<o->dt.ac;i++){
		if (!_read_object(c_dt,is)){
			return 0;
		}
	}
	return 1;
}



static sll_return_t _read_string(sll_input_data_stream_t* is,sll_string_t* o,sll_error_t* err){
	CHECK_ERROR(is,o->l,sll_string_length_t,err);
	sll_string_create(o->l,o);
	if (o->l<STRING_COMPRESSION_MIN_LENGTH){
		if (SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,o->v,o->l*sizeof(sll_char_t))==SLL_END_OF_DATA){
			err->t=SLL_ERROR_INVALID_FILE_FORMAT;
			return SLL_RETURN_ERROR;
		}
	}
	else{
		sll_char_t bf[1<<STRING_COMPRESSION_OFFSET_BIT_COUNT];
		sll_set_memory(bf,((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1),0xff);
		uint64_t v;
		if (SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(void*)(&v),sizeof(uint64_t))==SLL_END_OF_DATA){
			return SLL_RETURN_ERROR;
		}
		uint8_t bc=64;
		sll_string_length_t i=0;
		uint16_t r=((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1);
		do{
			if (!bc){
				if (SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(void*)(&v),sizeof(uint64_t))==SLL_END_OF_DATA){
					return SLL_RETURN_ERROR;
				}
				bc=64;
			}
			bc--;
			uint16_t e;
			uint8_t el=((v&(1ull<<bc))?8:STRING_COMPRESSION_OFFSET_BIT_COUNT+STRING_COMPRESSION_LENGTH_BIT_COUNT);
			if (bc<el){
				e=(v&((1<<bc)-1))<<(el-bc);
				if (SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(void*)(&v),sizeof(uint64_t))==SLL_END_OF_DATA){
					return SLL_RETURN_ERROR;
				}
				bc+=64-el;
				e|=v>>bc;
			}
			else{
				bc-=el;
				e=(v>>bc)&((1<<el)-1);
			}
			if (el==8){
				o->v[i]=(sll_char_t)e;
				bf[r]=(sll_char_t)e;
				i++;
				r=(r+1)&((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-1);
			}
			else{
				uint16_t k=e>>STRING_COMPRESSION_LENGTH_BIT_COUNT;
				uint16_t l=k+(e&((1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1))+2;
				do{
					bf[r]=bf[k&((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-1)];
					o->v[i]=bf[r];
					i++;
					r=(r+1)&((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-1);
					k++;
				} while (k<l);
			}
		} while (i<o->l);
	}
	sll_string_calculate_checksum(o);
	return SLL_RETURN_NO_ERROR;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_load_assembly(sll_input_data_stream_t* is,sll_assembly_data_t* a_dt,sll_error_t* e){
	uint32_t n;
	sll_version_t v;
	if (SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(&n),sizeof(uint32_t))==SLL_END_OF_DATA||n!=ASSEMBLY_FILE_MAGIC_NUMBER||SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(&v),sizeof(sll_version_t))==SLL_END_OF_DATA||v!=SLL_VERSION){
		e->t=SLL_ERROR_INVALID_FILE_FORMAT;
		return SLL_RETURN_ERROR;
	}
	CHECK_ERROR(is,a_dt->tm,sll_time_t,e);
	CHECK_ERROR(is,a_dt->ic,sll_instruction_index_t,e);
	CHECK_ERROR(is,a_dt->vc,sll_variable_index_t,e);
	CHECK_ERROR(is,a_dt->ft.l,sll_function_index_t,e);
	a_dt->ft.dt=sll_allocate(a_dt->ft.l*sizeof(sll_instruction_index_t));
	for (sll_function_index_t i=0;i<a_dt->ft.l;i++){
		CHECK_ERROR(is,*(a_dt->ft.dt+i),sll_instruction_index_t,e);
	}
	CHECK_ERROR(is,a_dt->st.l,sll_string_index_t,e);
	a_dt->st.dt=sll_allocate(a_dt->st.l*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<a_dt->st.l;i++){
		if (!_read_string(is,a_dt->st.dt+i,e)){
			return SLL_RETURN_ERROR;
		}
	}
	_init_assembly_stack(a_dt);
	a_dt->h=a_dt->_s.p;
	sll_instruction_index_t i=a_dt->ic;
	a_dt->ic=0;
	while (i){
		i--;
		sll_assembly_instruction_t* ai=_acquire_next_instruction(a_dt);
		sll_read_char_t c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
		if (c==SLL_END_OF_DATA){
			e->t=SLL_ERROR_INVALID_FILE_FORMAT;
			return SLL_RETURN_ERROR;
		}
		ai->t=(sll_assembly_instruction_type_t)c;
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				{
					uint8_t re=0;
					ai->dt.i=(sll_integer_t)_read_signed_integer(is,&re);
					if (re){
						return SLL_RETURN_ERROR;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				if (SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(&(ai->dt.f)),sizeof(sll_float_t))==SLL_END_OF_DATA){
					e->t=SLL_ERROR_INVALID_FILE_FORMAT;
					return SLL_RETURN_ERROR;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c==SLL_END_OF_DATA){
					e->t=SLL_ERROR_INVALID_FILE_FORMAT;
					return SLL_RETURN_ERROR;
				}
				ai->dt.c=(sll_char_t)c;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JSNE:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					uint8_t re=0;
					ai->dt.i=(sll_relative_instruction_index_t)_read_signed_integer(is,&re);
					if (re){
						return SLL_RETURN_ERROR;
					}
				}
				else{
					CHECK_ERROR(is,ai->dt.rj,sll_instruction_index_t,e);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_POP:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_MINUS_ONE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ZERO:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_ONE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_TWO:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_THREE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_STORE_FOUR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_VAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_VAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOAD_DEL:
				CHECK_ERROR(is,ai->dt.v,sll_variable_index_t,e);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				CHECK_ERROR(is,ai->dt.s,sll_string_index_t,e);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				CHECK_ERROR(is,ai->dt.al,sll_array_length_t,e);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				CHECK_ERROR(is,ai->dt.ml,sll_map_length_t,e);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NOT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INC:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DEC:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ADD:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_SUB:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MULT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DIV:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_FDIV:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MOD:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_AND:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_OR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_XOR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_INV:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_INPLACE(ai)){
					CHECK_ERROR(is,ai->dt.v,sll_variable_index_t,e);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c==SLL_END_OF_DATA){
					e->t=SLL_ERROR_INVALID_FILE_FORMAT;
					return SLL_RETURN_ERROR;
				}
				ai->dt.t=(sll_constant_type_t)c;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				CHECK_ERROR(is,ai->dt.ac,sll_arg_count_t,e);
				break;
		}
	}
	return SLL_RETURN_NO_ERROR;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_load_compiled_object(sll_input_data_stream_t* is,sll_compilation_data_t* c_dt,sll_error_t* e){
	uint32_t n=0;
	sll_version_t v=0;
	if (SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(&n),sizeof(uint32_t))==SLL_END_OF_DATA||n!=COMPLIED_OBJECT_FILE_MAGIC_NUMBER||SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(&v),sizeof(sll_version_t))==SLL_END_OF_DATA||v!=SLL_VERSION){
		e->t=SLL_ERROR_INVALID_FILE_FORMAT;
		return SLL_RETURN_ERROR;
	}
	c_dt->is=NULL;
	CHECK_ERROR(is,c_dt->tm,sll_time_t,e);
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		CHECK_ERROR(is,c_dt->idt.s[i].l,sll_identifier_list_length_t,e);
		c_dt->idt.s[i].dt=sll_allocate(c_dt->idt.s[i].l*sizeof(sll_identifier_t));
		for (sll_identifier_list_length_t j=0;j<c_dt->idt.s[i].l;j++){
			CHECK_ERROR(is,(c_dt->idt.s[i].dt+j)->sc,sll_scope_t,e);
			CHECK_ERROR(is,(c_dt->idt.s[i].dt+j)->i,sll_string_index_t,e);
		}
	}
	CHECK_ERROR(is,c_dt->idt.ill,sll_identifier_list_length_t,e);
	c_dt->idt.il=sll_allocate(c_dt->idt.ill*sizeof(sll_identifier_t));
	for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		CHECK_ERROR(is,(c_dt->idt.il+i)->sc,sll_scope_t,e);
		CHECK_ERROR(is,(c_dt->idt.il+i)->i,sll_string_index_t,e);
	}
	CHECK_ERROR(is,c_dt->et.l,sll_export_table_length_t,e);
	c_dt->et.dt=sll_allocate(c_dt->et.l*sizeof(sll_identifier_index_t));
	for (sll_export_table_length_t i=0;i<c_dt->et.l;i++){
		CHECK_ERROR(is,*(c_dt->et.dt+i),sll_identifier_index_t,e);
	}
	CHECK_ERROR(is,c_dt->ft.l,sll_function_index_t,e);
	c_dt->ft.dt=sll_allocate(c_dt->ft.l*sizeof(sll_function_t*));
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		sll_object_offset_t off;
		CHECK_ERROR(is,off,sll_object_offset_t,e);
		sll_arg_count_t al;
		CHECK_ERROR(is,al,sll_arg_count_t,e);
		sll_function_t* k=sll_allocate(sizeof(sll_function_t)+al*sizeof(sll_identifier_index_t));
		k->off=off;
		k->al=al;
		for (sll_arg_count_t j=0;j<al;j++){
			CHECK_ERROR(is,k->a[j],sll_identifier_index_t,e);
		}
		*(c_dt->ft.dt+i)=k;
	}
	CHECK_ERROR(is,c_dt->st.l,sll_string_index_t,e);
	c_dt->st.dt=sll_allocate(c_dt->st.l*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<c_dt->st.l;i++){
		if (!_read_string(is,c_dt->st.dt+i,e)){
			return SLL_RETURN_ERROR;
		}
	}
	CHECK_ERROR(is,c_dt->_n_sc_id,sll_scope_t,e);
	_init_object_stack(c_dt);
	c_dt->h=c_dt->_s.p;
	SLL_ASSERT(c_dt->h);
	if (!_read_object(c_dt,is)){
		e->t=SLL_ERROR_INVALID_FILE_FORMAT;
		return SLL_RETURN_ERROR;
	}
	return SLL_RETURN_NO_ERROR;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_load_object(sll_compilation_data_t* c_dt,sll_input_data_stream_t* is,sll_object_t** o,sll_error_t* e){
	*o=c_dt->_s.p;
	SLL_ASSERT(*o);
	if (!_read_object(c_dt,is)){
		e->t=SLL_ERROR_INVALID_FILE_FORMAT;
		return SLL_RETURN_ERROR;
	}
	return SLL_RETURN_NO_ERROR;
}

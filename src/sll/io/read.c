#include <sll/_sll_internal.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/version.h>
#include <stdint.h>



#define CHECK_ERROR(rf,o,ot,e) \
	do{ \
		sll_bool_t __e=0; \
		(o)=(ot)_read_integer(rf,&__e); \
		if (__e){ \
			e->t=SLL_ERROR_INVALID_FILE_FORMAT; \
			return 0; \
		} \
	} while (0)
#define CHECK_ERROR2(rf,f,ft) \
	do{ \
		sll_bool_t __e=0; \
		(f)=(ft)_read_integer(rf,&__e); \
		if (__e){ \
			return 0; \
		} \
	} while (0)
#define READ_FIELD(f,rf) \
	do{ \
		if (sll_file_read((rf),SLL_CHAR(&(f)),sizeof((f)))==SLL_END_OF_DATA){ \
			return 0; \
		} \
	} while(0)



static uint64_t _read_integer(sll_file_t* rf,sll_bool_t* e){
	sll_read_char_t c=sll_file_read_char(rf);
	if (c==SLL_END_OF_DATA){
		*e=1;
		return 0;
	}
	uint64_t v=0;
	uint8_t s=0;
	while (c&0x80){
		v|=((uint64_t)(c&0x7f))<<s;
		s+=7;
		c=sll_file_read_char(rf);
		if (c==SLL_END_OF_DATA){
			*e=1;
			return 0;
		}
	}
	return v|(((uint64_t)c)<<s);
}



static int64_t _read_signed_integer(sll_file_t* rf,sll_bool_t* e){
	uint64_t v=_read_integer(rf,e);
	if (*e){
		return 0;
	}
	return (v>>1)^(-((int64_t)(v&1)));
}



static uint8_t _read_object(sll_compilation_data_t* c_dt,sll_file_t* rf){
	sll_object_t* o=_acquire_next_object(c_dt);
	READ_FIELD(o->t,rf);
	while (o->t==SLL_OBJECT_TYPE_NOP){
		o=_acquire_next_object(c_dt);
		READ_FIELD(o->t,rf);
	}
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
			return 1;
		case SLL_OBJECT_TYPE_CHAR:
			READ_FIELD(o->dt.c,rf);
			return 1;
		case SLL_OBJECT_TYPE_INT:
			{
				sll_bool_t e=0;
				o->dt.i=(sll_integer_t)_read_signed_integer(rf,&e);
				if (e){
					return 0;
				}
				return 1;
			}
		case SLL_OBJECT_TYPE_FLOAT:
			READ_FIELD(o->dt.f,rf);
			return 1;
		case SLL_OBJECT_TYPE_STRING:
			CHECK_ERROR2(rf,o->dt.s,sll_string_index_t);
			return 1;
		case SLL_OBJECT_TYPE_ARRAY:
			CHECK_ERROR2(rf,o->dt.al,sll_array_length_t);
			for (sll_array_length_t i=0;i<o->dt.al;i++){
				if (!_read_object(c_dt,rf)){
					return 0;
				}
			}
			return 1;
		case SLL_OBJECT_TYPE_MAP:
			CHECK_ERROR2(rf,o->dt.ml,sll_map_length_t);
			for (sll_map_length_t i=0;i<o->dt.ml;i++){
				if (!_read_object(c_dt,rf)){
					return 0;
				}
			}
			return 1;
		case SLL_OBJECT_TYPE_IDENTIFIER:
			CHECK_ERROR2(rf,o->dt.id,sll_identifier_index_t);
			return 1;
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			CHECK_ERROR2(rf,o->dt.fn_id,sll_function_index_t);
			return 1;
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				CHECK_ERROR2(rf,o->dt.fn.ac,sll_arg_count_t);
				CHECK_ERROR2(rf,o->dt.fn.id,sll_function_index_t);
				if (o->t==SLL_OBJECT_TYPE_FUNC){
					CHECK_ERROR2(rf,o->dt.fn.sc,sll_scope_t);
				}
				for (sll_arg_count_t i=0;i<o->dt.fn.ac;i++){
					if (!_read_object(c_dt,rf)){
						return 0;
					}
				}
				return 1;
			}
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_WHILE:
		case SLL_OBJECT_TYPE_LOOP:
			CHECK_ERROR2(rf,o->dt.l.ac,sll_arg_count_t);
			CHECK_ERROR2(rf,o->dt.l.sc,sll_scope_t);
			for (sll_arg_count_t i=0;i<o->dt.l.ac;i++){
				if (!_read_object(c_dt,rf)){
					return 0;
				}
			}
			return 1;
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			CHECK_ERROR2(rf,o->dt.dbg.fpi,sll_string_index_t);
			CHECK_ERROR2(rf,o->dt.dbg.ln,sll_file_offset_t);
			CHECK_ERROR2(rf,o->dt.dbg.cn,sll_file_offset_t);
			CHECK_ERROR2(rf,o->dt.dbg.ln_off,sll_file_offset_t);
			return _read_object(c_dt,rf);
	}
	CHECK_ERROR2(rf,o->dt.ac,sll_arg_count_t);
	for (sll_arg_count_t i=0;i<o->dt.ac;i++){
		if (!_read_object(c_dt,rf)){
			return 0;
		}
	}
	return 1;
}



static sll_bool_t _read_string(sll_file_t* rf,sll_string_t* o,sll_error_t* err){
	CHECK_ERROR(rf,o->l,sll_string_length_t,err);
	sll_string_create(o->l,o);
	if (o->l<STRING_COMPRESSION_MIN_LENGTH){
		if (sll_file_read(rf,o->v,o->l*sizeof(sll_char_t))==SLL_END_OF_DATA){
			err->t=SLL_ERROR_INVALID_FILE_FORMAT;
			return 0;
		}
	}
	else{
		sll_char_t bf[1<<STRING_COMPRESSION_OFFSET_BIT_COUNT];
		sll_set_memory(bf,((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1),0xff);
		uint64_t v;
		if (sll_file_read(rf,(void*)(&v),sizeof(uint64_t))==SLL_END_OF_DATA){
			return 0;
		}
		uint8_t bc=64;
		sll_string_length_t i=0;
		uint16_t r=((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1);
		do{
			if (!bc){
				if (sll_file_read(rf,(void*)(&v),sizeof(uint64_t))==SLL_END_OF_DATA){
					return 0;
				}
				bc=64;
			}
			bc--;
			uint16_t e;
			uint8_t el=((v&(1ull<<bc))?8:STRING_COMPRESSION_OFFSET_BIT_COUNT+STRING_COMPRESSION_LENGTH_BIT_COUNT);
			if (bc<el){
				e=(v&((1<<bc)-1))<<(el-bc);
				if (sll_file_read(rf,(void*)(&v),sizeof(uint64_t))==SLL_END_OF_DATA){
					return 0;
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
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_assembly(sll_file_t* rf,sll_assembly_data_t* a_dt,sll_error_t* e){
	uint32_t n;
	sll_version_t v;
	if (sll_file_read(rf,(uint8_t*)(&n),sizeof(uint32_t))==SLL_END_OF_DATA||n!=ASSEMBLY_FILE_MAGIC_NUMBER||sll_file_read(rf,(uint8_t*)(&v),sizeof(sll_version_t))==SLL_END_OF_DATA||v!=SLL_VERSION){
		e->t=SLL_ERROR_INVALID_FILE_FORMAT;
		return 0;
	}
	CHECK_ERROR(rf,a_dt->tm,sll_time_t,e);
	CHECK_ERROR(rf,a_dt->ic,sll_instruction_index_t,e);
	CHECK_ERROR(rf,a_dt->vc,sll_variable_index_t,e);
	CHECK_ERROR(rf,a_dt->ft.l,sll_function_index_t,e);
	a_dt->ft.dt=sll_allocate(a_dt->ft.l*sizeof(sll_instruction_index_t));
	for (sll_function_index_t i=0;i<a_dt->ft.l;i++){
		CHECK_ERROR(rf,*(a_dt->ft.dt+i),sll_instruction_index_t,e);
	}
	CHECK_ERROR(rf,a_dt->st.l,sll_string_index_t,e);
	a_dt->st.dt=sll_allocate(a_dt->st.l*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<a_dt->st.l;i++){
		if (!_read_string(rf,a_dt->st.dt+i,e)){
			return 0;
		}
	}
	_init_assembly_stack(a_dt);
	a_dt->h=a_dt->_s.p;
	sll_instruction_index_t i=a_dt->ic;
	a_dt->ic=0;
	while (i){
		i--;
		sll_assembly_instruction_t* ai=_acquire_next_instruction(a_dt);
		sll_read_char_t c=sll_file_read_char(rf);
		if (c==SLL_END_OF_DATA){
			e->t=SLL_ERROR_INVALID_FILE_FORMAT;
			return 0;
		}
		ai->t=(sll_assembly_instruction_type_t)c;
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				{
					sll_bool_t re=0;
					ai->dt.i=(sll_integer_t)_read_signed_integer(rf,&re);
					if (re){
						return 0;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				if (sll_file_read(rf,(void*)(&(ai->dt.f)),sizeof(sll_float_t))==SLL_END_OF_DATA){
					e->t=SLL_ERROR_INVALID_FILE_FORMAT;
					return 0;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				c=sll_file_read_char(rf);
				if (c==SLL_END_OF_DATA){
					e->t=SLL_ERROR_INVALID_FILE_FORMAT;
					return 0;
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
					sll_bool_t re=0;
					ai->dt.i=(sll_relative_instruction_index_t)_read_signed_integer(rf,&re);
					if (re){
						return 0;
					}
				}
				else{
					CHECK_ERROR(rf,ai->dt.rj,sll_instruction_index_t,e);
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
				CHECK_ERROR(rf,ai->dt.v,sll_variable_index_t,e);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				CHECK_ERROR(rf,ai->dt.s,sll_string_index_t,e);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				CHECK_ERROR(rf,ai->dt.al,sll_array_length_t,e);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				CHECK_ERROR(rf,ai->dt.ml,sll_map_length_t,e);
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
					CHECK_ERROR(rf,ai->dt.v,sll_variable_index_t,e);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				c=sll_file_read_char(rf);
				if (c==SLL_END_OF_DATA){
					e->t=SLL_ERROR_INVALID_FILE_FORMAT;
					return 0;
				}
				ai->dt.t=(sll_constant_type_t)c;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				CHECK_ERROR(rf,ai->dt.ac,sll_arg_count_t,e);
				break;
		}
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_compiled_object(sll_file_t* rf,sll_compilation_data_t* c_dt,sll_error_t* e){
	uint32_t n=0;
	sll_version_t v=0;
	if (sll_file_read(rf,(uint8_t*)(&n),sizeof(uint32_t))==SLL_END_OF_DATA||n!=COMPLIED_OBJECT_FILE_MAGIC_NUMBER||sll_file_read(rf,(void*)(&v),sizeof(sll_version_t))==SLL_END_OF_DATA||v!=SLL_VERSION){
		e->t=SLL_ERROR_INVALID_FILE_FORMAT;
		return 0;
	}
	c_dt->rf=NULL;
	CHECK_ERROR(rf,c_dt->tm,sll_time_t,e);
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		CHECK_ERROR(rf,c_dt->idt.s[i].l,sll_identifier_list_length_t,e);
		c_dt->idt.s[i].dt=sll_allocate(c_dt->idt.s[i].l*sizeof(sll_identifier_t));
		for (sll_identifier_list_length_t j=0;j<c_dt->idt.s[i].l;j++){
			CHECK_ERROR(rf,(c_dt->idt.s[i].dt+j)->sc,sll_scope_t,e);
			CHECK_ERROR(rf,(c_dt->idt.s[i].dt+j)->i,sll_string_index_t,e);
		}
	}
	CHECK_ERROR(rf,c_dt->idt.ill,sll_identifier_list_length_t,e);
	c_dt->idt.il=sll_allocate(c_dt->idt.ill*sizeof(sll_identifier_t));
	for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		CHECK_ERROR(rf,(c_dt->idt.il+i)->sc,sll_scope_t,e);
		CHECK_ERROR(rf,(c_dt->idt.il+i)->i,sll_string_index_t,e);
	}
	CHECK_ERROR(rf,c_dt->et.l,sll_export_table_length_t,e);
	c_dt->et.dt=sll_allocate(c_dt->et.l*sizeof(sll_identifier_index_t));
	for (sll_export_table_length_t i=0;i<c_dt->et.l;i++){
		CHECK_ERROR(rf,*(c_dt->et.dt+i),sll_identifier_index_t,e);
	}
	CHECK_ERROR(rf,c_dt->ft.l,sll_function_index_t,e);
	c_dt->ft.dt=sll_allocate(c_dt->ft.l*sizeof(sll_function_t*));
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		sll_object_offset_t off;
		CHECK_ERROR(rf,off,sll_object_offset_t,e);
		sll_arg_count_t al;
		CHECK_ERROR(rf,al,sll_arg_count_t,e);
		sll_function_t* k=sll_allocate(sizeof(sll_function_t)+al*sizeof(sll_identifier_index_t));
		k->off=off;
		k->al=al;
		for (sll_arg_count_t j=0;j<al;j++){
			CHECK_ERROR(rf,k->a[j],sll_identifier_index_t,e);
		}
		*(c_dt->ft.dt+i)=k;
	}
	CHECK_ERROR(rf,c_dt->st.l,sll_string_index_t,e);
	c_dt->st.dt=sll_allocate(c_dt->st.l*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<c_dt->st.l;i++){
		if (!_read_string(rf,c_dt->st.dt+i,e)){
			return 0;
		}
	}
	CHECK_ERROR(rf,c_dt->_n_sc_id,sll_scope_t,e);
	_init_object_stack(c_dt);
	c_dt->h=c_dt->_s.p;
	SLL_ASSERT(c_dt->h);
	if (!_read_object(c_dt,rf)){
		e->t=SLL_ERROR_INVALID_FILE_FORMAT;
		return 0;
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_object(sll_compilation_data_t* c_dt,sll_file_t* rf,sll_object_t** o,sll_error_t* e){
	*o=c_dt->_s.p;
	SLL_ASSERT(*o);
	if (!_read_object(c_dt,rf)){
		e->t=SLL_ERROR_INVALID_FILE_FORMAT;
		return 0;
	}
	return 1;
}

#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/types.h>
#include <sll/version.h>
#include <stdint.h>
#include <stdlib.h>



#define WRITE_FIELD(f,os) SLL_WRITE_TO_OUTPUT_DATA_STREAM((os),(uint8_t*)(&(f)),sizeof((f)))
#define WRITE_SIGNED_INTEGER(os,n) _write_integer((os),((n)<0?((~(n))<<1)|1:(n)<<1))



void _write_integer(sll_output_data_stream_t* os,uint64_t v){
	while (v>0x7f){
		SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,(uint8_t)((v&0x7f)|0x80));
		v>>=7;
	}
	SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,(uint8_t)v);
}



sll_object_offset_t _write_object(sll_output_data_stream_t* os,const sll_object_t* o){
	sll_object_offset_t eoff=0;
	while (o->t==SLL_OBJECT_TYPE_NOP){
		WRITE_FIELD(o->t,os);
		eoff++;
		o++;
	}
	WRITE_FIELD(o->t,os);
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
			return eoff+1;
		case SLL_OBJECT_TYPE_CHAR:
			WRITE_FIELD(o->dt.c,os);
			return eoff+1;
		case SLL_OBJECT_TYPE_INT:
			WRITE_SIGNED_INTEGER(os,o->dt.i);
			return eoff+1;
		case SLL_OBJECT_TYPE_FLOAT:
			WRITE_FIELD(o->dt.f,os);
			return eoff+1;
		case SLL_OBJECT_TYPE_STRING:
			_write_integer(os,o->dt.s);
			return eoff+1;
		case SLL_OBJECT_TYPE_IDENTIFIER:
			_write_integer(os,o->dt.id);
			return eoff+1;
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				_write_integer(os,o->dt.fn.ac);
				_write_integer(os,o->dt.fn.id);
				if (o->t==SLL_OBJECT_TYPE_FUNC){
					_write_integer(os,o->dt.fn.sc);
				}
				sll_object_offset_t off=1;
				for (sll_arg_count_t i=0;i<o->dt.fn.ac;i++){
					off+=_write_object(os,o+off);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_WHILE:
		case SLL_OBJECT_TYPE_LOOP:
			{
				_write_integer(os,o->dt.l.ac);
				_write_integer(os,o->dt.l.sc);
				sll_object_offset_t off=1;
				for (sll_arg_count_t i=0;i<o->dt.l.ac;i++){
					off+=_write_object(os,o+off);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				_write_integer(os,o->dt.sc);
				sll_object_offset_t off=1;
				for (sll_statement_count_t i=0;i<o->dt.sc;i++){
					off+=_write_object(os,o+off);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			_write_integer(os,o->dt.dbg.fpi);
			_write_integer(os,o->dt.dbg.ln);
			_write_integer(os,o->dt.dbg.cn);
			_write_integer(os,o->dt.dbg.ln_off);
			return eoff+_write_object(os,o+1)+1;
	}
	_write_integer(os,o->dt.ac);
	sll_object_offset_t off=1;
	for (sll_arg_count_t i=0;i<o->dt.ac;i++){
		off+=_write_object(os,o+off);
	}
	return off+eoff;
}



__SLL_FUNC void sll_write_assembly(sll_output_data_stream_t* os,const sll_assembly_data_t* a_dt){
	uint32_t n=ASSEMBLY_FILE_MAGIC_NUMBER;
	SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&n),sizeof(uint32_t));
	sll_version_t v=SLL_VERSION;
	SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&v),sizeof(sll_version_t));
	_write_integer(os,a_dt->tm);
	_write_integer(os,a_dt->ic);
	_write_integer(os,a_dt->vc);
	_write_integer(os,a_dt->ft.l);
	for (sll_function_index_t i=0;i<a_dt->ft.l;i++){
		_write_integer(os,*(a_dt->ft.dt+i));
	}
	_write_integer(os,a_dt->st.l);
	for (sll_string_index_t i=0;i<a_dt->st.l;i++){
		const sll_string_t* s=*(a_dt->st.dt+i);
		_write_integer(os,s->l);
		SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,s->v,s->l*sizeof(sll_char_t));
	}
	const sll_assembly_instruction_t* ai=a_dt->h;
	for (sll_instruction_index_t i=0;i<a_dt->ic;i++){
		SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,(uint8_t)ai->t);
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				WRITE_SIGNED_INTEGER(os,ai->dt.i);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&(ai->dt.f)),sizeof(sll_float_t));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,(uint8_t)ai->dt.c);
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
				_write_integer(os,ai->dt.v);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				_write_integer(os,ai->dt.s);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
				_write_integer(os,ai->dt.al);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JB:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JBE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JA:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JAE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JZ:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNZ:
				if (SLL_ASSEMBLY_INSTRUCTION_IS_RELATIVE(ai)){
					WRITE_SIGNED_INTEGER(os,ai->dt.rj);
				}
				else{
					_write_integer(os,ai->dt.rj);
				}
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
					_write_integer(os,ai->dt.v);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,(uint8_t)ai->dt.ac);
				break;
		}
		ai++;
	}
}



__SLL_FUNC void sll_write_object(sll_output_data_stream_t* os,const sll_object_t* o){
	_write_object(os,o);
}



__SLL_FUNC void sll_write_compiled_object(sll_output_data_stream_t* os,const sll_compilation_data_t* c_dt){
	uint32_t n=COMPLIED_OBJECT_FILE_MAGIC_NUMBER;
	SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&n),sizeof(uint32_t));
	sll_version_t v=SLL_VERSION;
	SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)(&v),sizeof(sll_version_t));
	_write_integer(os,c_dt->tm);
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		const sll_identifier_list_t* l=c_dt->idt.s+i;
		_write_integer(os,l->l);
		for (sll_identifier_list_length_t j=0;j<l->l;j++){
			_write_integer(os,(l->dt+j)->sc);
			_write_integer(os,(l->dt+j)->i);
		}
	}
	_write_integer(os,c_dt->idt.ill);
	for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		_write_integer(os,(c_dt->idt.il+i)->sc);
		_write_integer(os,(c_dt->idt.il+i)->i);
	}
	_write_integer(os,c_dt->et.l);
	for (sll_export_table_length_t i=0;i<c_dt->et.l;i++){
		_write_integer(os,*(c_dt->et.dt+i));
	}
	_write_integer(os,c_dt->ft.l);
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		const sll_function_t* k=*(c_dt->ft.dt+i);
		_write_integer(os,k->off);
		_write_integer(os,k->al);
		for (sll_arg_count_t j=0;j<k->al;j++){
			_write_integer(os,k->a[j]);
		}
	}
	_write_integer(os,c_dt->st.l);
	for (sll_string_index_t i=0;i<c_dt->st.l;i++){
		const sll_string_t* s=*(c_dt->st.dt+i);
		_write_integer(os,s->l);
		SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,s->v,s->l*sizeof(sll_char_t));
	}
	_write_integer(os,c_dt->_n_sc_id);
	_write_object(os,c_dt->h);
}

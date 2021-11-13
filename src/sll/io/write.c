#include <sll/_sll_internal.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/object.h>
#include <sll/stream.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/version.h>
#include <stdint.h>
#include <stdlib.h>



#define WRITE_FIELD(f,os) SLL_WRITE_TO_OUTPUT_DATA_STREAM((os),(uint8_t*)(&(f)),sizeof((f)))
#define WRITE_SIGNED_INTEGER(os,n) _write_integer((os),((n)<0?((~(n))<<1)|1:(n)<<1))



static void _write_integer(sll_output_data_stream_t* os,uint64_t v){
	while (v>0x7f){
		SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,(uint8_t)((v&0x7f)|0x80));
		v>>=7;
	}
	SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,(uint8_t)v);
}



static const sll_object_t* _write_object(sll_output_data_stream_t* os,const sll_object_t* o){
	while (o->t==SLL_OBJECT_TYPE_NOP||o->t==OBJECT_TYPE_CHANGE_STACK){
		if (o->t==OBJECT_TYPE_CHANGE_STACK){
			o=o->dt._p;
			continue;
		}
		WRITE_FIELD(o->t,os);
		o++;
	}
	WRITE_FIELD(o->t,os);
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
			return o+1;
		case SLL_OBJECT_TYPE_CHAR:
			WRITE_FIELD(o->dt.c,os);
			return o+1;
		case SLL_OBJECT_TYPE_INT:
			WRITE_SIGNED_INTEGER(os,o->dt.i);
			return o+1;
		case SLL_OBJECT_TYPE_FLOAT:
			WRITE_FIELD(o->dt.f,os);
			return o+1;
		case SLL_OBJECT_TYPE_STRING:
			_write_integer(os,o->dt.s);
			return o+1;
		case SLL_OBJECT_TYPE_ARRAY:
			{
				_write_integer(os,o->dt.al);
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=_write_object(os,o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_MAP:
			{
				_write_integer(os,o->dt.ml);
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_write_object(os,o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_IDENTIFIER:
			_write_integer(os,o->dt.id);
			return o+1;
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			_write_integer(os,o->dt.fn_id);
			return o+1;
		case SLL_OBJECT_TYPE_FUNC:
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				_write_integer(os,o->dt.fn.ac);
				_write_integer(os,o->dt.fn.id);
				if (o->t==SLL_OBJECT_TYPE_FUNC){
					_write_integer(os,o->dt.fn.sc);
				}
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_write_object(os,o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_WHILE:
		case SLL_OBJECT_TYPE_LOOP:
			{
				_write_integer(os,o->dt.l.ac);
				_write_integer(os,o->dt.l.sc);
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_write_object(os,o);
				}
				return o;
			}
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			_write_integer(os,o->dt.dbg.fpi);
			_write_integer(os,o->dt.dbg.ln);
			_write_integer(os,o->dt.dbg.cn);
			_write_integer(os,o->dt.dbg.ln_off);
			return _write_object(os,o+1);
	}
	_write_integer(os,o->dt.ac);
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_write_object(os,o);
	}
	return o;
}



static void _write_string(const sll_string_t* s,sll_output_data_stream_t* os){
	_write_integer(os,s->l);
	if (s->l<STRING_COMPRESSION_MIN_LENGTH){
		SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,s->v,s->l*sizeof(sll_char_t));
		return;
	}
	uint64_t v=0;
	uint8_t bc=64;
	sll_char_t bf[1<<(STRING_COMPRESSION_OFFSET_BIT_COUNT+1)];
	sll_set_memory(bf,((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1),0xff);
	sll_string_length_t si=0;
	uint16_t i=((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1);
	do{
		bf[i]=s->v[si];
		i++;
		si++;
	} while (si<s->l&&i<(1<<(STRING_COMPRESSION_OFFSET_BIT_COUNT+1)));
	uint16_t r=((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1);
	do{
		uint16_t st=0;
		uint16_t l=1;
		uint16_t mn=i-r;
		if (mn>(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)+1){
			mn=(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)+1;
		}
		sll_char_t c=bf[r];
		for (uint16_t j=r-(((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1));j<r;j++){
			if (bf[j]==c){
				uint16_t k=1;
				while (k<mn&&bf[j+k]==bf[r+k]){
					k++;
				}
				if (k>l){
					st=j;
					l=k;
				}
			}
		}
		uint16_t e;
		uint8_t el;
		if (l==1){
			e=256|c;
			el=9;
		}
		else{
			e=((st&((1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)-1))<<STRING_COMPRESSION_LENGTH_BIT_COUNT)|(l-2);
			el=STRING_COMPRESSION_OFFSET_BIT_COUNT+STRING_COMPRESSION_LENGTH_BIT_COUNT+1;
		}
		SLL_ASSERT(el<=15);
		if (bc<el){
			v=(v<<bc)|(e>>(el-bc));
			SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(void*)(&v),sizeof(uint64_t));
			v=e;
			bc+=64-el;
		}
		else{
			v=(v<<el)|e;
			bc-=el;
		}
		r+=l;
		if (r>=(1<<(STRING_COMPRESSION_OFFSET_BIT_COUNT+1))-(1<<STRING_COMPRESSION_LENGTH_BIT_COUNT)-1){
			for (uint16_t j=0;j<(1<<STRING_COMPRESSION_OFFSET_BIT_COUNT);j++){
				bf[j]=bf[j+(1<<STRING_COMPRESSION_OFFSET_BIT_COUNT)];
			}
			i-=1<<STRING_COMPRESSION_OFFSET_BIT_COUNT;
			r-=1<<STRING_COMPRESSION_OFFSET_BIT_COUNT;
			while (i<(1<<(STRING_COMPRESSION_OFFSET_BIT_COUNT+1))&&si<s->l){
				bf[i]=s->v[si];
				i++;
				si++;
			}
		}
	} while (r<i);
	if (bc!=64){
		v<<=bc;
		SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(void*)(&v),sizeof(uint64_t));
	}
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
		_write_string(a_dt->st.dt+i,os);
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
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,ai->dt.c);
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
					WRITE_SIGNED_INTEGER(os,ai->dt.rj);
				}
				else{
					_write_integer(os,ai->dt.rj);
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
				_write_integer(os,ai->dt.v);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				_write_integer(os,ai->dt.s);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				_write_integer(os,ai->dt.al);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				_write_integer(os,ai->dt.ml);
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,ai->dt.t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
				_write_integer(os,ai->dt.ac);
				break;
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->dt._p;
		}
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
		_write_string(c_dt->st.dt+i,os);
	}
	_write_integer(os,c_dt->_n_sc_id);
	_write_object(os,c_dt->h);
}

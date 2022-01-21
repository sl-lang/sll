#include <sll/_sll_internal.h>
#include <sll/api/serial.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/types.h>
#include <sll/version.h>



#define WRITE_FIELD(f,wf) sll_file_write((wf),&(f),sizeof((f)))
#define WRITE_SIGNED_INTEGER(wf,n) _write_integer((wf),((n)<0?((~(n))<<1)|1:(n)<<1))



static void _write_integer(sll_file_t* wf,sll_size_t v){
	unsigned int i=0;
	while (i<8&&v>0x7f){
		sll_file_write_char(wf,(v&0x7f)|0x80);
		v>>=7;
		i++;
	}
	sll_file_write_char(wf,(sll_char_t)v);
}



static const sll_node_t* _write_node(sll_file_t* wf,const sll_node_t* o){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==NODE_TYPE_CHANGE_STACK){
		if (o->t==NODE_TYPE_CHANGE_STACK){
			o=o->dt._p;
			continue;
		}
		WRITE_FIELD(o->t,wf);
		if (o->t==SLL_NODE_TYPE_DBG){
			_write_integer(wf,o->dt.s+1);
		}
		o++;
	}
	WRITE_FIELD(o->t,wf);
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
			WRITE_FIELD(o->dt.c,wf);
			return o+1;
		case SLL_NODE_TYPE_INT:
			WRITE_SIGNED_INTEGER(wf,o->dt.i);
			return o+1;
		case SLL_NODE_TYPE_FLOAT:
			WRITE_FIELD(o->dt.f,wf);
			return o+1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			_write_integer(wf,o->dt.s);
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				_write_integer(wf,o->dt.al);
				sll_array_length_t l=o->dt.al;
				o++;
				while (l){
					l--;
					o=_write_node(wf,o);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				_write_integer(wf,o->dt.ml);
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_write_node(wf,o);
				}
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			_write_integer(wf,o->dt.id);
			return o+1;
		case SLL_NODE_TYPE_FUNCTION_ID:
			_write_integer(wf,o->dt.fn_id);
			return o+1;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				_write_integer(wf,o->dt.fn.ac);
				_write_integer(wf,o->dt.fn.id);
				if (o->t==SLL_NODE_TYPE_FUNC){
					_write_integer(wf,o->dt.fn.sc);
				}
				sll_arg_count_t l=o->dt.fn.ac;
				o++;
				while (l){
					l--;
					o=_write_node(wf,o);
				}
				return o;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				_write_integer(wf,o->dt.l.ac);
				_write_integer(wf,o->dt.l.sc);
				sll_arg_count_t l=o->dt.l.ac;
				o++;
				while (l){
					l--;
					o=_write_node(wf,o);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				_write_integer(wf,o->dt.d.ac);
				_write_integer(wf,o->dt.d.nm+1);
				sll_arg_count_t l=o->dt.d.ac;
				o++;
				while (l){
					l--;
					o=_write_node(wf,o);
				}
				return o;
			}
	}
	_write_integer(wf,o->dt.ac);
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_write_node(wf,o);
	}
	return o;
}



__SLL_EXTERNAL void sll_write_assembly(sll_file_t* wf,const sll_assembly_data_t* a_dt){
	magic_number_t n=ASSEMBLY_FILE_MAGIC_NUMBER;
	sll_file_write(wf,&n,sizeof(magic_number_t));
	sll_version_t v=SLL_VERSION;
	sll_file_write(wf,&v,sizeof(sll_version_t));
	_write_integer(wf,a_dt->tm);
	_write_integer(wf,a_dt->ic);
	_write_integer(wf,a_dt->vc);
	_write_integer(wf,a_dt->ft.l);
	for (sll_function_index_t i=0;i<a_dt->ft.l;i++){
		_write_integer(wf,(a_dt->ft.dt+i)->i);
		_write_integer(wf,(a_dt->ft.dt+i)->ac);
		_write_integer(wf,(a_dt->ft.dt+i)->nm);
	}
	_write_integer(wf,a_dt->st.l);
	for (sll_string_index_t i=0;i<a_dt->st.l;i++){
		sll_encode_string(wf,a_dt->st.dt+i);
	}
	_write_integer(wf,a_dt->dbg.l);
	for (sll_instruction_index_t i=0;i<a_dt->dbg.l;i++){
		_write_integer(wf,(a_dt->dbg.dt+i)->ii);
		_write_integer(wf,ROTATE_BITS64((a_dt->dbg.dt+i)->ln,1));
	}
	const sll_assembly_instruction_t* ai=a_dt->h;
	for (sll_instruction_index_t i=0;i<a_dt->ic;i++){
		sll_file_write_char(wf,ai->t);
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				WRITE_SIGNED_INTEGER(wf,ai->dt.i);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				sll_file_write(wf,&(ai->dt.f),sizeof(sll_float_t));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				sll_file_write_char(wf,ai->dt.c);
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JI:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					WRITE_SIGNED_INTEGER(wf,ai->dt.rj);
				}
				else{
					_write_integer(wf,ai->dt.rj);
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
				_write_integer(wf,ai->dt.v);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				_write_integer(wf,ai->dt.s);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				_write_integer(wf,ai->dt.al);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				_write_integer(wf,ai->dt.ml);
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_TWO:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_THREE:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_INPLACE(ai)){
					_write_integer(wf,ai->dt.v);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				_write_integer(wf,ai->dt.va.v);
				_write_integer(wf,ai->dt.va.l);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				sll_file_write_char(wf,ai->dt.t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				_write_integer(wf,ai->dt.ac);
				break;
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->dt._p;
		}
	}
}



__SLL_EXTERNAL void sll_write_node(sll_file_t* wf,const sll_node_t* o){
	_write_node(wf,o);
}



__SLL_EXTERNAL void sll_write_compiled_node(sll_file_t* wf,const sll_compilation_data_t* c_dt){
	magic_number_t n=COMPLIED_OBJECT_FILE_MAGIC_NUMBER;
	sll_file_write(wf,&n,sizeof(magic_number_t));
	sll_version_t v=SLL_VERSION;
	sll_file_write(wf,&v,sizeof(sll_version_t));
	_write_integer(wf,c_dt->tm);
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		const sll_identifier_list_t* l=c_dt->idt.s+i;
		_write_integer(wf,l->l);
		for (sll_identifier_list_length_t j=0;j<l->l;j++){
			_write_integer(wf,(l->dt+j)->sc);
			_write_integer(wf,(l->dt+j)->i);
		}
	}
	_write_integer(wf,c_dt->idt.ill);
	for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		_write_integer(wf,(c_dt->idt.il+i)->sc);
		_write_integer(wf,(c_dt->idt.il+i)->i);
	}
	_write_integer(wf,c_dt->et.l);
	for (sll_export_table_length_t i=0;i<c_dt->et.l;i++){
		_write_integer(wf,*(c_dt->et.dt+i));
	}
	_write_integer(wf,c_dt->ft.l);
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		const sll_function_t* k=*(c_dt->ft.dt+i);
		_write_integer(wf,k->off);
		_write_integer(wf,k->al);
		_write_integer(wf,k->nm+1);
		for (sll_arg_count_t j=0;j<k->al;j++){
			_write_integer(wf,k->a[j]);
		}
	}
	_write_integer(wf,c_dt->st.l);
	for (sll_string_index_t i=0;i<c_dt->st.l;i++){
		sll_encode_string(wf,c_dt->st.dt+i);
	}
	_write_integer(wf,c_dt->fpt.l);
	for (sll_string_index_t i=0;i<c_dt->fpt.l;i++){
		_write_integer(wf,*(c_dt->fpt.dt+i));
	}
	_write_integer(wf,c_dt->_n_sc_id);
	_write_node(wf,c_dt->h);
}

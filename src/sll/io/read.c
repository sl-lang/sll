#include <sll/_sll_internal.h>
#include <sll/api/serial.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/version.h>



#define CHECK_ERROR(rf,o,ot) \
	do{ \
		sll_bool_t __e=0; \
		(o)=(ot)_read_integer((rf),&__e); \
		if (__e){ \
			return 0; \
		} \
	} while (0)
#define READ_FIELD(f,rf) \
	do{ \
		if (sll_file_read((rf),&(f),sizeof(f))==SLL_END_OF_DATA){ \
			return 0; \
		} \
	} while(0)



static sll_size_t _read_integer(sll_file_t* rf,sll_bool_t* e){
	sll_read_char_t c=sll_file_read_char(rf);
	if (c==SLL_END_OF_DATA){
		*e=1;
		return 0;
	}
	sll_size_t v=0;
	sll_string_length_t s=0;
	while ((c&0x80)&&s<56){
		v|=((sll_size_t)(c&0x7f))<<s;
		s+=7;
		c=sll_file_read_char(rf);
		if (c==SLL_END_OF_DATA){
			*e=1;
			return 0;
		}
	}
	return v|(((sll_size_t)c)<<s);
}



static sll_integer_t _read_signed_integer(sll_file_t* rf,sll_bool_t* e){
	sll_size_t v=_read_integer(rf,e);
	return (v>>1)^(-((sll_integer_t)(v&1)));
}



static sll_bool_t _read_node(sll_compilation_data_t* c_dt,sll_file_t* rf){
	sll_node_t* o=_acquire_next_node(c_dt);
	READ_FIELD(o->t,rf);
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG){
		if (o->t==SLL_NODE_TYPE_DBG){
			CHECK_ERROR(rf,o->dt.s,sll_string_index_t);
			o->dt.s--;
		}
		o=_acquire_next_node(c_dt);
		READ_FIELD(o->t,rf);
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
			READ_FIELD(o->dt.c,rf);
			return 1;
		case SLL_NODE_TYPE_INT:
			{
				sll_bool_t e=0;
				o->dt.i=(sll_integer_t)_read_signed_integer(rf,&e);
				if (e){
					return 0;
				}
				return 1;
			}
		case SLL_NODE_TYPE_FLOAT:
			READ_FIELD(o->dt.f,rf);
			return 1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			CHECK_ERROR(rf,o->dt.s,sll_string_index_t);
			return 1;
		case SLL_NODE_TYPE_ARRAY:
			CHECK_ERROR(rf,o->dt.al,sll_array_length_t);
			for (sll_array_length_t i=0;i<o->dt.al;i++){
				if (!_read_node(c_dt,rf)){
					return 0;
				}
			}
			return 1;
		case SLL_NODE_TYPE_MAP:
			CHECK_ERROR(rf,o->dt.ml,sll_map_length_t);
			for (sll_map_length_t i=0;i<o->dt.ml;i++){
				if (!_read_node(c_dt,rf)){
					return 0;
				}
			}
			return 1;
		case SLL_NODE_TYPE_IDENTIFIER:
			CHECK_ERROR(rf,o->dt.id,sll_identifier_index_t);
			return 1;
		case SLL_NODE_TYPE_FUNCTION_ID:
			CHECK_ERROR(rf,o->dt.fn_id,sll_function_index_t);
			return 1;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				CHECK_ERROR(rf,o->dt.fn.ac,sll_arg_count_t);
				CHECK_ERROR(rf,o->dt.fn.id,sll_function_index_t);
				if (o->t==SLL_NODE_TYPE_FUNC){
					CHECK_ERROR(rf,o->dt.fn.sc,sll_scope_t);
				}
				for (sll_arg_count_t i=0;i<o->dt.fn.ac;i++){
					if (!_read_node(c_dt,rf)){
						return 0;
					}
				}
				return 1;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			CHECK_ERROR(rf,o->dt.l.ac,sll_arg_count_t);
			CHECK_ERROR(rf,o->dt.l.sc,sll_scope_t);
			for (sll_arg_count_t i=0;i<o->dt.l.ac;i++){
				if (!_read_node(c_dt,rf)){
					return 0;
				}
			}
			return 1;
		case SLL_NODE_TYPE_DECL:
			CHECK_ERROR(rf,o->dt.d.ac,sll_arg_count_t);
			CHECK_ERROR(rf,o->dt.d.nm,sll_string_index_t);
			o->dt.d.nm--;
			for (sll_arg_count_t i=0;i<o->dt.d.ac;i++){
				if (!_read_node(c_dt,rf)){
					return 0;
				}
			}
			return 1;
	}
	CHECK_ERROR(rf,o->dt.ac,sll_arg_count_t);
	for (sll_arg_count_t i=0;i<o->dt.ac;i++){
		if (!_read_node(c_dt,rf)){
			return 0;
		}
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_assembly(sll_file_t* rf,sll_assembly_data_t* a_dt){
	magic_number_t n;
	sll_version_t v;
	if (sll_file_read(rf,&n,sizeof(magic_number_t))==SLL_END_OF_DATA||n!=ASSEMBLY_FILE_MAGIC_NUMBER||sll_file_read(rf,&v,sizeof(sll_version_t))==SLL_END_OF_DATA||v!=SLL_VERSION){
		return 0;
	}
	CHECK_ERROR(rf,a_dt->tm,sll_time_t);
	CHECK_ERROR(rf,a_dt->ic,sll_instruction_index_t);
	CHECK_ERROR(rf,a_dt->vc,sll_variable_index_t);
	CHECK_ERROR(rf,a_dt->ft.l,sll_function_index_t);
	a_dt->ft.dt=sll_allocate(a_dt->ft.l*sizeof(sll_assembly_function_t));
	for (sll_function_index_t i=0;i<a_dt->ft.l;i++){
		CHECK_ERROR(rf,(a_dt->ft.dt+i)->i,sll_instruction_index_t);
		CHECK_ERROR(rf,(a_dt->ft.dt+i)->ac,sll_arg_count_t);
		CHECK_ERROR(rf,(a_dt->ft.dt+i)->nm,sll_string_index_t);
	}
	CHECK_ERROR(rf,a_dt->st.l,sll_string_index_t);
	a_dt->st.dt=sll_allocate(a_dt->st.l*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<a_dt->st.l;i++){
		if (!sll_decode_string(rf,a_dt->st.dt+i)){
			return 0;
		}
	}
	CHECK_ERROR(rf,a_dt->dbg.l,sll_instruction_index_t);
	a_dt->dbg.dt=sll_allocate(a_dt->dbg.l*sizeof(sll_debug_line_data_t));
	for (sll_instruction_index_t i=0;i<a_dt->dbg.l;i++){
		CHECK_ERROR(rf,(a_dt->dbg.dt+i)->ii,sll_instruction_index_t);
		sll_file_offset_t ln;
		CHECK_ERROR(rf,ln,sll_file_offset_t);
		(a_dt->dbg.dt+i)->ln=ROTATE_BITS64(ln,63);
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
				if (sll_file_read(rf,PTR(&(ai->dt.f)),sizeof(sll_float_t))==SLL_END_OF_DATA){
					return 0;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				c=sll_file_read_char(rf);
				if (c==SLL_END_OF_DATA){
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
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JI:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI:
				if (SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)){
					sll_bool_t re=0;
					ai->dt.i=(sll_relative_instruction_index_t)_read_signed_integer(rf,&re);
					if (re){
						return 0;
					}
				}
				else{
					CHECK_ERROR(rf,ai->dt.rj,sll_instruction_index_t);
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
				CHECK_ERROR(rf,ai->dt.v,sll_variable_index_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				CHECK_ERROR(rf,ai->dt.s,sll_string_index_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				CHECK_ERROR(rf,ai->dt.al,sll_array_length_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				CHECK_ERROR(rf,ai->dt.ml,sll_map_length_t);
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
					CHECK_ERROR(rf,ai->dt.v,sll_variable_index_t);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				CHECK_ERROR(rf,ai->dt.va.v,sll_variable_index_t);
				CHECK_ERROR(rf,ai->dt.va.l,sll_arg_count_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				c=sll_file_read_char(rf);
				if (c==SLL_END_OF_DATA){
					return 0;
				}
				ai->dt.t=(sll_object_type_t)c;
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				CHECK_ERROR(rf,ai->dt.ac,sll_arg_count_t);
				break;
		}
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_compiled_node(sll_file_t* rf,sll_compilation_data_t* c_dt){
	magic_number_t n;
	sll_version_t v;
	if (sll_file_read(rf,&n,sizeof(magic_number_t))==SLL_END_OF_DATA||n!=COMPLIED_OBJECT_FILE_MAGIC_NUMBER||sll_file_read(rf,&v,sizeof(sll_version_t))==SLL_END_OF_DATA||v!=SLL_VERSION){
		return 0;
	}
	CHECK_ERROR(rf,c_dt->tm,sll_time_t);
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		CHECK_ERROR(rf,c_dt->idt.s[i].l,sll_identifier_list_length_t);
		c_dt->idt.s[i].dt=sll_allocate(c_dt->idt.s[i].l*sizeof(sll_identifier_t));
		for (sll_identifier_list_length_t j=0;j<c_dt->idt.s[i].l;j++){
			CHECK_ERROR(rf,(c_dt->idt.s[i].dt+j)->sc,sll_scope_t);
			CHECK_ERROR(rf,(c_dt->idt.s[i].dt+j)->i,sll_string_index_t);
		}
	}
	CHECK_ERROR(rf,c_dt->idt.ill,sll_identifier_list_length_t);
	c_dt->idt.il=sll_allocate(c_dt->idt.ill*sizeof(sll_identifier_t));
	for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
		CHECK_ERROR(rf,(c_dt->idt.il+i)->sc,sll_scope_t);
		CHECK_ERROR(rf,(c_dt->idt.il+i)->i,sll_string_index_t);
	}
	CHECK_ERROR(rf,c_dt->et.l,sll_export_table_length_t);
	c_dt->et.dt=sll_allocate(c_dt->et.l*sizeof(sll_identifier_index_t));
	for (sll_export_table_length_t i=0;i<c_dt->et.l;i++){
		CHECK_ERROR(rf,*(c_dt->et.dt+i),sll_identifier_index_t);
	}
	CHECK_ERROR(rf,c_dt->ft.l,sll_function_index_t);
	c_dt->ft.dt=sll_allocate(c_dt->ft.l*sizeof(sll_function_t*));
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		sll_node_offset_t off;
		CHECK_ERROR(rf,off,sll_node_offset_t);
		sll_arg_count_t al;
		CHECK_ERROR(rf,al,sll_arg_count_t);
		sll_function_t* k=sll_allocate(sizeof(sll_function_t)+al*sizeof(sll_identifier_index_t));
		k->off=off;
		k->al=al;
		CHECK_ERROR(rf,k->nm,sll_string_index_t);
		k->nm--;
		for (sll_arg_count_t j=0;j<al;j++){
			CHECK_ERROR(rf,k->a[j],sll_identifier_index_t);
		}
		*(c_dt->ft.dt+i)=k;
	}
	CHECK_ERROR(rf,c_dt->st.l,sll_string_index_t);
	c_dt->st.dt=sll_allocate(c_dt->st.l*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<c_dt->st.l;i++){
		if (!sll_decode_string(rf,c_dt->st.dt+i)){
			return 0;
		}
	}
	CHECK_ERROR(rf,c_dt->fpt.l,sll_string_index_t);
	c_dt->fpt.dt=sll_allocate(c_dt->fpt.l*sizeof(sll_string_index_t));
	for (sll_string_index_t i=0;i<c_dt->fpt.l;i++){
		CHECK_ERROR(rf,*(c_dt->fpt.dt+i),sll_string_index_t);
	}
	CHECK_ERROR(rf,c_dt->_n_sc_id,sll_scope_t);
	_init_node_stack(c_dt);
	c_dt->h=c_dt->_s.p;
	return _read_node(c_dt,rf);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_node(sll_compilation_data_t* c_dt,sll_file_t* rf,sll_node_t** o){
	*o=c_dt->_s.p;
	SLL_ASSERT(*o);
	return _read_node(c_dt,rf);
}

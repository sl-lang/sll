#include <sll/_internal/intrinsics.h>
#include <sll/_internal/magic_number.h>
#include <sll/_internal/stack.h>
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
		(o)=(ot)sll_decode_integer((rf),&__e); \
		if (__e){ \
			return 0; \
		} \
	} while (0)
#define READ_FIELD(f,rf) \
	do{ \
		if (sll_file_read((rf),&(f),sizeof(f),NULL)!=sizeof(f)){ \
			return 0; \
		} \
	} while(0)



static sll_bool_t _read_node(sll_source_file_t* sf,sll_file_t* rf){
	sll_node_t* o=_acquire_next_node(sf);
	READ_FIELD(o->t,rf);
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG){
		if (o->t==SLL_NODE_TYPE_DBG){
			CHECK_ERROR(rf,o->dt.s,sll_string_index_t);
			o->dt.s--;
		}
		o=_acquire_next_node(sf);
		READ_FIELD(o->t,rf);
	}
	switch (o->t){
		case SLL_NODE_TYPE_CHAR:
			READ_FIELD(o->dt.c,rf);
			return 1;
		case SLL_NODE_TYPE_INT:
			{
				sll_bool_t e=0;
				o->dt.i=sll_decode_signed_integer(rf,&e);
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
				if (!_read_node(sf,rf)){
					return 0;
				}
			}
			return 1;
		case SLL_NODE_TYPE_MAP:
			CHECK_ERROR(rf,o->dt.ml,sll_map_length_t);
			for (sll_map_length_t i=0;i<o->dt.ml;i++){
				if (!_read_node(sf,rf)){
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
					if (!_read_node(sf,rf)){
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
				if (!_read_node(sf,rf)){
					return 0;
				}
			}
			return 1;
		case SLL_NODE_TYPE_DECL:
			CHECK_ERROR(rf,o->dt.d.ac,sll_arg_count_t);
			CHECK_ERROR(rf,o->dt.d.nm,sll_string_index_t);
			o->dt.d.nm--;
			for (sll_arg_count_t i=0;i<o->dt.d.ac;i++){
				if (!_read_node(sf,rf)){
					return 0;
				}
			}
			return 1;
	}
	CHECK_ERROR(rf,o->dt.ac,sll_arg_count_t);
	for (sll_arg_count_t i=0;i<o->dt.ac;i++){
		if (!_read_node(sf,rf)){
			return 0;
		}
	}
	return 1;
}



static sll_bool_t _read_source_file(sll_file_t* rf,sll_source_file_t* sf){
	CHECK_ERROR(rf,sf->tm,sll_time_t);
	CHECK_ERROR(rf,sf->sz,sll_file_offset_t);
	if (sll_file_read(rf,&(sf->h),sizeof(sll_sha256_data_t),NULL)!=sizeof(sll_sha256_data_t)){
		return 0;
	}
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		CHECK_ERROR(rf,sf->idt.s[i].l,sll_identifier_list_length_t);
		sf->idt.s[i].dt=sll_allocate(sf->idt.s[i].l*sizeof(sll_identifier_t));
		for (sll_identifier_list_length_t j=0;j<sf->idt.s[i].l;j++){
			CHECK_ERROR(rf,(sf->idt.s[i].dt+j)->sc,sll_scope_t);
			CHECK_ERROR(rf,(sf->idt.s[i].dt+j)->i,sll_string_index_t);
		}
	}
	CHECK_ERROR(rf,sf->idt.ill,sll_identifier_list_length_t);
	sf->idt.il=sll_allocate(sf->idt.ill*sizeof(sll_identifier_t));
	for (sll_identifier_list_length_t i=0;i<sf->idt.ill;i++){
		CHECK_ERROR(rf,(sf->idt.il+i)->sc,sll_scope_t);
		CHECK_ERROR(rf,(sf->idt.il+i)->i,sll_string_index_t);
	}
	CHECK_ERROR(rf,sf->et.l,sll_export_table_length_t);
	sf->et.dt=sll_allocate(sf->et.l*sizeof(sll_identifier_index_t));
	for (sll_export_table_length_t i=0;i<sf->et.l;i++){
		CHECK_ERROR(rf,*(sf->et.dt+i),sll_identifier_index_t);
	}
	CHECK_ERROR(rf,sf->ft.l,sll_function_index_t);
	sf->ft.dt=sll_allocate(sf->ft.l*sizeof(sll_function_t*));
	for (sll_function_index_t i=0;i<sf->ft.l;i++){
		sll_node_offset_t off;
		CHECK_ERROR(rf,off,sll_node_offset_t);
		sll_arg_count_t al;
		CHECK_ERROR(rf,al,sll_arg_count_t);
		sll_function_t* k=sll_allocate(sizeof(sll_function_t)+SLL_FUNCTION_GET_ARGUMENT_COUNT_RAW(al)*sizeof(sll_identifier_index_t));
		k->off=off;
		k->al=al;
		CHECK_ERROR(rf,k->nm,sll_string_index_t);
		k->nm--;
		for (sll_arg_count_t j=0;j<SLL_FUNCTION_GET_ARGUMENT_COUNT_RAW(al);j++){
			CHECK_ERROR(rf,k->a[j],sll_identifier_index_t);
		}
		*(sf->ft.dt+i)=k;
	}
	CHECK_ERROR(rf,sf->st.l,sll_string_index_t);
	sf->st.dt=sll_allocate(sf->st.l*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<sf->st.l;i++){
		if (!sll_decode_string(rf,sf->st.dt+i)){
			return 0;
		}
	}
	CHECK_ERROR(rf,sf->it.l,sll_import_index_t);
	sf->it.dt=sll_allocate(sf->it.l*sizeof(sll_import_file_t*));
	for (sll_import_index_t i=0;i<sf->it.l;i++){
		sll_source_file_index_t sfi;
		CHECK_ERROR(rf,sfi,sll_source_file_index_t);
		sll_identifier_list_length_t l;
		CHECK_ERROR(rf,l,sll_identifier_list_length_t);
		sll_import_file_t* if_=sll_allocate(sizeof(sll_import_file_t)+l*sizeof(sll_identifier_index_t));
		*(sf->it.dt+i)=if_;
		if_->sfi=sfi;
		if_->l=l;
		for (sll_identifier_list_length_t j=0;j<l;j++){
			CHECK_ERROR(rf,if_->dt[j],sll_identifier_index_t);
		}
	}
	CHECK_ERROR(rf,sf->fp_nm,sll_string_index_t);
	CHECK_ERROR(rf,sf->_n_sc_id,sll_scope_t);
	_init_node_stack(sf);
	sf->dt=sf->_s.p;
	return _read_node(sf,rf);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_assembly(sll_file_t* rf,sll_assembly_data_t* a_dt){
	magic_number_t n;
	sll_version_t v;
	if (sll_file_read(rf,&n,sizeof(magic_number_t),NULL)!=sizeof(magic_number_t)||n!=ASSEMBLY_FILE_MAGIC_NUMBER||sll_file_read(rf,&v,sizeof(sll_version_t),NULL)!=sizeof(sll_version_t)||v!=SLL_VERSION){
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
		(a_dt->dbg.dt+i)->ln=ROTATE_BITS_RIGHT64(ln,2);
	}
	_init_assembly_stack(a_dt);
	a_dt->h=a_dt->_s.p;
	sll_instruction_index_t i=a_dt->ic;
	a_dt->ic=0;
	while (i){
		i--;
		sll_assembly_instruction_t* ai=_acquire_next_instruction(a_dt);
		sll_read_char_t c=sll_file_read_char(rf,NULL);
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
					ai->dt.i=sll_decode_signed_integer(rf,&re);
					if (re){
						return 0;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				if (sll_file_read(rf,PTR(&(ai->dt.f)),sizeof(sll_float_t),NULL)==SLL_END_OF_DATA){
					return 0;
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				c=sll_file_read_char(rf,NULL);
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
					ai->dt.i=(sll_relative_instruction_index_t)sll_decode_signed_integer(rf,&re);
					if (re){
						return 0;
					}
				}
				else{
					CHECK_ERROR(rf,ai->dt.rj,sll_instruction_index_t);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK:
				CHECK_ERROR(rf,ai->dt.so,sll_stack_offset_t);
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
				CHECK_ERROR(rf,ai->dt.t,sll_object_type_t);
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



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_bundle(sll_file_t* rf,sll_bundle_t* b){
	magic_number_t n=0;
	sll_version_t v=0;
	if (sll_file_read(rf,&n,sizeof(magic_number_t),NULL)!=sizeof(magic_number_t)||n!=BUNDLE_FILE_MAGIC_NUMBER||sll_file_read(rf,&v,sizeof(sll_version_t),NULL)!=sizeof(sll_version_t)||v!=SLL_VERSION||!sll_decode_string(rf,&(b->nm))){
		return 0;
	}
	CHECK_ERROR(rf,b->l,sll_source_file_index_t);
	b->dt=sll_allocate(b->l*sizeof(sll_bundle_source_file_t*));
	for (sll_source_file_index_t i=0;i<b->l;i++){
		sll_bundle_source_file_t* bsf=sll_allocate(sizeof(sll_bundle_source_file_t));
		*(b->dt+i)=bsf;
		if (!sll_decode_string(rf,&(bsf->nm))||!_read_source_file(rf,&(bsf->dt))){
			return 0;
		}
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_compiled_node(sll_file_t* rf,sll_compilation_data_t* c_dt){
	magic_number_t n=0;
	sll_version_t v=0;
	if (sll_file_read(rf,&n,sizeof(magic_number_t),NULL)!=sizeof(magic_number_t)||n!=COMPLIED_OBJECT_FILE_MAGIC_NUMBER||sll_file_read(rf,&v,sizeof(sll_version_t),NULL)!=sizeof(sll_version_t)||v!=SLL_VERSION){
		return 0;
	}
	CHECK_ERROR(rf,c_dt->l,sll_source_file_index_t);
	c_dt->dt=sll_allocate(c_dt->l*sizeof(sll_source_file_t*));
	for (sll_source_file_index_t i=0;i<c_dt->l;i++){
		sll_source_file_t* sf=sll_allocate(sizeof(sll_source_file_t));
		*(c_dt->dt+i)=sf;
		if (!_read_source_file(rf,sf)){
			return 0;
		}
	}
	return 1;
}

#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/_internal/magic_number.h>
#include <sll/api/serial.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/types.h>
#include <sll/version.h>



#define WRITE_FIELD(f,wf) sll_file_write((wf),&(f),sizeof((f)),NULL)
#define CHECK_ERROR(fn) SLL_CRITICAL((fn)==SLL_NO_ERROR)



static const sll_node_t* _write_node(sll_file_t* wf,const sll_node_t* o){
	while (o->t==SLL_NODE_TYPE_NOP||o->t==SLL_NODE_TYPE_DBG||o->t==SLL_NODE_TYPE_CHANGE_STACK){
		if (o->t==SLL_NODE_TYPE_CHANGE_STACK){
			o=o->dt._p;
			continue;
		}
		sll_file_write_char(wf,o->t,NULL);
		if (o->t==SLL_NODE_TYPE_DBG){
			CHECK_ERROR(sll_encode_integer(wf,o->dt.s+1));
		}
		o++;
	}
	sll_file_write_char(wf,o->t,NULL);
	switch (o->t){
		case SLL_NODE_TYPE_INT:
			CHECK_ERROR(sll_encode_signed_integer(wf,o->dt.i));
			return o+1;
		case SLL_NODE_TYPE_FLOAT:
			WRITE_FIELD(o->dt.f,wf);
			return o+1;
		case SLL_NODE_TYPE_CHAR:
			sll_file_write_char(wf,o->dt.c,NULL);
			return o+1;
		case SLL_NODE_TYPE_COMPLEX:
			WRITE_FIELD(o->dt.d,wf);
			return o+1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			CHECK_ERROR(sll_encode_integer(wf,o->dt.s));
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				CHECK_ERROR(sll_encode_integer(wf,o->dt.al));
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
				CHECK_ERROR(sll_encode_integer(wf,o->dt.ml));
				sll_map_length_t l=o->dt.ml;
				o++;
				while (l){
					l--;
					o=_write_node(wf,o);
				}
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			CHECK_ERROR(sll_encode_integer(wf,o->dt.id));
			return o+1;
		case SLL_NODE_TYPE_FUNCTION_ID:
			CHECK_ERROR(sll_encode_integer(wf,o->dt.fn_id));
			return o+1;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				CHECK_ERROR(sll_encode_integer(wf,o->dt.fn.ac));
				CHECK_ERROR(sll_encode_integer(wf,o->dt.fn.id));
				if (o->t==SLL_NODE_TYPE_FUNC){
					CHECK_ERROR(sll_encode_integer(wf,o->dt.fn.sc));
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
				CHECK_ERROR(sll_encode_integer(wf,o->dt.l.ac));
				CHECK_ERROR(sll_encode_integer(wf,o->dt.l.sc));
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
				CHECK_ERROR(sll_encode_integer(wf,o->dt.dc.ac));
				CHECK_ERROR(sll_encode_integer(wf,o->dt.dc.nm+1));
				sll_arg_count_t l=o->dt.dc.ac;
				o++;
				while (l){
					l--;
					o=_write_node(wf,o);
				}
				return o;
			}
	}
	CHECK_ERROR(sll_encode_integer(wf,o->dt.ac));
	sll_arg_count_t l=o->dt.ac;
	o++;
	while (l){
		l--;
		o=_write_node(wf,o);
	}
	return o;
}



static void _write_source_file(sll_file_t* wf,const sll_source_file_t* sf){
	CHECK_ERROR(sll_encode_integer(wf,sf->tm));
	CHECK_ERROR(sll_encode_integer(wf,sf->sz));
	sll_file_write(wf,&(sf->h),sizeof(sll_sha256_data_t),NULL);
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		const sll_identifier_list_t* l=sf->idt.s+i;
		CHECK_ERROR(sll_encode_integer(wf,l->l));
		for (sll_identifier_list_length_t j=0;j<l->l;j++){
			CHECK_ERROR(sll_encode_integer(wf,(l->dt+j)->sc));
			CHECK_ERROR(sll_encode_integer(wf,(l->dt+j)->i));
		}
	}
	CHECK_ERROR(sll_encode_integer(wf,sf->idt.ill));
	for (sll_identifier_list_length_t i=0;i<sf->idt.ill;i++){
		CHECK_ERROR(sll_encode_integer(wf,(sf->idt.il+i)->sc));
		CHECK_ERROR(sll_encode_integer(wf,(sf->idt.il+i)->i));
	}
	CHECK_ERROR(sll_encode_integer(wf,sf->et.l));
	for (sll_export_table_length_t i=0;i<sf->et.l;i++){
		CHECK_ERROR(sll_encode_integer(wf,*(sf->et.dt+i)));
	}
	CHECK_ERROR(sll_encode_integer(wf,sf->ft.l));
	for (sll_function_index_t i=0;i<sf->ft.l;i++){
		const sll_function_t* k=*(sf->ft.dt+i);
		CHECK_ERROR(sll_encode_integer(wf,k->off));
		CHECK_ERROR(sll_encode_integer(wf,k->al));
		CHECK_ERROR(sll_encode_integer(wf,k->nm+1));
		CHECK_ERROR(sll_encode_integer(wf,k->desc+1));
		for (sll_arg_count_t j=0;j<SLL_FUNCTION_GET_ARGUMENT_COUNT(k);j++){
			CHECK_ERROR(sll_encode_integer(wf,k->a[j]));
		}
	}
	CHECK_ERROR(sll_encode_integer(wf,sf->st.l));
	for (sll_string_index_t i=0;i<sf->st.l;i++){
		CHECK_ERROR(sll_encode_string(wf,sf->st.dt+i));
	}
	CHECK_ERROR(sll_encode_integer(wf,sf->it.l));
	for (sll_import_index_t i=0;i<sf->it.l;i++){
		sll_import_file_t* if_=*(sf->it.dt+i);
		CHECK_ERROR(sll_encode_integer(wf,if_->sfi));
		CHECK_ERROR(sll_encode_integer(wf,if_->l));
		for (sll_identifier_list_length_t j=0;j<if_->l;j++){
			CHECK_ERROR(sll_encode_integer(wf,if_->dt[j]));
		}
	}
	CHECK_ERROR(sll_encode_integer(wf,sf->fp_nm));
	CHECK_ERROR(sll_encode_integer(wf,sf->_n_sc_id));
	if (sf->dt){
		_write_node(wf,sf->dt);
	}
	else{
		sll_file_write_char(wf,SLL_NODE_TYPE_INT,NULL);
		CHECK_ERROR(sll_encode_signed_integer(wf,0));
	}
}



__SLL_EXTERNAL void sll_write_assembly(sll_file_t* wf,const sll_assembly_data_t* a_dt){
	magic_number_t n=ASSEMBLY_FILE_MAGIC_NUMBER;
	sll_file_write(wf,&n,sizeof(magic_number_t),NULL);
	sll_version_t v=SLL_VERSION;
	sll_file_write(wf,&v,sizeof(sll_version_t),NULL);
	CHECK_ERROR(sll_encode_integer(wf,a_dt->tm));
	CHECK_ERROR(sll_encode_integer(wf,a_dt->ic));
	CHECK_ERROR(sll_encode_integer(wf,a_dt->vc));
	CHECK_ERROR(sll_encode_integer(wf,a_dt->tls_vc));
	CHECK_ERROR(sll_encode_integer(wf,a_dt->ft.l));
	for (sll_function_index_t i=0;i<a_dt->ft.l;i++){
		CHECK_ERROR(sll_encode_integer(wf,(a_dt->ft.dt+i)->i));
		CHECK_ERROR(sll_encode_integer(wf,(a_dt->ft.dt+i)->ac));
		CHECK_ERROR(sll_encode_integer(wf,(a_dt->ft.dt+i)->nm));
	}
	CHECK_ERROR(sll_encode_integer(wf,a_dt->st.l));
	for (sll_string_index_t i=0;i<a_dt->st.l;i++){
		CHECK_ERROR(sll_encode_string(wf,a_dt->st.dt+i));
	}
	CHECK_ERROR(sll_encode_integer(wf,a_dt->dbg.l));
	for (sll_debug_data_length_t i=0;i<a_dt->dbg.l;i++){
		CHECK_ERROR(sll_encode_integer(wf,(a_dt->dbg.dt+i)->ii));
		CHECK_ERROR(sll_encode_integer(wf,(a_dt->dbg.dt+i)->ln));
	}
	const sll_assembly_instruction_t* ai=a_dt->h;
	for (sll_instruction_index_t i=0;i<a_dt->ic;i++){
		sll_file_write_char(wf,ai->t,NULL);
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				CHECK_ERROR(sll_encode_signed_integer(wf,ai->dt.i));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED:
				CHECK_ERROR(sll_encode_integer(wf,ai->dt.ci));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				sll_file_write(wf,&(ai->dt.f),sizeof(sll_float_t),NULL);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_COMPLEX:
				sll_file_write(wf,&(ai->dt.d),sizeof(sll_complex_t),NULL);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				sll_file_write_char(wf,ai->dt.c,NULL);
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
					CHECK_ERROR(sll_encode_signed_integer(wf,ai->dt.j.t.rel));
				}
				else{
					CHECK_ERROR(sll_encode_integer(wf,ai->dt.j.t.abs));
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK:
				CHECK_ERROR(sll_encode_integer(wf,ai->dt.so));
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
				CHECK_ERROR(sll_encode_integer(wf,ai->dt.v));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				CHECK_ERROR(sll_encode_integer(wf,ai->dt.s));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				CHECK_ERROR(sll_encode_integer(wf,ai->dt.al));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				CHECK_ERROR(sll_encode_integer(wf,ai->dt.ml));
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
					CHECK_ERROR(sll_encode_integer(wf,ai->dt.v));
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				CHECK_ERROR(sll_encode_integer(wf,ai->dt.va.v));
				CHECK_ERROR(sll_encode_integer(wf,ai->dt.va.l));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				CHECK_ERROR(sll_encode_integer(wf,ai->dt.t));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				CHECK_ERROR(sll_encode_integer(wf,ai->dt.ac));
				break;
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->dt._p;
		}
	}
}



__SLL_EXTERNAL void sll_write_bundle(sll_file_t* wf,const sll_bundle_t* b){
	magic_number_t n=BUNDLE_FILE_MAGIC_NUMBER;
	sll_file_write(wf,&n,sizeof(magic_number_t),NULL);
	sll_version_t v=SLL_VERSION;
	sll_file_write(wf,&v,sizeof(sll_version_t),NULL);
	CHECK_ERROR(sll_encode_string(wf,&(b->nm)));
	CHECK_ERROR(sll_encode_integer(wf,b->l));
	for (sll_source_file_index_t i=0;i<b->l;i++){
		const sll_bundle_source_file_t* bsf=*(b->dt+i);
		CHECK_ERROR(sll_encode_string(wf,&(bsf->nm)));
		_write_source_file(wf,&(bsf->dt));
	}
}



__SLL_EXTERNAL void sll_write_compiled_node(sll_file_t* wf,const sll_compilation_data_t* c_dt){
	magic_number_t n=COMPLIED_OBJECT_FILE_MAGIC_NUMBER;
	sll_file_write(wf,&n,sizeof(magic_number_t),NULL);
	sll_version_t v=SLL_VERSION;
	sll_file_write(wf,&v,sizeof(sll_version_t),NULL);
	CHECK_ERROR(sll_encode_integer(wf,c_dt->l));
	for (sll_source_file_index_t i=0;i<c_dt->l;i++){
		_write_source_file(wf,*(c_dt->dt+i));
	}
}

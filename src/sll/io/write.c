#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/_internal/magic_number.h>
#include <sll/api/hash.h>
#include <sll/api/serial.h>
#include <sll/assembly.h>
#include <sll/bundle.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/string_table.h>
#include <sll/types.h>
#include <sll/version.h>



#define WRITE_FIELD(f,wf) sll_file_write((wf),&(f),sizeof((f)),NULL)
#define CHECK_ERROR(fn) SLL_CRITICAL((fn)==SLL_NO_ERROR)



static const sll_node_t* _write_node(sll_file_t* wf,const sll_node_t* o){
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG||o->type==SLL_NODE_TYPE_CHANGE_STACK){
		if (o->type==SLL_NODE_TYPE_CHANGE_STACK){
			o=o->data._next_node;
			continue;
		}
		sll_file_write_char(wf,o->type,NULL);
		if (o->type==SLL_NODE_TYPE_DBG){
			CHECK_ERROR(sll_encode_integer(wf,o->data.string_index+1));
		}
		o++;
	}
	sll_file_write_char(wf,o->type,NULL);
	switch (o->type){
		case SLL_NODE_TYPE_INT:
			CHECK_ERROR(sll_encode_signed_integer(wf,o->data.int_));
			return o+1;
		case SLL_NODE_TYPE_FLOAT:
			WRITE_FIELD(o->data.float_,wf);
			return o+1;
		case SLL_NODE_TYPE_CHAR:
			sll_file_write_char(wf,o->data.char_,NULL);
			return o+1;
		case SLL_NODE_TYPE_COMPLEX:
			WRITE_FIELD(o->data.complex_,wf);
			return o+1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			CHECK_ERROR(sll_encode_integer(wf,o->data.string_index));
			return o+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				CHECK_ERROR(sll_encode_integer(wf,o->data.array_length));
				sll_array_length_t l=o->data.array_length;
				o++;
				while (l){
					l--;
					o=_write_node(wf,o);
				}
				return o;
			}
		case SLL_NODE_TYPE_MAP:
			{
				CHECK_ERROR(sll_encode_integer(wf,o->data.map_length));
				sll_map_length_t l=o->data.map_length;
				o++;
				while (l){
					l--;
					o=_write_node(wf,o);
				}
				return o;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			CHECK_ERROR(sll_encode_integer(wf,o->data.identifier_index));
			return o+1;
		case SLL_NODE_TYPE_FUNCTION_ID:
			CHECK_ERROR(sll_encode_integer(wf,o->data.function_index));
			return o+1;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				CHECK_ERROR(sll_encode_integer(wf,o->data.function.arg_count));
				CHECK_ERROR(sll_encode_integer(wf,o->data.function.function_index));
				if (o->type==SLL_NODE_TYPE_FUNC){
					CHECK_ERROR(sll_encode_integer(wf,o->data.function.scope));
				}
				sll_arg_count_t l=o->data.function.arg_count;
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
				CHECK_ERROR(sll_encode_integer(wf,o->data.loop.arg_count));
				CHECK_ERROR(sll_encode_integer(wf,o->data.loop.scope));
				sll_arg_count_t l=o->data.loop.arg_count;
				o++;
				while (l){
					l--;
					o=_write_node(wf,o);
				}
				return o;
			}
		case SLL_NODE_TYPE_DECL:
			{
				CHECK_ERROR(sll_encode_integer(wf,o->data.declaration.arg_count));
				CHECK_ERROR(sll_encode_integer(wf,o->data.declaration.name_string_index+1));
				sll_arg_count_t l=o->data.declaration.arg_count;
				o++;
				while (l){
					l--;
					o=_write_node(wf,o);
				}
				return o;
			}
	}
	CHECK_ERROR(sll_encode_integer(wf,o->data.arg_count));
	sll_arg_count_t l=o->data.arg_count;
	o++;
	while (l){
		l--;
		o=_write_node(wf,o);
	}
	return o;
}



static void _write_source_file(sll_file_t* wf,const sll_source_file_t* sf){
	CHECK_ERROR(sll_encode_integer(wf,sf->time));
	CHECK_ERROR(sll_encode_integer(wf,sf->file_size));
	sll_file_write(wf,&(sf->file_hash),sizeof(sll_sha256_data_t),NULL);
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		const sll_identifier_list_t* l=sf->identifier_table.short_+i;
		CHECK_ERROR(sll_encode_integer(wf,l->length));
		for (sll_identifier_list_length_t j=0;j<l->length;j++){
			CHECK_ERROR(sll_encode_integer(wf,(l->data+j)->scope));
			CHECK_ERROR(sll_encode_integer(wf,(l->data+j)->name_string_index));
		}
	}
	CHECK_ERROR(sll_encode_integer(wf,sf->identifier_table.long_data_length));
	for (sll_identifier_list_length_t i=0;i<sf->identifier_table.long_data_length;i++){
		CHECK_ERROR(sll_encode_integer(wf,(sf->identifier_table.long_data+i)->scope));
		CHECK_ERROR(sll_encode_integer(wf,(sf->identifier_table.long_data+i)->name_string_index));
	}
	CHECK_ERROR(sll_encode_integer(wf,sf->export_table.length));
	for (sll_export_table_length_t i=0;i<sf->export_table.length;i++){
		CHECK_ERROR(sll_encode_integer(wf,*(sf->export_table.data+i)));
	}
	CHECK_ERROR(sll_encode_integer(wf,sf->function_table.length));
	for (sll_function_index_t i=0;i<sf->function_table.length;i++){
		const sll_function_t* k=*(sf->function_table.data+i);
		CHECK_ERROR(sll_encode_integer(wf,k->offset));
		CHECK_ERROR(sll_encode_integer(wf,k->arg_count));
		CHECK_ERROR(sll_encode_integer(wf,k->name_string_index+1));
		CHECK_ERROR(sll_encode_integer(wf,k->description_string_index+1));
		for (sll_arg_count_t j=0;j<SLL_FUNCTION_GET_ARGUMENT_COUNT(k);j++){
			CHECK_ERROR(sll_encode_integer(wf,k->args[j]));
		}
	}
	CHECK_ERROR(sll_encode_integer(wf,sf->string_table.length));
	for (sll_string_index_t i=0;i<sf->string_table.length;i++){
		CHECK_ERROR(sll_encode_string(wf,sf->string_table.data+i));
	}
	CHECK_ERROR(sll_encode_integer(wf,sf->import_table.length));
	for (sll_import_index_t i=0;i<sf->import_table.length;i++){
		sll_import_file_t* if_=*(sf->import_table.data+i);
		CHECK_ERROR(sll_encode_integer(wf,if_->source_file_index));
		CHECK_ERROR(sll_encode_integer(wf,if_->length));
		for (sll_identifier_list_length_t j=0;j<if_->length;j++){
			CHECK_ERROR(sll_encode_integer(wf,if_->data[j]));
		}
	}
	CHECK_ERROR(sll_encode_integer(wf,sf->file_path_string_index));
	CHECK_ERROR(sll_encode_integer(wf,sf->_next_scope));
	if (sf->first_node){
		_write_node(wf,sf->first_node);
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
	CHECK_ERROR(sll_encode_integer(wf,a_dt->time));
	CHECK_ERROR(sll_encode_integer(wf,a_dt->instruction_count));
	CHECK_ERROR(sll_encode_integer(wf,a_dt->variable_count));
	CHECK_ERROR(sll_encode_integer(wf,a_dt->tls_variable_count));
	CHECK_ERROR(sll_encode_integer(wf,a_dt->function_table.length));
	for (sll_function_index_t i=0;i<a_dt->function_table.length;i++){
		CHECK_ERROR(sll_encode_integer(wf,(a_dt->function_table.data+i)->instruction_index));
		CHECK_ERROR(sll_encode_integer(wf,(a_dt->function_table.data+i)->arg_count));
		CHECK_ERROR(sll_encode_integer(wf,(a_dt->function_table.data+i)->name_string_index));
	}
	CHECK_ERROR(sll_encode_integer(wf,a_dt->string_table.length));
	for (sll_string_index_t i=0;i<a_dt->string_table.length;i++){
		CHECK_ERROR(sll_encode_string(wf,a_dt->string_table.data+i));
	}
	CHECK_ERROR(sll_encode_integer(wf,a_dt->debug_data.length));
	for (sll_debug_data_length_t i=0;i<a_dt->debug_data.length;i++){
		CHECK_ERROR(sll_encode_integer(wf,(a_dt->debug_data.data+i)->delta_instruction_index));
		CHECK_ERROR(sll_encode_integer(wf,(a_dt->debug_data.data+i)->line));
	}
	const sll_assembly_instruction_t* ai=a_dt->first_instruction;
	for (sll_instruction_index_t i=0;i<a_dt->instruction_count;i++){
		sll_file_write_char(wf,ai->type,NULL);
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				CHECK_ERROR(sll_encode_signed_integer(wf,ai->data.int_));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED:
				CHECK_ERROR(sll_encode_integer(wf,ai->data.compressed_int));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				WRITE_FIELD(ai->data.float_,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_COMPLEX:
				WRITE_FIELD(ai->data.complex_,wf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				sll_file_write_char(wf,ai->data.char_,NULL);
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
					CHECK_ERROR(sll_encode_signed_integer(wf,ai->data.jump.target.rel));
				}
				else{
					CHECK_ERROR(sll_encode_integer(wf,ai->data.jump.target.abs));
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK:
				CHECK_ERROR(sll_encode_integer(wf,ai->data.stack_offset));
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
				CHECK_ERROR(sll_encode_integer(wf,ai->data.variable));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				CHECK_ERROR(sll_encode_integer(wf,ai->data.string_index));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				CHECK_ERROR(sll_encode_integer(wf,ai->data.array_length));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				CHECK_ERROR(sll_encode_integer(wf,ai->data.map_length));
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
					CHECK_ERROR(sll_encode_integer(wf,ai->data.variable));
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				CHECK_ERROR(sll_encode_integer(wf,ai->data.variable_access.variable));
				CHECK_ERROR(sll_encode_integer(wf,ai->data.variable_access.arg_count));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				CHECK_ERROR(sll_encode_integer(wf,ai->data.type));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				CHECK_ERROR(sll_encode_integer(wf,ai->data.arg_count));
				break;
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->data._next_instruction;
		}
	}
}



__SLL_EXTERNAL void sll_write_bundle(sll_file_t* wf,const sll_bundle_t* b){
	magic_number_t n=BUNDLE_FILE_MAGIC_NUMBER;
	sll_file_write(wf,&n,sizeof(magic_number_t),NULL);
	sll_version_t v=SLL_VERSION;
	sll_file_write(wf,&v,sizeof(sll_version_t),NULL);
	CHECK_ERROR(sll_encode_string(wf,&(b->name)));
	CHECK_ERROR(sll_encode_integer(wf,b->length));
	for (sll_source_file_index_t i=0;i<b->length;i++){
		const sll_bundle_source_file_t* bsf=*(b->data+i);
		CHECK_ERROR(sll_encode_string(wf,&(bsf->name)));
		_write_source_file(wf,&(bsf->data));
	}
}



__SLL_EXTERNAL void sll_write_compiled_node(sll_file_t* wf,const sll_compilation_data_t* c_dt){
	magic_number_t n=COMPLIED_OBJECT_FILE_MAGIC_NUMBER;
	sll_file_write(wf,&n,sizeof(magic_number_t),NULL);
	sll_version_t v=SLL_VERSION;
	sll_file_write(wf,&v,sizeof(sll_version_t),NULL);
	CHECK_ERROR(sll_encode_integer(wf,c_dt->length));
	for (sll_source_file_index_t i=0;i<c_dt->length;i++){
		_write_source_file(wf,*(c_dt->data+i));
	}
}

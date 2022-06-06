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



#define WRITE_FIELD(field,out) sll_file_write((out),&(field),sizeof((field)),NULL)



static const sll_node_t* _write_node(const sll_node_t* node,sll_file_t* out){
	while (node->type==SLL_NODE_TYPE_NOP||node->type==SLL_NODE_TYPE_DBG||node->type==SLL_NODE_TYPE_CHANGE_STACK){
		if (node->type==SLL_NODE_TYPE_CHANGE_STACK){
			node=node->data._next_node;
			continue;
		}
		sll_file_write_char(out,node->type,NULL);
		if (node->type==SLL_NODE_TYPE_DBG){
			SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.string_index+1));
		}
		node++;
	}
	sll_file_write_char(out,node->type,NULL);
	switch (node->type){
		case SLL_NODE_TYPE_INT:
			SLL_CRITICAL_ERROR(sll_encode_signed_integer(out,node->data.int_));
			return node+1;
		case SLL_NODE_TYPE_FLOAT:
			WRITE_FIELD(node->data.float_,out);
			return node+1;
		case SLL_NODE_TYPE_CHAR:
			sll_file_write_char(out,node->data.char_,NULL);
			return node+1;
		case SLL_NODE_TYPE_COMPLEX:
			WRITE_FIELD(node->data.complex_,out);
			return node+1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.string_index));
			return node+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.array_length));
				sll_array_length_t l=node->data.array_length;
				node++;
				while (l){
					l--;
					node=_write_node(node,out);
				}
				return node;
			}
		case SLL_NODE_TYPE_MAP:
			{
				SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.map_length));
				sll_map_length_t l=node->data.map_length;
				node++;
				while (l){
					l--;
					node=_write_node(node,out);
				}
				return node;
			}
		case SLL_NODE_TYPE_IDENTIFIER:
			SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.identifier_index));
			return node+1;
		case SLL_NODE_TYPE_FUNCTION_ID:
			SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.function_index));
			return node+1;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.function.arg_count));
				SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.function.function_index));
				if (node->type==SLL_NODE_TYPE_FUNC){
					SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.function.scope));
				}
				sll_arg_count_t l=node->data.function.arg_count;
				node++;
				while (l){
					l--;
					node=_write_node(node,out);
				}
				return node;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.loop.arg_count));
				SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.loop.scope));
				sll_arg_count_t l=node->data.loop.arg_count;
				node++;
				while (l){
					l--;
					node=_write_node(node,out);
				}
				return node;
			}
		case SLL_NODE_TYPE_DECL:
			{
				SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.declaration.arg_count));
				SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.declaration.name_string_index+1));
				sll_arg_count_t l=node->data.declaration.arg_count;
				node++;
				while (l){
					l--;
					node=_write_node(node,out);
				}
				return node;
			}
	}
	SLL_CRITICAL_ERROR(sll_encode_integer(out,node->data.arg_count));
	sll_arg_count_t l=node->data.arg_count;
	node++;
	while (l){
		l--;
		node=_write_node(node,out);
	}
	return node;
}



static void _write_source_file(const sll_source_file_t* source_file,sll_file_t* out){
	SLL_CRITICAL_ERROR(sll_encode_integer(out,source_file->time));
	SLL_CRITICAL_ERROR(sll_encode_integer(out,source_file->file_size));
	sll_file_write(out,&(source_file->file_hash),sizeof(sll_sha256_data_t),NULL);
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		const sll_identifier_list_t* l=source_file->identifier_table.short_+i;
		SLL_CRITICAL_ERROR(sll_encode_integer(out,l->length));
		for (sll_identifier_list_length_t j=0;j<l->length;j++){
			SLL_CRITICAL_ERROR(sll_encode_integer(out,(l->data+j)->scope));
			SLL_CRITICAL_ERROR(sll_encode_integer(out,(l->data+j)->name_string_index));
		}
	}
	SLL_CRITICAL_ERROR(sll_encode_integer(out,source_file->identifier_table.long_data_length));
	for (sll_identifier_list_length_t i=0;i<source_file->identifier_table.long_data_length;i++){
		SLL_CRITICAL_ERROR(sll_encode_integer(out,(source_file->identifier_table.long_data+i)->scope));
		SLL_CRITICAL_ERROR(sll_encode_integer(out,(source_file->identifier_table.long_data+i)->name_string_index));
	}
	SLL_CRITICAL_ERROR(sll_encode_integer(out,source_file->export_table.length));
	for (sll_export_table_length_t i=0;i<source_file->export_table.length;i++){
		SLL_CRITICAL_ERROR(sll_encode_integer(out,*(source_file->export_table.data+i)));
	}
	SLL_CRITICAL_ERROR(sll_encode_integer(out,source_file->function_table.length));
	for (sll_function_index_t i=0;i<source_file->function_table.length;i++){
		const sll_function_t* k=*(source_file->function_table.data+i);
		SLL_CRITICAL_ERROR(sll_encode_integer(out,k->offset));
		SLL_CRITICAL_ERROR(sll_encode_integer(out,k->arg_count));
		SLL_CRITICAL_ERROR(sll_encode_integer(out,k->name_string_index+1));
		SLL_CRITICAL_ERROR(sll_encode_integer(out,k->description_string_index+1));
		for (sll_arg_count_t j=0;j<SLL_FUNCTION_GET_ARGUMENT_COUNT(k);j++){
			SLL_CRITICAL_ERROR(sll_encode_integer(out,k->args[j]));
		}
	}
	SLL_CRITICAL_ERROR(sll_encode_integer(out,source_file->string_table.length));
	for (sll_string_index_t i=0;i<source_file->string_table.length;i++){
		SLL_CRITICAL_ERROR(sll_encode_string(out,source_file->string_table.data+i));
	}
	SLL_CRITICAL_ERROR(sll_encode_integer(out,source_file->import_table.length));
	for (sll_import_index_t i=0;i<source_file->import_table.length;i++){
		sll_import_file_t* if_=*(source_file->import_table.data+i);
		SLL_CRITICAL_ERROR(sll_encode_integer(out,if_->source_file_index));
		SLL_CRITICAL_ERROR(sll_encode_integer(out,if_->length));
		for (sll_identifier_list_length_t j=0;j<if_->length;j++){
			SLL_CRITICAL_ERROR(sll_encode_integer(out,if_->data[j]));
		}
	}
	SLL_CRITICAL_ERROR(sll_encode_integer(out,source_file->file_path_string_index));
	SLL_CRITICAL_ERROR(sll_encode_integer(out,source_file->_next_scope));
	if (source_file->first_node){
		_write_node(source_file->first_node,out);
	}
	else{
		sll_file_write_char(out,SLL_NODE_TYPE_INT,NULL);
		SLL_CRITICAL_ERROR(sll_encode_signed_integer(out,0));
	}
}



__SLL_EXTERNAL void sll_write_assembly(const sll_assembly_data_t* assembly_data,sll_file_t* out){
	magic_number_t n=ASSEMBLY_FILE_MAGIC_NUMBER;
	sll_file_write(out,&n,sizeof(magic_number_t),NULL);
	sll_version_t v=SLL_VERSION;
	sll_file_write(out,&v,sizeof(sll_version_t),NULL);
	SLL_CRITICAL_ERROR(sll_encode_integer(out,assembly_data->time));
	SLL_CRITICAL_ERROR(sll_encode_integer(out,assembly_data->instruction_count));
	SLL_CRITICAL_ERROR(sll_encode_integer(out,assembly_data->variable_count));
	SLL_CRITICAL_ERROR(sll_encode_integer(out,assembly_data->tls_variable_count));
	SLL_CRITICAL_ERROR(sll_encode_integer(out,assembly_data->function_table.length));
	for (sll_function_index_t i=0;i<assembly_data->function_table.length;i++){
		SLL_CRITICAL_ERROR(sll_encode_integer(out,(assembly_data->function_table.data+i)->instruction_index));
		SLL_CRITICAL_ERROR(sll_encode_integer(out,(assembly_data->function_table.data+i)->arg_count));
	}
	SLL_CRITICAL_ERROR(sll_encode_integer(out,assembly_data->string_table.length));
	for (sll_string_index_t i=0;i<assembly_data->string_table.length;i++){
		SLL_CRITICAL_ERROR(sll_encode_string(out,assembly_data->string_table.data+i));
	}
	SLL_CRITICAL_ERROR(sll_encode_integer(out,assembly_data->debug_data.length));
	for (sll_debug_data_length_t i=0;i<assembly_data->debug_data.length;i++){
		SLL_CRITICAL_ERROR(sll_encode_integer(out,(assembly_data->debug_data.data+i)->delta_instruction_index));
		SLL_CRITICAL_ERROR(sll_encode_integer(out,(assembly_data->debug_data.data+i)->line));
	}
	const sll_assembly_instruction_t* ai=assembly_data->first_instruction;
	for (sll_instruction_index_t i=0;i<assembly_data->instruction_count;i++){
		sll_file_write_char(out,ai->type,NULL);
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				SLL_CRITICAL_ERROR(sll_encode_signed_integer(out,ai->data.int_));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED:
				SLL_CRITICAL_ERROR(sll_encode_integer(out,ai->data.compressed_int));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				WRITE_FIELD(ai->data.float_,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_COMPLEX:
				WRITE_FIELD(ai->data.complex_,out);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				sll_file_write_char(out,ai->data.char_,NULL);
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
					SLL_CRITICAL_ERROR(sll_encode_signed_integer(out,ai->data.jump.target.rel));
				}
				else{
					SLL_CRITICAL_ERROR(sll_encode_integer(out,ai->data.jump.target.abs));
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK:
				SLL_CRITICAL_ERROR(sll_encode_integer(out,ai->data.stack_offset));
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
				SLL_CRITICAL_ERROR(sll_encode_integer(out,ai->data.variable));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				SLL_CRITICAL_ERROR(sll_encode_integer(out,ai->data.string_index));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				SLL_CRITICAL_ERROR(sll_encode_integer(out,ai->data.array_length));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				SLL_CRITICAL_ERROR(sll_encode_integer(out,ai->data.map_length));
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
					SLL_CRITICAL_ERROR(sll_encode_integer(out,ai->data.variable));
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				SLL_CRITICAL_ERROR(sll_encode_integer(out,ai->data.variable_access.variable));
				SLL_CRITICAL_ERROR(sll_encode_integer(out,ai->data.variable_access.arg_count));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				SLL_CRITICAL_ERROR(sll_encode_integer(out,ai->data.type));
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				SLL_CRITICAL_ERROR(sll_encode_integer(out,ai->data.arg_count));
				break;
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->data._next_instruction;
		}
	}
}



__SLL_EXTERNAL void sll_write_bundle(const sll_bundle_t* bundle,sll_file_t* out){
	magic_number_t n=BUNDLE_FILE_MAGIC_NUMBER;
	sll_file_write(out,&n,sizeof(magic_number_t),NULL);
	sll_version_t v=SLL_VERSION;
	sll_file_write(out,&v,sizeof(sll_version_t),NULL);
	SLL_CRITICAL_ERROR(sll_encode_string(out,&(bundle->name)));
	SLL_CRITICAL_ERROR(sll_encode_integer(out,bundle->length));
	for (sll_source_file_index_t i=0;i<bundle->length;i++){
		const sll_bundle_source_file_t* bsf=*(bundle->data+i);
		SLL_CRITICAL_ERROR(sll_encode_string(out,&(bsf->name)));
		_write_source_file(&(bsf->data),out);
	}
}



__SLL_EXTERNAL void sll_write_compiled_node(const sll_compilation_data_t* compilation_data,sll_file_t* out){
	magic_number_t n=COMPLIED_OBJECT_FILE_MAGIC_NUMBER;
	sll_file_write(out,&n,sizeof(magic_number_t),NULL);
	sll_version_t v=SLL_VERSION;
	sll_file_write(out,&v,sizeof(sll_version_t),NULL);
	SLL_CRITICAL_ERROR(sll_encode_integer(out,compilation_data->length));
	for (sll_source_file_index_t i=0;i<compilation_data->length;i++){
		_write_source_file(*(compilation_data->data+i),out);
	}
}

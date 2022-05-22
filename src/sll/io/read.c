#include <sll/_internal/common.h>
#include <sll/_internal/intrinsics.h>
#include <sll/_internal/magic_number.h>
#include <sll/_internal/stack.h>
#include <sll/api/serial.h>
#include <sll/api/time.h>
#include <sll/assembly.h>
#include <sll/bundle.h>
#include <sll/common.h>
#include <sll/compression.h>
#include <sll/data.h>
#include <sll/error.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/object.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>
#include <sll/version.h>



#define CHECK_ERROR(rf,o,ot) \
	do{ \
		sll_error_t __e=0; \
		(o)=(ot)sll_decode_integer((rf),&__e); \
		if (__e!=SLL_NO_ERROR){ \
			return 0; \
		} \
	} while (0)
#define READ_CHAR(f,rf) \
	do{ \
		sll_read_char_t __c=sll_file_read_char((rf),NULL); \
		if (__c==SLL_END_OF_DATA){ \
			return 0; \
		} \
		f=(sll_char_t)__c; \
	} while(0)
#define READ_FIELD(f,rf) \
	do{ \
		if (sll_file_read((rf),&(f),sizeof(f),NULL)!=sizeof(f)){ \
			return 0; \
		} \
	} while(0)



static sll_bool_t _read_node(sll_source_file_t* sf,sll_file_t* rf){
	sll_node_t* o=_acquire_next_node(sf);
	READ_CHAR(o->type,rf);
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG){
		if (o->type==SLL_NODE_TYPE_DBG){
			CHECK_ERROR(rf,o->data.string_index,sll_string_index_t);
			o->data.string_index--;
		}
		o=_acquire_next_node(sf);
		READ_CHAR(o->type,rf);
	}
	switch (o->type){
		case SLL_NODE_TYPE_INT:
			{
				sll_error_t err;
				o->data.int_=sll_decode_signed_integer(rf,&err);
				if (err!=SLL_NO_ERROR){
					return 0;
				}
				return 1;
			}
		case SLL_NODE_TYPE_FLOAT:
			READ_FIELD(o->data.float_,rf);
			return 1;
		case SLL_NODE_TYPE_CHAR:
			READ_CHAR(o->data.char_,rf);
			return 1;
		case SLL_NODE_TYPE_COMPLEX:
			READ_FIELD(o->data.complex_,rf);
			return 1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			CHECK_ERROR(rf,o->data.string_index,sll_string_index_t);
			return 1;
		case SLL_NODE_TYPE_ARRAY:
			CHECK_ERROR(rf,o->data.array_length,sll_array_length_t);
			for (sll_array_length_t i=0;i<o->data.array_length;i++){
				if (!_read_node(sf,rf)){
					return 0;
				}
			}
			return 1;
		case SLL_NODE_TYPE_MAP:
			CHECK_ERROR(rf,o->data.map_length,sll_map_length_t);
			for (sll_map_length_t i=0;i<o->data.map_length;i++){
				if (!_read_node(sf,rf)){
					return 0;
				}
			}
			return 1;
		case SLL_NODE_TYPE_IDENTIFIER:
			CHECK_ERROR(rf,o->data.identifier_index,sll_identifier_index_t);
			return 1;
		case SLL_NODE_TYPE_FUNCTION_ID:
			CHECK_ERROR(rf,o->data.function_index,sll_function_index_t);
			return 1;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			CHECK_ERROR(rf,o->data.function.arg_count,sll_arg_count_t);
			CHECK_ERROR(rf,o->data.function.function_index,sll_function_index_t);
			if (o->type==SLL_NODE_TYPE_FUNC){
				CHECK_ERROR(rf,o->data.function.scope,sll_scope_t);
			}
			for (sll_arg_count_t i=0;i<o->data.function.arg_count;i++){
				if (!_read_node(sf,rf)){
					return 0;
				}
			}
			return 1;
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			CHECK_ERROR(rf,o->data.loop.arg_count,sll_arg_count_t);
			CHECK_ERROR(rf,o->data.loop.scope,sll_scope_t);
			for (sll_arg_count_t i=0;i<o->data.loop.arg_count;i++){
				if (!_read_node(sf,rf)){
					return 0;
				}
			}
			return 1;
		case SLL_NODE_TYPE_DECL:
			CHECK_ERROR(rf,o->data.declaration.arg_count,sll_arg_count_t);
			CHECK_ERROR(rf,o->data.declaration.name_string_index,sll_string_index_t);
			o->data.declaration.name_string_index--;
			for (sll_arg_count_t i=0;i<o->data.declaration.arg_count;i++){
				if (!_read_node(sf,rf)){
					return 0;
				}
			}
			return 1;
	}
	CHECK_ERROR(rf,o->data.arg_count,sll_arg_count_t);
	for (sll_arg_count_t i=0;i<o->data.arg_count;i++){
		if (!_read_node(sf,rf)){
			return 0;
		}
	}
	return 1;
}



static sll_bool_t _read_source_file(sll_file_t* rf,sll_source_file_t* sf){
	CHECK_ERROR(rf,sf->time,sll_time_t);
	CHECK_ERROR(rf,sf->file_size,sll_file_offset_t);
	READ_FIELD(sf->file_hash,rf);
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		CHECK_ERROR(rf,sf->identifier_table.short_[i].length,sll_identifier_list_length_t);
		sf->identifier_table.short_[i].data=sll_allocate(sf->identifier_table.short_[i].length*sizeof(sll_identifier_t));
		for (sll_identifier_list_length_t j=0;j<sf->identifier_table.short_[i].length;j++){
			CHECK_ERROR(rf,(sf->identifier_table.short_[i].data+j)->scope,sll_scope_t);
			CHECK_ERROR(rf,(sf->identifier_table.short_[i].data+j)->name_string_index,sll_string_index_t);
		}
	}
	CHECK_ERROR(rf,sf->identifier_table.long_data_length,sll_identifier_list_length_t);
	sf->identifier_table.long_data=sll_allocate(sf->identifier_table.long_data_length*sizeof(sll_identifier_t));
	for (sll_identifier_list_length_t i=0;i<sf->identifier_table.long_data_length;i++){
		CHECK_ERROR(rf,(sf->identifier_table.long_data+i)->scope,sll_scope_t);
		CHECK_ERROR(rf,(sf->identifier_table.long_data+i)->name_string_index,sll_string_index_t);
	}
	CHECK_ERROR(rf,sf->export_table.length,sll_export_table_length_t);
	sf->export_table.data=sll_allocate(sf->export_table.length*sizeof(sll_identifier_index_t));
	for (sll_export_table_length_t i=0;i<sf->export_table.length;i++){
		CHECK_ERROR(rf,*(sf->export_table.data+i),sll_identifier_index_t);
	}
	CHECK_ERROR(rf,sf->function_table.length,sll_function_index_t);
	sf->function_table.data=sll_allocate(sf->function_table.length*sizeof(sll_function_t*));
	for (sll_function_index_t i=0;i<sf->function_table.length;i++){
		sll_node_offset_t off;
		CHECK_ERROR(rf,off,sll_node_offset_t);
		sll_arg_count_t al;
		CHECK_ERROR(rf,al,sll_arg_count_t);
		sll_function_t* k=sll_allocate(sizeof(sll_function_t)+SLL_FUNCTION_GET_ARGUMENT_COUNT_RAW(al)*sizeof(sll_identifier_index_t));
		k->offset=off;
		CHECK_ERROR(rf,k->name_string_index,sll_string_index_t);
		k->name_string_index--;
		CHECK_ERROR(rf,k->description_string_index,sll_string_index_t);
		k->description_string_index--;
		k->arg_count=al;
		for (sll_arg_count_t j=0;j<SLL_FUNCTION_GET_ARGUMENT_COUNT_RAW(al);j++){
			CHECK_ERROR(rf,k->args[j],sll_identifier_index_t);
		}
		*(sf->function_table.data+i)=k;
	}
	CHECK_ERROR(rf,sf->string_table.length,sll_string_index_t);
	sf->string_table.data=sll_allocate(sf->string_table.length*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<sf->string_table.length;i++){
		if (!sll_decode_string(rf,sf->string_table.data+i)){
			return 0;
		}
	}
	CHECK_ERROR(rf,sf->import_table.length,sll_import_index_t);
	sf->import_table.data=sll_allocate(sf->import_table.length*sizeof(sll_import_file_t*));
	for (sll_import_index_t i=0;i<sf->import_table.length;i++){
		sll_source_file_index_t sfi;
		CHECK_ERROR(rf,sfi,sll_source_file_index_t);
		sll_identifier_list_length_t l;
		CHECK_ERROR(rf,l,sll_identifier_list_length_t);
		sll_import_file_t* if_=sll_allocate(sizeof(sll_import_file_t)+l*sizeof(sll_identifier_index_t));
		*(sf->import_table.data+i)=if_;
		if_->source_file_index=sfi;
		if_->length=l;
		for (sll_identifier_list_length_t j=0;j<l;j++){
			CHECK_ERROR(rf,if_->data[j],sll_identifier_index_t);
		}
	}
	CHECK_ERROR(rf,sf->file_path_string_index,sll_string_index_t);
	CHECK_ERROR(rf,sf->_next_scope,sll_scope_t);
	_init_node_stack(sf);
	sf->first_node=sf->_stack.next_node;
	return _read_node(sf,rf);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_assembly(sll_file_t* rf,sll_assembly_data_t* assembly_data){
	magic_number_t n;
	sll_version_t v;
	if (sll_file_read(rf,&n,sizeof(magic_number_t),NULL)!=sizeof(magic_number_t)||n!=ASSEMBLY_FILE_MAGIC_NUMBER||sll_file_read(rf,&v,sizeof(sll_version_t),NULL)!=sizeof(sll_version_t)||v!=SLL_VERSION){
		return 0;
	}
	CHECK_ERROR(rf,assembly_data->time,sll_time_t);
	CHECK_ERROR(rf,assembly_data->instruction_count,sll_instruction_index_t);
	CHECK_ERROR(rf,assembly_data->variable_count,sll_variable_index_t);
	CHECK_ERROR(rf,assembly_data->tls_variable_count,sll_variable_index_t);
	CHECK_ERROR(rf,assembly_data->function_table.length,sll_function_index_t);
	assembly_data->function_table.data=sll_allocate(assembly_data->function_table.length*sizeof(sll_assembly_function_t));
	for (sll_function_index_t i=0;i<assembly_data->function_table.length;i++){
		CHECK_ERROR(rf,(assembly_data->function_table.data+i)->instruction_index,sll_instruction_index_t);
		CHECK_ERROR(rf,(assembly_data->function_table.data+i)->arg_count,sll_arg_count_t);
		CHECK_ERROR(rf,(assembly_data->function_table.data+i)->name_string_index,sll_string_index_t);
	}
	CHECK_ERROR(rf,assembly_data->string_table.length,sll_string_index_t);
	assembly_data->string_table.data=sll_allocate(assembly_data->string_table.length*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<assembly_data->string_table.length;i++){
		if (!sll_decode_string(rf,assembly_data->string_table.data+i)){
			return 0;
		}
	}
	CHECK_ERROR(rf,assembly_data->debug_data.length,sll_instruction_index_t);
	assembly_data->debug_data.data=sll_allocate(assembly_data->debug_data.length*sizeof(sll_debug_line_data_t));
	for (sll_debug_data_length_t i=0;i<assembly_data->debug_data.length;i++){
		CHECK_ERROR(rf,(assembly_data->debug_data.data+i)->delta_instruction_index,sll_instruction_index_t);
		CHECK_ERROR(rf,(assembly_data->debug_data.data+i)->line,sll_file_offset_t);
	}
	_init_assembly_stack(assembly_data);
	assembly_data->first_instruction=assembly_data->_instruction_stack.next_instruction;
	sll_instruction_index_t i=assembly_data->instruction_count;
	assembly_data->instruction_count=0;
	while (i){
		i--;
		sll_assembly_instruction_t* ai=_acquire_next_instruction(assembly_data);
		sll_read_char_t c=sll_file_read_char(rf,NULL);
		if (c==SLL_END_OF_DATA){
			return 0;
		}
		ai->type=(sll_assembly_instruction_type_t)c;
		switch (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)){
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ZERO:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_ONE:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_INT:
				{
					sll_error_t err=0;
					ai->data.int_=sll_decode_signed_integer(rf,&err);
					if (err!=SLL_NO_ERROR){
						return 0;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED:
				CHECK_ERROR(rf,ai->data.compressed_int,sll_compressed_integer_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				READ_FIELD(ai->data.float_,rf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_COMPLEX:
				READ_FIELD(ai->data.complex_,rf);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				c=sll_file_read_char(rf,NULL);
				if (c==SLL_END_OF_DATA){
					return 0;
				}
				ai->data.char_=(sll_char_t)c;
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
					sll_error_t err=0;
					ai->data.jump.target.rel=(sll_relative_instruction_index_t)sll_decode_signed_integer(rf,&err);
					if (err!=SLL_NO_ERROR){
						return 0;
					}
				}
				else{
					CHECK_ERROR(rf,ai->data.jump.target.abs,sll_instruction_index_t);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK:
				CHECK_ERROR(rf,ai->data.stack_offset,sll_stack_offset_t);
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
				CHECK_ERROR(rf,ai->data.variable,sll_variable_index_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				CHECK_ERROR(rf,ai->data.string_index,sll_string_index_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				CHECK_ERROR(rf,ai->data.array_length,sll_array_length_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				CHECK_ERROR(rf,ai->data.map_length,sll_map_length_t);
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
					CHECK_ERROR(rf,ai->data.variable,sll_variable_index_t);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				CHECK_ERROR(rf,ai->data.variable_access.variable,sll_variable_index_t);
				CHECK_ERROR(rf,ai->data.variable_access.arg_count,sll_arg_count_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				CHECK_ERROR(rf,ai->data.type,sll_object_type_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				CHECK_ERROR(rf,ai->data.arg_count,sll_arg_count_t);
				break;
		}
	}
	sll_assembly_instruction_t* ai=assembly_data->first_instruction;
	for (i=0;i<assembly_data->instruction_count;i++){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL||(SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)>=SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP&&SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)<=SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI)){
			ai->data.jump._instruction=_get_instruction_at_offset(assembly_data,(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?i+ai->data.jump.target.rel:ai->data.jump.target.abs));
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->data._next_instruction;
		}
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_bundle(sll_file_t* rf,sll_bundle_t* b){
	magic_number_t n;
	sll_version_t v;
	if (sll_file_read(rf,&n,sizeof(magic_number_t),NULL)!=sizeof(magic_number_t)||n!=BUNDLE_FILE_MAGIC_NUMBER||sll_file_read(rf,&v,sizeof(sll_version_t),NULL)!=sizeof(sll_version_t)||v!=SLL_VERSION||!sll_decode_string(rf,&(b->name))){
		return 0;
	}
	CHECK_ERROR(rf,b->length,sll_source_file_index_t);
	b->data=sll_allocate(b->length*sizeof(sll_bundle_source_file_t*));
	for (sll_source_file_index_t i=0;i<b->length;i++){
		sll_bundle_source_file_t* bsf=sll_allocate(sizeof(sll_bundle_source_file_t));
		*(b->data+i)=bsf;
		if (!sll_decode_string(rf,&(bsf->name))||!_read_source_file(rf,&(bsf->data))){
			return 0;
		}
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_compiled_node(sll_file_t* rf,sll_compilation_data_t* compilation_data){
	magic_number_t n;
	sll_version_t v;
	if (sll_file_read(rf,&n,sizeof(magic_number_t),NULL)!=sizeof(magic_number_t)||n!=COMPLIED_OBJECT_FILE_MAGIC_NUMBER||sll_file_read(rf,&v,sizeof(sll_version_t),NULL)!=sizeof(sll_version_t)||v!=SLL_VERSION){
		return 0;
	}
	CHECK_ERROR(rf,compilation_data->length,sll_source_file_index_t);
	compilation_data->data=sll_allocate(compilation_data->length*sizeof(sll_source_file_t*));
	for (sll_source_file_index_t i=0;i<compilation_data->length;i++){
		sll_source_file_t* sf=sll_allocate(sizeof(sll_source_file_t));
		*(compilation_data->data+i)=sf;
		if (!_read_source_file(rf,sf)){
			return 0;
		}
	}
	return 1;
}

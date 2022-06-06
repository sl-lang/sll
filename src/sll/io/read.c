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



#define CHECK_ERROR(file,var,var_type) \
	do{ \
		sll_error_t __e=0; \
		(var)=(var_type)sll_decode_integer((file),&__e); \
		if (__e!=SLL_NO_ERROR){ \
			return 0; \
		} \
	} while (0)
#define READ_CHAR(char_,file) \
	do{ \
		sll_read_char_t __c=sll_file_read_char((file),NULL); \
		if (__c==SLL_END_OF_DATA){ \
			return 0; \
		} \
		char_=(sll_char_t)__c; \
	} while(0)
#define READ_FIELD(field,file) \
	do{ \
		if (sll_file_read((file),&(field),sizeof(field),NULL)!=sizeof(field)){ \
			return 0; \
		} \
	} while(0)



static sll_bool_t _read_node(sll_source_file_t* source_file,sll_file_t* file){
	sll_node_t* o=_acquire_next_node(source_file);
	READ_CHAR(o->type,file);
	while (o->type==SLL_NODE_TYPE_NOP||o->type==SLL_NODE_TYPE_DBG){
		if (o->type==SLL_NODE_TYPE_DBG){
			CHECK_ERROR(file,o->data.string_index,sll_string_index_t);
			o->data.string_index--;
		}
		o=_acquire_next_node(source_file);
		READ_CHAR(o->type,file);
	}
	switch (o->type){
		case SLL_NODE_TYPE_INT:
			{
				sll_error_t err;
				o->data.int_=sll_decode_signed_integer(file,&err);
				if (err!=SLL_NO_ERROR){
					return 0;
				}
				return 1;
			}
		case SLL_NODE_TYPE_FLOAT:
			READ_FIELD(o->data.float_,file);
			return 1;
		case SLL_NODE_TYPE_CHAR:
			READ_CHAR(o->data.char_,file);
			return 1;
		case SLL_NODE_TYPE_COMPLEX:
			READ_FIELD(o->data.complex_,file);
			return 1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			CHECK_ERROR(file,o->data.string_index,sll_string_index_t);
			return 1;
		case SLL_NODE_TYPE_ARRAY:
			CHECK_ERROR(file,o->data.array_length,sll_array_length_t);
			for (sll_array_length_t i=0;i<o->data.array_length;i++){
				if (!_read_node(source_file,file)){
					return 0;
				}
			}
			return 1;
		case SLL_NODE_TYPE_MAP:
			CHECK_ERROR(file,o->data.map_length,sll_map_length_t);
			for (sll_map_length_t i=0;i<o->data.map_length;i++){
				if (!_read_node(source_file,file)){
					return 0;
				}
			}
			return 1;
		case SLL_NODE_TYPE_IDENTIFIER:
			CHECK_ERROR(file,o->data.identifier_index,sll_identifier_index_t);
			return 1;
		case SLL_NODE_TYPE_FUNCTION_ID:
			CHECK_ERROR(file,o->data.function_index,sll_function_index_t);
			return 1;
		case SLL_NODE_TYPE_FUNC:
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			CHECK_ERROR(file,o->data.function.arg_count,sll_arg_count_t);
			CHECK_ERROR(file,o->data.function.function_index,sll_function_index_t);
			if (o->type==SLL_NODE_TYPE_FUNC){
				CHECK_ERROR(file,o->data.function.scope,sll_scope_t);
			}
			for (sll_arg_count_t i=0;i<o->data.function.arg_count;i++){
				if (!_read_node(source_file,file)){
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
			CHECK_ERROR(file,o->data.loop.arg_count,sll_arg_count_t);
			CHECK_ERROR(file,o->data.loop.scope,sll_scope_t);
			for (sll_arg_count_t i=0;i<o->data.loop.arg_count;i++){
				if (!_read_node(source_file,file)){
					return 0;
				}
			}
			return 1;
		case SLL_NODE_TYPE_DECL:
			CHECK_ERROR(file,o->data.declaration.arg_count,sll_arg_count_t);
			CHECK_ERROR(file,o->data.declaration.name_string_index,sll_string_index_t);
			o->data.declaration.name_string_index--;
			for (sll_arg_count_t i=0;i<o->data.declaration.arg_count;i++){
				if (!_read_node(source_file,file)){
					return 0;
				}
			}
			return 1;
	}
	CHECK_ERROR(file,o->data.arg_count,sll_arg_count_t);
	for (sll_arg_count_t i=0;i<o->data.arg_count;i++){
		if (!_read_node(source_file,file)){
			return 0;
		}
	}
	return 1;
}



static sll_bool_t _read_source_file(sll_file_t* file,sll_source_file_t* source_file){
	CHECK_ERROR(file,source_file->time,sll_time_t);
	CHECK_ERROR(file,source_file->file_size,sll_file_offset_t);
	READ_FIELD(source_file->file_hash,file);
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		CHECK_ERROR(file,source_file->identifier_table.short_[i].length,sll_identifier_list_length_t);
		source_file->identifier_table.short_[i].data=sll_allocate(source_file->identifier_table.short_[i].length*sizeof(sll_identifier_t));
		for (sll_identifier_list_length_t j=0;j<source_file->identifier_table.short_[i].length;j++){
			CHECK_ERROR(file,(source_file->identifier_table.short_[i].data+j)->scope,sll_scope_t);
			CHECK_ERROR(file,(source_file->identifier_table.short_[i].data+j)->name_string_index,sll_string_index_t);
		}
	}
	CHECK_ERROR(file,source_file->identifier_table.long_data_length,sll_identifier_list_length_t);
	source_file->identifier_table.long_data=sll_allocate(source_file->identifier_table.long_data_length*sizeof(sll_identifier_t));
	for (sll_identifier_list_length_t i=0;i<source_file->identifier_table.long_data_length;i++){
		CHECK_ERROR(file,(source_file->identifier_table.long_data+i)->scope,sll_scope_t);
		CHECK_ERROR(file,(source_file->identifier_table.long_data+i)->name_string_index,sll_string_index_t);
	}
	CHECK_ERROR(file,source_file->export_table.length,sll_export_table_length_t);
	source_file->export_table.data=sll_allocate(source_file->export_table.length*sizeof(sll_identifier_index_t));
	for (sll_export_table_length_t i=0;i<source_file->export_table.length;i++){
		CHECK_ERROR(file,*(source_file->export_table.data+i),sll_identifier_index_t);
	}
	CHECK_ERROR(file,source_file->function_table.length,sll_function_index_t);
	source_file->function_table.data=sll_allocate(source_file->function_table.length*sizeof(sll_function_t*));
	for (sll_function_index_t i=0;i<source_file->function_table.length;i++){
		sll_node_offset_t off;
		CHECK_ERROR(file,off,sll_node_offset_t);
		sll_arg_count_t al;
		CHECK_ERROR(file,al,sll_arg_count_t);
		sll_function_t* k=sll_allocate(sizeof(sll_function_t)+SLL_FUNCTION_GET_ARGUMENT_COUNT_RAW(al)*sizeof(sll_identifier_index_t));
		k->offset=off;
		CHECK_ERROR(file,k->name_string_index,sll_string_index_t);
		k->name_string_index--;
		CHECK_ERROR(file,k->description_string_index,sll_string_index_t);
		k->description_string_index--;
		k->arg_count=al;
		for (sll_arg_count_t j=0;j<SLL_FUNCTION_GET_ARGUMENT_COUNT_RAW(al);j++){
			CHECK_ERROR(file,k->args[j],sll_identifier_index_t);
		}
		*(source_file->function_table.data+i)=k;
	}
	CHECK_ERROR(file,source_file->string_table.length,sll_string_index_t);
	source_file->string_table.data=sll_allocate(source_file->string_table.length*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<source_file->string_table.length;i++){
		if (!sll_decode_string(file,source_file->string_table.data+i)){
			return 0;
		}
	}
	CHECK_ERROR(file,source_file->import_table.length,sll_import_index_t);
	source_file->import_table.data=sll_allocate(source_file->import_table.length*sizeof(sll_import_file_t*));
	for (sll_import_index_t i=0;i<source_file->import_table.length;i++){
		sll_source_file_index_t sfi;
		CHECK_ERROR(file,sfi,sll_source_file_index_t);
		sll_identifier_list_length_t l;
		CHECK_ERROR(file,l,sll_identifier_list_length_t);
		sll_import_file_t* if_=sll_allocate(sizeof(sll_import_file_t)+l*sizeof(sll_identifier_index_t));
		*(source_file->import_table.data+i)=if_;
		if_->source_file_index=sfi;
		if_->length=l;
		for (sll_identifier_list_length_t j=0;j<l;j++){
			CHECK_ERROR(file,if_->data[j],sll_identifier_index_t);
		}
	}
	CHECK_ERROR(file,source_file->file_path_string_index,sll_string_index_t);
	CHECK_ERROR(file,source_file->_next_scope,sll_scope_t);
	_init_node_stack(source_file);
	source_file->first_node=source_file->_stack.next_node;
	return _read_node(source_file,file);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_assembly(sll_file_t* file,sll_assembly_data_t* out){
	magic_number_t n;
	sll_version_t v;
	if (sll_file_read(file,&n,sizeof(magic_number_t),NULL)!=sizeof(magic_number_t)||n!=ASSEMBLY_FILE_MAGIC_NUMBER||sll_file_read(file,&v,sizeof(sll_version_t),NULL)!=sizeof(sll_version_t)||v!=SLL_VERSION){
		return 0;
	}
	CHECK_ERROR(file,out->time,sll_time_t);
	CHECK_ERROR(file,out->instruction_count,sll_instruction_index_t);
	CHECK_ERROR(file,out->variable_count,sll_variable_index_t);
	CHECK_ERROR(file,out->tls_variable_count,sll_variable_index_t);
	CHECK_ERROR(file,out->function_table.length,sll_function_index_t);
	out->function_table.data=sll_allocate(out->function_table.length*sizeof(sll_assembly_function_t));
	for (sll_function_index_t i=0;i<out->function_table.length;i++){
		CHECK_ERROR(file,(out->function_table.data+i)->instruction_index,sll_instruction_index_t);
		CHECK_ERROR(file,(out->function_table.data+i)->arg_count,sll_arg_count_t);
	}
	CHECK_ERROR(file,out->string_table.length,sll_string_index_t);
	out->string_table.data=sll_allocate(out->string_table.length*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<out->string_table.length;i++){
		if (!sll_decode_string(file,out->string_table.data+i)){
			return 0;
		}
	}
	CHECK_ERROR(file,out->debug_data.length,sll_instruction_index_t);
	out->debug_data.data=sll_allocate(out->debug_data.length*sizeof(sll_debug_line_data_t));
	for (sll_debug_data_length_t i=0;i<out->debug_data.length;i++){
		CHECK_ERROR(file,(out->debug_data.data+i)->delta_instruction_index,sll_instruction_index_t);
		CHECK_ERROR(file,(out->debug_data.data+i)->line,sll_file_offset_t);
	}
	_init_assembly_stack(out);
	out->first_instruction=out->_instruction_stack.next_instruction;
	sll_instruction_index_t i=out->instruction_count;
	out->instruction_count=0;
	while (i){
		i--;
		sll_assembly_instruction_t* ai=_acquire_next_instruction(out);
		sll_read_char_t c=sll_file_read_char(file,NULL);
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
					ai->data.int_=sll_decode_signed_integer(file,&err);
					if (err!=SLL_NO_ERROR){
						return 0;
					}
					break;
				}
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_INT_COMPRESSED:
				CHECK_ERROR(file,ai->data.compressed_int,sll_compressed_integer_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_FLOAT:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_FLOAT:
				READ_FIELD(ai->data.float_,file);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_COMPLEX:
				READ_FIELD(ai->data.complex_,file);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_CHAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_CHAR:
				c=sll_file_read_char(file,NULL);
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
					ai->data.jump.target.rel=(sll_relative_instruction_index_t)sll_decode_signed_integer(file,&err);
					if (err!=SLL_NO_ERROR){
						return 0;
					}
				}
				else{
					CHECK_ERROR(file,ai->data.jump.target.abs,sll_instruction_index_t);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_STACK:
				CHECK_ERROR(file,ai->data.stack_offset,sll_stack_offset_t);
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
				CHECK_ERROR(file,ai->data.variable,sll_variable_index_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOADS:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_LOOKUP_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PRINT_STR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_RET_STR:
				CHECK_ERROR(file,ai->data.string_index,sll_string_index_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_PACK:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_JT:
				CHECK_ERROR(file,ai->data.array_length,sll_array_length_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_MAP:
				CHECK_ERROR(file,ai->data.map_length,sll_map_length_t);
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
					CHECK_ERROR(file,ai->data.variable,sll_variable_index_t);
				}
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ACCESS_VAR:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_ASSIGN_VAR_ACCESS:
				CHECK_ERROR(file,ai->data.variable_access.variable,sll_variable_index_t);
				CHECK_ERROR(file,ai->data.variable_access.arg_count,sll_arg_count_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CAST_TYPE:
				CHECK_ERROR(file,ai->data.type,sll_object_type_t);
				break;
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_CALL_POP:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_DECL:
			case SLL_ASSEMBLY_INSTRUCTION_TYPE_NEW:
				CHECK_ERROR(file,ai->data.arg_count,sll_arg_count_t);
				break;
		}
	}
	sll_assembly_instruction_t* ai=out->first_instruction;
	for (i=0;i<out->instruction_count;i++){
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_PUSH_LABEL||(SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)>=SLL_ASSEMBLY_INSTRUCTION_TYPE_JMP&&SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)<=SLL_ASSEMBLY_INSTRUCTION_TYPE_JNI)){
			ai->data.jump._instruction=_get_instruction_at_offset(out,(SLL_ASSEMBLY_INSTRUCTION_FLAG_IS_RELATIVE(ai)?i+ai->data.jump.target.rel:ai->data.jump.target.abs));
		}
		ai++;
		if (SLL_ASSEMBLY_INSTRUCTION_GET_TYPE(ai)==SLL_ASSEMBLY_INSTRUCTION_TYPE_CHANGE_STACK){
			ai=ai->data._next_instruction;
		}
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_bundle(sll_file_t* file,sll_bundle_t* out){
	magic_number_t n;
	sll_version_t v;
	if (sll_file_read(file,&n,sizeof(magic_number_t),NULL)!=sizeof(magic_number_t)||n!=BUNDLE_FILE_MAGIC_NUMBER||sll_file_read(file,&v,sizeof(sll_version_t),NULL)!=sizeof(sll_version_t)||v!=SLL_VERSION||!sll_decode_string(file,&(out->name))){
		return 0;
	}
	CHECK_ERROR(file,out->length,sll_source_file_index_t);
	out->data=sll_allocate(out->length*sizeof(sll_bundle_source_file_t*));
	for (sll_source_file_index_t i=0;i<out->length;i++){
		sll_bundle_source_file_t* bsf=sll_allocate(sizeof(sll_bundle_source_file_t));
		*(out->data+i)=bsf;
		if (!sll_decode_string(file,&(bsf->name))||!_read_source_file(file,&(bsf->data))){
			return 0;
		}
	}
	return 1;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_load_compiled_node(sll_file_t* file,sll_compilation_data_t* out){
	magic_number_t n;
	sll_version_t v;
	if (sll_file_read(file,&n,sizeof(magic_number_t),NULL)!=sizeof(magic_number_t)||n!=COMPLIED_OBJECT_FILE_MAGIC_NUMBER||sll_file_read(file,&v,sizeof(sll_version_t),NULL)!=sizeof(sll_version_t)||v!=SLL_VERSION){
		return 0;
	}
	CHECK_ERROR(file,out->length,sll_source_file_index_t);
	out->data=sll_allocate(out->length*sizeof(sll_source_file_t*));
	for (sll_source_file_index_t i=0;i<out->length;i++){
		sll_source_file_t* sf=sll_allocate(sizeof(sll_source_file_t));
		*(out->data+i)=sf;
		if (!_read_source_file(file,sf)){
			return 0;
		}
	}
	return 1;
}

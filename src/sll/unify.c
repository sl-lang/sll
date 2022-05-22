#include <sll/_internal/common.h>
#include <sll/_internal/stack.h>
#include <sll/_internal/unify.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/identifier.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



static const sll_node_t* _clone_node(const sll_node_t* src,sll_source_file_t* out,source_file_mapping_data_t* source_file_mapping_data){
	while (src->type==SLL_NODE_TYPE_NOP||src->type==SLL_NODE_TYPE_DBG||src->type==SLL_NODE_TYPE_CHANGE_STACK){
		if (src->type==SLL_NODE_TYPE_CHANGE_STACK){
			src=src->data._next_node;
		}
		else{
			sll_node_t* dst=_acquire_next_node(out);
			*dst=*src;
			if (source_file_mapping_data&&src->type==SLL_NODE_TYPE_DBG&&src->data.string_index!=SLL_MAX_STRING_INDEX){
				dst->data.string_index=*(source_file_mapping_data->string_map+src->data.string_index);
			}
			src++;
		}
	}
	sll_node_t* dst=_acquire_next_node(out);
	*dst=*src;
	switch (src->type){
		case SLL_NODE_TYPE_INT:
		case SLL_NODE_TYPE_FLOAT:
		case SLL_NODE_TYPE_CHAR:
		case SLL_NODE_TYPE_COMPLEX:
		case SLL_NODE_TYPE_FUNCTION_ID:
			return src+1;
		case SLL_NODE_TYPE_STRING:
		case SLL_NODE_TYPE_FIELD:
			if (source_file_mapping_data){
				dst->data.string_index=*(source_file_mapping_data->string_map+dst->data.string_index);
			}
			return src+1;
		case SLL_NODE_TYPE_IDENTIFIER:
			if (source_file_mapping_data){
				dst->data.identifier_index=*(source_file_mapping_data->identifier_index_offset[SLL_IDENTIFIER_GET_ARRAY_ID(dst->data.identifier_index)]+SLL_IDENTIFIER_GET_ARRAY_INDEX(dst->data.identifier_index));
			}
			return src+1;
		case SLL_NODE_TYPE_ARRAY:
			{
				sll_array_length_t l=src->data.array_length;
				src++;
				while (l){
					l--;
					src=_clone_node(src,out,source_file_mapping_data);
				}
				return src;
			}
		case SLL_NODE_TYPE_MAP:
			{
				sll_map_length_t l=src->data.map_length;
				src++;
				while (l){
					l--;
					src=_clone_node(src,out,source_file_mapping_data);
				}
				return src;
			}
		case SLL_NODE_TYPE_FUNC:
			if (source_file_mapping_data){
				dst->data.function.function_index+=source_file_mapping_data->function_index_offset;
				dst->data.function.scope+=source_file_mapping_data->scope_offset;
			}
		case SLL_NODE_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=src->data.function.arg_count;
				src++;
				while (l){
					l--;
					src=_clone_node(src,out,source_file_mapping_data);
				}
				return src;
			}
		case SLL_NODE_TYPE_FOR:
		case SLL_NODE_TYPE_WHILE:
		case SLL_NODE_TYPE_LOOP:
		case SLL_NODE_TYPE_FOR_ARRAY:
		case SLL_NODE_TYPE_WHILE_ARRAY:
		case SLL_NODE_TYPE_FOR_MAP:
		case SLL_NODE_TYPE_WHILE_MAP:
			{
				if (source_file_mapping_data){
					dst->data.loop.scope+=source_file_mapping_data->scope_offset;
				}
				sll_arg_count_t l=src->data.loop.arg_count;
				src++;
				while (l){
					l--;
					src=_clone_node(src,out,source_file_mapping_data);
				}
				return src;
			}
		case SLL_NODE_TYPE_DECL:
			{
				if (source_file_mapping_data&&dst->data.declaration.name_string_index!=SLL_MAX_STRING_INDEX){
					dst->data.declaration.name_string_index=*(source_file_mapping_data->string_map+dst->data.declaration.name_string_index);
				}
				sll_arg_count_t l=src->data.declaration.arg_count;
				src++;
				while (l){
					l--;
					src=_clone_node(src,out,source_file_mapping_data);
				}
				return src;
			}
	}
	sll_arg_count_t l=src->data.arg_count;
	src++;
	while (l){
		l--;
		src=_clone_node(src,out,source_file_mapping_data);
	}
	return src;
}



__SLL_EXTERNAL void sll_unify_compilation_data(const sll_compilation_data_t* compilation_data,sll_source_file_t* out){
	SLL_ASSERT(compilation_data->length);
	_init_node_stack(out);
	out->first_node=_acquire_next_node(out);
	out->first_node->type=SLL_NODE_TYPE_OPERATION_LIST;
	out->first_node->data.arg_count=0;
	out->_next_scope=0;
	out->import_table.data=NULL;
	out->import_table.length=0;
	out->file_path_string_index=0;
	sll_source_file_index_t idx=compilation_data->length;
	sll_identifier_index_t** export_dt=sll_allocate_stack((idx-1)*sizeof(sll_identifier_index_t*));
	do{
		idx--;
		sll_source_file_t* sf=*(compilation_data->data+idx);
		if (!idx){
			out->time=sf->time;
			out->file_size=sf->file_size;
			out->file_hash=sf->file_hash;
			out->export_table.length=sf->export_table.length;
			out->export_table.data=sll_allocate(sf->export_table.length*sizeof(sll_identifier_index_t));
			sll_copy_data(sf->export_table.data,sf->export_table.length*sizeof(sll_identifier_index_t),out->export_table.data);
		}
		if (!sf->first_node){
			continue;
		}
		if (!out->first_node->data.arg_count){
			if (idx){
				sll_identifier_index_t* ex_dt=sll_allocate_stack(sf->export_table.length*sizeof(sll_identifier_index_t));
				*(export_dt+idx-1)=ex_dt;
				sll_copy_data(sf->export_table.data,sf->export_table.length*sizeof(sll_identifier_index_t),ex_dt);
			}
			out->file_path_string_index=sf->file_path_string_index;
			sll_node_t* dbg=_acquire_next_node(out);
			dbg->type=SLL_NODE_TYPE_DBG;
			dbg->data.string_index=sf->file_path_string_index;
			for (unsigned int i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
				const sll_identifier_list_t* il=sf->identifier_table.short_+i;
				sll_identifier_list_t* oil=out->identifier_table.short_+i;
				oil->length=il->length;
				oil->data=sll_allocate(il->length*sizeof(sll_identifier_t));
				sll_copy_data(il->data,il->length*sizeof(sll_identifier_t),oil->data);
			}
			out->identifier_table.long_data_length=sf->identifier_table.long_data_length;
			out->identifier_table.long_data=sll_allocate(sf->identifier_table.long_data_length*sizeof(sll_identifier_t));
			sll_copy_data(sf->identifier_table.long_data,sf->identifier_table.long_data_length*sizeof(sll_identifier_t),out->identifier_table.long_data);
			out->function_table.length=sf->function_table.length;
			out->function_table.data=sll_allocate(sf->function_table.length*sizeof(sll_function_t*));
			for (sll_function_index_t i=0;i<sf->function_table.length;i++){
				const sll_function_t* s=*(sf->function_table.data+i);
				sll_function_t* d=sll_allocate(sizeof(sll_function_t)+s->arg_count*sizeof(sll_identifier_index_t));
				*(out->function_table.data+i)=d;
				sll_copy_data(s,sizeof(sll_function_t)+s->arg_count*sizeof(sll_identifier_index_t),d);
				d->offset+=out->_stack.offset;
			}
			out->string_table.length=sf->string_table.length;
			out->string_table.data=sll_allocate(sf->string_table.length*sizeof(sll_string_t));
			for (sll_string_index_t i=0;i<sf->string_table.length;i++){
				sll_string_clone(sf->string_table.data+i,out->string_table.data+i);
			}
			_clone_node(sf->first_node,out,NULL);
		}
		else{
			source_file_mapping_data_t source_file_mapping_data={
				sll_allocate_stack(sf->string_table.length*sizeof(sll_string_index_t)),
				out->function_table.length,
				out->_next_scope
			};
			for (sll_string_index_t i=0;i<sf->string_table.length;i++){
				sll_string_t tmp;
				sll_string_clone(sf->string_table.data+i,&tmp);
				*(source_file_mapping_data.string_map+i)=sll_add_string(&(out->string_table),&tmp,1);
			}
			sll_node_t* dbg=_acquire_next_node(out);
			dbg->type=SLL_NODE_TYPE_DBG;
			dbg->data.string_index=*(source_file_mapping_data.string_map+sf->file_path_string_index);
			if (!idx){
				out->file_path_string_index=dbg->data.string_index;
			}
			for (unsigned int i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
				const sll_identifier_list_t* il=sf->identifier_table.short_+i;
				if (!il->length){
					source_file_mapping_data.identifier_index_offset[i]=NULL;
					continue;
				}
				sll_identifier_list_t* oil=out->identifier_table.short_+i;
				sll_identifier_list_length_t off=oil->length;
				source_file_mapping_data.identifier_index_offset[i]=sll_allocate_stack(il->length*sizeof(sll_identifier_index_t));
				oil->length+=il->length;
				oil->data=sll_reallocate(oil->data,oil->length*sizeof(sll_identifier_t));
				for (sll_identifier_list_length_t j=0;j<il->length;j++){
					*(source_file_mapping_data.identifier_index_offset[i]+j)=SLL_CREATE_IDENTIFIER(off+j,i);
					(oil->data+off+j)->scope=(il->data+j)->scope+out->_next_scope;
					SLL_IDENTIFIER_SET_STRING_INDEX(oil->data+off+j,*(source_file_mapping_data.string_map+SLL_IDENTIFIER_GET_STRING_INDEX(il->data+j)),SLL_IDENTIFIER_IS_TLS(il->data+j));
				}
			}
			if (sf->identifier_table.long_data_length){
				sll_identifier_list_length_t off=out->identifier_table.long_data_length;
				source_file_mapping_data.identifier_index_offset[SLL_MAX_SHORT_IDENTIFIER_LENGTH]=sll_allocate_stack(sf->identifier_table.long_data_length*sizeof(sll_identifier_index_t));
				out->identifier_table.long_data_length+=sf->identifier_table.long_data_length;
				out->identifier_table.long_data=sll_reallocate(out->identifier_table.long_data,out->identifier_table.long_data_length*sizeof(sll_identifier_t));
				for (sll_identifier_list_length_t i=0;i<sf->identifier_table.long_data_length;i++){
					*(source_file_mapping_data.identifier_index_offset[SLL_MAX_SHORT_IDENTIFIER_LENGTH]+i)=SLL_CREATE_IDENTIFIER(off+i,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
					(out->identifier_table.long_data+off+i)->scope=(sf->identifier_table.long_data+i)->scope+out->_next_scope;
					SLL_IDENTIFIER_SET_STRING_INDEX(out->identifier_table.long_data+off+i,*(source_file_mapping_data.string_map+SLL_IDENTIFIER_GET_STRING_INDEX(sf->identifier_table.long_data+i)),SLL_IDENTIFIER_IS_TLS(sf->identifier_table.long_data+i));
				}
			}
			else{
				source_file_mapping_data.identifier_index_offset[SLL_MAX_SHORT_IDENTIFIER_LENGTH]=NULL;
			}
			for (sll_import_index_t i=0;i<sf->import_table.length;i++){
				sll_import_file_t* k=*(sf->import_table.data+i);
				SLL_ASSERT(k->source_file_index);
				sll_identifier_index_t* ex_dt=*(export_dt+k->source_file_index-1);
				for (sll_identifier_list_length_t j=0;j<k->length;j++){
					*(source_file_mapping_data.identifier_index_offset[SLL_IDENTIFIER_GET_ARRAY_ID(k->data[j])]+SLL_IDENTIFIER_GET_ARRAY_INDEX(k->data[j]))=*(ex_dt+j);
				}
			}
			if (idx){
				sll_identifier_index_t* ex_dt=sll_allocate_stack(sf->export_table.length*sizeof(sll_identifier_index_t));
				*(export_dt+idx-1)=ex_dt;
				for (sll_export_table_length_t i=0;i<sf->export_table.length;i++){
					*(ex_dt+i)=*(source_file_mapping_data.identifier_index_offset[SLL_IDENTIFIER_GET_ARRAY_ID(*(sf->export_table.data+i))]+SLL_IDENTIFIER_GET_ARRAY_INDEX(*(sf->export_table.data+i)));
				}
			}
			if (sf->function_table.length){
				out->function_table.length+=sf->function_table.length;
				out->function_table.data=sll_reallocate(out->function_table.data,out->function_table.length*sizeof(sll_function_t*));
				for (sll_function_index_t i=0;i<sf->function_table.length;i++){
					const sll_function_t* s=*(sf->function_table.data+i);
					sll_function_t* d=sll_allocate(sizeof(sll_function_t)+s->arg_count*sizeof(sll_identifier_index_t));
					*(out->function_table.data+source_file_mapping_data.function_index_offset+i)=d;
					d->offset=s->offset+out->_stack.offset;
					d->name_string_index=(s->name_string_index==SLL_MAX_STRING_INDEX?SLL_MAX_STRING_INDEX:*(source_file_mapping_data.string_map+s->name_string_index));
					d->description_string_index=(s->description_string_index==SLL_MAX_STRING_INDEX?SLL_MAX_STRING_INDEX:*(source_file_mapping_data.string_map+s->description_string_index));
					d->arg_count=s->arg_count;
					for (sll_arg_count_t j=0;j<SLL_FUNCTION_GET_ARGUMENT_COUNT(s);j++){
						d->args[j]=*(source_file_mapping_data.identifier_index_offset[SLL_IDENTIFIER_GET_ARRAY_ID(s->args[j])]+SLL_IDENTIFIER_GET_ARRAY_INDEX(s->args[j]));
					}
				}
			}
			_clone_node(sf->first_node,out,&source_file_mapping_data);
			for (unsigned int i=0;i<=SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
				sll_deallocate(source_file_mapping_data.identifier_index_offset[i]);
			}
			sll_deallocate(source_file_mapping_data.string_map);
		}
		out->_next_scope+=sf->_next_scope;
		out->first_node->data.arg_count++;
	} while (idx);
	for (sll_source_file_index_t i=0;i<compilation_data->length-1;i++){
		sll_deallocate(*(export_dt+i));
	}
	sll_deallocate(export_dt);
}

#include <sll/_internal/bundle.h>
#include <sll/_internal/common.h>
#include <sll/_internal/stack.h>
#include <sll/_internal/string.h>
#include <sll/api/hash.h>
#include <sll/bundle.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/platform/memory.h>
#include <sll/platform/time.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



static sll_source_file_index_t _add_source_file(const sll_bundle_t* bundle,const sll_bundle_source_file_t* bundle_source_file,source_file_list_t* source_file_list){
	for (sll_source_file_index_t i=0;i<source_file_list->length;i++){
		sll_source_file_t* source_file=(source_file_list->data+i)->source_file;
		if (bundle_source_file->data.file_size==source_file->file_size&&sll_compare_data(&(bundle_source_file->data.file_hash),&(source_file->file_hash),sizeof(sll_sha256_data_t))==SLL_COMPARE_RESULT_EQUAL){
			return i;
		}
	}
	sll_source_file_index_t out=source_file_list->length;
	source_file_list->length++;
	source_file_list->data=sll_reallocate(source_file_list->data,source_file_list->length*sizeof(source_file_with_index_t));
	sll_source_file_t* source_file=sll_allocate(sizeof(sll_source_file_t));
	(source_file_list->data+out)->source_file=source_file;
	(source_file_list->data+out)->index=out;
	source_file->time=bundle_source_file->data.time;
	source_file->file_size=bundle_source_file->data.file_size;
	source_file->file_hash=bundle_source_file->data.file_hash;
	source_file->identifier_table.length=bundle_source_file->data.identifier_table.length;
	source_file->identifier_table.data=sll_allocate(source_file->identifier_table.length*sizeof(sll_identifier_t));
	sll_copy_data(bundle_source_file->data.identifier_table.data,source_file->identifier_table.length*sizeof(sll_identifier_t),source_file->identifier_table.data);
	source_file->export_table.length=bundle_source_file->data.export_table.length;
	source_file->export_table.data=sll_allocate(source_file->export_table.length*sizeof(sll_identifier_index_t));
	sll_copy_data(bundle_source_file->data.export_table.data,source_file->export_table.length*sizeof(sll_identifier_index_t),source_file->export_table.data);
	source_file->function_table.length=bundle_source_file->data.function_table.length;
	source_file->function_table.data=sll_allocate(source_file->function_table.length*sizeof(sll_function_t*));
	for (sll_function_index_t i=0;i<source_file->function_table.length;i++){
		const sll_function_t* function=*(bundle_source_file->data.function_table.data+i);
		sll_function_t* new_function=sll_allocate(sizeof(sll_function_t)+SLL_FUNCTION_GET_ARGUMENT_COUNT(function)*sizeof(sll_identifier_index_t));
		sll_copy_data(function,sizeof(sll_function_t)+SLL_FUNCTION_GET_ARGUMENT_COUNT(function)*sizeof(sll_identifier_index_t),new_function);
		*(source_file->function_table.data+i)=new_function;
	}
	source_file->string_table.length=bundle_source_file->data.string_table.length;
	source_file->string_table.data=sll_allocate(source_file->string_table.length*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<source_file->string_table.length;i++){
		sll_string_clone(bundle_source_file->data.string_table.data+i,source_file->string_table.data+i);
	}
	source_file->import_table.length=bundle_source_file->data.import_table.length;
	source_file->import_table.data=sll_allocate(source_file->import_table.length*sizeof(sll_import_file_t*));
	for (sll_import_index_t i=0;i<source_file->import_table.length;i++){
		const sll_import_file_t* import_file=*(bundle_source_file->data.import_table.data+i);
		sll_import_file_t* new_import_file=sll_allocate(sizeof(sll_import_file_t)+import_file->length*sizeof(sll_identifier_index_t));
		sll_copy_data(import_file,sizeof(sll_import_file_t)+import_file->length*sizeof(sll_identifier_index_t),new_import_file);
		new_import_file->source_file_index=_add_source_file(bundle,*(bundle->data+import_file->source_file_index),source_file_list);
		*(source_file->import_table.data+i)=new_import_file;
	}
	source_file->file_path_string_index=bundle_source_file->data.file_path_string_index;
	source_file->_next_scope=bundle_source_file->data._next_scope;
	_clone_node_stack(&(bundle_source_file->data),source_file);
	return out;
}



__SLL_EXTERNAL void sll_bundle_add_file(const sll_char_t* name,sll_compilation_data_t* compilation_data,sll_bundle_t* out){
	sll_source_file_index_t* idx_m=sll_allocate_stack(compilation_data->length*sizeof(sll_source_file_index_t));
	for (sll_source_file_index_t idx=0;idx<compilation_data->length;idx++){
		sll_source_file_t* source_file=*(compilation_data->data+idx);
		sll_source_file_index_t bundle_source_file_i=0;
		while (bundle_source_file_i<out->length){
			sll_bundle_source_file_t* bundle_source_file=*(out->data+bundle_source_file_i);
			if (bundle_source_file->data.file_size==source_file->file_size&&sll_compare_data(&(bundle_source_file->data.file_hash),&(source_file->file_hash),sizeof(sll_sha256_data_t))==SLL_COMPARE_RESULT_EQUAL){
				break;
			}
			bundle_source_file_i++;
		}
		*(idx_m+idx)=bundle_source_file_i;
		if (bundle_source_file_i<out->length){
			sll_free_source_file(source_file);
			if (!idx){
				sll_bundle_source_file_t* bundle_source_file=*(out->data+bundle_source_file_i);
				if (!bundle_source_file->name.length){
					sll_string_from_pointer(name,&(bundle_source_file->name));
				}
			}
			(*(idx_m+idx))|=BUNDLE_DO_NOT_REMAP_IMPORT;
			continue;
		}
		out->length++;
		out->data=sll_reallocate(out->data,out->length*sizeof(sll_bundle_source_file_t));
		sll_bundle_source_file_t* bundle_source_file=sll_allocate(sizeof(sll_bundle_source_file_t));
		if (!idx){
			sll_string_from_pointer(name,&(bundle_source_file->name));
		}
		else{
			SLL_INIT_STRING(&(bundle_source_file->name));
		}
		bundle_source_file->data=*source_file;
		*(out->data+out->length-1)=bundle_source_file;
	}
	for (sll_source_file_index_t i=0;i<compilation_data->length;i++){
		sll_deallocate(*(compilation_data->data+i));
		if ((*(idx_m+i))&BUNDLE_DO_NOT_REMAP_IMPORT){
			continue;
		}
		sll_bundle_source_file_t* bundle_source_file=*(out->data+(*(idx_m+i)));
		for (sll_import_index_t j=0;j<bundle_source_file->data.import_table.length;j++){
			sll_import_file_t* bundle_source_file_if=*(bundle_source_file->data.import_table.data+j);
			bundle_source_file_if->source_file_index=BUNDLE_GET_INDEX(idx_m+bundle_source_file_if->source_file_index);
		}
	}
	sll_deallocate(idx_m);
	sll_deallocate(compilation_data->data);
	compilation_data->data=NULL;
	compilation_data->length=0;
}



__SLL_EXTERNAL void sll_bundle_create(const sll_char_t* name,sll_bundle_t* out){
	out->time=sll_platform_get_current_time();
	sll_string_from_pointer(name,&(out->name));
	out->data=NULL;
	out->length=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_bundle_fetch(const sll_bundle_t* bundle,const sll_string_t* name,sll_compilation_data_t* out){
	if (!sll_string_starts(name,&(bundle->name))){
		return 0;
	}
	sll_string_t f_nm_str;
	sll_string_resize(name,-((sll_integer_t)(bundle->name.length)),&f_nm_str);
	const sll_bundle_source_file_t* bundle_source_file=NULL;
	for (sll_source_file_index_t i=0;i<bundle->length;i++){
		bundle_source_file=*(bundle->data+i);
		if (STRING_EQUAL(&(bundle_source_file->name),&f_nm_str)){
			break;
		}
		bundle_source_file=NULL;
	}
	sll_free_string(&f_nm_str);
	if (!bundle_source_file){
		return 0;
	}
	source_file_list_t source_file_list={
		sll_allocate_stack(1),
		0
	};
	_add_source_file(bundle,bundle_source_file,&source_file_list);
	sll_source_file_index_t* sf_idx_map=sll_allocate_stack(source_file_list.length*sizeof(sll_source_file_index_t));
	for (sll_source_file_index_t idx=1;idx<source_file_list.length;idx++){
		*(sf_idx_map+idx)=idx;
	}
	sll_source_file_index_t idx=1;
	while (idx<source_file_list.length){
		source_file_with_index_t source_file=*(source_file_list.data+idx);
		sll_source_file_index_t off=0;
		for (sll_import_index_t i=0;i<source_file.source_file->import_table.length;i++){
			sll_import_file_t* if_=*(source_file.source_file->import_table.data+i);
			sll_source_file_index_t j=*(sf_idx_map+if_->source_file_index);
			if (j<idx){
				*(sf_idx_map+if_->source_file_index)=idx;
				source_file_with_index_t mv=*(source_file_list.data+j);
				for (sll_source_file_index_t k=j;k<idx;k++){
					source_file_with_index_t* e=source_file_list.data+k+1;
					(*(sf_idx_map+e->index))--;
					*(source_file_list.data+k)=*e;
				}
				*(source_file_list.data+idx)=mv;
				off++;
			}
		}
		idx+=1-off;
	}
	out->length=source_file_list.length;
	out->data=sll_allocate(source_file_list.length*sizeof(sll_source_file_t*));
	for (idx=0;idx<out->length;idx++){
		source_file_with_index_t* source_file=source_file_list.data+idx;
		*(out->data+idx)=source_file->source_file;
		for (sll_import_index_t i=0;i<source_file->source_file->import_table.length;i++){
			sll_import_file_t* if_=*(source_file->source_file->import_table.data+i);
			if_->source_file_index=*(sf_idx_map+if_->source_file_index);
			SLL_ASSERT(if_->source_file_index>idx);
		}
	}
	sll_deallocate(sf_idx_map);
	sll_deallocate(source_file_list.data);
	return 1;
}



__SLL_EXTERNAL void sll_free_bundle(sll_bundle_t* bundle){
	bundle->time=0;
	sll_free_string(&(bundle->name));
	while (bundle->length){
		bundle->length--;
		sll_bundle_source_file_t* bundle_source_file=*(bundle->data+bundle->length);
		sll_free_string(&(bundle_source_file->name));
		sll_free_source_file(&(bundle_source_file->data));
		sll_deallocate(bundle_source_file);
	}
	sll_deallocate(bundle->data);
	bundle->data=NULL;
}

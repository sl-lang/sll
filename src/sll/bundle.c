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



static sll_source_file_index_t _add_source_file(const sll_bundle_t* b,const sll_bundle_source_file_t* bsf,source_file_list_t* sfl){
	for (sll_source_file_index_t i=0;i<sfl->length;i++){
		sll_source_file_t* sf=(sfl->data+i)->source_file;
		if (bsf->data.file_size==sf->file_size&&sll_compare_data(&(bsf->data.file_hash),&(sf->file_hash),sizeof(sll_sha256_data_t))==SLL_COMPARE_RESULT_EQUAL){
			return i;
		}
	}
	sll_source_file_index_t out=sfl->length;
	sfl->length++;
	sfl->data=sll_reallocate(sfl->data,sfl->length*sizeof(source_file_with_index_t));
	sll_source_file_t* sf=sll_allocate(sizeof(sll_source_file_t));
	(sfl->data+out)->source_file=sf;
	(sfl->data+out)->index=out;
	sll_source_file_t bsf_sf=bsf->data;
	sf->time=bsf_sf.time;
	sf->file_size=bsf_sf.file_size;
	sf->file_hash=bsf_sf.file_hash;
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sf->identifier_table.short_[i].length=bsf_sf.identifier_table.short_[i].length;
		sf->identifier_table.short_[i].data=sll_allocate(sf->identifier_table.short_[i].length*sizeof(sll_identifier_t));
		sll_copy_data(bsf_sf.identifier_table.short_[i].data,sf->identifier_table.short_[i].length*sizeof(sll_identifier_t),sf->identifier_table.short_[i].data);
	}
	sf->identifier_table.long_data_length=bsf_sf.identifier_table.long_data_length;
	sf->identifier_table.long_data=sll_allocate(sf->identifier_table.long_data_length*sizeof(sll_identifier_t));
	sll_copy_data(bsf_sf.identifier_table.long_data,sf->identifier_table.long_data_length*sizeof(sll_identifier_t),sf->identifier_table.long_data);
	sf->export_table.length=bsf_sf.export_table.length;
	sf->export_table.data=sll_allocate(sf->export_table.length*sizeof(sll_identifier_index_t));
	sll_copy_data(bsf_sf.export_table.data,sf->export_table.length*sizeof(sll_identifier_index_t),sf->export_table.data);
	sf->function_table.length=bsf_sf.function_table.length;
	sf->function_table.data=sll_allocate(sf->function_table.length*sizeof(sll_function_t*));
	for (sll_function_index_t i=0;i<sf->function_table.length;i++){
		const sll_function_t* bsf_fn=*(bsf_sf.function_table.data+i);
		sll_function_t* k=sll_allocate(sizeof(sll_function_t)+SLL_FUNCTION_GET_ARGUMENT_COUNT(bsf_fn)*sizeof(sll_identifier_index_t));
		sll_copy_data(bsf_fn,sizeof(sll_function_t)+SLL_FUNCTION_GET_ARGUMENT_COUNT(bsf_fn)*sizeof(sll_identifier_index_t),k);
		*(sf->function_table.data+i)=k;
	}
	sf->string_table.length=bsf_sf.string_table.length;
	sf->string_table.data=sll_allocate(sf->string_table.length*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<sf->string_table.length;i++){
		sll_string_clone(bsf_sf.string_table.data+i,sf->string_table.data+i);
	}
	sf->import_table.length=bsf_sf.import_table.length;
	sf->import_table.data=sll_allocate(sf->import_table.length*sizeof(sll_import_file_t*));
	for (sll_import_index_t i=0;i<sf->import_table.length;i++){
		const sll_import_file_t* bsf_if=*(bsf_sf.import_table.data+i);
		sll_import_file_t* if_=sll_allocate(sizeof(sll_import_file_t)+bsf_if->length*sizeof(sll_identifier_index_t));
		sll_copy_data(bsf_if,sizeof(sll_import_file_t)+bsf_if->length*sizeof(sll_identifier_index_t),if_);
		if_->source_file_index=_add_source_file(b,*(b->data+bsf_if->source_file_index),sfl);
		*(sf->import_table.data+i)=if_;
	}
	sf->file_path_string_index=bsf_sf.file_path_string_index;
	sf->_next_scope=bsf_sf._next_scope;
	_clone_node_stack(&bsf_sf,sf);
	return out;
}



__SLL_EXTERNAL void sll_bundle_add_file(const sll_char_t* nm,sll_compilation_data_t* c_dt,sll_bundle_t* o){
	sll_source_file_index_t* idx_m=sll_allocate_stack(c_dt->length*sizeof(sll_source_file_index_t));
	for (sll_source_file_index_t idx=0;idx<c_dt->length;idx++){
		sll_source_file_t* sf=*(c_dt->data+idx);
		sll_source_file_index_t bsf_i=0;
		while (bsf_i<o->length){
			sll_bundle_source_file_t* bsf=*(o->data+bsf_i);
			if (bsf->data.file_size==sf->file_size&&sll_compare_data(&(bsf->data.file_hash),&(sf->file_hash),sizeof(sll_sha256_data_t))==SLL_COMPARE_RESULT_EQUAL){
				break;
			}
			bsf_i++;
		}
		*(idx_m+idx)=bsf_i;
		if (bsf_i<o->length){
			sll_free_source_file(sf);
			if (!idx){
				sll_bundle_source_file_t* bsf=*(o->data+bsf_i);
				if (!bsf->name.length){
					sll_string_from_pointer(nm,&(bsf->name));
				}
			}
			(*(idx_m+idx))|=BUNDLE_DO_NOT_REMAP_IMPORT;
			continue;
		}
		o->length++;
		o->data=sll_reallocate(o->data,o->length*sizeof(sll_bundle_source_file_t));
		sll_bundle_source_file_t* bsf=sll_allocate(sizeof(sll_bundle_source_file_t));
		if (!idx){
			sll_string_from_pointer(nm,&(bsf->name));
		}
		else{
			SLL_INIT_STRING(&(bsf->name));
		}
		bsf->data=*sf;
		*(o->data+o->length-1)=bsf;
	}
	for (sll_source_file_index_t i=0;i<c_dt->length;i++){
		sll_deallocate(*(c_dt->data+i));
		if ((*(idx_m+i))&BUNDLE_DO_NOT_REMAP_IMPORT){
			continue;
		}
		sll_bundle_source_file_t* bsf=*(o->data+(*(idx_m+i)));
		for (sll_import_index_t j=0;j<bsf->data.import_table.length;j++){
			sll_import_file_t* bsf_if=*(bsf->data.import_table.data+j);
			bsf_if->source_file_index=BUNDLE_GET_INDEX(idx_m+bsf_if->source_file_index);
		}
	}
	sll_deallocate(idx_m);
	sll_deallocate(c_dt->data);
	c_dt->data=NULL;
	c_dt->length=0;
}



__SLL_EXTERNAL void sll_bundle_create(const sll_char_t* nm,sll_bundle_t* o){
	o->time=sll_platform_get_current_time();
	sll_string_from_pointer(nm,&(o->name));
	o->data=NULL;
	o->length=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_bundle_fetch(const sll_bundle_t* b,const sll_string_t* nm,sll_compilation_data_t* c_dt){
	if (!sll_string_starts(nm,&(b->name))){
		return 0;
	}
	sll_string_t f_nm_str;
	sll_string_resize(nm,-((sll_integer_t)(b->name.length)),&f_nm_str);
	const sll_bundle_source_file_t* bsf=NULL;
	for (sll_source_file_index_t i=0;i<b->length;i++){
		bsf=*(b->data+i);
		if (STRING_EQUAL(&(bsf->name),&f_nm_str)){
			break;
		}
		bsf=NULL;
	}
	sll_free_string(&f_nm_str);
	if (!bsf){
		return 0;
	}
	source_file_list_t sfl={
		sll_allocate_stack(1),
		0
	};
	_add_source_file(b,bsf,&sfl);
	sll_source_file_index_t* sf_idx_map=sll_allocate_stack(sfl.length*sizeof(sll_source_file_index_t));
	for (sll_source_file_index_t idx=1;idx<sfl.length;idx++){
		*(sf_idx_map+idx)=idx;
	}
	sll_source_file_index_t idx=1;
	while (idx<sfl.length){
		source_file_with_index_t sf=*(sfl.data+idx);
		sll_source_file_index_t off=0;
		for (sll_import_index_t i=0;i<sf.source_file->import_table.length;i++){
			sll_import_file_t* if_=*(sf.source_file->import_table.data+i);
			sll_source_file_index_t j=*(sf_idx_map+if_->source_file_index);
			if (j<idx){
				*(sf_idx_map+if_->source_file_index)=idx;
				source_file_with_index_t mv=*(sfl.data+j);
				for (sll_source_file_index_t k=j;k<idx;k++){
					source_file_with_index_t* e=sfl.data+k+1;
					(*(sf_idx_map+e->index))--;
					*(sfl.data+k)=*e;
				}
				*(sfl.data+idx)=mv;
				off++;
			}
		}
		idx+=1-off;
	}
	c_dt->length=sfl.length;
	c_dt->data=sll_allocate(sfl.length*sizeof(sll_source_file_t*));
	for (idx=0;idx<c_dt->length;idx++){
		source_file_with_index_t* sf=sfl.data+idx;
		*(c_dt->data+idx)=sf->source_file;
		for (sll_import_index_t i=0;i<sf->source_file->import_table.length;i++){
			sll_import_file_t* if_=*(sf->source_file->import_table.data+i);
			if_->source_file_index=*(sf_idx_map+if_->source_file_index);
			SLL_ASSERT(if_->source_file_index>idx);
		}
	}
	sll_deallocate(sf_idx_map);
	sll_deallocate(sfl.data);
	return 1;
}



__SLL_EXTERNAL void sll_free_bundle(sll_bundle_t* b){
	b->time=0;
	sll_free_string(&(b->name));
	while (b->length){
		b->length--;
		sll_bundle_source_file_t* bsf=*(b->data+b->length);
		sll_free_string(&(bsf->name));
		sll_free_source_file(&(bsf->data));
		sll_deallocate(bsf);
	}
	sll_deallocate(b->data);
	b->data=NULL;
}

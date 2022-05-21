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
	for (sll_source_file_index_t i=0;i<sfl->l;i++){
		sll_source_file_t* sf=(sfl->dt+i)->dt;
		if (bsf->data.sz==sf->sz&&sll_compare_data(&(bsf->data.h),&(sf->h),sizeof(sll_sha256_data_t))==SLL_COMPARE_RESULT_EQUAL){
			return i;
		}
	}
	sll_source_file_index_t out=sfl->l;
	sfl->l++;
	sfl->dt=sll_reallocate(sfl->dt,sfl->l*sizeof(source_file_with_index_t));
	sll_source_file_t* sf=sll_allocate(sizeof(sll_source_file_t));
	(sfl->dt+out)->dt=sf;
	(sfl->dt+out)->idx=out;
	sll_source_file_t bsf_sf=bsf->data;
	sf->tm=bsf_sf.tm;
	sf->sz=bsf_sf.sz;
	sf->h=bsf_sf.h;
	for (sll_identifier_index_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sf->idt.s[i].l=bsf_sf.idt.s[i].l;
		sf->idt.s[i].dt=sll_allocate(sf->idt.s[i].l*sizeof(sll_identifier_t));
		sll_copy_data(bsf_sf.idt.s[i].dt,sf->idt.s[i].l*sizeof(sll_identifier_t),sf->idt.s[i].dt);
	}
	sf->idt.ill=bsf_sf.idt.ill;
	sf->idt.il=sll_allocate(sf->idt.ill*sizeof(sll_identifier_t));
	sll_copy_data(bsf_sf.idt.il,sf->idt.ill*sizeof(sll_identifier_t),sf->idt.il);
	sf->et.l=bsf_sf.et.l;
	sf->et.dt=sll_allocate(sf->et.l*sizeof(sll_identifier_index_t));
	sll_copy_data(bsf_sf.et.dt,sf->et.l*sizeof(sll_identifier_index_t),sf->et.dt);
	sf->ft.l=bsf_sf.ft.l;
	sf->ft.dt=sll_allocate(sf->ft.l*sizeof(sll_function_t*));
	for (sll_function_index_t i=0;i<sf->ft.l;i++){
		const sll_function_t* bsf_fn=*(bsf_sf.ft.dt+i);
		sll_function_t* k=sll_allocate(sizeof(sll_function_t)+SLL_FUNCTION_GET_ARGUMENT_COUNT(bsf_fn)*sizeof(sll_identifier_index_t));
		sll_copy_data(bsf_fn,sizeof(sll_function_t)+SLL_FUNCTION_GET_ARGUMENT_COUNT(bsf_fn)*sizeof(sll_identifier_index_t),k);
		*(sf->ft.dt+i)=k;
	}
	sf->st.l=bsf_sf.st.l;
	sf->st.dt=sll_allocate(sf->st.l*sizeof(sll_string_t));
	for (sll_string_index_t i=0;i<sf->st.l;i++){
		sll_string_clone(bsf_sf.st.dt+i,sf->st.dt+i);
	}
	sf->it.l=bsf_sf.it.l;
	sf->it.dt=sll_allocate(sf->it.l*sizeof(sll_import_file_t*));
	for (sll_import_index_t i=0;i<sf->it.l;i++){
		const sll_import_file_t* bsf_if=*(bsf_sf.it.dt+i);
		sll_import_file_t* if_=sll_allocate(sizeof(sll_import_file_t)+bsf_if->l*sizeof(sll_identifier_index_t));
		sll_copy_data(bsf_if,sizeof(sll_import_file_t)+bsf_if->l*sizeof(sll_identifier_index_t),if_);
		if_->sfi=_add_source_file(b,*(b->data+bsf_if->sfi),sfl);
		*(sf->it.dt+i)=if_;
	}
	sf->fp_nm=bsf_sf.fp_nm;
	sf->_n_sc_id=bsf_sf._n_sc_id;
	_clone_node_stack(&bsf_sf,sf);
	return out;
}



__SLL_EXTERNAL void sll_bundle_add_file(const sll_char_t* nm,sll_compilation_data_t* c_dt,sll_bundle_t* o){
	sll_source_file_index_t* idx_m=sll_allocate_stack(c_dt->l*sizeof(sll_source_file_index_t));
	for (sll_source_file_index_t idx=0;idx<c_dt->l;idx++){
		sll_source_file_t* sf=*(c_dt->dt+idx);
		sll_source_file_index_t bsf_i=0;
		while (bsf_i<o->length){
			sll_bundle_source_file_t* bsf=*(o->data+bsf_i);
			if (bsf->data.sz==sf->sz&&sll_compare_data(&(bsf->data.h),&(sf->h),sizeof(sll_sha256_data_t))==SLL_COMPARE_RESULT_EQUAL){
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
	for (sll_source_file_index_t i=0;i<c_dt->l;i++){
		sll_deallocate(*(c_dt->dt+i));
		if ((*(idx_m+i))&BUNDLE_DO_NOT_REMAP_IMPORT){
			continue;
		}
		sll_bundle_source_file_t* bsf=*(o->data+(*(idx_m+i)));
		for (sll_import_index_t j=0;j<bsf->data.it.l;j++){
			sll_import_file_t* bsf_if=*(bsf->data.it.dt+j);
			bsf_if->sfi=BUNDLE_GET_INDEX(idx_m+bsf_if->sfi);
		}
	}
	sll_deallocate(idx_m);
	sll_deallocate(c_dt->dt);
	c_dt->dt=NULL;
	c_dt->l=0;
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
	sll_source_file_index_t* sf_idx_map=sll_allocate_stack(sfl.l*sizeof(sll_source_file_index_t));
	for (sll_source_file_index_t idx=1;idx<sfl.l;idx++){
		*(sf_idx_map+idx)=idx;
	}
	sll_source_file_index_t idx=1;
	while (idx<sfl.l){
		source_file_with_index_t sf=*(sfl.dt+idx);
		sll_source_file_index_t off=0;
		for (sll_import_index_t i=0;i<sf.dt->it.l;i++){
			sll_import_file_t* if_=*(sf.dt->it.dt+i);
			sll_source_file_index_t j=*(sf_idx_map+if_->sfi);
			if (j<idx){
				*(sf_idx_map+if_->sfi)=idx;
				source_file_with_index_t mv=*(sfl.dt+j);
				for (sll_source_file_index_t k=j;k<idx;k++){
					source_file_with_index_t* e=sfl.dt+k+1;
					(*(sf_idx_map+e->idx))--;
					*(sfl.dt+k)=*e;
				}
				*(sfl.dt+idx)=mv;
				off++;
			}
		}
		idx+=1-off;
	}
	c_dt->l=sfl.l;
	c_dt->dt=sll_allocate(sfl.l*sizeof(sll_source_file_t*));
	for (idx=0;idx<c_dt->l;idx++){
		source_file_with_index_t* sf=sfl.dt+idx;
		*(c_dt->dt+idx)=sf->dt;
		for (sll_import_index_t i=0;i<sf->dt->it.l;i++){
			sll_import_file_t* if_=*(sf->dt->it.dt+i);
			if_->sfi=*(sf_idx_map+if_->sfi);
			SLL_ASSERT(if_->sfi>idx);
		}
	}
	sll_deallocate(sf_idx_map);
	sll_deallocate(sfl.dt);
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

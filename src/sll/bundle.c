#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/node.h>
#include <sll/memory.h>
#include <sll/platform/memory.h>
#include <sll/platform/util.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_bundle_add_file(const sll_char_t* nm,sll_compilation_data_t* c_dt,sll_bundle_t* o){
	sll_source_file_index_t* idx_m=sll_allocate_stack(c_dt->l*sizeof(sll_source_file_index_t));
	for (sll_source_file_index_t idx=0;idx<c_dt->l;idx++){
		sll_source_file_t* sf=*(c_dt->dt+idx);
		sll_source_file_index_t bsf_i=0;
		while (bsf_i<o->l){
			sll_bundle_source_file_t* bsf=*(o->dt+bsf_i);
			if (bsf->dt.sz==sf->sz&&sll_compare_data(&(bsf->dt.h),&(sf->h),sizeof(sll_sha256_data_t))==SLL_COMPARE_RESULT_EQUAL){
				break;
			}
			bsf_i++;
		}
		*(idx_m+idx)=bsf_i;
		if (bsf_i<o->l){
			sll_free_source_file(sf);
			if (!idx){
				sll_bundle_source_file_t* bsf=*(o->dt+bsf_i);
				if (!bsf->nm.l){
					sll_string_from_pointer(nm,&(bsf->nm));
				}
			}
			(*(idx_m+idx))|=BUNDLE_DO_NOT_REMAP_IMPORT;
			continue;
		}
		o->l++;
		o->dt=sll_reallocate(o->dt,o->l*sizeof(sll_bundle_source_file_t));
		sll_bundle_source_file_t* bsf=sll_allocate(sizeof(sll_bundle_source_file_t));
		if (!idx){
			sll_string_from_pointer(nm,&(bsf->nm));
		}
		else{
			SLL_INIT_STRING(&(bsf->nm));
		}
		bsf->dt=*sf;
		*(o->dt+o->l-1)=bsf;
	}
	for (sll_source_file_index_t i=0;i<c_dt->l;i++){
		sll_deallocate(*(c_dt->dt+i));
		if ((*(idx_m+i))&BUNDLE_DO_NOT_REMAP_IMPORT){
			continue;
		}
		sll_bundle_source_file_t* bsf=*(o->dt+(*(idx_m+i)));
		for (sll_import_index_t j=0;j<bsf->dt.it.l;j++){
			sll_import_file_t* bsf_if=*(bsf->dt.it.dt+j);
			bsf_if->sfi=BUNDLE_GET_INDEX(idx_m+bsf_if->sfi);
		}
	}
	sll_deallocate(idx_m);
	sll_deallocate(c_dt->dt);
	c_dt->dt=NULL;
	c_dt->l=0;
}



__SLL_EXTERNAL void sll_bundle_create(const sll_char_t* nm,sll_bundle_t* o){
	o->tm=sll_platform_get_current_time();
	sll_string_from_pointer(nm,&(o->nm));
	o->dt=NULL;
	o->l=0;
}



__SLL_EXTERNAL void sll_free_bundle(sll_bundle_t* b){
	b->tm=0;
	sll_free_string(&(b->nm));
	while (b->l){
		b->l--;
		sll_bundle_source_file_t* bsf=*(b->dt+b->l);
		sll_free_string(&(bsf->nm));
		sll_free_source_file(&(bsf->dt));
		sll_deallocate(bsf);
	}
	sll_deallocate(b->dt);
	b->dt=NULL;
}

#include <lll/_lll_internal.h>
#include <lll/common.h>
#include <lll/core.h>
#include <lll/types.h>
#include <lll/api.h>
#include <stdlib.h>



IINTERNAL_FUNCTION_SETUP



__LLL_FUNC void lll_create_internal_function_table(lll_internal_function_table_t* o){
	o->dt=NULL;
	o->l=0;
}



__LLL_FUNC __LLL_RETURN_FUNCTION_INDEX lll_lookup_internal_function(const lll_internal_function_table_t* i_ft,const char* nm){
	uint8_t l=0;
	lll_string_checksum_t c=0;
	while (*(nm+l)){
		c^=*(nm+l);
		l++;
	}
	for (lll_function_index_t i=0;i<i_ft->l;i++){
		lll_internal_function_t* f=*(i_ft->dt+i);
		if (f->c==c&&f->nml==l){
			for (uint8_t j=0;j<l;j++){
				if (*(nm+j)!=f->nm[j]){
					goto _check_next;
				}
			}
			return i;
		}
_check_next:;
	}
	return LLL_MAX_FUNCTION_INDEX;
}



__LLL_FUNC lll_function_index_t lll_register_internal_function(lll_internal_function_table_t* i_ft,const char* nm,lll_internal_function_pointer_t f){
	i_ft->l++;
	i_ft->dt=realloc(i_ft->dt,i_ft->l*sizeof(lll_internal_function_t*));
	lll_internal_function_t* i_f=malloc(sizeof(lll_internal_function_t));
	i_f->nml=0;
	i_f->c=0;
	while (*(nm+i_f->nml)){
		i_f->nm[i_f->nml]=*(nm+i_f->nml);
		i_f->c^=*(nm+i_f->nml);
		i_f->nml++;
	}
	i_f->p=f;
	*(i_ft->dt+i_ft->l-1)=i_f;
	return i_ft->l-1;
}



__LLL_FUNC void lll_register_standard_internal_functions(lll_internal_function_table_t* i_ft){
	const internal_function_t** f=&__ifunc_start;
	while (f<&__ifunc_end){
		if (*f){
			lll_register_internal_function(i_ft,(*f)->nm,(*f)->f);
		}
		f++;
	}
}

#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/types.h>
#include <stdlib.h>



INTERNAL_FUNCTION_SETUP;



__SLL_FUNC void sll_create_internal_function_table(sll_internal_function_table_t* o){
	o->dt=NULL;
	o->l=0;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_function_index_t sll_lookup_internal_function(const sll_internal_function_table_t* i_ft,const sll_char_t* nm){
	uint8_t l=0;
	sll_string_checksum_t c=0;
	while (*(nm+l)){
		c^=*(nm+l);
		l++;
	}
	for (sll_function_index_t i=0;i<i_ft->l;i++){
		sll_internal_function_t* f=*(i_ft->dt+i);
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
	return SLL_MAX_FUNCTION_INDEX;
}



__SLL_FUNC sll_function_index_t sll_register_internal_function(sll_internal_function_table_t* i_ft,const sll_char_t* nm,sll_internal_function_pointer_t f,sll_internal_function_type_t t){
	i_ft->l++;
	i_ft->dt=realloc(i_ft->dt,i_ft->l*sizeof(sll_internal_function_t*));
	sll_internal_function_t* i_f=malloc(sizeof(sll_internal_function_t));
	i_f->nml=0;
	i_f->c=0;
	while (*(nm+i_f->nml)){
		i_f->nm[i_f->nml]=*(nm+i_f->nml);
		i_f->c^=*(nm+i_f->nml);
		i_f->nml++;
	}
	i_f->p=f;
	i_f->t=t;
	*(i_ft->dt+i_ft->l-1)=i_f;
	return i_ft->l-1;
}



__SLL_FUNC void sll_register_standard_internal_functions(sll_internal_function_table_t* i_ft){
	const internal_function_t*const* f=&__ifunc_start;
	while (f<&__ifunc_end){
		if (*f){
			sll_register_internal_function(i_ft,(*f)->nm,(*f)->f,(*f)->t);
		}
		f++;
	}
}

#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/ift.h>
#include <sll/memory.h>
#include <sll/types.h>
#include <sll/util.h>



INTERNAL_FUNCTION_SETUP;



__SLL_EXTERNAL void sll_create_internal_function_table(sll_internal_function_table_t* o){
	o->dt=NULL;
	o->l=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_function_index_t sll_lookup_internal_function(const sll_internal_function_table_t* i_ft,const sll_char_t* nm){
	sll_name_length_t l=0;
	sll_checksum_t c=0;
	while (*(nm+l)){
		c^=*(nm+l);
		l++;
	}
	for (sll_function_index_t i=0;i<i_ft->l;i++){
		sll_internal_function_t* f=*(i_ft->dt+i);
		if (f->c!=c||f->nml!=l||sll_compare_data(nm,f->nm,l)!=SLL_COMPARE_RESULT_EQUAL){
			continue;
		}
		return i;
	}
	return SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX;
}



__SLL_EXTERNAL sll_function_index_t sll_register_internal_function(sll_internal_function_table_t* i_ft,const sll_char_t* nm,sll_internal_function_pointer_t f,sll_internal_function_type_t t){
	i_ft->l++;
	i_ft->dt=sll_reallocate(i_ft->dt,i_ft->l*sizeof(sll_internal_function_t*));
	sll_internal_function_t* i_f=sll_allocate(sizeof(sll_internal_function_t));
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



__SLL_EXTERNAL void sll_register_standard_internal_functions(sll_internal_function_table_t* i_ft){
	const internal_function_t*const* f=&__ifunc_start;
	while (f<&__ifunc_end){
		if (*f){
			sll_register_internal_function(i_ft,(*f)->nm,(*f)->f,(*f)->t);
		}
		f++;
	}
}

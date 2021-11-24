#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/ift.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>



extern const sll_function_index_t _ifunc_size;
extern const internal_function_t* _ifunc_data;



__SLL_EXTERNAL void sll_create_internal_function_table(sll_internal_function_table_t* o){
	o->dt=NULL;
	o->l=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_function_index_t sll_lookup_internal_function(const sll_internal_function_table_t* i_ft,const sll_char_t* nm){
	sll_string_t tmp;
	sll_string_from_pointer(nm,&tmp);
	for (sll_function_index_t i=0;i<i_ft->l;i++){
		const sll_internal_function_t* f=*(i_ft->dt+i);
		if (sll_string_equal(&(f->nm),&tmp)){
			return i;
		}
	}
	return SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX;
}



__SLL_EXTERNAL sll_function_index_t sll_register_internal_function(sll_internal_function_table_t* i_ft,const sll_char_t* nm,sll_internal_function_pointer_t f,sll_internal_function_type_t t){
	i_ft->l++;
	i_ft->dt=sll_reallocate((void*)(i_ft->dt),i_ft->l*sizeof(const sll_internal_function_t*));
	sll_internal_function_t* i_f=sll_allocate(sizeof(sll_internal_function_t));
	sll_string_from_pointer(nm,(sll_string_t*)&(i_f->nm));
	*((sll_internal_function_pointer_t*)(&(i_f->p)))=f;
	*((sll_internal_function_type_t*)(&(i_f->t)))=t;
	*((const sll_internal_function_t**)(i_ft->dt+i_ft->l-1))=i_f;
	return i_ft->l-1;
}



__SLL_EXTERNAL void sll_register_builtin_internal_functions(sll_internal_function_table_t* i_ft){
	i_ft->l+=_ifunc_size;
	i_ft->dt=sll_reallocate((void*)(i_ft->dt),i_ft->l*sizeof(const sll_internal_function_t*));
	const internal_function_t* f=_ifunc_data;
	const sll_internal_function_t** p=(const sll_internal_function_t**)(i_ft->dt+i_ft->l-_ifunc_size);
	for (sll_function_index_t i=0;i<_ifunc_size;i++){
		sll_internal_function_t* nf=sll_allocate(sizeof(sll_internal_function_t));
		sll_string_from_pointer(f->nm,(sll_string_t*)&(nf->nm));
		*((sll_internal_function_pointer_t*)(&(nf->p)))=f->f;
		*((sll_internal_function_type_t*)(&(nf->t)))=f->t;
		*p=nf;
		f++;
		p++;
	}
}

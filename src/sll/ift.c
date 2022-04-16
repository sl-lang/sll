#include <sll/_internal/api.h>
#include <sll/_internal/common.h>
#include <sll/_internal/platform.h>
#include <sll/_internal/string.h>
#include <sll/common.h>
#include <sll/ift.h>
#include <sll/memory.h>
#include <sll/parse_args.h>
#include <sll/string.h>
#include <sll/types.h>



extern const sll_function_index_t _ifunc_size;
extern const internal_function_t* _ifunc_data;



__SLL_EXTERNAL void sll_clone_internal_function_table(sll_internal_function_table_t* ift,sll_internal_function_table_t* o){
	o->l=ift->l;
	o->dt=sll_allocate(o->l*sizeof(const sll_internal_function_t));
	sll_internal_function_t* p=(sll_internal_function_t*)(o->dt);
	for (sll_function_index_t i=0;i<ift->l;i++){
		sll_string_clone(&((ift->dt+i)->nm),&(p->nm));
		p->p=(ift->dt+i)->p;
		p->fmt=(ift->dt+i)->fmt;
		p++;
	}
}



__SLL_EXTERNAL void sll_create_internal_function_table(sll_internal_function_table_t* o){
	o->dt=NULL;
	o->l=0;
}



__SLL_EXTERNAL void sll_free_internal_function_table(sll_internal_function_table_t* ift){
	sll_internal_function_t* f=(sll_internal_function_t*)(ift->dt);
	for (sll_function_index_t i=0;i<ift->l;i++){
		sll_free_string(&(f->nm));
		f++;
	}
	sll_deallocate(PTR(ift->dt));
	ift->dt=NULL;
	ift->l=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_function_index_t sll_lookup_internal_function(const sll_internal_function_table_t* i_ft,const sll_char_t* nm){
	sll_string_t tmp;
	sll_string_from_pointer(nm,&tmp);
	for (sll_function_index_t i=0;i<i_ft->l;i++){
		if (STRING_EQUAL(&((i_ft->dt+i)->nm),&tmp)){
			sll_free_string(&tmp);
			return i;
		}
	}
	sll_free_string(&tmp);
	return SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX;
}



__SLL_EXTERNAL sll_function_index_t sll_register_internal_function(sll_internal_function_table_t* i_ft,const sll_char_t* nm,const sll_char_t* fmt,sll_internal_function_pointer_t f){
	i_ft->l++;
	i_ft->dt=sll_reallocate(PTR(i_ft->dt),i_ft->l*sizeof(const sll_internal_function_t));
	sll_internal_function_t* nf=(sll_internal_function_t*)(i_ft->dt+i_ft->l-1);
	sll_string_from_pointer(nm,&(nf->nm));
	nf->p=f;
	nf->fmt=fmt;
	nf->_fmt_len=sll_parse_arg_count(fmt);
	return i_ft->l-1;
}



__SLL_EXTERNAL void sll_register_builtin_internal_functions(sll_internal_function_table_t* ift){
	ift->l+=_ifunc_size;
	ift->dt=sll_reallocate(PTR(ift->dt),ift->l*sizeof(const sll_internal_function_t));
	const internal_function_t* f=_ifunc_data;
	sll_internal_function_t* p=(sll_internal_function_t*)(ift->dt+ift->l-_ifunc_size);
	for (sll_function_index_t i=0;i<_ifunc_size;i++){
		sll_string_from_pointer(f->nm,&(p->nm));
		p->p=f->f;
		p->fmt=f->fmt;
		p->_fmt_len=sll_parse_arg_count(f->fmt);
		f++;
		p++;
	}
}

#include <sll/_internal/api_call.h>
#include <sll/_internal/common.h>
#include <sll/_internal/string.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/ift.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>



static void _create_function(sll_internal_function_pointer_t fn,const sll_char_t* nm,const sll_char_t* fmt,sll_internal_function_t* o){
	sll_string_from_pointer(nm,&(o->nm));
	o->p=fn;
	_parse_api_call_format(fmt,o);
}



__SLL_EXTERNAL void sll_clone_internal_function_table(sll_internal_function_table_t* ift,sll_internal_function_table_t* o){
	o->l=ift->l;
	o->dt=sll_allocate(o->l*sizeof(const sll_internal_function_t));
	sll_internal_function_t* p=(sll_internal_function_t*)(o->dt);
	for (sll_function_index_t i=0;i<ift->l;i++){
		sll_string_clone(&((ift->dt+i)->nm),&(p->nm));
		p->p=(ift->dt+i)->p;
		sll_string_length_t sz=sll_string_length((ift->dt+i)->fmt)+1;
		p->fmt=sll_allocate(sz);
		sll_copy_data((ift->dt+i)->fmt,sz,p->fmt);
		p->_ret=(ift->dt+i)->_ret;
		p->_arg_cnt=(ift->dt+i)->_arg_cnt;
		p->_arg_sz=(ift->dt+i)->_arg_sz;
		sz=(((p->_arg_cnt<<1)+65)>>6)*sizeof(bitmap_t);
		p->_regs=sll_allocate(sz);
		sll_copy_data((ift->dt+i)->_regs,sz,p->_regs);
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
		sll_deallocate(f->fmt);
		sll_deallocate(f->_regs);
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
	_create_function(f,nm,fmt,(sll_internal_function_t*)(i_ft->dt+i_ft->l-1));
	return i_ft->l-1;
}



__SLL_EXTERNAL void sll_register_internal_functions(sll_internal_function_table_t* i_ft,const sll_internal_function_descriptor_t* dt,sll_function_index_t len){
	i_ft->l+=len;
	i_ft->dt=sll_reallocate(PTR(i_ft->dt),i_ft->l*sizeof(const sll_internal_function_t));
	sll_internal_function_t* p=(sll_internal_function_t*)(i_ft->dt+i_ft->l-len);
	for (sll_function_index_t i=0;i<len;i++){
		_create_function(dt->f,dt->nm,dt->fmt,p);
		dt++;
		p++;
	}
}

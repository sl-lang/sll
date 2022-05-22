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
	sll_string_from_pointer(nm,&(o->name));
	o->function=fn;
	_parse_api_call_format(fmt,o);
}



__SLL_EXTERNAL void sll_clone_internal_function_table(sll_internal_function_table_t* ift,sll_internal_function_table_t* o){
	o->length=ift->length;
	o->data=sll_allocate(o->length*sizeof(const sll_internal_function_t));
	sll_internal_function_t* p=(sll_internal_function_t*)(o->data);
	for (sll_function_index_t i=0;i<ift->length;i++){
		sll_string_clone(&((ift->data+i)->name),&(p->name));
		p->function=(ift->data+i)->function;
		sll_string_length_t sz=sll_string_length((ift->data+i)->format)+1;
		p->format=sll_allocate(sz);
		sll_copy_data((ift->data+i)->format,sz,p->format);
		p->_return_value=(ift->data+i)->_return_value;
		p->_arg_count=(ift->data+i)->_arg_count;
		p->_arg_size=(ift->data+i)->_arg_size;
		sz=(((p->_arg_count<<1)+65)>>6)*sizeof(bitmap_t);
		p->_registers=sll_allocate(sz);
		sll_copy_data((ift->data+i)->_registers,sz,p->_registers);
		p++;
	}
}



__SLL_EXTERNAL void sll_create_internal_function_table(sll_internal_function_table_t* o){
	o->data=NULL;
	o->length=0;
}



__SLL_EXTERNAL void sll_free_internal_function_table(sll_internal_function_table_t* ift){
	sll_internal_function_t* f=(sll_internal_function_t*)(ift->data);
	for (sll_function_index_t i=0;i<ift->length;i++){
		sll_free_string(&(f->name));
		sll_deallocate(f->format);
		sll_deallocate(f->_registers);
		f++;
	}
	sll_deallocate(PTR(ift->data));
	ift->data=NULL;
	ift->length=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_function_index_t sll_lookup_internal_function(const sll_internal_function_table_t* i_ft,const sll_char_t* nm){
	sll_string_t tmp;
	sll_string_from_pointer(nm,&tmp);
	for (sll_function_index_t i=0;i<i_ft->length;i++){
		if (STRING_EQUAL(&((i_ft->data+i)->name),&tmp)){
			sll_free_string(&tmp);
			return i;
		}
	}
	sll_free_string(&tmp);
	return SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX;
}



__SLL_EXTERNAL sll_function_index_t sll_register_internal_function(sll_internal_function_table_t* i_ft,const sll_char_t* nm,const sll_char_t* fmt,sll_internal_function_pointer_t f){
	i_ft->length++;
	i_ft->data=sll_reallocate(PTR(i_ft->data),i_ft->length*sizeof(const sll_internal_function_t));
	_create_function(f,nm,fmt,(sll_internal_function_t*)(i_ft->data+i_ft->length-1));
	return i_ft->length-1;
}



__SLL_EXTERNAL void sll_register_internal_functions(sll_internal_function_table_t* i_ft,const sll_internal_function_descriptor_t* dt,sll_function_index_t len){
	i_ft->length+=len;
	i_ft->data=sll_reallocate(PTR(i_ft->data),i_ft->length*sizeof(const sll_internal_function_t));
	sll_internal_function_t* p=(sll_internal_function_t*)(i_ft->data+i_ft->length-len);
	for (sll_function_index_t i=0;i<len;i++){
		_create_function(dt->function,dt->name,dt->format,p);
		dt++;
		p++;
	}
}

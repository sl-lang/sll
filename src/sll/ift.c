#include <sll/_internal/api_call.h>
#include <sll/_internal/common.h>
#include <sll/_internal/string.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/ift.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>



static void _create_function(sll_internal_function_pointer_t fn,const sll_char_t* nm,const sll_char_t* fmt,sll_internal_function_t* out){
	sll_string_from_pointer(nm,&(out->name));
	out->function=fn;
	_parse_api_call_format(fmt,out);
}



__SLL_EXTERNAL void sll_clone_internal_function_table(sll_internal_function_table_t* internal_function_table,sll_internal_function_table_t* out){
	out->length=internal_function_table->length;
	out->data=sll_allocate(out->length*sizeof(const sll_internal_function_t));
	sll_internal_function_t* ptr=(sll_internal_function_t*)(out->data);
	for (sll_function_index_t i=0;i<internal_function_table->length;i++){
		sll_string_clone(&((internal_function_table->data+i)->name),&(ptr->name));
		ptr->function=(internal_function_table->data+i)->function;
		sll_string_length_t size=sll_string_length((internal_function_table->data+i)->format)+1;
		ptr->format=sll_allocate(size);
		sll_copy_data((internal_function_table->data+i)->format,size,ptr->format);
		ptr->_return_value=(internal_function_table->data+i)->_return_value;
		ptr->_arg_count=(internal_function_table->data+i)->_arg_count;
		ptr->_arg_size=(internal_function_table->data+i)->_arg_size;
		size=(((ptr->_arg_count<<1)+65)>>6)*sizeof(bitmap_t);
		ptr->_registers=sll_allocate(size);
		sll_copy_data((internal_function_table->data+i)->_registers,size,ptr->_registers);
		ptr++;
	}
}



__SLL_EXTERNAL void sll_create_internal_function_table(sll_internal_function_table_t* out){
	out->data=NULL;
	out->length=0;
}



__SLL_EXTERNAL void sll_free_internal_function_table(sll_internal_function_table_t* internal_function_table){
	sll_internal_function_t* ptr=(sll_internal_function_t*)(internal_function_table->data);
	for (sll_function_index_t i=0;i<internal_function_table->length;i++){
		sll_free_string(&(ptr->name));
		sll_deallocate(ptr->format);
		sll_deallocate(ptr->_registers);
		ptr++;
	}
	sll_deallocate(PTR(internal_function_table->data));
	internal_function_table->data=NULL;
	internal_function_table->length=0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_function_index_t sll_lookup_internal_function(const sll_internal_function_table_t* internal_function_table,const sll_char_t* name){
	sll_string_t name_str;
	sll_string_from_pointer(name,&name_str);
	for (sll_function_index_t i=0;i<internal_function_table->length;i++){
		if (STRING_EQUAL(&((internal_function_table->data+i)->name),&name_str)){
			sll_free_string(&name_str);
			return i;
		}
	}
	sll_free_string(&name_str);
	return SLL_UNKNOWN_INTERNAL_FUNCTION_INDEX;
}



__SLL_EXTERNAL sll_function_index_t sll_register_internal_function(sll_internal_function_table_t* internal_function_table,const sll_char_t* name,const sll_char_t* format,sll_internal_function_pointer_t function){
	internal_function_table->length++;
	internal_function_table->data=sll_reallocate(PTR(internal_function_table->data),internal_function_table->length*sizeof(const sll_internal_function_t));
	_create_function(function,name,format,(sll_internal_function_t*)(internal_function_table->data+internal_function_table->length-1));
	return internal_function_table->length-1;
}



__SLL_EXTERNAL void sll_register_internal_functions(sll_internal_function_table_t* internal_function_table,const sll_internal_function_descriptor_t* data,sll_function_index_t length){
	internal_function_table->length+=length;
	internal_function_table->data=sll_reallocate(PTR(internal_function_table->data),internal_function_table->length*sizeof(const sll_internal_function_t));
	sll_internal_function_t* p=(sll_internal_function_t*)(internal_function_table->data+internal_function_table->length-length);
	for (sll_function_index_t i=0;i<length;i++){
		_create_function(data->function,data->name,data->format,p);
		data++;
		p++;
	}
}

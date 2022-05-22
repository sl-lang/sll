#include <sll/_internal/common.h>
#include <sll/_internal/stack.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/node.h>
#include <sll/platform/memory.h>
#include <sll/platform/time.h>
#include <sll/string.h>
#include <sll/string_table.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_compilation_data_from_source_file(const sll_source_file_t* sf,sll_compilation_data_t* out){
	out->length=1;
	out->data=sll_allocate(sizeof(sll_source_file_t*));
	*(out->data)=sll_allocate(sizeof(sll_source_file_t));
	**(out->data)=*sf;
}



__SLL_EXTERNAL void sll_free_compilation_data(sll_compilation_data_t* compilation_data){
	for (sll_source_file_index_t i=0;i<compilation_data->length;i++){
		sll_source_file_t* sf=*(compilation_data->data+i);
		sll_free_source_file(sf);
		sll_deallocate(sf);
	}
	sll_deallocate(compilation_data->data);
	compilation_data->data=NULL;
	compilation_data->length=0;
}



__SLL_EXTERNAL void sll_free_source_file(sll_source_file_t* sf){
	sf->time=0;
	sf->file_size=0;
	sf->first_node=NULL;
	for (unsigned int i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sll_identifier_list_t* e=sf->identifier_table.short_+i;
		sll_deallocate(e->data);
		e->data=NULL;
		e->length=0;
	}
	sll_deallocate(sf->identifier_table.long_data);
	sf->identifier_table.long_data=NULL;
	sf->identifier_table.long_data_length=0;
	sll_deallocate(sf->export_table.data);
	sf->export_table.data=NULL;
	sf->export_table.length=0;
	for (sll_function_index_t i=0;i<sf->function_table.length;i++){
		sll_deallocate(*(sf->function_table.data+i));
	}
	sll_deallocate(sf->function_table.data);
	sf->function_table.data=NULL;
	sf->function_table.length=0;
	for (sll_string_index_t i=0;i<sf->string_table.length;i++){
		sll_free_string(sf->string_table.data+i);
	}
	sll_deallocate(sf->string_table.data);
	sf->string_table.data=NULL;
	sf->string_table.length=0;
	void* pg=sf->_stack.start;
	while (pg){
		void* n=*((void**)pg);
		SLL_CRITICAL_ERROR(sll_platform_free_page(pg,SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)));
		pg=n;
	}
	for (sll_import_index_t i=0;i<sf->import_table.length;i++){
		sll_deallocate(*(sf->import_table.data+i));
	}
	sll_deallocate(sf->import_table.data);
	sf->import_table.data=NULL;
	sf->import_table.length=0;
	sf->file_path_string_index=0;
	sf->_stack.start=NULL;
	sf->_stack.end=NULL;
	sf->_stack.count=0;
	sf->_stack.next_node=NULL;
	sf->_stack.offset=0;
	sf->_next_scope=1;
}



__SLL_EXTERNAL void sll_init_compilation_data(const sll_char_t* fp,sll_compilation_data_t* o){
	sll_source_file_t* sf=sll_allocate(sizeof(sll_source_file_t));
	sf->time=sll_platform_get_current_time();
	sf->file_size=0;
	sf->first_node=NULL;
	for (unsigned int i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sf->identifier_table.short_[i].data=NULL;
		sf->identifier_table.short_[i].length=0;
	}
	sf->identifier_table.long_data=NULL;
	sf->identifier_table.long_data_length=0;
	sf->export_table.data=NULL;
	sf->export_table.length=0;
	sf->function_table.data=NULL;
	sf->function_table.length=0;
	sf->string_table.data=sll_allocate(sizeof(sll_string_t));
	sf->string_table.length=1;
	sll_string_from_pointer(fp,sf->string_table.data);
	sf->import_table.data=NULL;
	sf->import_table.length=0;
	sf->file_path_string_index=0;
	_init_node_stack(sf);
	sf->_next_scope=1;
	o->data=sll_allocate(sizeof(sll_source_file_t*));
	o->length=1;
	*(o->data)=sf;
}

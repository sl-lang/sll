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



__SLL_EXTERNAL void sll_compilation_data_from_source_file(const sll_source_file_t* source_file,sll_compilation_data_t* out){
	out->length=1;
	out->data=sll_allocate(sizeof(sll_source_file_t*));
	*(out->data)=sll_allocate(sizeof(sll_source_file_t));
	**(out->data)=*source_file;
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



__SLL_EXTERNAL void sll_free_source_file(sll_source_file_t* source_file){
	source_file->time=0;
	source_file->file_size=0;
	source_file->first_node=NULL;
	sll_deallocate(source_file->identifier_table.data);
	source_file->identifier_table.data=NULL;
	source_file->identifier_table.length=0;
	sll_deallocate(source_file->export_table.data);
	source_file->export_table.data=NULL;
	source_file->export_table.length=0;
	for (sll_function_index_t i=0;i<source_file->function_table.length;i++){
		sll_deallocate(*(source_file->function_table.data+i));
	}
	sll_deallocate(source_file->function_table.data);
	source_file->function_table.data=NULL;
	source_file->function_table.length=0;
	for (sll_string_index_t i=0;i<source_file->string_table.length;i++){
		sll_free_string(source_file->string_table.data+i);
	}
	sll_deallocate(source_file->string_table.data);
	source_file->string_table.data=NULL;
	source_file->string_table.length=0;
	void* page=source_file->_stack.start;
	while (page){
		void* next=*((void**)page);
		SLL_CRITICAL_ERROR(sll_platform_free_page(page,SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)));
		page=next;
	}
	for (sll_import_index_t i=0;i<source_file->import_table.length;i++){
		sll_deallocate(*(source_file->import_table.data+i));
	}
	sll_deallocate(source_file->import_table.data);
	source_file->import_table.data=NULL;
	source_file->import_table.length=0;
	source_file->file_path_string_index=0;
	source_file->_stack.start=NULL;
	source_file->_stack.end=NULL;
	source_file->_stack.count=0;
	source_file->_stack.next_node=NULL;
	source_file->_stack.offset=0;
	source_file->_next_scope=1;
}



__SLL_EXTERNAL void sll_init_compilation_data(const sll_char_t* file_path,sll_compilation_data_t* out){
	sll_source_file_t* sf=sll_allocate(sizeof(sll_source_file_t));
	sf->time=sll_platform_get_current_time();
	sf->file_size=0;
	sf->first_node=NULL;
	sf->identifier_table.data=NULL;
	sf->identifier_table.length=0;
	sf->export_table.data=NULL;
	sf->export_table.length=0;
	sf->function_table.data=NULL;
	sf->function_table.length=0;
	sf->string_table.data=sll_allocate(sizeof(sll_string_t));
	sf->string_table.length=1;
	sll_string_from_pointer(file_path,sf->string_table.data);
	sf->import_table.data=NULL;
	sf->import_table.length=0;
	sf->file_path_string_index=0;
	_init_node_stack(sf);
	sf->_next_scope=1;
	out->data=sll_allocate(sizeof(sll_source_file_t*));
	out->length=1;
	*(out->data)=sf;
}

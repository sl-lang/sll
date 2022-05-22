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
	out->l=1;
	out->dt=sll_allocate(sizeof(sll_source_file_t*));
	*(out->dt)=sll_allocate(sizeof(sll_source_file_t));
	**(out->dt)=*sf;
}



__SLL_EXTERNAL void sll_free_compilation_data(sll_compilation_data_t* c_dt){
	for (sll_source_file_index_t i=0;i<c_dt->l;i++){
		sll_source_file_t* sf=*(c_dt->dt+i);
		sll_free_source_file(sf);
		sll_deallocate(sf);
	}
	sll_deallocate(c_dt->dt);
	c_dt->dt=NULL;
	c_dt->l=0;
}



__SLL_EXTERNAL void sll_free_source_file(sll_source_file_t* sf){
	sf->tm=0;
	sf->sz=0;
	sf->dt=NULL;
	for (unsigned int i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sll_identifier_list_t* e=sf->idt.short_+i;
		sll_deallocate(e->data);
		e->data=NULL;
		e->length=0;
	}
	sll_deallocate(sf->idt.long_data);
	sf->idt.long_data=NULL;
	sf->idt.long_data_length=0;
	sll_deallocate(sf->et.dt);
	sf->et.dt=NULL;
	sf->et.l=0;
	for (sll_function_index_t i=0;i<sf->ft.l;i++){
		sll_deallocate(*(sf->ft.dt+i));
	}
	sll_deallocate(sf->ft.dt);
	sf->ft.dt=NULL;
	sf->ft.l=0;
	for (sll_string_index_t i=0;i<sf->st.l;i++){
		sll_free_string(sf->st.dt+i);
	}
	sll_deallocate(sf->st.dt);
	sf->st.dt=NULL;
	sf->st.l=0;
	void* pg=sf->_s.s;
	while (pg){
		void* n=*((void**)pg);
		SLL_CRITICAL_ERROR(sll_platform_free_page(pg,SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE)));
		pg=n;
	}
	for (sll_import_index_t i=0;i<sf->it.l;i++){
		sll_deallocate(*(sf->it.dt+i));
	}
	sll_deallocate(sf->it.dt);
	sf->it.dt=NULL;
	sf->it.l=0;
	sf->fp_nm=0;
	sf->_s.s=NULL;
	sf->_s.e=NULL;
	sf->_s.c=0;
	sf->_s.p=NULL;
	sf->_s.off=0;
	sf->_n_sc_id=1;
}



__SLL_EXTERNAL void sll_init_compilation_data(const sll_char_t* fp,sll_compilation_data_t* o){
	sll_source_file_t* sf=sll_allocate(sizeof(sll_source_file_t));
	sf->tm=sll_platform_get_current_time();
	sf->sz=0;
	sf->dt=NULL;
	for (unsigned int i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sf->idt.short_[i].data=NULL;
		sf->idt.short_[i].length=0;
	}
	sf->idt.long_data=NULL;
	sf->idt.long_data_length=0;
	sf->et.dt=NULL;
	sf->et.l=0;
	sf->ft.dt=NULL;
	sf->ft.l=0;
	sf->st.dt=sll_allocate(sizeof(sll_string_t));
	sf->st.l=1;
	sll_string_from_pointer(fp,sf->st.dt);
	sf->it.dt=NULL;
	sf->it.l=0;
	sf->fp_nm=0;
	_init_node_stack(sf);
	sf->_n_sc_id=1;
	o->dt=sll_allocate(sizeof(sll_source_file_t*));
	o->l=1;
	*(o->dt)=sf;
}

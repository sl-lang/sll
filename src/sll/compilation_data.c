#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/platform.h>
#include <sll/string_table.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_free_compilation_data(sll_compilation_data_t* c_dt){
	c_dt->tm=0;
	c_dt->h=NULL;
	for (unsigned int i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sll_identifier_list_t* e=c_dt->idt.s+i;
		sll_deallocate(e->dt);
		e->dt=NULL;
		e->l=0;
	}
	sll_deallocate(c_dt->idt.il);
	c_dt->idt.il=NULL;
	c_dt->idt.ill=0;
	sll_deallocate(c_dt->et.dt);
	c_dt->et.dt=NULL;
	c_dt->et.l=0;
	for (sll_function_index_t i=0;i<c_dt->ft.l;i++){
		sll_deallocate(*(c_dt->ft.dt+i));
	}
	sll_deallocate(c_dt->ft.dt);
	c_dt->ft.dt=NULL;
	c_dt->ft.l=0;
	for (sll_string_index_t i=0;i<c_dt->st.l;i++){
		sll_free_string(c_dt->st.dt+i);
	}
	sll_deallocate(c_dt->st.dt);
	c_dt->st.dt=NULL;
	c_dt->st.l=0;
	void* pg=c_dt->_s.s;
	while (pg){
		void* n=*((void**)pg);
		sll_platform_free_page(pg,SLL_ROUND_PAGE(NODE_STACK_ALLOC_SIZE));
		pg=n;
	}
	sll_deallocate(c_dt->fpt.dt);
	c_dt->fpt.dt=NULL;
	c_dt->fpt.l=0;
	c_dt->_s.s=NULL;
	c_dt->_s.e=NULL;
	c_dt->_s.c=0;
	c_dt->_s.p=NULL;
	c_dt->_s.off=0;
	c_dt->_n_sc_id=1;
}



__SLL_EXTERNAL void sll_init_compilation_data(const sll_char_t* fp,sll_compilation_data_t* o){
	o->tm=sll_platform_get_current_time();
	o->h=NULL;
	for (unsigned int i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o->idt.s[i].dt=NULL;
		o->idt.s[i].l=0;
	}
	o->idt.il=NULL;
	o->idt.ill=0;
	o->et.dt=NULL;
	o->et.l=0;
	o->ft.dt=NULL;
	o->ft.l=0;
	o->st.dt=sll_allocate(sizeof(sll_string_t));
	o->st.l=1;
	sll_string_from_pointer(fp,o->st.dt);
	o->fpt.dt=sll_allocate(sizeof(sll_file_path_t));
	o->fpt.l=1;
	o->fpt.dt->nm=0;
	_init_node_stack(o);
	o->_n_sc_id=1;
}

#include <sll/common.h>
#include <sll/constants.h>
#include <sll/core.h>
#include <sll/gc.h>
#include <sll/types.h>
#include <stdlib.h>



__SLL_FUNC void sll_free_assembly_function_table(sll_assembly_function_table_t* ft){
	free(ft->dt);
	ft->dt=NULL;
	ft->l=0;
}



__SLL_FUNC void sll_free_assembly_data(sll_assembly_data_t* a_dt){
	a_dt->tm=0;
	a_dt->h=NULL;
	a_dt->ic=0;
	a_dt->vc=0;
	sll_free_assembly_function_table(&(a_dt->ft));
	sll_free_string_table(&(a_dt->st));
	a_dt->_s.ptr=NULL;
	a_dt->_s.off=0;
	a_dt->_s.sz=0;
}



__SLL_FUNC void sll_free_compilation_data(sll_compilation_data_t* c_dt){
	c_dt->is=NULL;
	c_dt->tm=0;
	c_dt->h=NULL;
	sll_free_identifier_table(&(c_dt->idt));
	sll_free_export_table(&(c_dt->et));
	sll_free_function_table(&(c_dt->ft));
	sll_free_string_table(&(c_dt->st));
	c_dt->_s.ptr=NULL;
	c_dt->_s.off=0;
	c_dt->_s.sz=0;
	c_dt->_n_sc_id=1;
}



__SLL_FUNC void sll_free_export_table(sll_export_table_t* et){
	free(et->dt);
	et->dt=NULL;
	et->l=0;
}



__SLL_FUNC void sll_free_function_table(sll_function_table_t* ft){
	for (sll_function_index_t i=0;i<ft->l;i++){
		free(*(ft->dt+i));
	}
	free(ft->dt);
	ft->dt=NULL;
	ft->l=0;
}



__SLL_FUNC void sll_free_identifier_table(sll_identifier_table_t* idt){
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		sll_identifier_list_t* e=idt->s+i;
		free(e->dt);
		e->dt=NULL;
		e->l=0;
	}
	free(idt->il);
	idt->il=NULL;
	idt->ill=0;
}



__SLL_FUNC void sll_free_internal_function_table(sll_internal_function_table_t* ift){
	for (sll_function_index_t i=0;i<ift->l;i++){
		free(*(ift->dt+i));
	}
	free(ift->dt);
	ift->dt=NULL;
	ift->l=0;
}



__SLL_FUNC void sll_free_runtime_object_stack_data(sll_runtime_object_stack_data_t* rst){
	rst->off=0;
	free(rst->m);
	rst->m=NULL;
	rst->ml=0;
}



__SLL_FUNC void sll_free_string_table(sll_string_table_t* st){
	for (sll_string_index_t i=0;i<st->l;i++){
		free((st->dt+i)->v);
	}
	free(st->dt);
	st->dt=NULL;
	st->l=0;
}

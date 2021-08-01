#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdlib.h>



__LLL_IMPORT_EXPORT void lll_free_assembly_function_table(lll_assembly_function_table_t* ft){
	if (ft->dt){
		free(ft->dt);
		ft->dt=NULL;
	}
	ft->l=0;
}



__LLL_IMPORT_EXPORT void lll_free_assembly_data(lll_assembly_data_t* a_dt){
	a_dt->tm=0;
	a_dt->h=NULL;
	a_dt->ic=0;
	a_dt->vc=0;
	lll_free_assembly_function_table(&(a_dt->ft));
	lll_free_string_table(&(a_dt->st));
	a_dt->_s.ptr=NULL;
	a_dt->_s.off=0;
	a_dt->_s.sz=0;
}



__LLL_IMPORT_EXPORT void lll_free_compilation_data(lll_compilation_data_t* c_dt){
	c_dt->is=NULL;
	c_dt->tm=0;
	c_dt->h=NULL;
	lll_free_identifier_table(&(c_dt->idt));
	lll_free_export_table(&(c_dt->et));
	lll_free_function_table(&(c_dt->ft));
	lll_free_string_table(&(c_dt->st));
	c_dt->_s.ptr=NULL;
	c_dt->_s.off=0;
	c_dt->_s.sz=0;
	c_dt->_n_sc_id=1;
}



__LLL_IMPORT_EXPORT void lll_free_export_table(lll_export_table_t* et){
	if (et->dt){
		free(et->dt);
		et->dt=NULL;
	}
	et->l=0;
}



__LLL_IMPORT_EXPORT void lll_free_function_table(lll_function_table_t* ft){
	for (lll_function_index_t i=0;i<ft->l;i++){
		free(*(ft->dt+i));
	}
	if (ft->dt){
		free(ft->dt);
		ft->dt=NULL;
	}
	ft->l=0;
}



__LLL_IMPORT_EXPORT void lll_free_identifier_table(lll_identifier_table_t* idt){
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		lll_identifier_list_t* e=idt->s+i;
		if (e->dt){
			free(e->dt);
			e->dt=NULL;
		}
		e->l=0;
	}
	if (idt->il){
		free(idt->il);
		idt->il=NULL;
	}
	idt->ill=0;
}



__LLL_IMPORT_EXPORT void lll_free_internal_function_table(lll_internal_function_table_t* ift){
	for (lll_function_index_t i=0;i<ift->l;i++){
		free(*(ift->dt+i));
	}
	if (ift->dt){
		free(ift->dt);
		ift->dt=NULL;
	}
	ift->l=0;
}



__LLL_IMPORT_EXPORT void lll_free_string_table(lll_string_table_t* st){
	for (lll_string_index_t i=0;i<st->l;i++){
		free(*(st->dt+i));
	}
	if (st->dt){
		free(st->dt);
		st->dt=NULL;
	}
	st->l=0;
}

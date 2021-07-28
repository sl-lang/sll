#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdlib.h>



__LLL_IMPORT_EXPORT void lll_free_assembly_data(lll_assembly_data_t* a_dt){
	a_dt->tm=0;
	a_dt->h=NULL;
	a_dt->ic=0;
	a_dt->vc=0;
	lll_free_function_table(&(a_dt->ft));
	lll_free_string_table(&(a_dt->st));
	a_dt->_s.ptr=NULL;
	a_dt->_s.off=0;
	a_dt->_s.sz=0;
}



__LLL_IMPORT_EXPORT void lll_free_compilation_data(lll_compilation_data_t* c_dt){
	c_dt->is=NULL;
	c_dt->tm=0;
	c_dt->h=NULL;
	lll_free_function_data(&(c_dt->f_dt));
	lll_free_identifier_data(&(c_dt->i_dt));
	lll_free_import_data(&(c_dt->im));
	lll_free_string_table(&(c_dt->st));
	c_dt->_s.ptr=NULL;
	c_dt->_s.off=0;
	c_dt->_s.sz=0;
	c_dt->_n_sc_id=1;
}



__LLL_IMPORT_EXPORT void lll_free_identifier_data(lll_identifier_data_t* i_dt){
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		lll_identifier_list_t* e=i_dt->s+i;
		if (e->dt){
			free(e->dt);
			e->dt=NULL;
		}
		e->l=0;
	}
	if (i_dt->il){
		free(i_dt->il);
		i_dt->il=NULL;
	}
	i_dt->ill=0;
}



__LLL_IMPORT_EXPORT void lll_free_function_data(lll_function_data_t* f_dt){
	for (lll_function_index_t i=0;i<f_dt->l;i++){
		free(*(f_dt->dt+i));
	}
	if (f_dt->dt){
		free(f_dt->dt);
		f_dt->dt=NULL;
	}
	f_dt->l=0;
}



__LLL_IMPORT_EXPORT void lll_free_function_table(lll_function_table_t* ft){
	if (ft->dt){
		free(ft->dt);
		ft->dt=NULL;
	}
	ft->l=0;
}



__LLL_IMPORT_EXPORT void lll_free_import_data(lll_import_data_t* im){
	if (im->dt){
		free(im->dt);
		im->dt=NULL;
	}
	im->l=0;
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

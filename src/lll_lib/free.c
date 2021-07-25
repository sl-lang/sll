#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdint.h>
#include <stdlib.h>



__LLL_IMPORT_EXPORT void lll_free_compilation_data(lll_compilation_data_t* c_dt){
	lll_free_file_path_data(&(c_dt->fp_dt));
	c_dt->is=NULL;
	c_dt->tm=0;
	c_dt->h=NULL;
	lll_free_function_data(&(c_dt->f_dt));
	lll_free_identifier_data(&(c_dt->i_dt));
	lll_free_import_data(&(c_dt->im));
	lll_free_string_table(&(c_dt->st));
	c_dt->_n_sc_id=1;
}



__LLL_IMPORT_EXPORT void lll_free_file_path_data(lll_file_path_data_t* fp_dt){
	if (fp_dt->dt){
		free(fp_dt->dt);
		fp_dt->dt=NULL;
	}
	fp_dt->l=0;
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
	if (f_dt->l){
		free(f_dt->dt);
		f_dt->dt=NULL;
	}
	f_dt->l=0;
}



__LLL_IMPORT_EXPORT void lll_free_import_data(lll_import_data_t* im){
	if (im->l){
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

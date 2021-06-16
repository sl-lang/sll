#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>



__LLL_IMPORT_EXPORT void lll_init_compilation_data(char* fp,lll_input_data_stream_t* is,lll_compilation_data_t* o){
	o->fp_dt.dt=malloc(sizeof(lll_file_path_t));
	lll_file_path_t* ifp=o->fp_dt.dt;
	ifp->l=0;
	while (*fp){
		ifp->fp[ifp->l]=*fp;
		ifp->l++;
		fp++;
	}
	ifp->fp[ifp->l]=0;
	o->fp_dt.l=1;
	o->is=is;
	o->tm=(uint64_t)time(NULL);
	o->h=NULL;
	for (uint32_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o->i_dt.s[i].dt=NULL;
		o->i_dt.s[i].l=0;
	}
	o->i_dt.il=NULL;
	o->i_dt.ill=0;
	o->im.dt=NULL;
	o->im.l=0;
	o->_n_sc_id=1;
}



__LLL_IMPORT_EXPORT void lll_free_file_path_data(lll_file_path_data_t* fp_dt){
	if (fp_dt->dt){
		free(fp_dt->dt);
		fp_dt->dt=NULL;
	}
	fp_dt->l=0;
}



__LLL_IMPORT_EXPORT void lll_free_identifier_data(lll_identifier_data_t* i_dt){
	for (uint32_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		lll_identifier_list_t* e=i_dt->s+i;
		for (uint32_t j=0;j<e->l;j++){
			free((e->dt+j)->v);
		}
		if (e->dt){
			free(e->dt);
			e->dt=NULL;
		}
		e->l=0;
	}
	for (uint32_t i=0;i<i_dt->ill;i++){
		free(*(i_dt->il+i));
	}
	if (i_dt->il){
		free(i_dt->il);
		i_dt->il=NULL;
	}
	i_dt->ill=0;
}



__LLL_IMPORT_EXPORT void lll_free_import_data(lll_import_data_t* im){
	for (uint32_t i=0;i<im->l;i++){
		free((im->dt+i)->nm);
	}
	if (im->l){
		free(im->dt);
		im->dt=NULL;
	}
	im->l=0;
}



__LLL_IMPORT_EXPORT void lll_free_function_data(lll_function_data_t* f_dt){
	for (uint32_t i=0;i<f_dt->l;i++){
		free(*(f_dt->dt+i));
	}
	if (f_dt->l){
		free(f_dt->dt);
		f_dt->dt=NULL;
	}
	f_dt->l=0;
}

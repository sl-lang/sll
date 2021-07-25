#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>



__LLL_IMPORT_EXPORT void lll_init_compilation_data(const char* fp,lll_input_data_stream_t* is,lll_compilation_data_t* o){
	o->fp_dt.dt=malloc(sizeof(lll_string_index_t));
	o->fp_dt.l=1;
	o->is=is;
	o->tm=(lll_time_t)time(NULL);
	o->h=NULL;
	for (uint8_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o->i_dt.s[i].dt=NULL;
		o->i_dt.s[i].l=0;
	}
	o->i_dt.il=NULL;
	o->i_dt.ill=0;
	o->im.dt=NULL;
	o->im.l=0;
	o->f_dt.dt=NULL;
	o->f_dt.l=0;
	o->st.dt=NULL;
	o->st.l=0;
	o->_n_sc_id=1;
	lll_string_length_t l=0;
	while (*(fp+l)){
		l++;
	}
	*(o->fp_dt.dt)=_create_string(o,fp,l);
}

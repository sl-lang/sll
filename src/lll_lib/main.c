#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#endif
#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>



uint8_t* _bf=NULL;
uint32_t _bf_off;
uint32_t _bf_sz;



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_set_internal_stack(uint8_t* bf,uint32_t sz,lll_error_t* e){
	_bf=bf;
	_bf_off=0;
	_bf_sz=sz;
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT void lll_init_compilation_data(const char* fp,lll_input_data_stream_t* is,lll_compilation_data_t* o){
	o->fpl=0;
	while (*fp){
		o->fp[o->fpl]=*fp;
		o->fpl++;
		fp++;
	}
	o->fp[o->fpl]=0;
	o->_n_sc_id=1;
	o->is=is;
	o->tm=(uint64_t)time(NULL);
	o->h=NULL;
	for (uint32_t i=0;i<LLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o->i_dt.s[i].dt=NULL;
		o->i_dt.s[i].l=0;
	}
	o->i_dt.il=NULL;
	o->i_dt.ill=0;
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



#ifdef _MSC_VER
BOOL WINAPI DllMain(HINSTANCE h,DWORD r,LPVOID rs){
	return TRUE;
}
#endif

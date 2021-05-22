#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>



uint8_t* _bf=NULL;
uint32_t _bf_off;
uint32_t _bf_sz;



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_set_internal_stack(uint8_t* bf,uint32_t sz,lll_error_t* e){
	if (sz>=LLL_MAX_INTERNAL_STACK_SIZE){
		e->t=LLL_ERROR_STACK_TOO_BIG;
		return LLL_RETURN_ERROR;
	}
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
	o->is=is;
	o->h=NULL;
}



#ifdef _MSC_VER
BOOL WINAPI DllMain(HINSTANCE h,DWORD r,LPVOID rs){
	return TRUE;
}
#endif

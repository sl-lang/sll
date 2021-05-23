#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <intrin.h>
#endif
#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>
#include <time.h>



#ifdef _MSC_VER
#pragma intrinsic(__stosb)
#define REPEAT_BYTE_SET(d,v,sz) __stosb(d,v,sz)
#else
static inline void REPEAT_BYTE_SET(unsigned char* d,uint8_t v,size_t n){
	__asm__("rep stosb":"=D"(d),"=A"(v),"=c"(n):"0"(d),"1"(v),"2"(n):"memory");
}
#endif



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
	o->is=is;
	o->tm=(uint64_t)time(NULL);
	o->h=NULL;
	REPEAT_BYTE_SET((uint8_t*)(&(o->i_dt)),0,sizeof(lll_identifier_data_t));
	o->_mx_v=0;
}



#ifdef _MSC_VER
BOOL WINAPI DllMain(HINSTANCE h,DWORD r,LPVOID rs){
	return TRUE;
}
#endif

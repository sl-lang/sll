#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#undef IGNORE
#else
#include <time.h>
#endif
#include <lll_lib.h>
#include <_lll_lib_internal.h>



__LLL_IMPORT_EXPORT void lll_api_get_time(lll_runtime_object_t* o,lll_arg_count_t ac,lll_runtime_object_t* a){
	o->t=LLL_RUNTIME_OBJECT_TYPE_INT;
#ifdef _MSC_VER
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	o->dt.i=((((uint64_t)ft.dwHighDateTime)<<32)|ft.dwLowDateTime)*100-11644473600000000000;
#else
	struct timespec tm;
	clock_gettime(CLOCK_REALTIME,&tm);
	o->dt.i=tm.tv_sec*1000000000+tm.tv_nsec;
#endif
}

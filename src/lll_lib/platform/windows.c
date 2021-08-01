#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#undef IGNORE
#include <lll/common.h>
#include <lll/types.h>



__LLL_FUNC lll_time_t lll_platform_get_current_time(void){
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return ((((lll_time_t)ft.dwHighDateTime)<<32)|ft.dwLowDateTime)*100-11644473600000000000;
}

#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <lll/common.h>
#include <lll/types.h>



const char* lll_platform_string="windows";



__LLL_FUNC lll_time_t lll_platform_get_current_time(void){
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return ((((lll_time_t)ft.dwHighDateTime)<<32)|ft.dwLowDateTime)*100-11644473600000000000;
}



__LLL_FUNC void lll_platform_setup_console(void){
	SetConsoleOutputCP(CP_UTF8);
	DWORD v;
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(h,&v);
	SetConsoleMode(h,v|ENABLE_PROCESSED_OUTPUT|ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	h=GetStdHandle(STD_ERROR_HANDLE);
	GetConsoleMode(h,&v);
	SetConsoleMode(h,v|ENABLE_PROCESSED_OUTPUT|ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

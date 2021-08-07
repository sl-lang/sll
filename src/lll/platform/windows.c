#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <lll/common.h>
#include <lll/platform.h>
#include <lll/types.h>



const char* lll_platform_string="windows";



__LLL_FUNC void* lll_platform_allocate_page(lll_page_size_t sz){
	return VirtualAlloc(NULL,sz,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
}



__LLL_FUNC void lll_platform_free_page(void* pg){
	VirtualFree(pg,0,MEM_RELEASE);
}



__LLL_FUNC lll_time_t lll_platform_get_current_time(void){
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return ((((lll_time_t)ft.dwHighDateTime)<<32)|ft.dwLowDateTime)*100-11644473600000000000;
}



__LLL_FUNC lll_page_size_t lll_platform_get_page_size(void){
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwPageSize;
}



__LLL_FUNC lll_buffer_size_t lll_platform_path_absolute(const char* fp,lll_buffer_t bf,lll_buffer_size_t bfl){
	return (lll_buffer_size_t)GetFullPathNameA(fp,bfl,bf,NULL);
}



__LLL_FUNC lll_bool_t lll_platform_path_exists(const char* fp){
	return (GetFileAttributesA(fp)!=INVALID_FILE_ATTRIBUTES);
}



__LLL_FUNC lll_bool_t lll_platform_path_is_directory(const char* fp){
	DWORD a=GetFileAttributesA(fp);
	return (a==INVALID_FILE_ATTRIBUTES?0:!!(a&FILE_ATTRIBUTE_DIRECTORY));
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

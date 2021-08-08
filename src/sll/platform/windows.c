#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <sll/common.h>
#include <sll/platform.h>
#include <sll/types.h>



const char* sll_platform_string="windows";



__SLL_FUNC void* sll_platform_allocate_page(sll_page_size_t sz){
	return VirtualAlloc(NULL,sz,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
}



__SLL_FUNC void sll_platform_free_page(void* pg,sll_page_size_t sz){
	VirtualFree(pg,0,MEM_RELEASE);
}



__SLL_FUNC sll_time_t sll_platform_get_current_time(void){
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return ((((sll_time_t)ft.dwHighDateTime)<<32)|ft.dwLowDateTime)*100-11644473600000000000;
}



__SLL_FUNC sll_page_size_t sll_platform_get_page_size(void){
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwPageSize;
}



__SLL_FUNC sll_buffer_size_t sll_platform_path_absolute(const char* fp,sll_buffer_t bf,sll_buffer_size_t bfl){
	return (sll_buffer_size_t)GetFullPathNameA(fp,bfl,bf,NULL);
}



__SLL_FUNC sll_bool_t sll_platform_path_exists(const char* fp){
	return (GetFileAttributesA(fp)!=INVALID_FILE_ATTRIBUTES);
}



__SLL_FUNC sll_bool_t sll_platform_path_is_directory(const char* fp){
	DWORD a=GetFileAttributesA(fp);
	return (a==INVALID_FILE_ATTRIBUTES?0:!!(a&FILE_ATTRIBUTE_DIRECTORY));
}



__SLL_FUNC void sll_platform_setup_console(void){
	SetConsoleOutputCP(CP_UTF8);
	DWORD v;
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(h,&v);
	SetConsoleMode(h,v|ENABLE_PROCESSED_OUTPUT|ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	h=GetStdHandle(STD_ERROR_HANDLE);
	GetConsoleMode(h,&v);
	SetConsoleMode(h,v|ENABLE_PROCESSED_OUTPUT|ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

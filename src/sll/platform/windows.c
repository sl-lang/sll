#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <sll/common.h>
#include <sll/types.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>



const char* sll_platform_string="windows";



static HANDLE _win_wh=INVALID_HANDLE_VALUE;
static int _win_stdout_m=0;
static int _win_stderr_m=0;



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
	return (a!=INVALID_FILE_ATTRIBUTES&&(a&FILE_ATTRIBUTE_DIRECTORY));
}



__SLL_FUNC void sll_platform_sleep(sll_time_t tm){
	if (_win_wh==INVALID_HANDLE_VALUE){
		_win_wh=CreateEvent(NULL,TRUE,FALSE,FALSE);
	}
	sll_time_t e=GetTickCount64()*1000000+tm;
	while (1){
		ResetEvent(_win_wh);
		if (WaitForSingleObjectEx(_win_wh,(DWORD)((tm+999999)/1000000),FALSE)!=WAIT_OBJECT_0){
			return;
		}
		sll_time_t c=GetTickCount64()*1000000;
		if (c>=e){
			break;
		}
		tm=c-e;
	}
}



__SLL_FUNC void sll_platform_setup_console(void){
	_win_stdout_m=_setmode(_fileno(stdout),_O_BINARY);
	_win_stderr_m=_setmode(_fileno(stderr),_O_BINARY);
}



__SLL_FUNC void sll_platform_reset_console(void){
	_setmode(_fileno(stdout),_win_stdout_m);
	_setmode(_fileno(stderr),_win_stderr_m);
}

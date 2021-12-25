#include <windows.h>
#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <signal.h>



static HANDLE _win_wh=INVALID_HANDLE_VALUE;



void* _win_dll_handle=NULL;



__SLL_EXTERNAL const sll_char_t* sll_platform_string=SLL_CHAR("windows");



static void _release_handle(void){
	CloseHandle(_win_wh);
	_win_wh=INVALID_HANDLE_VALUE;
}



__SLL_NO_RETURN void _force_exit_platform(void){
	TerminateProcess(GetCurrentProcess(),SIGABRT);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_time_t sll_platform_get_current_time(void){
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return ((((sll_time_t)ft.dwHighDateTime)<<32)|ft.dwLowDateTime)*100-11644473600000000000;
}



__SLL_EXTERNAL void sll_platform_sleep(sll_time_t tm){
	if (_win_wh==INVALID_HANDLE_VALUE){
		_win_wh=CreateEventA(NULL,TRUE,FALSE,FALSE);
		sll_register_cleanup(_release_handle);
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



BOOL WINAPI DllMain(HINSTANCE dll_h,DWORD t,LPVOID r){
	if (!_win_dll_handle){
		_win_dll_handle=dll_h;
	}
	return TRUE;
}

#include <windows.h>
#include <sll/_sll_internal.h>
#include <sll/api/process.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>



__SLL_EXTERNAL void sll_platform_close_process_handle(sll_process_handle_t ph){
	CloseHandle(ph);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_execute_shell(const sll_char_t* cmd){
	sll_string_t s;
	sll_string_format(SLL_CHAR("C:\\Windows\\System32\\cmd.exe /q /c %s"),&s,cmd);
	STARTUPINFOA si={0};
	si.cb=sizeof(STARTUPINFOA);
	si.dwFlags=0;
	PROCESS_INFORMATION pi;
	if (!CreateProcessA("C:\\Windows\\System32\\cmd.exe",s.v,NULL,NULL,TRUE,CREATE_NEW_PROCESS_GROUP,NULL,NULL,&si,&pi)){
		sll_free_string(&s);
		return -1;
	}
	sll_free_string(&s);
	CloseHandle(pi.hThread);
	DWORD ec;
	if (WaitForSingleObject(pi.hProcess,INFINITE)!=WAIT_OBJECT_0||!GetExitCodeProcess(pi.hProcess,&ec)){
		return 0;
	}
	CloseHandle(pi.hProcess);
	return !!ec;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_pid_t sll_platform_get_pid(void){
	return GetCurrentProcessId();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_process_handle_t sll_platform_start_process(const sll_char_t*const* a,const sll_char_t*const* env){
	if (!a||!(*a)){
		return 0;
	}
	sll_string_t s;
	sll_process_join_args(a,&s);
	STARTUPINFOA si={0};
	si.cb=sizeof(STARTUPINFOA);
	si.dwFlags=0;
	PROCESS_INFORMATION pi;
	if (!CreateProcessA((char*)(*a),s.v,NULL,NULL,TRUE,CREATE_NEW_PROCESS_GROUP,PTR(env),NULL,&si,&pi)){
		sll_free_string(&s);
		return 0;
	}
	sll_free_string(&s);
	CloseHandle(pi.hThread);
	return pi.hProcess;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_platform_wait_process(sll_process_handle_t ph){
	DWORD ec;
	if (WaitForSingleObject(ph,INFINITE)!=WAIT_OBJECT_0||!GetExitCodeProcess(ph,&ec)){
		return -1;
	}
	return ec;
}


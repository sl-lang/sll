#include <windows.h>
#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>



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
	DWORD ec;
	if (WaitForSingleObject(pi.hProcess,INFINITE)!=WAIT_OBJECT_0||!GetExitCodeProcess(pi.hProcess,&ec)){
		return 0;
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return !!ec;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_pid_t sll_platform_get_pid(void){
	return GetCurrentProcessId();
}

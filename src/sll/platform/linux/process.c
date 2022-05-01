#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/platform/process.h>
#include <sll/types.h>
#include <sys/wait.h>
#include <unistd.h>



extern char** environ;



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_close_process_handle(sll_process_handle_t ph){
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_execute_shell(const sll_char_t* cmd,sll_error_t* err){
	ERROR_PTR_RESET;
	pid_t f=fork();
	if (f==-1){
		ERROR_PTR_SYSTEM;
		return 0;
	}
	if (!f){
		execl("/bin/sh","sh","-c",cmd,NULL);
		ERROR_PTR_SYSTEM;
		_exit(1);
	}
	int st;
	if (waitpid(f,&st,0)==-1){
		ERROR_PTR_SYSTEM;
		return 0;
	}
	return (WIFEXITED(st)&&!WEXITSTATUS(st));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_pid_t sll_platform_get_pid(void){
	return getpid();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_process_handle_t sll_platform_start_process(const sll_char_t*const* a,const sll_char_t*const* env,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!a||!(*a)){
		return 0;
	}
	if (!env){
		env=(const sll_char_t*const*)environ;
	}
	pid_t f=fork();
	if (f==-1){
		ERROR_PTR_SYSTEM;
		return 0;
	}
	if (!f){
		execve((char*)(*a),(char*const*)a,(char*const*)env);
		ERROR_PTR_SYSTEM;
		_exit(1);
	}
	return PTR(f);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_platform_wait_process(sll_process_handle_t pid,sll_error_t* err){
	ERROR_PTR_RESET;
	int st;
	if (waitpid((pid_t)ADDR(pid),&st,0)==-1){
		ERROR_PTR_SYSTEM;
		return 0;
	}
	return (WIFEXITED(st)?WEXITSTATUS(st):1);
}

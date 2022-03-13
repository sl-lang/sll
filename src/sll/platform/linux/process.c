#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/types.h>
#include <sys/wait.h>
#include <unistd.h>



extern char** environ;



static __SLL_NO_RETURN void _start_child(const char*const* a,const char*const* env){
	execve(*a,(char*const*)a,(char*const*)env);
	_exit(1);
}



__SLL_EXTERNAL void sll_platform_close_process_handle(sll_process_handle_t ph){
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_execute_shell(const sll_char_t* cmd){
	pid_t f=fork();
	if (f==-1){
		return 0;
	}
	if (!f){
		execl("/bin/sh","sh","-c",cmd,NULL);
		_exit(1);
	}
	int st;
	if (waitpid(f,&st,0)==-1){
		return 0;
	}
	return (WIFEXITED(st)&&!WEXITSTATUS(st));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_pid_t sll_platform_get_pid(void){
	return getpid();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_pid_t sll_platform_start_process(const sll_char_t*const* a,const sll_char_t*const* env){
	if (!a||!(*a)){
		return 0;
	}
	if (!env){
		env=(const sll_char_t*const*)environ;
	}
	pid_t f=fork();
	if (f==-1){
		return 0;
	}
	if (!f){
		_start_child((const char*const*)a,(const char*const*)env);
	}
	return f;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_return_code_t sll_platform_wait_process(sll_pid_t pid){
	int st;
	if (waitpid(pid,&st,0)==-1){
		return 0;
	}
	return (WIFEXITED(st)?WEXITSTATUS(st):1);
}

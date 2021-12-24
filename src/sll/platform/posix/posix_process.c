#include <sll/common.h>
#include <sll/types.h>
#include <sys/wait.h>
#include <unistd.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_platform_execute_shell(const sll_char_t* cmd){
	pid_t f=vfork();
	if (f==-1){
		return 0;
	}
	if (!f){
		if(!execl("/bin/sh","sh","-c",cmd,NULL)){
			_exit(1);
		}
	}
	int st;
	if (waitpid(-1,&st,0)==-1){
		return 0;
	}
	return (WIFEXITED(st)&&!WEXITSTATUS(st));
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_pid_t sll_platform_get_pid(void){
	return getpid();
}

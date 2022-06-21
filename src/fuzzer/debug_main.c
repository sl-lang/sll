#include <sll.h>
#include <unistd.h>



int __lsan_is_turned_off(void){
	return 1;
}



const char* __asan_default_options(void){
	return "handle_abort=1:log_path=stderr";
}



int main(int argc,const char*const* argv){
	char cwd[SLL_API_MAX_FILE_PATH_LENGTH];
	getcwd(cwd,SLL_API_MAX_FILE_PATH_LENGTH);
	argv++;
	argc--;
	while (argc){
		const sll_char_t* args[2]={
			SLL_CHAR("-s"),
			SLL_CHAR(*argv)
		};
		(void)sll_cli_main(2,args);
		argv++;
		argc--;
		chdir(cwd);
	}
	return 0;
}

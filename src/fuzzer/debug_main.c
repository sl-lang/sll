#include <sll.h>



int __lsan_is_turned_off(void){
	return 1;
}



const char* __asan_default_options(void){
	return "handle_abort=1:log_path=stderr";
}



int main(int argc,const char*const* argv){
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
	}
	return 0;
}

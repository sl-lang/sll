#include <sll.h>



#define MAX_INPUT_LENGTH 16384



__attribute__((used)) int __lsan_is_turned_off(void){
	return 1;
}



__attribute__((used)) const char* __asan_default_options(void){
	return "log_path=stderr:handle_abort=1";
}



int LLVMFuzzerTestOneInput(const sll_char_t* data,size_t size){
	sll_char_t bf[MAX_INPUT_LENGTH];
	size=(size>MAX_INPUT_LENGTH-1?MAX_INPUT_LENGTH-1:size);
	sll_copy_data(data,size,bf);
	bf[size]=0;
	const sll_char_t* args[2]={
		SLL_CHAR("-s"),
		bf
	};
	(void)sll_cli_main(2,args);
	return 0;
}

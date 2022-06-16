#include <sll.h>
#include <stdio.h>



#define MAX_INPUT_LENGTH 16384



const char* __asan_default_options(void){
	return "halt_on_error=0:log_path=build/fuzzer_output/asan:report_globals=0";
}



int LLVMFuzzerTestOneInput(const sll_char_t* data,size_t size){
	if (!size){
		return 0;
	}
	sll_char_t bf[MAX_INPUT_LENGTH+1];
	size=(size>MAX_INPUT_LENGTH?MAX_INPUT_LENGTH:size);
	sll_copy_data(data,size,bf);
	bf[size]=0;
	const sll_char_t* args[2]={
		SLL_CHAR("-s"),
		bf
	};
	(void)sll_cli_main(2,args);
	return 0;
}

#include <dlfcn.h>
#include <fcntl.h>
#include <sll.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



#define MAX_INPUT_LENGTH 16384



extern size_t __asan_get_report_access_size(void);
extern int __asan_get_report_access_type(void);
extern void* __asan_get_report_address(void);
extern const char* __asan_get_report_description(void);
extern void* __asan_region_is_poisoned(void*,size_t);
extern void __asan_set_error_report_callback(void (*)(const char*));



static void _process_error(const char* err_str){
	if (!__asan_get_report_access_type()&&__asan_get_report_access_size()==8&&(!strcmp(__asan_get_report_description(),"global-buffer-overflow")||!strcmp(__asan_get_report_description(),"heap-buffer-overflow"))&&(!__asan_region_is_poisoned(__asan_get_report_address(),1)||!__asan_region_is_poisoned(((char*)__asan_get_report_address())-1,1))){
		return;
	}
	char fp[128];
	snprintf(fp,128,"build/fuzzer_output/asan_%u.txt",getpid());
	FILE* fh=fopen(fp,"a");
	fputs(err_str,fh);
	fputc('\n',fh);
	fclose(fh);
}



const char* __asan_default_options(void){
	return "halt_on_error=0";
}



int __lsan_is_turned_off(void){
	return 1;
}



int LLVMFuzzerInitialize(int* argc,char*** argv){
	__asan_set_error_report_callback(_process_error);
	return 0;
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

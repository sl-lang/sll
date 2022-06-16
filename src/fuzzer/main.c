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
extern void* __asan_get_report_pc(void);
extern const char* __asan_locate_address(void*,char*,size_t,void**,size_t*);
extern void* __asan_region_is_poisoned(void*,size_t);
extern void __asan_set_error_report_callback(void (*)(const char*));
extern void __sanitizer_set_report_fd(void*);
extern int __sanitizer_get_module_and_offset_for_pc(void*,char*,size_t,void**);



static int dev_null_fd=-1;
static FILE* asan_output_file=NULL;



static void _close_fds(void){
	close(dev_null_fd);
	dev_null_fd=-1;
	if (!asan_output_file){
		return;
	}
	fclose(asan_output_file);
	asan_output_file=NULL;
}



static void _process_error(const char* err_str){
	if (!__asan_get_report_access_type()&&__asan_get_report_access_size()==8&&!strcmp(__asan_get_report_description(),"global-buffer-overflow")&&(!__asan_region_is_poisoned(__asan_get_report_address(),1)||!__asan_region_is_poisoned(((char*)__asan_get_report_address())-1,1))){
		return;
	}
	if (!strcmp(__asan_get_report_description(),"stack-buffer-underflow")||!strcmp(__asan_get_report_description(),"unknown-crash")){
		Dl_info info;
		if (dladdr(__asan_get_report_pc(),&info)&&info.dli_sname&&!strcmp(info.dli_sname,"_execute_wrapper")){
			return;
		}
	}
	if (!asan_output_file){
		char fp[128];
		snprintf(fp,128,"build/fuzzer_output/asan_%u.txt",getpid());
		asan_output_file=fopen(fp,"w");
	}
	fputs(err_str,asan_output_file);
	fputc('\n',asan_output_file);
	fflush(asan_output_file);
}



const char* __asan_default_options(void){
	return "halt_on_error=0:log_path=/dev/null";
}



int LLVMFuzzerTestOneInput(const sll_char_t* data,size_t size){
	if (!size){
		return 0;
	}
	if (dev_null_fd==-1){
		dev_null_fd=open("/dev/null",O_WRONLY,S_IRWXU|S_IRWXG|S_IRWXO);
		atexit(_close_fds);
		__sanitizer_set_report_fd((void*)(intptr_t)dev_null_fd);
		__asan_set_error_report_callback(_process_error);
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

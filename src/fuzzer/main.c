#include <sll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



#define MAX_INPUT_LENGTH 16384



extern void __asan_set_error_report_callback(void (*)(const char*));
extern void __sll_fuzzer_set_fd(int);



static const char* _base64_alphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static FILE* _output_file;
static char _output_file_path[SLL_API_MAX_FILE_PATH_LENGTH];
static sll_bool_t _has_error=0;



static void _cleanup_data(void){
	if (!_has_error){
		unlink(_output_file_path);
	}
}



static void _write_error(const char* error){
	fputs(error,_output_file);
	fflush(_output_file);
	_has_error=1;
}



static void _write_input(const sll_char_t* data,sll_size_t size){
	fprintf(_output_file,"$$$$$ [%llu]: ",size);
	sll_string_length_t i=0;
	if (size>2){
		do{
			char bf[4]={
				_base64_alphabet[data[i]>>2],
				_base64_alphabet[((data[i]<<4)&0x3f)|(data[i+1]>>4)],
				_base64_alphabet[((data[i+1]<<2)&0x3f)|(data[i+2]>>6)],
				_base64_alphabet[data[i+2]&0x3f]
			};
			fwrite(bf,4,1,_output_file);
			i+=3;
		} while (i<size-2);
	}
	sll_string_length_t p=(3-(size%3))%3;
	if (p==1){
		fprintf(_output_file,"%c%c%c=\n",_base64_alphabet[data[i]>>2],_base64_alphabet[((data[i]<<4)&0x3f)|(data[i+1]>>4)],_base64_alphabet[(data[i+1]<<2)&0x3f]);
	}
	else if (p==2){
		fprintf(_output_file,"%c%c==\n",_base64_alphabet[data[i]>>2],_base64_alphabet[((data[i]<<4)&0x3f)]);
	}
	else{
		fputc('\n',_output_file);
	}
	fflush(_output_file);
}



int __lsan_is_turned_off(void){
	return 1;
}



const char* __asan_default_options(void){
	return "handle_abort=1";
}



int LLVMFuzzerInitialize(int* argc,const char*** argv){
	snprintf(_output_file_path,SLL_API_MAX_FILE_PATH_LENGTH,"build/fuzzer_output/%u",getpid());
	_output_file=fopen(_output_file_path,"a");
	__asan_set_error_report_callback(_write_error);
	__sll_fuzzer_set_fd(fileno(_output_file));
	atexit(_cleanup_data);
	return 0;
}



int LLVMFuzzerTestOneInput(const sll_char_t* data,sll_size_t size){
	if (!size){
		return 0;
	}
	_write_input(data,size);
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

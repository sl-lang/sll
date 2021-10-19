#include <sll/common.h>
#include <sll/stream.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>



static void _output_data_stream_file_write_char(sll_output_data_stream_t* os,char c){
	fputc(c,(FILE*)(os->ctx));
}



static void _output_data_stream_file_write_string(sll_output_data_stream_t* os,const char* s){
	while (*s){
		fputc(*s,(FILE*)(os->ctx));
		s++;
	}
}



static void _output_data_stream_file_write(sll_output_data_stream_t* os,const sll_char_t* bf,sll_string_length_t sz){
	fwrite(bf,sizeof(uint8_t),sz,(FILE*)(os->ctx));
}



__SLL_FUNC void sll_stream_create_output_from_file(FILE* f,sll_output_data_stream_t* o){
	o->ctx=f;
	o->wcf=_output_data_stream_file_write_char;
	o->wsf=_output_data_stream_file_write_string;
	o->wf=_output_data_stream_file_write;
}

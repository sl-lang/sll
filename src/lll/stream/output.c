#include <lll/common.h>
#include <lll/stream.h>
#include <lll/types.h>
#include <stdint.h>
#include <stdio.h>



void _output_data_stream_file_write_char(lll_output_data_stream_t* os,char c){
	fputc(c,(FILE*)(os->ctx));
}



void _output_data_stream_file_write_string(lll_output_data_stream_t* os,const char* s){
	while (*s){
		fputc(*s,(FILE*)(os->ctx));
		s++;
	}
}



void _output_data_stream_file_write(lll_output_data_stream_t* os,lll_const_buffer_t bf,lll_buffer_size_t sz){
	fwrite(bf,sizeof(uint8_t),sz,(FILE*)(os->ctx));
}



__LLL_FUNC void lll_stream_create_output_from_file(FILE* f,lll_output_data_stream_t* o){
	o->ctx=f;
	o->wcf=_output_data_stream_file_write_char;
	o->wsf=_output_data_stream_file_write_string;
	o->wf=_output_data_stream_file_write;
}

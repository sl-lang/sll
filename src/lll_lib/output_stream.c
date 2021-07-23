#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>
#include <stdio.h>



uint8_t _output_data_stream_file_write_char(const lll_output_data_stream_t* os,char c){
	fputc(c,(FILE*)(os->ctx));
	return 1;
}



uint8_t _output_data_stream_file_write_string(const lll_output_data_stream_t* os,const char* s){
	while (*s){
		fputc(*s,(FILE*)(os->ctx));
		s++;
	}
	return 1;
}



uint8_t _output_data_stream_file_write(const lll_output_data_stream_t* os,const uint8_t* bf,size_t sz){
	return (fwrite(bf,sizeof(uint8_t),sz,(FILE*)(os->ctx))==sz);
}



__LLL_IMPORT_EXPORT void lll_create_output_data_stream(FILE* f,lll_output_data_stream_t* o){
	o->ctx=f;
	o->wcf=_output_data_stream_file_write_char;
	o->wsf=_output_data_stream_file_write_string;
	o->wf=_output_data_stream_file_write;
}

#include <sll/common.h>
#include <sll/stream.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>



static void _output_data_stream_file_write(sll_output_data_stream_t* os,sll_char_t c){
	fputc(c,(FILE*)(os->ctx));
}



static void _output_data_stream_file_write_buffer(sll_output_data_stream_t* os,const sll_char_t* bf,sll_string_length_t l){
	fwrite(bf,sizeof(uint8_t),l,(FILE*)(os->ctx));
}



__SLL_FUNC void sll_stream_create_output_from_file(FILE* f,sll_output_data_stream_t* o){
	o->ctx=f;
	o->wcf=_output_data_stream_file_write;
	o->wf=_output_data_stream_file_write_buffer;
}



__SLL_FUNC void sll_stream_write(sll_output_data_stream_t* os,sll_char_t c){
	SLL_WRITE_CHAR_TO_OUTPUT_DATA_STREAM(os,c);
}



__SLL_FUNC void sll_stream_write_buffer(sll_output_data_stream_t* os,const sll_char_t* bf,sll_string_length_t l){
	SLL_WRITE_TO_OUTPUT_DATA_STREAM(os,bf,l);
}

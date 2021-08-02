#include <lll/common.h>
#include <lll/stream.h>
#include <lll/types.h>
#include <stdint.h>
#include <stdio.h>



lll_small_char_t _input_data_stream_file_read(lll_input_data_stream_t* is){
	int o=fgetc((FILE*)(is->ctx));
	if (o==EOF){
		return LLL_END_OF_DATA;
	}
	is->_off++;
	if (o=='\n'){
		is->_lc++;
		is->_loff=is->_off;
	}
	return (lll_small_char_t)o;
}



lll_small_char_t _input_data_stream_file_read_buffer(lll_input_data_stream_t* is,lll_buffer_t bf,lll_buffer_size_t sz){
	return (fread(bf,sizeof(uint8_t),sz,(FILE*)(is->ctx))==sz?0:LLL_END_OF_DATA);
}



void _input_data_stream_file_restart_line(lll_input_data_stream_t* is,lll_file_offset_t lp){
	FILE* f=(FILE*)(is->ctx);
	fseek(f,lp,SEEK_SET);
	int c=fgetc(f);
	while (lp&&c!='\n'&&c!='\r'){
		lp--;
		fseek(f,lp,SEEK_SET);
		c=fgetc(f);
	}
	if (c!='\n'&&c!='\r'){
		fseek(f,lp,SEEK_SET);
	}
	else{
		lp++;
	}
	is->_off=lp;
}



__LLL_FUNC void lll_stream_create_input_from_file(FILE* f,lll_input_data_stream_t* o){
	fseek(f,0,SEEK_SET);
	clearerr(f);
	o->ctx=f;
	o->rf=_input_data_stream_file_read;
	o->rbf=_input_data_stream_file_read_buffer;
	o->rlf=_input_data_stream_file_restart_line;
	o->_lc=0;
	o->_off=0;
	o->_loff=0;
}

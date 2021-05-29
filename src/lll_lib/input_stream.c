#include <lll_lib.h>
#include <_lll_internal.h>
#include <stdint.h>
#include <stdio.h>



int _input_data_stream_file_read(lll_input_data_stream_t* is){
	int o=fgetc((FILE*)(is->ctx));
	if (o==EOF){
		return LLL_END_OF_DATA;
	}
	is->_off++;
	if (o=='\n'){
		is->_lc++;
		is->_loff=is->_off;
	}
	return o;
}



uint8_t _input_data_stream_file_read_buffer(lll_input_data_stream_t* is,uint8_t* bf,uint32_t sz){
	return (fread(bf,sizeof(uint8_t),sz,(FILE*)(is->ctx))==sz);
}



void _input_data_stream_file_restart_line(lll_input_data_stream_t* is,uint32_t lp){
	FILE* f=(FILE*)(is->ctx);
	fseek(f,lp,SEEK_SET);
	int c=fgetc(f);
	while (lp&&c!='\n'&&c!='\r'){
		lp--;
		fseek(f,lp,SEEK_SET);
		c=fgetc(f);
	}
	is->_off=lp;
}



__LLL_IMPORT_EXPORT void lll_create_input_data_stream(FILE* f,lll_input_data_stream_t* o){
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

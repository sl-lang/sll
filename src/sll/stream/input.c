#include <sll/common.h>
#include <sll/stream.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>



static sll_read_char_t _input_data_stream_file_read_char(sll_input_data_stream_t* is){
	int o=fgetc((FILE*)(is->ctx));
	if (o==EOF){
		return SLL_END_OF_DATA;
	}
	is->_off++;
	if (o=='\n'){
		is->_lc++;
		is->_loff=is->_off;
	}
	return (sll_read_char_t)o;
}



static sll_read_char_t _input_data_stream_file_read_buffer(sll_input_data_stream_t* is,sll_char_t* bf,sll_string_length_t l){
	return (fread(bf,sizeof(uint8_t),l,(FILE*)(is->ctx))==l?0:SLL_END_OF_DATA);
}



static void _input_data_stream_file_restart_line(sll_input_data_stream_t* is,sll_file_offset_t lp){
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



static sll_read_char_t _input_data_stream_buffer_read_char(sll_input_data_stream_t* is){
	sll_input_buffer_t* bf=(sll_input_buffer_t*)(is->ctx);
	if (is->_off>=bf->sz){
		return SLL_END_OF_DATA;
	}
	uint8_t o=*(bf->bf+is->_off);
	is->_off++;
	if (o=='\n'){
		is->_lc++;
		is->_loff=is->_off;
	}
	return o;
}



static sll_read_char_t _input_data_stream_buffer_read_buffer(sll_input_data_stream_t* is,sll_char_t* bf,sll_string_length_t l){
	sll_input_buffer_t* is_bf=(sll_input_buffer_t*)(is->ctx);
	if (is->_off+l>=is_bf->sz){
		return SLL_END_OF_DATA;
	}
	for (sll_string_length_t i=0;i<l;i++){
		*(bf+i)=*(is_bf->bf+i+is->_off);
	}
	is->_off+=l;
	return 0;
}



static void _input_data_stream_buffer_restart_line(sll_input_data_stream_t* is,sll_file_offset_t lp){
	sll_input_buffer_t* bf=(sll_input_buffer_t*)(is->ctx);
	while (lp&&*(bf->bf+lp)!='\n'&&*(bf->bf+lp)!='\r'){
		lp--;
	}
	if (*(bf->bf+lp)=='\n'||*(bf->bf+lp)=='\r'){
		lp++;
	}
	is->_off=lp;
}



__SLL_FUNC void sll_stream_create_input_from_file(FILE* f,sll_input_data_stream_t* o){
	fseek(f,0,SEEK_SET);
	clearerr(f);
	o->ctx=f;
	o->rf=_input_data_stream_file_read_char;
	o->rbf=_input_data_stream_file_read_buffer;
	o->rlf=_input_data_stream_file_restart_line;
	o->_lc=0;
	o->_off=0;
	o->_loff=0;
}



__SLL_FUNC void sll_stream_create_input_from_buffer(sll_input_buffer_t* bf,sll_input_data_stream_t* o){
	o->ctx=bf;
	o->rf=_input_data_stream_buffer_read_char;
	o->rbf=_input_data_stream_buffer_read_buffer;
	o->rlf=_input_data_stream_buffer_restart_line;
	o->_lc=0;
	o->_off=0;
	o->_loff=0;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_read_char_t sll_stream_read(sll_input_data_stream_t* is){
	return SLL_READ_FROM_INPUT_DATA_STREAM(is);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_read_char_t sll_stream_read_buffer(sll_input_data_stream_t* is,sll_char_t* bf,sll_string_length_t l){
	return SLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,bf,l);
}



__SLL_FUNC void sll_stream_restart_line(sll_input_data_stream_t* is,sll_file_offset_t lp){
	SLL_INPUT_DATA_STREAM_RESTART_LINE(is,lp);
}

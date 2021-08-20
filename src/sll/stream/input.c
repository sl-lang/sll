#include <sll/common.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>



sll_read_char_t _input_data_stream_file_read(sll_input_data_stream_t* restrict is){
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



sll_read_char_t _input_data_stream_file_read_buffer(sll_input_data_stream_t* restrict is,sll_buffer_t restrict bf,sll_buffer_size_t sz){
	return (fread(bf,sizeof(uint8_t),sz,(FILE*)(is->ctx))==sz?0:SLL_END_OF_DATA);
}



void _input_data_stream_file_restart_line(sll_input_data_stream_t* restrict is,sll_file_offset_t lp){
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



sll_read_char_t _input_data_stream_buffer_read(sll_input_data_stream_t* restrict is){
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



sll_read_char_t _input_data_stream_buffer_read_buffer(sll_input_data_stream_t* restrict is,sll_buffer_t restrict bf,sll_buffer_size_t sz){
	sll_input_buffer_t* is_bf=(sll_input_buffer_t*)(is->ctx);
	if (is->_off+sz>=is_bf->sz){
		return SLL_END_OF_DATA;
	}
	for (sll_buffer_size_t i=0;i<sz;i++){
		*(bf+i)=*(is_bf->bf+i+is->_off);
	}
	is->_off+=sz;
	return 0;
}



void _input_data_stream_buffer_restart_line(sll_input_data_stream_t* restrict is,sll_file_offset_t lp){
	sll_input_buffer_t* bf=(sll_input_buffer_t*)(is->ctx);
	while (lp&&*(bf->bf+lp)!='\n'&&*(bf->bf+lp)!='\r'){
		lp--;
	}
	if (*(bf->bf+lp)=='\n'||*(bf->bf+lp)=='\r'){
		lp++;
	}
	is->_off=lp;
}



__SLL_FUNC void sll_stream_create_input_from_file(FILE* restrict f,sll_input_data_stream_t* restrict o){
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



__SLL_FUNC void sll_stream_create_input_from_buffer(sll_input_buffer_t* restrict bf,sll_input_data_stream_t* restrict o){
	o->ctx=bf;
	o->rf=_input_data_stream_buffer_read;
	o->rbf=_input_data_stream_buffer_read_buffer;
	o->rlf=_input_data_stream_buffer_restart_line;
	o->_lc=0;
	o->_off=0;
	o->_loff=0;
}

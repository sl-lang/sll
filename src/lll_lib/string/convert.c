#include <lll/_lll_internal.h>
#include <lll/common.h>
#include <lll/string.h>
#include <lll/types.h>
#include <stdint.h>



__LLL_FUNC lll_string_length_t lll_string_to_ascii(const lll_string_t* s,char* d){
	for (lll_string_length_t i=0;i<s->l;i++){
		if (s->v[i]>127){
			return 0;
		}
		*(d+i)=s->v[i];
	}
	return s->l;
}



__LLL_FUNC lll_string_length_t lll_string_length_as_bytes(const lll_string_t* s){
	return s->l;
}



__LLL_FUNC lll_string_length_t lll_string_to_bytes(const lll_string_t* s,lll_buffer_t d){
	for (lll_string_length_t i=0;i<s->l;i++){
		*(d+i)=s->v[i];
	}
	return s->l;
}



__LLL_FUNC void lll_string_to_stream(const lll_string_t* s,lll_output_data_stream_t* os){
	for (lll_string_length_t i=0;i<s->l;i++){
		lll_string_encode_char_to_stream(s->v[i],os);
	}
}



__LLL_FUNC void lll_string_encode_char_to_stream(lll_char_t c,lll_output_data_stream_t* os){
	uint8_t bf[4];
	uint8_t i=1;
	if (c<128){
		bf[0]=c;
	}
	else if (c<2048){
		bf[0]=0xc0|(c>>6);
		bf[1]=0x80|(c&0x3f);
		i=2;
	}
	else if (c<16384){
		bf[0]=0xe0|(c>>12);
		bf[1]=0x80|((c>>6)&0x3f);
		bf[2]=0x80|(c&0x3f);
		i=3;
	}
	else{
		bf[0]=0xf0|(c>>18);
		bf[1]=0x80|((c>>12)&0x3f);
		bf[2]=0x80|((c>>6)&0x3f);
		bf[3]=0x80|(c&0x3f);
		i=4;
	}
	LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,bf,i);
}

#include <sll/_internal/static_object.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>



static const sll_char_t* _base64_alphabet=SLL_CHAR("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
static const sll_char_t _base64_index_map[256]={
	64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,64,64,64,62,64,64,64,63,
	52,53,54,55,56,57,58,59,60,61,64,64,64,64,64,64,
	64,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
	15,16,17,18,19,20,21,22,23,24,25,64,64,64,64,64,
	64,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
	41,42,43,44,45,46,47,48,49,50,51,64,64,64,64,64,
	64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,
	64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64
};



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_error_t sll_api_base64_decode(const sll_string_t* str,sll_string_t* out){
	SLL_INIT_STRING(out);
	if (!str->length){
		return SLL_NO_ERROR;
	}
	if (str->length&3){
		return SLL_ERROR_BASE64_PADDING;
	}
	sll_string_length_t p=sll_string_count_right(str,'=');
	if (p>2){
		return SLL_ERROR_BASE64_PADDING;
	}
	sll_string_create(((str->length*3)>>2)-p,out);
	sll_string_length_t i=0;
	sll_string_length_t j=0;
	sll_string_length_t l=(str->length-p)&0xfffffffc;
	while (i<l){
		sll_char_t v0=_base64_index_map[str->data[i]];
		sll_char_t v1=_base64_index_map[str->data[i+1]];
		sll_char_t v2=_base64_index_map[str->data[i+2]];
		sll_char_t v3=_base64_index_map[str->data[i+3]];
		if ((v0|v1|v2|v3)>>6){
			sll_string_length_t idx=i;
			if (v0!=64){
				if (v1==64){
					idx++;
				}
				else if (v2==64){
					idx+=2;
				}
				else if (v3==64){
					idx+=3;
				}
			}
			sll_free_string(out);
			return SLL_ERROR_FROM_EXTRA(SLL_ERROR_BASE64_CHARACTER,idx);
		}
		out->data[j]=(v0<<2)|(v1>>4);
		out->data[j+1]=(v1<<4)|(v2>>2);
		out->data[j+2]=(v2<<6)|v3;
		i+=4;
		j+=3;
	}
	if (p==1){
		sll_char_t v0=_base64_index_map[str->data[i]];
		sll_char_t v1=_base64_index_map[str->data[i+1]];
		sll_char_t v2=_base64_index_map[str->data[i+2]];
		if ((v0|v1|v2)>>6){
			sll_string_length_t idx=i;
			if (v0!=64){
				if (v1==64){
					idx++;
				}
				else if (v2==64){
					idx+=2;
				}
			}
			sll_free_string(out);
			return SLL_ERROR_FROM_EXTRA(SLL_ERROR_BASE64_CHARACTER,idx);
		}
		out->data[j]=(v0<<2)|(v1>>4);
		out->data[j+1]=(v1<<4)|(v2>>2);
	}
	else if (p==2){
		sll_char_t v0=_base64_index_map[str->data[i]];
		sll_char_t v1=_base64_index_map[str->data[i+1]];
		if ((v0|v1)>>6){
			sll_free_string(out);
			return SLL_ERROR_FROM_EXTRA(SLL_ERROR_BASE64_CHARACTER,i+(v0!=64));
		}
		out->data[j]=(v0<<2)|(v1>>4);
	}
	sll_string_calculate_checksum(out);
	return SLL_NO_ERROR;
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_base64_encode(const sll_string_t* str,sll_string_t* out){
	if (!str->length){
		SLL_INIT_STRING(out);
		return;
	}
	sll_string_length_t p=(3-(str->length%3))%3;
	sll_string_create(((str->length+p)<<2)/3,out);
	sll_string_length_t i=0;
	sll_string_length_t j=0;
	if (str->length>2){
		do{
			out->data[j]=_base64_alphabet[str->data[i]>>2];
			out->data[j+1]=_base64_alphabet[((str->data[i]<<4)&0x3f)|(str->data[i+1]>>4)];
			out->data[j+2]=_base64_alphabet[((str->data[i+1]<<2)&0x3f)|(str->data[i+2]>>6)];
			out->data[j+3]=_base64_alphabet[str->data[i+2]&0x3f];
			i+=3;
			j+=4;
		} while (i<str->length-2);
	}
	if (p==1){
		out->data[j]=_base64_alphabet[str->data[i]>>2];
		out->data[j+1]=_base64_alphabet[((str->data[i]<<4)&0x3f)|(str->data[i+1]>>4)];
		out->data[j+2]=_base64_alphabet[(str->data[i+1]<<2)&0x3f];
		out->data[j+3]='=';
	}
	else if (p==2){
		out->data[j]=_base64_alphabet[str->data[i]>>2];
		out->data[j+1]=_base64_alphabet[(str->data[i]<<4)&0x3f];
		out->data[j+2]='=';
		out->data[j+3]='=';
	}
	sll_string_calculate_checksum(out);
}

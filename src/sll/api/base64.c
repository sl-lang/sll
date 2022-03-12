#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/string.h>
#include <sll/object.h>
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



__API_FUNC(base64_decode){
	sll_object_t* o=sll_create_object();
	o->t=SLL_OBJECT_TYPE_STRING;
	if (!a->l){
		SLL_INIT_STRING(&(o->dt.s));
		return o;
	}
	sll_integer_t oi=-1;
	if (a->l&3){
_error:
		o->t=SLL_OBJECT_TYPE_INT;
		o->dt.i=oi;
		return o;
	}
	sll_string_length_t p=sll_string_count_right(a,'=');
	if (p>2){
		goto _error;
	}
	sll_string_t* s=&(o->dt.s);
	sll_string_create(a->l*3/4-p,s);
	sll_string_length_t i=0;
	sll_string_length_t j=0;
	sll_string_length_t l=(a->l-p)&0xfffffffc;
	while (i<l){
		sll_char_t v0=_base64_index_map[a->v[i]];
		sll_char_t v1=_base64_index_map[a->v[i+1]];
		sll_char_t v2=_base64_index_map[a->v[i+2]];
		sll_char_t v3=_base64_index_map[a->v[i+3]];
		if ((v0|v1|v2|v3)>>6){
			if (v0==64){
				oi=i;
			}
			else if (v1==64){
				oi=i+1;
			}
			else if (v2==64){
				oi=i+2;
			}
			else{
				oi=i+3;
			}
_string_error:
			sll_free_string(s);
			goto _error;
		}
		s->v[j]=(v0<<2)|(v1>>4);
		s->v[j+1]=(v1<<4)|(v2>>2);
		s->v[j+2]=(v2<<6)|v3;
		i+=4;
		j+=3;
	}
	if (p==1){
		sll_char_t v0=_base64_index_map[a->v[i]];
		sll_char_t v1=_base64_index_map[a->v[i+1]];
		sll_char_t v2=_base64_index_map[a->v[i+2]];
		if ((v0|v1)>>6){
			if (v0==64){
				oi=i;
			}
			else if (v1==64){
				oi=i+1;
			}
			else{
				oi=i+2;
			}
			goto _string_error;
		}
		s->v[j]=(v0<<2)|(v1>>4);
		s->v[j+1]=(v1<<4)|(v2>>2);
	}
	else if (p==2){
		sll_char_t v0=_base64_index_map[a->v[i]];
		sll_char_t v1=_base64_index_map[a->v[i+1]];
		if ((v0|v1)>>6){
			if (v0==64){
				oi=i;
			}
			else{
				oi=i+1;
			}
			goto _string_error;
		}
		s->v[j]=(v0<<2)|(v1>>4);
	}
	sll_string_calculate_checksum(s);
	return o;
}



__API_FUNC(base64_encode){
	if (!a->l){
		SLL_INIT_STRING(out);
		return;
	}
	sll_string_length_t p=(3-(a->l%3))%3;
	sll_string_length_t l=(a->l+p)*4/3;
	sll_string_create(l,out);
	sll_string_length_t i=0;
	sll_string_length_t j=0;
	while (i<a->l-2){
		out->v[j]=_base64_alphabet[a->v[i]>>2];
		out->v[j+1]=_base64_alphabet[((a->v[i]<<4)&0x3f)|(a->v[i+1]>>4)];
		out->v[j+2]=_base64_alphabet[((a->v[i+1]<<2)&0x3f)|(a->v[i+2]>>6)];
		out->v[j+3]=_base64_alphabet[a->v[i+2]&0x3f];
		i+=3;
		j+=4;
	}
	if (p==1){
		out->v[j]=_base64_alphabet[a->v[i]>>2];
		out->v[j+1]=_base64_alphabet[((a->v[i]<<4)&0x3f)|(a->v[i+1]>>4)];
		out->v[j+2]=_base64_alphabet[(a->v[i+1]<<2)&0x3f];
		out->v[j+3]='=';
	}
	else if (p==2){
		out->v[j]=_base64_alphabet[a->v[i]>>2];
		out->v[j+1]=_base64_alphabet[(a->v[i]<<4)&0x3f];
		out->v[j+2]='=';
		out->v[j+3]='=';
	}
	sll_string_calculate_checksum(out);
}

#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/assembly.h>
#include <sll/constants.h>
#include <sll/gc.h>
#include <sll/handle.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>



#define INT_SIZE(v,o) \
	do{ \
		(o)++; \
		(v)/=10; \
	} while (v);



static sll_string_length_t _write_int(uint64_t v,sll_string_length_t i,sll_string_t* o){
	uint8_t k=0;
	char bf[20];
	do{
		bf[k]=v%10;
		k++;
		v/=10;
	} while (v);
	while (k){
		k--;
		o->v[i]=bf[k]+48;
		i++;
	}
	return i;
}



static sll_string_length_t _object_to_string(const sll_runtime_object_t* a,sll_bool_t q,sll_string_t* o,sll_string_index_t i){
	if (!a->rc){
		memcpy(o->v+i,"<released object>",17);
		return i+17;
	}
	switch (SLL_RUNTIME_OBJECT_GET_TYPE(a)){
		case SLL_RUNTIME_OBJECT_TYPE_INT:
			{
				sll_integer_t v=a->dt.i;
				if (v<0){
					o->v[i]='-';
					v=-v;
					i++;
				}
				return _write_int(v,i,o);
			}
		case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
			return i+snprintf((char*)(o->v+i),4096,"%.16lf",a->dt.f);
		case SLL_RUNTIME_OBJECT_TYPE_CHAR:
			if (q){
				SLL_UNIMPLEMENTED();
			}
			o->v[i]=a->dt.c;
			return i+1;
		case SLL_RUNTIME_OBJECT_TYPE_STRING:
			if (q){
				o->v[i]='\"';
				i++;
				for (sll_string_length_t j=0;j<a->dt.s.l;j++){
					sll_char_t c=a->dt.s.v[j];
					if (c=='\"'||c=='\''||c=='\\'){
						o->v[i]='\\';
						o->v[i+1]=c;
						i+=2;
					}
					else if (c=='\t'){
						o->v[i]='\\';
						o->v[i+1]='t';
						i+=2;
					}
					else if (c=='\n'){
						o->v[i]='\\';
						o->v[i+1]='n';
						i+=2;
					}
					else if (c=='\v'){
						o->v[i]='\\';
						o->v[i+1]='v';
						i+=2;
					}
					else if (c=='\f'){
						o->v[i]='\\';
						o->v[i+1]='f';
						i+=2;
					}
					else if (c=='\r'){
						o->v[i]='\\';
						o->v[i+1]='r';
						i+=2;
					}
					else if (SLL_STRING_HEX_ESCAPE(c)){
						o->v[i]='\\';
						o->v[i+1]='x';
						o->v[i+2]=(c>>4)+((c>>4)>9?87:48);
						o->v[i+3]=(c&15)+((c&15)>9?87:48);
						i+=4;
					}
					else{
						o->v[i]=c;
						i++;
					}
				}
				o->v[i]='\"';
				i++;
				return i;
			}
			memcpy(o->v+i,a->dt.s.v,a->dt.s.l);
			return i+a->dt.s.l;
		case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
			o->v[i]='[';
			i++;
			for (sll_array_length_t k=0;k<a->dt.a.l;k++){
				if (k){
					o->v[i]=' ';
					i++;
				}
				i=_object_to_string(*(a->dt.a.v+k),1,o,i);
			}
			o->v[i]=']';
			return i+1;
		case SLL_RUNTIME_OBJECT_TYPE_HANDLE:
			{
				sll_handle_descriptor_t* hd=(sll_current_runtime_data?sll_lookup_handle_descriptor(sll_current_runtime_data->hl,a->dt.h.t):NULL);
				sll_handle_t v=a->dt.h.h;
				if (hd&&hd->sf){
					return hd->sf(v,i,o);
				}
				i=_write_int(a->dt.h.h,i,o);
				o->v[i]='$';
				i++;
				if (hd){
					memcpy(o->v+i,hd->nm,hd->nml);
					return i+hd->nml;
				}
				return _write_int(a->dt.h.t,i,o);
			}
		case SLL_RUNTIME_OBJECT_TYPE_MAP:
			o->v[i]='<';
			i++;
			for (sll_map_length_t k=0;k<(a->dt.m.l<<1);k++){
				if (k){
					o->v[i]=' ';
					i++;
				}
				i=_object_to_string(*(a->dt.m.v+k),1,o,i);
			}
			o->v[i]='>';
			return i+1;
		default:
			SLL_UNREACHABLE();
	}
	return 0;
}



__SLL_FUNC void sll_object_to_string(const sll_runtime_object_t*const* a,sll_array_length_t al,sll_string_t* o){
	if (!al){
		SLL_ZERO_STRING(o);
		return;
	}
	sll_string_create(sll_object_to_string_length(a,al,0),o);
	sll_string_length_t i=0;
	for (sll_array_length_t j=0;j<al;j++){
		i=_object_to_string(*(a+j),0,o,i);
	}
	SLL_ASSERT(i==o->l);
	sll_string_hash(o);
}



__SLL_FUNC sll_string_length_t sll_object_to_string_length(const sll_runtime_object_t*const* al,sll_array_length_t all,sll_bool_t q){
	sll_string_length_t o=0;
	for (sll_array_length_t i=0;i<all;i++){
		const sll_runtime_object_t* a=*(al+i);
		if (!a->rc){
			o+=17;
			continue;
		}
		switch (SLL_RUNTIME_OBJECT_GET_TYPE(a)){
			case SLL_RUNTIME_OBJECT_TYPE_INT:
				{
					sll_integer_t v=a->dt.i;
					if (v<0){
						o++;
						v=-v;
					}
					INT_SIZE(v,o);
					break;
				}
			case SLL_RUNTIME_OBJECT_TYPE_FLOAT:
				o+=snprintf(NULL,0,"%.16lf",a->dt.f);
				break;
			case SLL_RUNTIME_OBJECT_TYPE_CHAR:
				o++;
				if (q){
					o+=2;
					if (SLL_STRING_ESCAPE(a->dt.c)){
						o++;
					}
					else if (SLL_STRING_HEX_ESCAPE(a->dt.c)){
						o+=3;
					}
				}
				break;
			case SLL_RUNTIME_OBJECT_TYPE_STRING:
				o+=a->dt.s.l;
				if (q){
					o+=2;
					for (sll_string_length_t j=0;j<a->dt.s.l;j++){
						if (SLL_STRING_ESCAPE(a->dt.s.v[j])){
							o++;
						}
						else if (SLL_STRING_HEX_ESCAPE(a->dt.s.v[j])){
							o+=3;
						}
					}
				}
				break;
			case SLL_RUNTIME_OBJECT_TYPE_ARRAY:
				o+=sll_object_to_string_length((const sll_runtime_object_t*const*)a->dt.a.v,a->dt.a.l,1)+a->dt.a.l+(!a->dt.a.l)+1;
				break;
			case SLL_RUNTIME_OBJECT_TYPE_HANDLE:
				{
					sll_handle_descriptor_t* hd=(sll_current_runtime_data?sll_lookup_handle_descriptor(sll_current_runtime_data->hl,a->dt.h.t):NULL);
					sll_handle_t v=a->dt.h.h;
					if (hd&&hd->sf){
						o+=hd->sf(v,0,NULL);
					}
					else{
						INT_SIZE(v,o);
						o++;
						if (hd){
							o+=hd->nml;
						}
						else{
							v=a->dt.h.t;
							INT_SIZE(v,o);
						}
					}
					break;
				}
			case SLL_RUNTIME_OBJECT_TYPE_MAP:
				o+=sll_object_to_string_length((const sll_runtime_object_t*const*)a->dt.m.v,a->dt.m.l<<1,1)+(a->dt.m.l<<1)+(!a->dt.m.l)+1;
				break;
			default:
				SLL_UNREACHABLE();
		}
	}
	return o;
}



__API_FUNC(string_convert){
	sll_object_to_string(a,ac,out);
}



__API_FUNC(string_length){
	return sll_object_to_string_length(a,ac,0);
}

#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/json.h>
#include <sll/array.h>
#include <sll/assembly.h>
#include <sll/common.h>
#include <sll/handle.h>
#include <sll/init.h>
#include <sll/map.h>
#include <sll/runtime_object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>



#define SETUP_HANDLE \
	do{ \
		if (_json_ht==SLL_HANDLE_UNKNOWN_TYPE){ \
			SLL_ASSERT(sll_current_runtime_data); \
			_json_ht=sll_create_handle(sll_current_runtime_data->hl,&_json_type); \
			_json_null=SLL_FROM_HANDLE(_json_ht,0); \
			_json_true=SLL_FROM_HANDLE(_json_ht,1); \
			_json_false=SLL_FROM_HANDLE(_json_ht,2); \
		} \
	} while (0)



static sll_handle_type_t _json_ht=SLL_HANDLE_UNKNOWN_TYPE;
static sll_runtime_object_t* _json_null=NULL;
static sll_runtime_object_t* _json_true=NULL;
static sll_runtime_object_t* _json_false=NULL;
static sll_handle_descriptor_t _json_type;



static void _json_cleanup(sll_handle_t h){
	if (h==SLL_HANDLE_FREE){
		SLL_RELEASE(_json_null);
		SLL_RELEASE(_json_true);
		SLL_RELEASE(_json_false);
		_json_null=NULL;
		_json_true=NULL;
		_json_false=NULL;
	}
}



static sll_string_length_t _json_stringify(sll_handle_t h,sll_string_length_t i,sll_string_t* o){
	if (!o){
		return (h<2?4:5);
	}
	if (!h){
		return sll_string_insert_pointer(SLL_CHAR("null"),i,o);
	}
	if (h==1){
		return sll_string_insert_pointer(SLL_CHAR("true"),i,o);
	}
	return sll_string_insert_pointer(SLL_CHAR("false"),i,o);
}



static void _parse_json_string(sll_json_parser_state_t* p,sll_string_t* o){
	o->l=0;
	o->v=malloc(SLL_STRING_ALIGN_LENGTH(0)*sizeof(sll_char_t));
	SLL_STRING_FORMAT_PADDING(o->v,0);
	sll_char_t c=**p;
	(*p)++;
	while (c!='\"'){
		sll_string_increase(o,1);
		if (c!='\\'){
			o->v[o->l]=c;
			o->l++;
		}
		else{
			c=**p;
			(*p)++;
			if (c=='/'||c=='\\'||c=='\''||c=='\"'){
				o->v[o->l]=c;
				o->l++;
			}
			else if (c=='b'){
				o->v[o->l]=8;
				o->l++;
			}
			else if (c=='f'){
				o->v[o->l]=12;
				o->l++;
			}
			else if (c=='n'){
				o->v[o->l]=10;
				o->l++;
			}
			else if (c=='r'){
				o->v[o->l]=13;
				o->l++;
			}
			else if (c=='t'){
				o->v[o->l]=9;
				o->l++;
			}
			else if (c=='v'){
				o->v[o->l]=11;
				o->l++;
			}
			else if (c=='x'){
				sll_char_t a=**p;
				(*p)++;
				sll_char_t b=**p;
				(*p)++;
				o->v[o->l]=((a>47&&a<58?a-48:(a>64&&a<71?a-55:a-87))<<4)|(b>47&&b<58?b-48:(b>64&&b<71?b-55:b-87));
				o->l++;
			}
			else{
				printf("Unknown Escape: \\%c\n",c);
				SLL_UNIMPLEMENTED();
			}
		}
		c=**p;
		(*p)++;
	}
	o->v[o->l]=0;
	sll_string_calculate_checksum(o);
}



static sll_runtime_object_t* _parse_json_as_object(sll_json_parser_state_t* p){
	sll_char_t c=**p;
	(*p)++;
	while (c==' '||c=='\t'||c=='\n'||c=='\r'){
		c=**p;
		(*p)++;
	}
	if (!c){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	if (c=='{'){
		sll_runtime_object_t* o=SLL_CREATE();
		o->t=SLL_RUNTIME_OBJECT_TYPE_MAP;
		sll_map_t* m=&(o->dt.m);
		SLL_INIT_MAP(m);
		while (1){
			c=**p;
			(*p)++;
			while (c!='\"'){
				if (c=='}'){
					return o;
				}
				if (c!=' '&&c!='\t'&&c!='\n'&&c!='\r'){
					SLL_RELEASE(o);
					return NULL;
				}
				c=**p;
				(*p)++;
			}
			m->l++;
			m->v=realloc(m->v,(m->l<<1)*sizeof(sll_runtime_object_t*));
			sll_runtime_object_t* k=SLL_CREATE();
			m->v[(m->l-1)<<1]=k;
			k->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
			_parse_json_string(p,&(k->dt.s));
			c=**p;
			(*p)++;
			while (c!=':'){
				c=**p;
				(*p)++;
			}
			sll_runtime_object_t* v=_parse_json_as_object(p);
			if (!v){
				m->v[m->l-1]=SLL_ACQUIRE_STATIC_INT(0);
				SLL_RELEASE(o);
				return NULL;
			}
			m->v[((m->l-1)<<1)+1]=v;
			c=**p;
			(*p)++;
			while (c!=','){
				if (c=='}'){
					return o;
				}
				if (c!=' '&&c!='\t'&&c!='\n'&&c!='\r'){
					SLL_RELEASE(o);
					return NULL;
				}
				c=**p;
				(*p)++;
			}
		}
	}
	if (c=='['){
		sll_runtime_object_t* o=SLL_CREATE();
		o->t=SLL_RUNTIME_OBJECT_TYPE_ARRAY;
		sll_array_t* a=&(o->dt.a);
		SLL_INIT_ARRAY(a);
		while (c==' '||c=='\t'||c=='\n'||c=='\r'){
			c=**p;
			(*p)++;
		}
		if (**p==']'){
			(*p)++;
			return o;
		}
		while (1){
			sll_runtime_object_t* k=_parse_json_as_object(p);
			if (!k){
				SLL_RELEASE(o);
				return NULL;
			}
			a->l++;
			a->v=realloc(a->v,a->l*sizeof(sll_runtime_object_t*));
			a->v[a->l-1]=k;
			c=**p;
			(*p)++;
			while (c!=','){
				if (c==']'){
					return o;
				}
				if (c!=' '&&c!='\t'&&c!='\n'&&c!='\r'){
					SLL_RELEASE(o);
					return NULL;
				}
				c=**p;
				(*p)++;
			}
		}
	}
	if (c=='\"'){
		sll_runtime_object_t* o=SLL_CREATE();
		o->t=SLL_RUNTIME_OBJECT_TYPE_STRING;
		_parse_json_string(p,&(o->dt.s));
		return o;
	}
	if (c=='t'&&**p=='r'&&*((*p)+1)=='u'&&*((*p)+2)=='e'){
		(*p)+=3;
		SLL_ACQUIRE(_json_true);
		return _json_true;
	}
	if (c=='f'&&**p=='a'&&*((*p)+1)=='l'&&*((*p)+2)=='s'&&*((*p)+3)=='e'){
		(*p)+=4;
		SLL_ACQUIRE(_json_false);
		return _json_false;
	}
	if (c=='n'&&**p=='u'&&*((*p)+1)=='l'&&*((*p)+2)=='l'){
		(*p)+=3;
		SLL_ACQUIRE(_json_null);
		return _json_null;
	}
	if ((c<48||c>57)&&c!='.'&&c!='e'&&c!='E'&&c!='-'&&c!='+'){
		return NULL;
	}
	int8_t s=1;
	if (c=='+'){
		c=**p;
		(*p)++;
	}
	else if (c=='-'){
		s=-1;
		c=**p;
		(*p)++;
	}
	sll_float_t v=0;
	while (c>47&&c<58){
		v=v*10+(c-48);
		c=**p;
		(*p)++;
	}
	if (c!='.'&&c!='e'&&c!='E'){
		(*p)--;
		return SLL_FROM_INT((sll_integer_t)(v*s));
	}
	if (c=='.'){
		sll_float_t pw=0.1;
		c=**p;
		(*p)++;
		while (c>47&&c<58){
			v+=pw*(c-48);
			pw*=0.1;
			c=**p;
			(*p)++;
		}
	}
	if (c=='e'||c=='E'){
		c=**p;
		(*p)++;
		int8_t pw_s=1;
		if (c=='+'){
			c=**p;
			(*p)++;
		}
		else if (c=='-'){
			c=**p;
			(*p)++;
			pw_s=-1;
		}
		int64_t pw=0;
		while (c>47&&c<58){
			pw=pw*10+(c-48);
			c=**p;
			(*p)++;
		}
		pw*=pw_s;
		v*=pow(2,(sll_float_t)pw)*pow(5,(sll_float_t)pw);
	}
	(*p)--;
	return SLL_FROM_FLOAT(v*s);
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_json_object_t* sll_json_get_by_key(sll_json_object_t* json,const sll_string_t* k){
	SLL_ASSERT(json->t==SLL_JSON_OBJECT_TYPE_MAP);
	for (sll_json_map_length_t i=0;i<json->dt.m.l;i++){
		sll_json_map_keypair_t* e=json->dt.m.dt+i;
		if (e->k.c==k->c&&e->k.l==k->l&&!memcmp(e->k.v,k->v,e->k.l)){
			return &(e->v);
		}
	}
	return NULL;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_json_parse(sll_json_parser_state_t* p,sll_json_object_t* o){
	sll_char_t c=**p;
	(*p)++;
	while (c==' '||c=='\t'||c=='\n'||c=='\r'){
		c=**p;
		(*p)++;
	}
	if (c=='{'){
		o->t=SLL_JSON_OBJECT_TYPE_MAP;
		o->dt.m.l=0;
		o->dt.m.dt=NULL;
		while (1){
			c=**p;
			(*p)++;
			while (c!='\"'){
				if (c=='}'){
					return SLL_RETURN_NO_ERROR;
				}
				if (c!=' '&&c!='\t'&&c!='\n'&&c!='\r'){
					return SLL_RETURN_ERROR;
				}
				c=**p;
				(*p)++;
			}
			o->dt.m.l++;
			o->dt.m.dt=realloc(o->dt.m.dt,o->dt.m.l*sizeof(sll_json_map_keypair_t));
			sll_json_map_keypair_t* k=o->dt.m.dt+o->dt.m.l-1;
			_parse_json_string(p,&(k->k));
			c=**p;
			(*p)++;
			while (c!=':'){
				c=**p;
				(*p)++;
			}
			if (!sll_json_parse(p,&(k->v))){
				return SLL_RETURN_ERROR;
			}
			c=**p;
			(*p)++;
			while (c!=','){
				if (c=='}'){
					return SLL_RETURN_NO_ERROR;
				}
				if (c!=' '&&c!='\t'&&c!='\n'&&c!='\r'){
					return SLL_RETURN_ERROR;
				}
				c=**p;
				(*p)++;
			}
		}
	}
	if (c=='['){
		o->t=SLL_JSON_OBJECT_TYPE_ARRAY;
		o->dt.a.l=0;
		o->dt.a.dt=NULL;
		while (c==' '||c=='\t'||c=='\n'||c=='\r'){
			c=**p;
			(*p)++;
		}
		if (**p==']'){
			(*p)++;
			return SLL_RETURN_NO_ERROR;
		}
		while (1){
			o->dt.a.l++;
			o->dt.a.dt=realloc(o->dt.a.dt,o->dt.a.l*sizeof(sll_json_object_t));
			if (!sll_json_parse(p,o->dt.a.dt+o->dt.a.l-1)){
				return SLL_RETURN_ERROR;
			}
			c=**p;
			(*p)++;
			while (c!=','){
				if (c==']'){
					return SLL_RETURN_NO_ERROR;
				}
				if (c!=' '&&c!='\t'&&c!='\n'&&c!='\r'){
					return SLL_RETURN_ERROR;
				}
				c=**p;
				(*p)++;
			}
		}
	}
	if (c=='\"'){
		o->t=SLL_JSON_OBJECT_TYPE_STRING;
		_parse_json_string(p,&(o->dt.s));
		return SLL_RETURN_NO_ERROR;
	}
	if (c=='t'&&**p=='r'&&*((*p)+1)=='u'&&*((*p)+2)=='e'){
		(*p)+=3;
		o->t=SLL_JSON_OBJECT_TYPE_TRUE;
		return SLL_RETURN_NO_ERROR;
	}
	if (c=='f'&&**p=='a'&&*((*p)+1)=='l'&&*((*p)+2)=='s'&&*((*p)+3)=='e'){
		(*p)+=4;
		o->t=SLL_JSON_OBJECT_TYPE_FALSE;
		return SLL_RETURN_NO_ERROR;
	}
	if (c=='n'&&**p=='u'&&*((*p)+1)=='l'&&*((*p)+2)=='l'){
		(*p)+=3;
		o->t=SLL_JSON_OBJECT_TYPE_NULL;
		return SLL_RETURN_NO_ERROR;
	}
	if ((c<48||c>57)&&c!='.'&&c!='e'&&c!='E'&&c!='-'&&c!='+'){
		return SLL_RETURN_ERROR;
	}
	int8_t s=1;
	if (c=='+'){
		c=**p;
		(*p)++;
	}
	else if (c=='-'){
		s=-1;
		c=**p;
		(*p)++;
	}
	sll_float_t v=0;
	while (c>47&&c<58){
		v=v*10+(c-48);
		c=**p;
		(*p)++;
	}
	if (c!='.'&&c!='e'&&c!='E'){
		(*p)--;
		o->t=SLL_JSON_OBJECT_TYPE_INTEGER;
		o->dt.i=(sll_integer_t)(v*s);
		return SLL_RETURN_NO_ERROR;
	}
	if (c=='.'){
		sll_float_t pw=0.1;
		c=**p;
		(*p)++;
		while (c>47&&c<58){
			v+=pw*(c-48);
			pw*=0.1;
			c=**p;
			(*p)++;
		}
	}
	if (c=='e'||c=='E'){
		c=**p;
		(*p)++;
		int8_t pw_s=1;
		if (c=='+'){
			c=**p;
			(*p)++;
		}
		else if (c=='-'){
			c=**p;
			(*p)++;
			pw_s=-1;
		}
		int64_t pw=0;
		while (c>47&&c<58){
			pw=pw*10+(c-48);
			c=**p;
			(*p)++;
		}
		pw*=pw_s;
		v*=pow(2,(sll_float_t)pw)*pow(5,(sll_float_t)pw);
	}
	(*p)--;
	o->t=SLL_JSON_OBJECT_TYPE_FLOAT;
	o->dt.f=v*s;
	return SLL_RETURN_NO_ERROR;
}



__API_FUNC(json_parse){
	SETUP_HANDLE;
	sll_json_parser_state_t p=a->v;
	sll_runtime_object_t* o=_parse_json_as_object(&p);
	if (o){
		return o;
	}
	return SLL_ACQUIRE_STATIC(handle_zero);
}



__API_FUNC(json_stringify){
	SLL_UNIMPLEMENTED();
}



__API_FUNC(json_type){
	SETUP_HANDLE;
	if (a>=0&&a<3){
		out->t=_json_ht;
		out->h=a;
		return;
	}
	SLL_INIT_HANDLE_DATA(out);
}



static sll_handle_descriptor_t _json_type={
	SLL_HANDLE_DESCRIPTOR_HEADER("sll_json_type_handle"),
	_json_stringify,
	_json_cleanup
};

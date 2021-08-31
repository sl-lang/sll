#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/array.h>
#include <sll/constants.h>
#include <sll/map.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <math.h>
#include <stdlib.h>



static void _parse_json_string(sll_json_parser_state_t* p,sll_string_t* o){
	o->l=1;
	o->c=0;
	o->v=malloc(sizeof(sll_char_t));
	sll_char_t c=**p;
	(*p)++;
	while (c!='\"'){
		if (c!='\\'){
			o->l++;
			o->v=realloc(o->v,o->l*sizeof(sll_char_t));
			o->v[o->l-2]=c;
		}
		else{
			c=**p;
			(*p)++;
			if (c=='/'||c=='\\'||c=='\''||c=='\"'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(sll_char_t));
				o->v[o->l-2]=c;
			}
			else if (c=='b'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(sll_char_t));
				o->v[o->l-2]=8;
			}
			else if (c=='f'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(sll_char_t));
				o->v[o->l-2]=12;
			}
			else if (c=='n'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(sll_char_t));
				o->v[o->l-2]=10;
			}
			else if (c=='r'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(sll_char_t));
				o->v[o->l-2]=13;
			}
			else if (c=='t'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(sll_char_t));
				o->v[o->l-2]=9;
			}
			else if (c=='v'){
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(sll_char_t));
				o->v[o->l-2]=11;
			}
			else if (c=='x'){
				sll_char_t a=**p;
				(*p)++;
				sll_char_t b=**p;
				(*p)++;
				o->l++;
				o->v=realloc(o->v,o->l*sizeof(sll_char_t));
				o->v[o->l-2]=((a>47&&a<58?a-48:(a>64&&a<71?a-55:a-87))<<4)|(b>47&&b<58?b-48:(b>64&&b<71?b-55:b-87));
			}
			else{
				printf("Unknown Escape: \\%c\n",c);
				SLL_UNIMPLEMENTED();
			}
		}
		o->c^=o->v[o->l-2];
		c=**p;
		(*p)++;
	}
	o->l--;
	o->v[o->l]=0;
}



static sll_runtime_object_t* _parse_json_as_object(sll_json_parser_state_t* p){
	sll_char_t c=**p;
	(*p)++;
	while (c==' '||c=='\t'||c=='\n'||c=='\r'){
		c=**p;
		(*p)++;
	}
	if (c=='{'){
		sll_runtime_object_t* o=SLL_CREATE();
		o->t=SLL_RUNTIME_OBJECT_TYPE_MAP;
		sll_map_t* m=&(o->dt.m);
		SLL_ZERO_MAP(m);
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
			m->l+=2;
			m->v=realloc(m->v,m->l*sizeof(sll_runtime_object_t*));
			sll_runtime_object_t* k=SLL_CREATE();
			m->v[m->l-2]=k;
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
				m->v[m->l-1]=SLL_ACQUIRE_STATIC(int_zero);
				SLL_RELEASE(o);
				return NULL;
			}
			m->v[m->l-1]=v;
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
		SLL_ZERO_ARRAY(a);
		while (c==' '||c=='\t'||c=='\n'||c=='\r'){
			c=**p;
			(*p)++;
		}
		if (c==']'){
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
		SLL_RETURN_ONE;
	}
	if (c=='f'&&**p=='a'&&*((*p)+1)=='l'&&*((*p)+2)=='s'&&*((*p)+3)=='e'){
		(*p)+=4;
		SLL_RETURN_ZERO;
	}
	if (c=='n'&&**p=='u'&&*((*p)+1)=='l'&&*((*p)+2)=='l'){
		(*p)+=3;
		SLL_RETURN_ZERO;
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



__SLL_FUNC void sll_free_json_object(sll_json_object_t* json){
	if (json->t==SLL_JSON_OBJECT_TYPE_STRING){
		free(json->dt.s.v);
	}
	else if (json->t==SLL_JSON_OBJECT_TYPE_ARRAY){
		for (sll_json_array_length_t i=0;i<json->dt.a.l;i++){
			sll_free_json_object(json->dt.a.dt+i);
		}
		free(json->dt.a.dt);
	}
	else if (json->t==SLL_JSON_OBJECT_TYPE_MAP){
		for (sll_json_map_length_t i=0;i<json->dt.m.l;i++){
			sll_json_map_keypair_t* e=json->dt.m.dt+i;
			free(e->k.v);
			sll_free_json_object(&(e->v));
		}
		free(json->dt.m.dt);
	}
}



__SLL_FUNC sll_json_object_t* sll_json_get_by_key(sll_json_object_t* json,const sll_string_t* k){
	SLL_ASSERT(json->t==SLL_JSON_OBJECT_TYPE_MAP);
	for (sll_json_map_length_t i=0;i<json->dt.m.l;i++){
		sll_json_map_keypair_t* e=json->dt.m.dt+i;
		if (e->k.c!=k->c||e->k.l!=k->l){
			continue;
		}
		for (sll_string_length_t j=0;j<e->k.l;j++){
			if (e->k.v[j]!=k->v[j]){
				goto _check_next_key;
			}
		}
		return &(e->v);
_check_next_key:;
	}
	return NULL;
}



__SLL_FUNC __SLL_RETURN sll_json_parse(sll_json_parser_state_t* p,sll_json_object_t* o){
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
		if (c==']'){
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
	if (!ac){
		SLL_RETURN_ZERO;
	}
	const sll_runtime_object_t* v=*a;
	if (v->t!=SLL_RUNTIME_OBJECT_TYPE_STRING){
		SLL_RETURN_ZERO;
	}
	sll_json_parser_state_t p=v->dt.s.v;
	sll_runtime_object_t* o=_parse_json_as_object(&p);
	if (!o){
		SLL_RETURN_ZERO;
	}
	return o;
}



__API_FUNC(json_stringify){
	SLL_UNIMPLEMENTED();
	SLL_RETURN_ZERO;
}

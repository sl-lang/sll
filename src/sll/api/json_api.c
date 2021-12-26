#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/api/json.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/file.h>
#include <sll/handle.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/vm.h>
#include <math.h>



static sll_object_t* _json_null=NULL;
static sll_object_t* _json_true=NULL;
static sll_object_t* _json_false=NULL;



static void _release_data(void){
	SLL_RELEASE(_json_null);
	SLL_RELEASE(_json_true);
	SLL_RELEASE(_json_false);
	_json_null=NULL;
	_json_true=NULL;
	_json_false=NULL;
}



static void _parse_json_string(sll_json_parser_state_t* p,sll_string_t* o){
	sll_string_create(0,o);
	o->v=sll_memory_move(o->v,SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
	sll_char_t c=**p;
	(*p)++;
	while (c!='\"'){
		sll_string_increase(o,1);
		if (c!='\\'){
			o->v[o->l]=c;
		}
		else{
			c=**p;
			(*p)++;
			if (c=='b'){
				o->v[o->l]=8;
			}
			else if (c=='f'){
				o->v[o->l]=12;
			}
			else if (c=='n'){
				o->v[o->l]=10;
			}
			else if (c=='r'){
				o->v[o->l]=13;
			}
			else if (c=='t'){
				o->v[o->l]=9;
			}
			else if (c=='v'){
				o->v[o->l]=11;
			}
			else if (c=='x'){
				sll_char_t a=**p;
				(*p)++;
				sll_char_t b=**p;
				(*p)++;
				o->v[o->l]=((a>47&&a<58?a-48:(a>64&&a<71?a-55:a-87))<<4)|(b>47&&b<58?b-48:(b>64&&b<71?b-55:b-87));
			}
			else{
				o->v[o->l]=c;
			}
		}
		o->l++;
		c=**p;
		(*p)++;
	}
	o->v=sll_memory_move(o->v,SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(o);
}



static sll_object_t* _parse_json_as_object(sll_json_parser_state_t* p){
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
		sll_object_t* o=SLL_CREATE();
		o->t=SLL_OBJECT_TYPE_MAP;
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
			m->v=sll_reallocate(m->v,(m->l<<1)*sizeof(sll_object_t*));
			sll_object_t* k=SLL_CREATE();
			m->v[(m->l-1)<<1]=k;
			k->t=SLL_OBJECT_TYPE_STRING;
			_parse_json_string(p,&(k->dt.s));
			c=**p;
			(*p)++;
			while (c!=':'){
				c=**p;
				(*p)++;
			}
			sll_object_t* v=_parse_json_as_object(p);
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
		sll_object_t* o=SLL_CREATE();
		o->t=SLL_OBJECT_TYPE_ARRAY;
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
			sll_object_t* k=_parse_json_as_object(p);
			if (!k){
				SLL_RELEASE(o);
				return NULL;
			}
			a->l++;
			a->v=sll_reallocate(a->v,a->l*sizeof(sll_object_t*));
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
		sll_object_t* o=SLL_CREATE();
		o->t=SLL_OBJECT_TYPE_STRING;
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



__SLL_EXTERNAL void sll_free_json_object(sll_json_object_t* json){
	if (json->t==SLL_JSON_OBJECT_TYPE_STRING){
		sll_free_string(&(json->dt.s));
	}
	else if (json->t==SLL_JSON_OBJECT_TYPE_ARRAY){
		for (sll_json_array_length_t i=0;i<json->dt.a.l;i++){
			sll_free_json_object(json->dt.a.dt+i);
		}
		sll_deallocate(json->dt.a.dt);
	}
	else if (json->t==SLL_JSON_OBJECT_TYPE_MAP){
		for (sll_json_map_length_t i=0;i<json->dt.m.l;i++){
			sll_json_map_keypair_t* e=json->dt.m.dt+i;
			sll_free_string(&(e->k));
			sll_free_json_object(&(e->v));
		}
		sll_deallocate(json->dt.m.dt);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_json_object_t* sll_json_get_by_key(sll_json_object_t* json,const sll_string_t* k){
	SLL_ASSERT(json->t==SLL_JSON_OBJECT_TYPE_MAP);
	for (sll_json_map_length_t i=0;i<json->dt.m.l;i++){
		sll_json_map_keypair_t* e=json->dt.m.dt+i;
		if (sll_string_equal(&(e->k),k)){
			return &(e->v);
		}
	}
	return NULL;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_json_parse(sll_json_parser_state_t* p,sll_json_object_t* o){
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
					return 1;
				}
				if (c!=' '&&c!='\t'&&c!='\n'&&c!='\r'){
					return 0;
				}
				c=**p;
				(*p)++;
			}
			o->dt.m.l++;
			o->dt.m.dt=sll_reallocate(o->dt.m.dt,o->dt.m.l*sizeof(sll_json_map_keypair_t));
			sll_json_map_keypair_t* k=o->dt.m.dt+o->dt.m.l-1;
			_parse_json_string(p,&(k->k));
			c=**p;
			(*p)++;
			while (c!=':'){
				c=**p;
				(*p)++;
			}
			if (!sll_json_parse(p,&(k->v))){
				return 0;
			}
			c=**p;
			(*p)++;
			while (c!=','){
				if (c=='}'){
					return 1;
				}
				if (c!=' '&&c!='\t'&&c!='\n'&&c!='\r'){
					return 0;
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
			return 1;
		}
		while (1){
			o->dt.a.l++;
			o->dt.a.dt=sll_reallocate(o->dt.a.dt,o->dt.a.l*sizeof(sll_json_object_t));
			if (!sll_json_parse(p,o->dt.a.dt+o->dt.a.l-1)){
				return 0;
			}
			c=**p;
			(*p)++;
			while (c!=','){
				if (c==']'){
					return 1;
				}
				if (c!=' '&&c!='\t'&&c!='\n'&&c!='\r'){
					return 0;
				}
				c=**p;
				(*p)++;
			}
		}
	}
	if (c=='\"'){
		o->t=SLL_JSON_OBJECT_TYPE_STRING;
		_parse_json_string(p,&(o->dt.s));
		return 1;
	}
	if (c=='t'&&**p=='r'&&*((*p)+1)=='u'&&*((*p)+2)=='e'){
		(*p)+=3;
		o->t=SLL_JSON_OBJECT_TYPE_TRUE;
		return 1;
	}
	if (c=='f'&&**p=='a'&&*((*p)+1)=='l'&&*((*p)+2)=='s'&&*((*p)+3)=='e'){
		(*p)+=4;
		o->t=SLL_JSON_OBJECT_TYPE_FALSE;
		return 1;
	}
	if (c=='n'&&**p=='u'&&*((*p)+1)=='l'&&*((*p)+2)=='l'){
		(*p)+=3;
		o->t=SLL_JSON_OBJECT_TYPE_NULL;
		return 1;
	}
	if ((c<48||c>57)&&c!='.'&&c!='e'&&c!='E'&&c!='-'&&c!='+'){
		return 0;
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
		return 1;
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
	return 1;
}



__API_FUNC(json__init){
	if (!_json_null){
		sll_register_cleanup(_release_data);
	}
	SLL_ACQUIRE(a);
	SLL_ACQUIRE(b);
	SLL_ACQUIRE(c);
	_json_null=a;
	_json_true=b;
	_json_false=c;
}



__API_FUNC(json_parse){
	sll_json_parser_state_t p=a->v;
	sll_object_t* o=_parse_json_as_object(&p);
	if (o){
		return o;
	}
	return SLL_ACQUIRE_STATIC(handle_zero);
}



__API_FUNC(json_stringify){
	SLL_UNIMPLEMENTED();
}

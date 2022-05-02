#include <sll/_internal/common.h>
#include <sll/_internal/gc.h>
#include <sll/_internal/json.h>
#include <sll/_internal/string.h>
#include <sll/allocator.h>
#include <sll/api/json.h>
#include <sll/array.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/file.h>
#include <sll/gc.h>
#include <sll/init.h>
#include <sll/map.h>
#include <sll/memory.h>
#include <sll/object.h>
#include <sll/operator.h>
#include <sll/static_object.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/vm.h>
#include <math.h>
#include <stdio.h>



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
	STRING_INIT_STACK(o);
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
			else if (c=='u'){
				sll_wide_char_t v=0;
				for (unsigned int i=0;i<4;i++){
					c=**p;
					v=(v<<4)|((__SLL_U16)(c>47&&c<58?c-48:(c>64&&c<71?c-55:c-87)));
				}
				if (v>255){
					SLL_UNIMPLEMENTED();
				}
				o->v[o->l]=v&0xff;
			}
			else{
				o->v[o->l]=c;
			}
		}
		o->l++;
		c=**p;
		(*p)++;
	}
	sll_allocator_move((void**)(&(o->v)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(o);
}



static sll_bool_t _parse_number(sll_char_t c,sll_json_parser_state_t* p,json_number_t* o){
	sll_bool_t neg=0;
	if (c=='+'){
		c=**p;
		(*p)++;
	}
	else if (c=='-'){
		neg=1;
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
		o->i=(sll_integer_t)(neg?-v:v);
		return JSON_NUMBER_INT;
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
		sll_bool_t neg_pw=0;
		if (c=='+'){
			c=**p;
			(*p)++;
		}
		else if (c=='-'){
			neg_pw=1;
			c=**p;
			(*p)++;
		}
		sll_integer_t pw=0;
		while (c>47&&c<58){
			pw=pw*10+(c-48);
			c=**p;
			(*p)++;
		}
		v*=pow(10,(sll_float_t)(neg_pw?-pw:pw));
	}
	(*p)--;
	o->f=(neg?-v:v);
	return JSON_NUMBER_FLOAT;
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
		sll_object_t* o=sll_map_to_object(NULL);
		sll_map_t* m=&(o->dt.m);
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
			sll_object_t* k=sll_create_object(SLL_OBJECT_TYPE_STRING);
			m->v[(m->l-1)<<1]=k;
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
		sll_object_t* o=sll_array_to_object(NULL);
		sll_array_t* a=&(o->dt.a);
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
			sll_allocator_resize((void**)(&(a->v)),a->l*sizeof(sll_object_t*));
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
		sll_object_t* o=sll_create_object(SLL_OBJECT_TYPE_STRING);
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
	json_number_t n;
	return (_parse_number(c,p,&n)==JSON_NUMBER_INT?sll_int_to_object(n.i):sll_float_to_object(n.f));
}



static void _stringify_string(const sll_char_t* c,sll_string_length_t l,sll_string_t* o){
	sll_string_increase(o,1);
	o->v[o->l]='"';
	o->l++;
	for (sll_string_length_t i=0;i<l;i++){
		sll_char_t v=*(c+i);
		if (v=='\"'||v=='\''||v=='\\'){
			sll_string_increase(o,2);
			o->v[o->l]='\\';
			o->v[o->l+1]=v;
			o->l+=2;
		}
		else if (v=='\t'){
			sll_string_increase(o,2);
			o->v[o->l]='\\';
			o->v[o->l+1]='t';
			o->l+=2;
		}
		else if (v=='\n'){
			sll_string_increase(o,2);
			o->v[o->l]='\\';
			o->v[o->l+1]='n';
			o->l+=2;
		}
		else if (v=='\v'){
			sll_string_increase(o,2);
			o->v[o->l]='\\';
			o->v[o->l+1]='v';
			o->l+=2;
		}
		else if (v=='\f'){
			sll_string_increase(o,2);
			o->v[o->l]='\\';
			o->v[o->l+1]='f';
			o->l+=2;
		}
		else if (v=='\r'){
			sll_string_increase(o,2);
			o->v[o->l]='\\';
			o->v[o->l+1]='r';
			o->l+=2;
		}
		else if (SLL_STRING_HEX_ESCAPE(v)){
			sll_string_increase(o,6);
			o->v[o->l]='\\';
			o->v[o->l+1]='u';
			o->v[o->l+2]='0';
			o->v[o->l+3]='0';
			o->v[o->l+4]=(v>>4)+((v>>4)>9?87:48);
			o->v[o->l+5]=(v&15)+((v&15)>9?87:48);
			o->l+=6;
		}
		else{
			sll_string_increase(o,1);
			o->v[o->l]=v;
			o->l++;
		}
	}
	sll_string_increase(o,1);
	o->v[o->l]='"';
	o->l++;
}



static void _stringify_object(sll_object_t* o,sll_string_t* s){
	switch (o->t){
		case SLL_OBJECT_TYPE_INT:
			{
				sll_integer_t v=o->dt.i;
				if (v<0){
					sll_string_increase(s,1);
					s->v[s->l]='-';
					s->l++;
					v=-v;
				}
				sll_string_length_t i=0;
				sll_char_t bf[20];
				do{
					bf[i]=v%10;
					i++;
					v/=10;
				} while (v);
				sll_string_increase(s,i);
				while (i){
					i--;
					s->v[s->l]=bf[i]+48;
					s->l++;
				}
				return;
			}
		case SLL_OBJECT_TYPE_FLOAT:
			{
				sll_char_t bf[256];
				sll_string_length_t l=snprintf((char*)bf,256,"%.16lf",o->dt.f);
				sll_string_increase(s,l);
				sll_copy_data(bf,l,s->v+s->l);
				s->l+=l;
				return;
			}
		case SLL_OBJECT_TYPE_CHAR:
			_stringify_string(&(o->dt.c),1,s);
			return;
		case SLL_OBJECT_TYPE_COMPLEX:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_STRING:
			_stringify_string(o->dt.s.v,o->dt.s.l,s);
			return;
		case SLL_OBJECT_TYPE_ARRAY:
			sll_string_increase(s,1);
			s->v[s->l]='[';
			s->l++;
			for (sll_array_length_t i=0;i<o->dt.a.l;i++){
				if (i){
					sll_string_increase(s,1);
					s->v[s->l]=',';
					s->l++;
				}
				_stringify_object(o->dt.a.v[i],s);
			}
			sll_string_increase(s,1);
			s->v[s->l]=']';
			s->l++;
			return;
		case SLL_OBJECT_TYPE_MAP:
			sll_string_increase(s,1);
			s->v[s->l]='{';
			s->l++;
			for (sll_map_length_t i=0;i<o->dt.m.l;i++){
				if (i){
					sll_string_increase(s,1);
					s->v[s->l]=',';
					s->l++;
				}
				sll_object_t* k=sll_operator_cast(o->dt.m.v[i<<1],sll_static_int[SLL_OBJECT_TYPE_STRING]);
				_stringify_string(k->dt.s.v,k->dt.s.l,s);
				SLL_RELEASE(k);
				sll_string_increase(s,1);
				s->v[s->l]=':';
				s->l++;
				_stringify_object(o->dt.m.v[(i<<1)+1],s);
			}
			sll_string_increase(s,1);
			s->v[s->l]='}';
			s->l++;
			return;
	}
	SLL_ASSERT(o->t!=SLL_OBJECT_TYPE_MAP_KEYS&&o->t!=SLL_OBJECT_TYPE_MAP_VALUES);
	if (o->t==_json_null->t){
		sll_string_increase(s,4);
		s->v[s->l]='n';
		s->v[s->l+1]='u';
		s->v[s->l+2]='l';
		s->v[s->l+3]='l';
		s->l+=4;
		return;
	}
	if (o->t==_json_true->t){
		sll_string_increase(s,4);
		s->v[s->l]='t';
		s->v[s->l+1]='r';
		s->v[s->l+2]='u';
		s->v[s->l+3]='e';
		s->l+=4;
		return;
	}
	if (o->t==_json_false->t){
		sll_string_increase(s,5);
		s->v[s->l]='f';
		s->v[s->l+1]='a';
		s->v[s->l+2]='l';
		s->v[s->l+3]='s';
		s->v[s->l+4]='e';
		s->l+=5;
		return;
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_json__init(sll_object_t* null_obj,sll_object_t* true_obj,sll_object_t* false_obj){
	if (!_json_null){
		sll_register_cleanup(_release_data);
	}
	else{
		_release_data();
	}
	SLL_ACQUIRE(null_obj);
	SLL_ACQUIRE(true_obj);
	SLL_ACQUIRE(false_obj);
	_json_null=null_obj;
	_json_true=true_obj;
	_json_false=false_obj;
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t* sll_api_json_parse(const sll_string_t* str){
	sll_json_parser_state_t p=str->v;
	sll_object_t* o=_parse_json_as_object(&p);
	if (o){
		return o;
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_json_stringify(sll_object_t* json,sll_string_t* out){
	STRING_INIT_STACK(out);
	_stringify_object(json,out);
	sll_allocator_move((void**)(&(out->v)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(out);
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
		if (STRING_EQUAL(&(e->k),k)){
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
	json_number_t n;
	if (_parse_number(c,p,&n)==JSON_NUMBER_INT){
		o->t=SLL_JSON_OBJECT_TYPE_INTEGER;
		o->dt.i=n.i;
	}
	else{
		o->t=SLL_JSON_OBJECT_TYPE_FLOAT;
		o->dt.f=n.f;
	}
	return 1;
}

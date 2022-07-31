#include <sll/_internal/api/json.h>
#include <sll/_internal/common.h>
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



static sll_object_t _json_null=NULL;
static sll_object_t _json_true=NULL;
static sll_object_t _json_false=NULL;



static void _release_data(void){
	sll_gc_remove_root(_json_null);
	SLL_RELEASE(_json_null);
	_json_null=NULL;
	sll_gc_remove_root(_json_true);
	SLL_RELEASE(_json_true);
	_json_true=NULL;
	sll_gc_remove_root(_json_false);
	SLL_RELEASE(_json_false);
	_json_false=NULL;
}



static void _parse_json_string(sll_json_parser_state_t* state,sll_string_t* out){
	STRING_INIT_STACK(out);
	sll_char_t char_=**state;
	(*state)++;
	while (char_!='\"'){
		sll_string_increase(out,1);
		if (char_!='\\'){
			out->data[out->length]=char_;
		}
		else{
			char_=**state;
			(*state)++;
			if (char_=='\\'||char_=='\''||char_=='"'){
				out->data[out->length]=char_;
			}
			else if (char_=='b'){
				out->data[out->length]=8;
			}
			else if (char_=='f'){
				out->data[out->length]=12;
			}
			else if (char_=='n'){
				out->data[out->length]=10;
			}
			else if (char_=='r'){
				out->data[out->length]=13;
			}
			else if (char_=='t'){
				out->data[out->length]=9;
			}
			else if (char_=='v'){
				out->data[out->length]=11;
			}
			else if (char_=='u'){
				sll_wide_char_t v=0;
				for (unsigned int i=0;i<4;i++){
					char_=**state;
					v=(v<<4)|((__SLL_U16)(char_>47&&char_<58?char_-48:(char_>64&&char_<71?char_-55:char_-87)));
				}
				if (v>255){
					SLL_UNIMPLEMENTED();
				}
				out->data[out->length]=v&0xff;
			}
			else{
				SLL_UNIMPLEMENTED();
			}
		}
		out->length++;
		char_=**state;
		(*state)++;
	}
	sll_allocator_move((void**)(&(out->data)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(out);
}



static sll_bool_t _parse_number(sll_char_t char_,sll_json_parser_state_t* state,json_number_t* out){
	sll_bool_t neg=0;
	if (char_=='+'){
		char_=**state;
		(*state)++;
	}
	else if (char_=='-'){
		neg=1;
		char_=**state;
		(*state)++;
	}
	sll_float_t v=0;
	while (char_>47&&char_<58){
		v=v*10+(char_-48);
		char_=**state;
		(*state)++;
	}
	if (char_!='.'&&char_!='e'&&char_!='E'){
		(*state)--;
		out->int_=(sll_integer_t)(neg?-v:v);
		return JSON_NUMBER_INT;
	}
	if (char_=='.'){
		sll_float_t pw=0.1;
		char_=**state;
		(*state)++;
		while (char_>47&&char_<58){
			v+=pw*(char_-48);
			pw*=0.1;
			char_=**state;
			(*state)++;
		}
	}
	if (char_=='e'||char_=='E'){
		char_=**state;
		(*state)++;
		sll_bool_t neg_pw=0;
		if (char_=='+'){
			char_=**state;
			(*state)++;
		}
		else if (char_=='-'){
			neg_pw=1;
			char_=**state;
			(*state)++;
		}
		sll_integer_t pw=0;
		while (char_>47&&char_<58){
			pw=pw*10+(char_-48);
			char_=**state;
			(*state)++;
		}
		v*=pow(10,(sll_float_t)(neg_pw?-pw:pw));
	}
	(*state)--;
	out->float_=(neg?-v:v);
	return JSON_NUMBER_FLOAT;
}



static sll_object_t _parse_json_as_object(sll_json_parser_state_t* state){
	sll_char_t char_=**state;
	(*state)++;
	while (char_==' '||char_=='\t'||char_=='\n'||char_=='\r'){
		char_=**state;
		(*state)++;
	}
	if (!char_){
		return SLL_ACQUIRE_STATIC_INT(0);
	}
	if (char_=='{'){
		sll_object_t out=sll_map_to_object(NULL);
		sll_map_t* m=&(out->data.map);
		while (1){
			char_=**state;
			(*state)++;
			while (char_!='\"'){
				if (char_=='}'){
					return out;
				}
				if (char_!=' '&&char_!='\t'&&char_!='\n'&&char_!='\r'){
					SLL_RELEASE(out);
					return NULL;
				}
				char_=**state;
				(*state)++;
			}
			m->length++;
			m->data=sll_reallocate(m->data,(m->length<<1)*sizeof(sll_object_t));
			sll_object_t k=sll_create_object(SLL_OBJECT_TYPE_STRING);
			m->data[(m->length-1)<<1]=k;
			_parse_json_string(state,&(k->data.string));
			char_=**state;
			(*state)++;
			while (char_!=':'){
				char_=**state;
				(*state)++;
			}
			sll_object_t v=_parse_json_as_object(state);
			if (!v){
				m->data[m->length-1]=SLL_ACQUIRE_STATIC_INT(0);
				SLL_RELEASE(out);
				return NULL;
			}
			m->data[((m->length-1)<<1)+1]=v;
			char_=**state;
			(*state)++;
			while (char_!=','){
				if (char_=='}'){
					return out;
				}
				if (char_!=' '&&char_!='\t'&&char_!='\n'&&char_!='\r'){
					SLL_RELEASE(out);
					return NULL;
				}
				char_=**state;
				(*state)++;
			}
		}
	}
	if (char_=='['){
		sll_object_t out=sll_array_to_object(NULL);
		sll_array_t* a=&(out->data.array);
		while (char_==' '||char_=='\t'||char_=='\n'||char_=='\r'){
			char_=**state;
			(*state)++;
		}
		if (**state==']'){
			(*state)++;
			return out;
		}
		while (1){
			sll_object_t k=_parse_json_as_object(state);
			if (!k){
				SLL_RELEASE(out);
				return NULL;
			}
			a->length++;
			sll_allocator_resize((void**)(&(a->data)),a->length*sizeof(sll_object_t));
			a->data[a->length-1]=k;
			char_=**state;
			(*state)++;
			while (char_!=','){
				if (char_==']'){
					return out;
				}
				if (char_!=' '&&char_!='\t'&&char_!='\n'&&char_!='\r'){
					SLL_RELEASE(out);
					return NULL;
				}
				char_=**state;
				(*state)++;
			}
		}
	}
	if (char_=='\"'){
		sll_object_t out=sll_create_object(SLL_OBJECT_TYPE_STRING);
		_parse_json_string(state,&(out->data.string));
		return out;
	}
	if (char_=='t'&&**state=='r'&&*((*state)+1)=='u'&&*((*state)+2)=='e'){
		(*state)+=3;
		SLL_ACQUIRE(_json_true);
		return _json_true;
	}
	if (char_=='f'&&**state=='a'&&*((*state)+1)=='l'&&*((*state)+2)=='s'&&*((*state)+3)=='e'){
		(*state)+=4;
		SLL_ACQUIRE(_json_false);
		return _json_false;
	}
	if (char_=='n'&&**state=='u'&&*((*state)+1)=='l'&&*((*state)+2)=='l'){
		(*state)+=3;
		SLL_ACQUIRE(_json_null);
		return _json_null;
	}
	if ((char_<48||char_>57)&&char_!='.'&&char_!='e'&&char_!='E'&&char_!='-'&&char_!='+'){
		return NULL;
	}
	json_number_t n;
	return (_parse_number(char_,state,&n)==JSON_NUMBER_INT?sll_int_to_object(n.int_):sll_float_to_object(n.float_));
}



static void _stringify_string(const sll_char_t* char_,sll_string_length_t l,sll_string_t* out){
	sll_string_increase(out,1);
	out->data[out->length]='"';
	out->length++;
	for (sll_string_length_t i=0;i<l;i++){
		sll_char_t v=*(char_+i);
		if (v=='\"'||v=='\''||v=='\\'){
			sll_string_increase(out,2);
			out->data[out->length]='\\';
			out->data[out->length+1]=v;
			out->length+=2;
		}
		else if (v=='\t'){
			sll_string_increase(out,2);
			out->data[out->length]='\\';
			out->data[out->length+1]='t';
			out->length+=2;
		}
		else if (v=='\n'){
			sll_string_increase(out,2);
			out->data[out->length]='\\';
			out->data[out->length+1]='n';
			out->length+=2;
		}
		else if (v=='\v'){
			sll_string_increase(out,2);
			out->data[out->length]='\\';
			out->data[out->length+1]='v';
			out->length+=2;
		}
		else if (v=='\f'){
			sll_string_increase(out,2);
			out->data[out->length]='\\';
			out->data[out->length+1]='f';
			out->length+=2;
		}
		else if (v=='\r'){
			sll_string_increase(out,2);
			out->data[out->length]='\\';
			out->data[out->length+1]='r';
			out->length+=2;
		}
		else if (SLL_STRING_HEX_ESCAPE(v)){
			sll_string_increase(out,6);
			out->data[out->length]='\\';
			out->data[out->length+1]='u';
			out->data[out->length+2]='0';
			out->data[out->length+3]='0';
			out->data[out->length+4]=(v>>4)+((v>>4)>9?87:48);
			out->data[out->length+5]=(v&15)+((v&15)>9?87:48);
			out->length+=6;
		}
		else{
			sll_string_increase(out,1);
			out->data[out->length]=v;
			out->length++;
		}
	}
	sll_string_increase(out,1);
	out->data[out->length]='"';
	out->length++;
}



static void _stringify_object(sll_object_t out,sll_string_t* s){
	switch (out->type){
		case SLL_OBJECT_TYPE_INT:
			{
				sll_integer_t v=out->data.int_;
				if (v<0){
					sll_string_increase(s,1);
					s->data[s->length]='-';
					s->length++;
					v=-v;
				}
				sll_string_length_t i=0;
				sll_char_t buffer[20];
				do{
					buffer[i]=v%10;
					i++;
					v/=10;
				} while (v);
				sll_string_increase(s,i);
				while (i){
					i--;
					s->data[s->length]=buffer[i]+48;
					s->length++;
				}
				return;
			}
		case SLL_OBJECT_TYPE_FLOAT:
			{
				sll_char_t buffer[256];
				sll_string_length_t l=snprintf((char*)buffer,256,"%.16lf",out->data.float_);
				sll_string_increase(s,l);
				sll_copy_data(buffer,l,s->data+s->length);
				s->length+=l;
				return;
			}
		case SLL_OBJECT_TYPE_CHAR:
			_stringify_string(&(out->data.char_),1,s);
			return;
		case SLL_OBJECT_TYPE_COMPLEX:
			SLL_UNIMPLEMENTED();
		case SLL_OBJECT_TYPE_STRING:
			_stringify_string(out->data.string.data,out->data.string.length,s);
			return;
		case SLL_OBJECT_TYPE_ARRAY:
			sll_string_increase(s,1);
			s->data[s->length]='[';
			s->length++;
			for (sll_array_length_t i=0;i<out->data.array.length;i++){
				if (i){
					sll_string_increase(s,1);
					s->data[s->length]=',';
					s->length++;
				}
				_stringify_object(out->data.array.data[i],s);
			}
			sll_string_increase(s,1);
			s->data[s->length]=']';
			s->length++;
			return;
		case SLL_OBJECT_TYPE_MAP:
			sll_string_increase(s,1);
			s->data[s->length]='{';
			s->length++;
			for (sll_map_length_t i=0;i<out->data.map.length;i++){
				if (i){
					sll_string_increase(s,1);
					s->data[s->length]=',';
					s->length++;
				}
				sll_object_t k=sll_operator_cast(out->data.map.data[i<<1],sll_static_int[SLL_OBJECT_TYPE_STRING]);
				_stringify_string(k->data.string.data,k->data.string.length,s);
				SLL_RELEASE(k);
				sll_string_increase(s,1);
				s->data[s->length]=':';
				s->length++;
				_stringify_object(out->data.map.data[(i<<1)+1],s);
			}
			sll_string_increase(s,1);
			s->data[s->length]='}';
			s->length++;
			return;
	}
	SLL_ASSERT(out->type!=SLL_OBJECT_TYPE_MAP_KEYS&&out->type!=SLL_OBJECT_TYPE_MAP_VALUES);
	if (out->type==_json_null->type){
		sll_string_increase(s,4);
		s->data[s->length]='n';
		s->data[s->length+1]='u';
		s->data[s->length+2]='l';
		s->data[s->length+3]='l';
		s->length+=4;
		return;
	}
	if (out->type==_json_true->type){
		sll_string_increase(s,4);
		s->data[s->length]='t';
		s->data[s->length+1]='r';
		s->data[s->length+2]='u';
		s->data[s->length+3]='e';
		s->length+=4;
		return;
	}
	if (out->type==_json_false->type){
		sll_string_increase(s,5);
		s->data[s->length]='f';
		s->data[s->length+1]='a';
		s->data[s->length+2]='l';
		s->data[s->length+3]='s';
		s->data[s->length+4]='e';
		s->length+=5;
		return;
	}
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_json__init(sll_object_t null_obj,sll_object_t true_obj,sll_object_t false_obj){
	if (!_json_null){
		sll_register_cleanup(_release_data,SLL_CLEANUP_TYPE_VM);
	}
	else{
		_release_data();
	}
	SLL_ACQUIRE(null_obj);
	_json_null=null_obj;
	sll_gc_add_root(_json_null,0);
	SLL_ACQUIRE(true_obj);
	_json_true=true_obj;
	sll_gc_add_root(_json_true,0);
	SLL_ACQUIRE(false_obj);
	_json_false=false_obj;
	sll_gc_add_root(_json_false,0);
}



__SLL_EXTERNAL __SLL_API_CALL __SLL_CHECK_OUTPUT sll_object_t sll_api_json_parse(const sll_string_t* string){
	if (string->length){
		sll_json_parser_state_t state=string->data;
		sll_object_t out=_parse_json_as_object(&state);
		if (out){
			return out;
		}
	}
	return SLL_ACQUIRE_STATIC_INT(0);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_json_stringify(sll_object_t json,sll_string_t* out){
	STRING_INIT_STACK(out);
	_stringify_object(json,out);
	sll_allocator_move((void**)(&(out->data)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL void sll_free_json_object(sll_json_object_t* json){
	if (json->type==SLL_JSON_OBJECT_TYPE_STRING){
		sll_free_string(&(json->data.string));
	}
	else if (json->type==SLL_JSON_OBJECT_TYPE_ARRAY){
		for (sll_json_array_length_t i=0;i<json->data.array.length;i++){
			sll_free_json_object(json->data.array.data+i);
		}
		sll_deallocate(json->data.array.data);
	}
	else if (json->type==SLL_JSON_OBJECT_TYPE_MAP){
		for (sll_json_map_length_t i=0;i<json->data.map.length;i++){
			sll_json_map_keypair_t* e=json->data.map.data+i;
			sll_free_string(&(e->key));
			sll_free_json_object(&(e->value));
		}
		sll_deallocate(json->data.map.data);
	}
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_json_object_t* sll_json_get_by_key(sll_json_object_t* json,const sll_string_t* key){
	SLL_ASSERT(json->type==SLL_JSON_OBJECT_TYPE_MAP);
	for (sll_json_map_length_t i=0;i<json->data.map.length;i++){
		sll_json_map_keypair_t* e=json->data.map.data+i;
		if (STRING_EQUAL(&(e->key),key)){
			return &(e->value);
		}
	}
	return NULL;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_json_parse(sll_json_parser_state_t* state,sll_json_object_t* out){
	sll_char_t char_=**state;
	(*state)++;
	while (char_==' '||char_=='\t'||char_=='\n'||char_=='\r'){
		char_=**state;
		(*state)++;
	}
	if (char_=='{'){
		out->type=SLL_JSON_OBJECT_TYPE_MAP;
		out->data.map.length=0;
		out->data.map.data=NULL;
		while (1){
			char_=**state;
			(*state)++;
			while (char_!='\"'){
				if (char_=='}'){
					return 1;
				}
				if (char_!=' '&&char_!='\t'&&char_!='\n'&&char_!='\r'){
					return 0;
				}
				char_=**state;
				(*state)++;
			}
			out->data.map.length++;
			out->data.map.data=sll_reallocate(out->data.map.data,out->data.map.length*sizeof(sll_json_map_keypair_t));
			sll_json_map_keypair_t* k=out->data.map.data+out->data.map.length-1;
			_parse_json_string(state,&(k->key));
			char_=**state;
			(*state)++;
			while (char_!=':'){
				char_=**state;
				(*state)++;
			}
			if (!sll_json_parse(state,&(k->value))){
				return 0;
			}
			char_=**state;
			(*state)++;
			while (char_!=','){
				if (char_=='}'){
					return 1;
				}
				if (char_!=' '&&char_!='\t'&&char_!='\n'&&char_!='\r'){
					return 0;
				}
				char_=**state;
				(*state)++;
			}
		}
	}
	if (char_=='['){
		out->type=SLL_JSON_OBJECT_TYPE_ARRAY;
		out->data.array.length=0;
		out->data.array.data=NULL;
		while (char_==' '||char_=='\t'||char_=='\n'||char_=='\r'){
			char_=**state;
			(*state)++;
		}
		if (**state==']'){
			(*state)++;
			return 1;
		}
		while (1){
			out->data.array.length++;
			out->data.array.data=sll_reallocate(out->data.array.data,out->data.array.length*sizeof(sll_json_object_t));
			if (!sll_json_parse(state,out->data.array.data+out->data.array.length-1)){
				return 0;
			}
			char_=**state;
			(*state)++;
			while (char_!=','){
				if (char_==']'){
					return 1;
				}
				if (char_!=' '&&char_!='\t'&&char_!='\n'&&char_!='\r'){
					return 0;
				}
				char_=**state;
				(*state)++;
			}
		}
	}
	if (char_=='\"'){
		out->type=SLL_JSON_OBJECT_TYPE_STRING;
		_parse_json_string(state,&(out->data.string));
		return 1;
	}
	if (char_=='t'&&**state=='r'&&*((*state)+1)=='u'&&*((*state)+2)=='e'){
		(*state)+=3;
		out->type=SLL_JSON_OBJECT_TYPE_TRUE;
		return 1;
	}
	if (char_=='f'&&**state=='a'&&*((*state)+1)=='l'&&*((*state)+2)=='s'&&*((*state)+3)=='e'){
		(*state)+=4;
		out->type=SLL_JSON_OBJECT_TYPE_FALSE;
		return 1;
	}
	if (char_=='n'&&**state=='u'&&*((*state)+1)=='l'&&*((*state)+2)=='l'){
		(*state)+=3;
		out->type=SLL_JSON_OBJECT_TYPE_NULL;
		return 1;
	}
	if ((char_<48||char_>57)&&char_!='.'&&char_!='e'&&char_!='E'&&char_!='-'&&char_!='+'){
		return 0;
	}
	json_number_t n;
	if (_parse_number(char_,state,&n)==JSON_NUMBER_INT){
		out->type=SLL_JSON_OBJECT_TYPE_INTEGER;
		out->data.int_=n.int_;
	}
	else{
		out->type=SLL_JSON_OBJECT_TYPE_FLOAT;
		out->data.float_=n.float_;
	}
	return 1;
}

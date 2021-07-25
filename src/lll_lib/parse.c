#include <lll_lib.h>
#include <_lll_lib_internal.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>



uint8_t _read_single_char(lll_input_data_stream_t* is,uint32_t st,lll_error_t* e,char* o){
	int c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
	if (c==LLL_END_OF_DATA){
		e->t=LLL_ERROR_UNMATCHED_QUOTES;
		e->dt.r.off=st;
		e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
		return READ_SINGLE_CHAR_ERROR;
	}
	if (c=='\r'||c=='\n'){
		e->t=LLL_ERROR_UNMATCHED_QUOTES;
		e->dt.r.off=st;
		e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
		return READ_SINGLE_CHAR_ERROR;
	}
	if (c=='\\'){
		c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
		if (c=='\r'||c=='\n'){
			e->t=LLL_ERROR_UNMATCHED_QUOTES;
			e->dt.r.off=st;
			e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
			return READ_SINGLE_CHAR_ERROR;
		}
		if (c==LLL_END_OF_DATA){
			e->t=LLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE;
			e->dt.r.off=st;
			e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
			return READ_SINGLE_CHAR_ERROR;
		}
		if (c=='\''||c=='"'||c=='\\'){
			*o=c;
			return READ_SINGLE_CHAR_OK;
		}
		else if (c=='x'){
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c==LLL_END_OF_DATA){
				e->t=LLL_ERROR_UNMATCHED_QUOTES;
				e->dt.r.off=st;
				e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
				return READ_SINGLE_CHAR_ERROR;
			}
			if (c>96){
				c-=32;
			}
			if (c<48||(c>57&&c<65)||c>70){
				if (c=='\r'||c=='\n'){
					e->t=LLL_ERROR_UNMATCHED_QUOTES;
					e->dt.r.off=st;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
					return READ_SINGLE_CHAR_ERROR;
				}
				e->t=LLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				return READ_SINGLE_CHAR_ERROR;
			}
			uint8_t v=(c>64?c-55:c-48)<<4;
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c==LLL_END_OF_DATA){
				e->t=LLL_ERROR_UNMATCHED_QUOTES;
				e->dt.r.off=st;
				e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
				return READ_SINGLE_CHAR_ERROR;
			}
			if (c>96){
				c-=32;
			}
			if (c<48||(c>57&&c<65)||c>70){
				if (c=='\r'||c=='\n'){
					e->t=LLL_ERROR_UNMATCHED_QUOTES;
					e->dt.r.off=st;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
					return READ_SINGLE_CHAR_ERROR;
				}
				e->t=LLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				return READ_SINGLE_CHAR_ERROR;
			}
			c=(c>64?c-55:c-48)|v;
		}
		else if (c=='t'){
			c='\t';
		}
		else if (c=='n'){
			c='\n';
		}
		else if (c=='v'){
			c='\v';
		}
		else if (c=='f'){
			c='\f';
		}
		else if (c=='r'){
			c='\r';
		}
		else{
			e->t=LLL_ERROR_UNKNOWN_ESCAPE_CHARACTER;
			e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
			e->dt.r.sz=1;
			return READ_SINGLE_CHAR_ERROR;
		}
	}
	*o=c;
	return READ_SINGLE_CHAR_OK;
}



uint8_t _read_object_internal(lll_compilation_data_t* c_dt,int c,const scope_data_t* l_sc,lll_error_t* e){
	scope_data_t n_l_sc={
		NULL
	};
	const scope_data_t* b_l_sc=NULL;
	lll_input_data_stream_t* is=c_dt->is;
	uint32_t st_off=UINT32_MAX;
	int ec=-1;
	lll_object_t* o=NULL;
	lll_arg_count_t* ac=NULL;
	lll_statement_count_t* sc=NULL;
	while (c!=LLL_END_OF_DATA){
		if ((c>8&&c<14)||c==' '){
			while ((c>8&&c<14)||c==' '){
				c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			}
		}
		else if (c==';'){
			while (c!='\n'&&c!='\r'&&c!=LLL_END_OF_DATA){
				c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			}
		}
		else if (c=='|'){
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c!='#'){
				e->t=LLL_ERROR_UNEXPECTED_CHARACTER;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-3;
				e->dt.r.sz=1;
				if (n_l_sc.m){
					free(n_l_sc.m);
				}
				return LLL_RETURN_ERROR;
			}
			int lc=c;
			do{
				lc=c;
				c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c==LLL_END_OF_DATA){
					e->t=LLL_ERROR_UNMATCHED_OPEN_QUOTE;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-3;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
			} while (c!='|'||lc!='#');
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
		}
		else if (c==')'){
			if (n_l_sc.m){
				free(n_l_sc.m);
			}
			if (!o||ec!=')'){
				e->t=LLL_ERROR_UNMATCHED_CLOSE_PARENTHESES;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				return LLL_RETURN_ERROR;
			}
			if (LLL_IS_OBJECT_UNKNOWN(o)){
				o->t=LLL_OBJECT_TYPE_NIL;
			}
			else if (LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_FOR){
				if (*ac<2){
					e->t=LLL_ERROR_FOR_NOT_ENOUGH_ARGUMENTS;
					e->dt.r.off=st_off;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st_off-1;
					return LLL_RETURN_ERROR;
				}
			}
			else if (LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_SET){
				if (*ac<2){
					e->t=LLL_ERROR_SET_NOT_ENOUGH_ARGUMENTS;
					e->dt.r.off=st_off;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st_off-1;
					return LLL_RETURN_ERROR;
				}
				lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_operator_object_t));
				while (LLL_GET_OBJECT_TYPE(a)==LLL_OBJECT_TYPE_DEBUG_DATA){
					a=LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)a);
				}
				if (LLL_GET_OBJECT_TYPE(a)!=LLL_OBJECT_TYPE_IDENTIFIER){
					e->t=LLL_ERROR_SET_NO_INDENTIFIER;
					e->dt.r.off=st_off;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st_off;
					return LLL_RETURN_ERROR;
				}
			}
			else if (LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_FUNC){
				uint32_t off=sizeof(lll_function_object_t);
				lll_arg_count_t i=0;
				for (;i<*ac;i++){
					lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
					while (LLL_GET_OBJECT_TYPE(a)==LLL_OBJECT_TYPE_DEBUG_DATA){
						a=LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)a);
						off+=sizeof(lll_debug_object_t);
					}
					if (LLL_GET_OBJECT_TYPE(a)!=LLL_OBJECT_TYPE_IDENTIFIER){
						break;
					}
					off+=sizeof(lll_identifier_object_t);
				}
				*ac-=i;
				((lll_function_object_t*)o)->id=c_dt->f_dt.l;
				c_dt->f_dt.l++;
				void* tmp=realloc(c_dt->f_dt.dt,c_dt->f_dt.l*sizeof(lll_function_t*));
				if (!tmp){
					ASSERT(!"Unable to Reallocate Function Array");
				}
				c_dt->f_dt.dt=tmp;
				lll_function_t* f=malloc(sizeof(lll_function_t)+i*sizeof(lll_identifier_index_t));
				f->off=(uint32_t)(((uint64_t)(void*)o)-((uint64_t)(void*)_bf));
				f->al=i;
				off=sizeof(lll_function_object_t);
				uint32_t eoff=sizeof(lll_function_object_t);
				for (lll_arg_count_t j=0;j<i;j++){
					lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
					while (LLL_GET_OBJECT_TYPE(a)==LLL_OBJECT_TYPE_DEBUG_DATA){
						a=LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)a);
						off+=sizeof(lll_debug_object_t);
					}
					if (LLL_GET_OBJECT_TYPE(a)!=LLL_OBJECT_TYPE_IDENTIFIER){
						break;
					}
					f->a[j]=((lll_identifier_object_t*)a)->idx;
					off+=sizeof(lll_identifier_object_t);
					eoff=off;
				}
				for (off=sizeof(lll_function_object_t);off<eoff;off+=sizeof(lll_object_type_t)){
					LLL_SET_OBJECT_NOP(o,off);
				}
				*(c_dt->f_dt.dt+c_dt->f_dt.l-1)=f;
			}
			return LLL_RETURN_NO_ERROR;
		}
		else if (c=='}'){
			if (n_l_sc.m){
				free(n_l_sc.m);
			}
			if (!o||ec!='}'){
				e->t=LLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				return LLL_RETURN_ERROR;
			}
			return LLL_RETURN_NO_ERROR;
		}
		else if (c=='('||c=='{'){
			if (!o){
				if (!lll_insert_debug_object(is,e)){
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				st_off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				o=(lll_object_t*)(_bf+_bf_off);
				_bf_off+=(c=='('?sizeof(lll_object_t):sizeof(lll_operation_list_object_t));
				if (_bf_off>=_bf_sz){
					e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				o->t=(c=='('?LLL_OBJECT_TYPE_UNKNOWN:LLL_OBJECT_TYPE_OPERATION_LIST|LLL_OBJECT_TYPE_CONST);
				ec=(c=='('?')':'}');
				if (c=='{'){
					sc=&(((lll_operation_list_object_t*)o)->sc);
					*sc=0;
				}
			}
			else{
				if (LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_IF&&!((*ac)&1)){
					if (n_l_sc.m){
						free(n_l_sc.m);
						l_sc=b_l_sc;
					}
					n_l_sc.l_sc=c_dt->_n_sc_id;
					n_l_sc.ml=(n_l_sc.l_sc+65)>>6;
					n_l_sc.m=malloc(n_l_sc.ml*sizeof(uint64_t));
					n_l_sc.m[n_l_sc.ml-1]=0;
					for (uint32_t i=0;i<l_sc->ml;i++){
						*(n_l_sc.m+i)=*(l_sc->m+i);
					}
					n_l_sc.m[n_l_sc.ml-1]|=1ull<<(n_l_sc.l_sc&63);
					c_dt->_n_sc_id++;
					b_l_sc=l_sc;
					l_sc=&n_l_sc;
				}
				uint32_t arg_s=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				if (LLL_IS_OBJECT_UNKNOWN(o)){
					e->t=LLL_ERROR_NO_SYMBOL;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				lll_object_t* so=(lll_object_t*)(_bf+_bf_off);
				if (!_read_object_internal(c_dt,c,l_sc,e)){
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				while (LLL_GET_OBJECT_TYPE(so)==LLL_OBJECT_TYPE_DEBUG_DATA){
					so=LLL_GET_DEBUG_OBJECT_CHILD((lll_debug_object_t*)so);
				}
				if (LLL_IS_OBJECT_TYPE_NOT_TYPE(so)&&LLL_GET_OBJECT_TYPE(so)!=LLL_OBJECT_TYPE_FUNC&&!LLL_IS_OBJECT_CONST(so)){
					o->t&=~LLL_OBJECT_TYPE_CONST;
				}
				if (LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_OPERATION_LIST){
					if (*sc==UINT16_MAX){
						e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
						e->dt.r.off=arg_s;
						e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
						if (n_l_sc.m){
							free(n_l_sc.m);
						}
						return LLL_RETURN_ERROR;
					}
					(*sc)++;
				}
				else{
					if (*ac==UINT8_MAX){
						e->t=LLL_ERROR_TOO_MANY_ARGUMENTS;
						e->dt.r.off=arg_s;
						e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
						if (n_l_sc.m){
							free(n_l_sc.m);
						}
						return LLL_RETURN_ERROR;
					}
					(*ac)++;
				}
			}
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
		}
		else if (o&&LLL_IS_OBJECT_UNKNOWN(o)){
			char* str=(char*)(_bf+_bf_off);
			uint16_t sz=0;
_read_symbol:
			if (sz==UINT16_MAX){
				e->t=LLL_ERROR_SYMBOL_TOO_LONG;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
				e->dt.r.sz=sz;
				if (n_l_sc.m){
					free(n_l_sc.m);
				}
				return LLL_RETURN_ERROR;
			}
			*(str+sz)=c;
			sz++;
			if (sz+_bf_off>=_bf_sz){
				e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				if (n_l_sc.m){
					free(n_l_sc.m);
				}
				return LLL_RETURN_ERROR;
			}
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c==LLL_END_OF_DATA){
				break;
			}
			if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='('&&c!='}'&&c!='{')){
				goto _read_symbol;
			}
			if (sz==1){
				if (FAST_COMPARE(str,!)){
					o->t=LLL_OBJECT_TYPE_NOT;
				}
				else if (FAST_COMPARE(str,=)){
					o->t=LLL_OBJECT_TYPE_SET;
				}
				else if (FAST_COMPARE(str,?)){
					o->t=LLL_OBJECT_TYPE_IF;
				}
				else if (FAST_COMPARE(str,+)){
					o->t=LLL_OBJECT_TYPE_ADD;
				}
				else if (FAST_COMPARE(str,-)){
					o->t=LLL_OBJECT_TYPE_SUB;
				}
				else if (FAST_COMPARE(str,*)){
					o->t=LLL_OBJECT_TYPE_MULT;
				}
				else if (FAST_COMPARE(str,/)){
					o->t=LLL_OBJECT_TYPE_DIV;
				}
				else if (FAST_COMPARE(str,%)){
					o->t=LLL_OBJECT_TYPE_MOD;
				}
				else if (FAST_COMPARE(str,&)){
					o->t=LLL_OBJECT_TYPE_BIT_AND;
				}
				else if (FAST_COMPARE(str,|)){
					o->t=LLL_OBJECT_TYPE_BIT_OR;
				}
				else if (FAST_COMPARE(str,^)){
					o->t=LLL_OBJECT_TYPE_BIT_XOR;
				}
				else if (FAST_COMPARE(str,~)){
					o->t=LLL_OBJECT_TYPE_BIT_NOT;
				}
				else if (FAST_COMPARE(str,<)){
					o->t=LLL_OBJECT_TYPE_LESS;
				}
				else if (FAST_COMPARE(str,>)){
					o->t=LLL_OBJECT_TYPE_MORE;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==2){
				if (FAST_COMPARE(str,:,>)){
					o->t=LLL_OBJECT_TYPE_PRINT;
				}
				else if (FAST_COMPARE(str,<,:)){
					o->t=LLL_OBJECT_TYPE_INPUT;
				}
				else if (FAST_COMPARE(str,&,&)){
					o->t=LLL_OBJECT_TYPE_AND;
				}
				else if (FAST_COMPARE(str,|,|)){
					o->t=LLL_OBJECT_TYPE_OR;
				}
				else if (FAST_COMPARE(str,<,-)){
					o->t=LLL_OBJECT_TYPE_CALL;
				}
				else if (FAST_COMPARE(str,-,>)){
					o->t=LLL_OBJECT_TYPE_FOR;
				}
				else if (FAST_COMPARE(str,/,/)){
					o->t=LLL_OBJECT_TYPE_FLOOR_DIV;
				}
				else if (FAST_COMPARE(str,<,=)){
					o->t=LLL_OBJECT_TYPE_LESS_EQUAL;
				}
				else if (FAST_COMPARE(str,=,=)){
					o->t=LLL_OBJECT_TYPE_EQUAL;
				}
				else if (FAST_COMPARE(str,!,=)){
					o->t=LLL_OBJECT_TYPE_NOT_EQUAL;
				}
				else if (FAST_COMPARE(str,>,=)){
					o->t=LLL_OBJECT_TYPE_MORE_EQUAL;
				}
				else if (FAST_COMPARE(str,-,-)){
					o->t=LLL_OBJECT_TYPE_IMPORT;
					ac=&(((lll_import_object_t*)o)->ac);
					*ac=0;
					_bf_off+=sizeof(lll_import_object_t)-sizeof(lll_object_t);
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==3){
				if (FAST_COMPARE_3(str,FAST_COMPARE_COMMA,FAST_COMPARE_COMMA,FAST_COMPARE_COMMA)){
					o->t=LLL_OBJECT_TYPE_FUNC;
					ac=&(((lll_function_object_t*)o)->ac);
					*ac=0;
					_bf_off+=sizeof(lll_function_object_t)-sizeof(lll_object_t);
				}
				else{
					goto _unknown_symbol;
				}
			}
			else{
_unknown_symbol:
				e->t=LLL_ERROR_UNKNOWN_SYMBOL;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-1;
				e->dt.r.sz=sz;
				if (n_l_sc.m){
					free(n_l_sc.m);
				}
				return LLL_RETURN_ERROR;
			}
			if (!ac){
				ac=&(((lll_operator_object_t*)o)->ac);
				*ac=0;
				_bf_off+=sizeof(lll_operator_object_t)-sizeof(lll_object_t);
			}
			if (o->t==LLL_OBJECT_TYPE_FOR||o->t==LLL_OBJECT_TYPE_FUNC){
				n_l_sc.l_sc=c_dt->_n_sc_id;
				n_l_sc.ml=(n_l_sc.l_sc+65)>>6;
				n_l_sc.m=malloc(n_l_sc.ml*sizeof(uint64_t));
				n_l_sc.m[n_l_sc.ml-1]=0;
				for (uint32_t i=0;i<l_sc->ml;i++){
					*(n_l_sc.m+i)=*(l_sc->m+i);
				}
				n_l_sc.m[n_l_sc.ml-1]|=1ull<<(n_l_sc.l_sc&63);
				c_dt->_n_sc_id++;
				l_sc=&n_l_sc;
			}
			if (o->t!=LLL_OBJECT_TYPE_IMPORT){
				o->t|=LLL_OBJECT_TYPE_CONST;
			}
			if (_bf_off>=_bf_sz){
				e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				if (n_l_sc.m){
					free(n_l_sc.m);
				}
				return LLL_RETURN_ERROR;
			}
		}
		else{
			uint32_t arg_bf_off=_bf_off;
			if (!lll_insert_debug_object(is,e)){
				if (n_l_sc.m){
					free(n_l_sc.m);
				}
				return LLL_RETURN_ERROR;
			}
			uint32_t arg_s=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
			lll_object_t* arg=(lll_object_t*)(_bf+_bf_off);
			_bf_off+=sizeof(lll_object_t);
			if (c=='\''){
				_bf_off+=sizeof(lll_char_object_t)-sizeof(lll_object_t);
				if (_bf_off>=_bf_sz){
					e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				arg->t=LLL_OBJECT_TYPE_CHAR;
				uint8_t ce=_read_single_char(is,arg_s,e,&(((lll_char_object_t*)arg)->v));
				if (ce==READ_SINGLE_CHAR_ERROR){
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				if (((lll_char_object_t*)arg)->v=='\''){
					e->t=LLL_ERROR_EMPTY_CHAR_STRING;
					e->dt.r.off=arg_s;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c!='\''){
					e->t=LLL_ERROR_UNTERMINATED_CHAR_STRING;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			}
			else if (c=='"'){
				_bf_off+=sizeof(lll_string_object_t)-sizeof(lll_object_t);
				if (_bf_off>=_bf_sz){
					e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				arg->t=LLL_OBJECT_TYPE_STRING;
				uint32_t ln=0;
				char* s=((lll_string_object_t*)arg)->v;
				while (1){
					uint8_t ce=_read_single_char(is,arg_s,e,s+ln);
					if (ce==READ_SINGLE_CHAR_ERROR){
						if (n_l_sc.m){
							free(n_l_sc.m);
						}
						return LLL_RETURN_ERROR;
					}
					if (*(s+ln)=='"'){
						break;
					}
					ln++;
				}
				_bf_off+=ln;
				if (_bf_off>=_bf_sz){
					e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				((lll_string_object_t*)arg)->ln=ln;
				c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			}
			else if ((c>47&&c<58)||c=='-'){
				_bf_off+=sizeof(lll_integer_object_t)-sizeof(lll_object_t);
				if (_bf_off>=_bf_sz){
					e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				int8_t m=1;
				if (c=='-'){
					m=-1;
					c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
					if (c==LLL_END_OF_DATA){
						break;
					}
				}
				arg->t=LLL_OBJECT_TYPE_INT;
				lll_integer_object_t* i_arg=(lll_integer_object_t*)arg;
				int64_t v=0;
				if (c=='0'){
					c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
					if (c==LLL_END_OF_DATA){
						break;
					}
					if (c=='x'||c=='X'){
						c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
						if (c==LLL_END_OF_DATA){
							break;
						}
_hexadecimal:
						if (c>96){
							c-=32;
						}
						if (c<48||(c>57&&c<65)||c>70){
							e->t=LLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
							e->dt.r.sz=1;
							if (n_l_sc.m){
								free(n_l_sc.m);
							}
							return LLL_RETURN_ERROR;
						}
						v=(v<<4)+(c>64?c-55:c-48);
						c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
						if (c==LLL_END_OF_DATA){
							break;
						}
						if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='('&&c!='}'&&c!='{')){
							goto _hexadecimal;
						}
					}
					else if (c=='o'||c=='O'){
						c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
						if (c==LLL_END_OF_DATA){
							break;
						}
_octal:
						if (c<48||c>55){
							e->t=LLL_ERROR_UNKNOWN_OCTAL_CHARCTER;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
							e->dt.r.sz=1;
							if (n_l_sc.m){
								free(n_l_sc.m);
							}
							return LLL_RETURN_ERROR;
						}
						v=(v<<3)+(c-48);
						c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
						if (c==LLL_END_OF_DATA){
							break;
						}
						if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='('&&c!='}'&&c!='{')){
							goto _octal;
						}
					}
					else if (c=='b'||c=='B'){
						c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
						if (c==LLL_END_OF_DATA){
							break;
						}
_binary:
						if (c!=48&&c!=49){
							e->t=LLL_ERROR_UNKNOWN_BINARY_CHARCTER;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
							e->dt.r.sz=1;
							if (n_l_sc.m){
								free(n_l_sc.m);
							}
							return LLL_RETURN_ERROR;
						}
						v=(v<<1)+(c-48);
						c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
						if (c==LLL_END_OF_DATA){
							break;
						}
						if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='('&&c!='}'&&c!='{')){
							goto _binary;
						}
					}
					else if (c=='.'||c=='e'||c=='E'){
						goto _parse_float;
					}
					else if (c>47&&c<58){
						goto _decimal;
					}
					if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='('&&c!='}'&&c!='{')){
						e->t=LLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
						e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
						e->dt.r.sz=1;
						if (n_l_sc.m){
							free(n_l_sc.m);
						}
						return LLL_RETURN_ERROR;
					}
				}
				else{
_decimal:
					v=v*10+(c-48);
					c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
					if (c==LLL_END_OF_DATA){
						break;
					}
					if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='('&&c!='}'&&c!='{')){
						if (c=='.'||c=='e'||c=='E'){
							goto _parse_float;
						}
						if (c<48||c>57){
							e->t=LLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
							e->dt.r.sz=1;
							if (n_l_sc.m){
								free(n_l_sc.m);
							}
							return LLL_RETURN_ERROR;
						}
						goto _decimal;
					}
				}
				i_arg->v=v*m;
				goto _skip_float_parse;
_parse_float:;
				int16_t ex=0;
				if (c=='.'){
_float:
					c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
					if (c==LLL_END_OF_DATA){
						break;
					}
					if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='('&&c!='}'&&c!='{')){
						if (c=='e'||c=='E'){
							goto _parse_float_exponent;
						}
						if (c<48||c>57){
							e->t=LLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
							e->dt.r.sz=1;
							if (n_l_sc.m){
								free(n_l_sc.m);
							}
							return LLL_RETURN_ERROR;
						}
						ex--;
						v=v*10+(c-48);
						goto _float;
					}
				}
				else{
_parse_float_exponent:;
					int8_t em=1;
					int16_t ev=0;
					c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
					if (c==LLL_END_OF_DATA){
						break;
					}
					if (c=='-'){
						em=-1;
					}
					else if (c=='+');
					else{
						goto _add_exponent_char;
					}
_float_exponent:
					c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
					if (c==LLL_END_OF_DATA){
						break;
					}
_add_exponent_char:
					if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='('&&c!='}'&&c!='{')){
						if (c<48||c>57){
							e->t=LLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
							e->dt.r.sz=1;
							if (n_l_sc.m){
								free(n_l_sc.m);
							}
							return LLL_RETURN_ERROR;
						}
						ev=ev*10+(c-48);
						goto _float_exponent;
					}
					ex+=ev*em;
				}
				arg->t=LLL_OBJECT_TYPE_FLOAT;
				((lll_float_object_t*)arg)->v=((double)v)*pow(5,ex)*pow(2,ex)*m;
_skip_float_parse:;
			}
			else if ((c>64&&c<91)||c=='_'||(c>96&&c<123)){
				if (_bf_off>=_bf_sz){
					e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				arg->t=LLL_OBJECT_TYPE_IDENTIFIER;
				lll_identifier_object_t* iarg=(lll_identifier_object_t*)arg;
				char* str=(char*)(_bf+_bf_off);
				uint32_t sz=0;
_read_identifier:
				*(str+sz)=c;
				sz++;
				if (sz+_bf_off>=_bf_sz){
					e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c==LLL_END_OF_DATA){
					break;
				}
				if ((c>47&&c<58)||(c>64&&c<91)||c=='_'||(c>96&&c<123)){
					goto _read_identifier;
				}
				if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='('&&c!='}'&&c!='{')){
					e->t=LLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				if (sz==3&&FAST_COMPARE(str,n,i,l)){
					arg->t=LLL_OBJECT_TYPE_NIL;
				}
				else if (sz==4&&FAST_COMPARE(str,t,r,u,e)){
					arg->t=LLL_OBJECT_TYPE_TRUE;
				}
				else if (sz==5&&FAST_COMPARE(str,f,a,l,s,e)){
					arg->t=LLL_OBJECT_TYPE_FALSE;
				}
				else{
					_bf_off+=sizeof(lll_identifier_object_t)-sizeof(lll_object_t);
					if (_bf_off>=_bf_sz){
						e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
						e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
						e->dt.r.sz=1;
						if (n_l_sc.m){
							free(n_l_sc.m);
						}
						return LLL_RETURN_ERROR;
					}
					if (sz<=LLL_MAX_SHORT_IDENTIFIER_LENGTH){
						lll_identifier_list_t* k=c_dt->i_dt.s+sz-1;
						uint32_t mx_sc=UINT32_MAX;
						lll_identifier_index_t mx_i=0;
						for (uint32_t i=0;i<k->l;i++){
							lll_small_identifier_t* si=k->dt+i;
							for (uint8_t j=0;j<sz;j++){
								if (*(str+j)!=*(si->v+j)){
									goto _next_short_identifier;
								}
							}
							if (si->sc==l_sc->l_sc){
								iarg->idx=LLL_CREATE_IDENTIFIER(i,sz-1);
								goto _identifier_found;
							}
							else if ((l_sc->m[si->sc>>6]&(1ull<<(si->sc&0x3f)))&&(mx_sc==UINT32_MAX||si->sc>mx_sc)){
								mx_sc=si->sc;
								mx_i=LLL_CREATE_IDENTIFIER(i,sz-1);
							}
_next_short_identifier:;
						}
						if (mx_sc!=UINT32_MAX){
							iarg->idx=mx_i;
							goto _identifier_found;
						}
						if (LLL_GET_OBJECT_TYPE(o)!=LLL_OBJECT_TYPE_SET&&LLL_GET_OBJECT_TYPE(o)!=LLL_OBJECT_TYPE_FUNC){
							e->t=LLL_ERROR_UNKNOWN_IDENTIFIER;
							e->dt.r.off=arg_s;
							e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
							if (n_l_sc.m){
								free(n_l_sc.m);
							}
							return LLL_RETURN_ERROR;
						}
						k->l++;
						k->dt=realloc(k->dt,k->l*sizeof(lll_small_identifier_t));
						(k->dt+k->l-1)->v=malloc(sz*sizeof(char));
						for (uint32_t j=0;j<sz;j++){
							*((k->dt+k->l-1)->v+j)=*(str+j);
						}
						(k->dt+k->l-1)->sc=l_sc->l_sc;
						iarg->idx=LLL_CREATE_IDENTIFIER(k->l-1,sz-1);
					}
					else{
						uint32_t mx_sc=UINT32_MAX;
						lll_identifier_index_t mx_i=0;
						for (uint32_t i=0;i<c_dt->i_dt.ill;i++){
							lll_identifier_t* k=*(c_dt->i_dt.il+i);
							if (k->sz!=sz){
								continue;
							}
							for (uint32_t j=0;j<sz;j++){
								if (*(str+j)!=*(k->v+j)){
									goto _next_long_identifier;
								}
							}
							if (k->sc==l_sc->l_sc){
								iarg->idx=LLL_CREATE_IDENTIFIER(i,LLL_MAX_SHORT_IDENTIFIER_LENGTH);
								goto _identifier_found;
							}
							else if (((l_sc)->m[k->sc>>6]&(1ull<<(k->sc&0x3f)))&&(mx_sc==UINT32_MAX||k->sc>mx_sc)){
								mx_sc=k->sc;
								mx_i=LLL_CREATE_IDENTIFIER(i,LLL_MAX_SHORT_IDENTIFIER_LENGTH);
							}
_next_long_identifier:;
						}
						if (mx_sc!=UINT32_MAX){
							iarg->idx=mx_i;
							goto _identifier_found;
						}
						if (LLL_GET_OBJECT_TYPE(o)!=LLL_OBJECT_TYPE_SET&&LLL_GET_OBJECT_TYPE(o)!=LLL_OBJECT_TYPE_FUNC){
							e->t=LLL_ERROR_UNKNOWN_IDENTIFIER;
							e->dt.r.off=arg_s;
							e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
							if (n_l_sc.m){
								free(n_l_sc.m);
							}
							return LLL_RETURN_ERROR;
						}
						c_dt->i_dt.ill++;
						c_dt->i_dt.il=realloc(c_dt->i_dt.il,c_dt->i_dt.ill*sizeof(lll_identifier_t*));
						lll_identifier_t* n=malloc(sizeof(lll_identifier_t)+sz);
						n->sz=sz;
						n->sc=l_sc->l_sc;
						for (uint32_t i=0;i<sz;i++){
							*(n->v+i)=*(str+i);
						}
						*(c_dt->i_dt.il+c_dt->i_dt.ill-1)=n;
						iarg->idx=LLL_CREATE_IDENTIFIER(c_dt->i_dt.ill-1,LLL_MAX_SHORT_IDENTIFIER_LENGTH);
					}
_identifier_found:
					if (o&&(LLL_GET_OBJECT_TYPE(o)!=LLL_OBJECT_TYPE_SET||*ac)){
						o->t&=~LLL_OBJECT_TYPE_CONST;
					}
				}
			}
			else{
				e->t=LLL_ERROR_UNEXPECTED_CHARACTER;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				if (n_l_sc.m){
					free(n_l_sc.m);
				}
				return LLL_RETURN_ERROR;
			}
			if (!o){
				return LLL_RETURN_NO_ERROR;
			}
			if (LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_OPERATION_LIST){
				if (*sc==UINT16_MAX){
					e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				(*sc)++;
			}
			else{
				if (*ac==UINT8_MAX){
					e->t=LLL_ERROR_TOO_MANY_ARGUMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return LLL_RETURN_ERROR;
				}
				if (LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_IMPORT){
					if (LLL_GET_OBJECT_TYPE(arg)!=LLL_OBJECT_TYPE_STRING){
						e->t=LLL_ERROR_STRING_REQUIRED;
						e->dt.r.off=arg_s;
						e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
						if (n_l_sc.m){
							free(n_l_sc.m);
						}
						return LLL_RETURN_ERROR;
					}
					else{
						lll_string_length_t ln=((lll_string_object_t*)arg)->ln;
						char* str=((lll_string_object_t*)arg)->v;
						lll_import_object_t* io=(lll_import_object_t*)o;
						for (lll_import_index_t i=0;i<c_dt->im.l;i++){
							if ((c_dt->im.dt+i)->sz!=ln){
								continue;
							}
							char* nm=(c_dt->im.dt+i)->nm;
							for (uint32_t j=0;j<ln;j++){
								if (*(str+j)!=*(nm+j)){
									goto _next_import;
								}
							}
							io->idx[*ac]=i;
							goto _found_import;
_next_import:;
						}
						c_dt->im.l++;
						c_dt->im.dt=realloc(c_dt->im.dt,c_dt->im.l*sizeof(lll_import_data_path_t));
						char* s=malloc(ln*sizeof(char));
						for (uint32_t i=0;i<ln;i++){
							*(s+i)=*(str+i);
						}
						(c_dt->im.dt+c_dt->im.l-1)->nm=s;
						(c_dt->im.dt+c_dt->im.l-1)->sz=ln;
						io->idx[*ac]=c_dt->im.l-1;
_found_import:;
						_bf_off=arg_bf_off+sizeof(lll_import_index_t);
					}
				}
				(*ac)++;
			}
		}
	}
	if (!o){
		o=(lll_object_t*)(_bf+_bf_off);
		_bf_off+=sizeof(lll_object_t);
		if (_bf_off>=_bf_sz){
			e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
			e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
			e->dt.r.sz=1;
			if (n_l_sc.m){
				free(n_l_sc.m);
			}
			return LLL_RETURN_ERROR;
		}
		o->t=LLL_OBJECT_TYPE_NIL;
		return LLL_RETURN_NO_ERROR;
	}
	e->t=LLL_ERROR_UNMATCHED_OPEN_PARENTHESES;
	e->dt.r.off=st_off;
	e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st_off-1;
	if (n_l_sc.m){
		free(n_l_sc.m);
	}
	return LLL_RETURN_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_read_object(lll_compilation_data_t* c_dt,lll_error_t* e,lll_object_t** o){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	lll_object_t* a=(lll_object_t*)(_bf+_bf_off);
	int c=LLL_READ_FROM_INPUT_DATA_STREAM(c_dt->is);
	if (c==LLL_END_OF_DATA){
		_bf_off+=sizeof(lll_object_t);
		a->t=LLL_OBJECT_TYPE_NIL;
		return LLL_RETURN_NO_ERROR;
	}
	scope_data_t sc={
		malloc(sizeof(uint64_t)),
		0,
		1
	};
	sc.m[0]=1;
	if (!_read_object_internal(c_dt,c,&sc,e)){
		free(sc.m);
		return LLL_RETURN_ERROR;
	}
	free(sc.m);
	*o=a;
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_RETURN lll_read_all_objects(lll_compilation_data_t* c_dt,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	c_dt->h=(lll_object_t*)(_bf+_bf_off);
	_bf_off+=sizeof(lll_operation_list_object_t);
	if (_bf_off>=_bf_sz){
		e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
		e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(c_dt->is)-1;
		e->dt.r.sz=1;
		return LLL_RETURN_ERROR;
	}
	c_dt->h->t=LLL_OBJECT_TYPE_OPERATION_LIST;
	scope_data_t l_sc={
		malloc(sizeof(uint64_t)),
		0,
		1
	};
	l_sc.m[0]=1;
	lll_statement_count_t sc=0;
	while (1){
		int c=LLL_READ_FROM_INPUT_DATA_STREAM(c_dt->is);
		if (c==LLL_END_OF_DATA){
			((lll_operation_list_object_t*)c_dt->h)->sc=sc;
			free(l_sc.m);
			return LLL_RETURN_NO_ERROR;
		}
		uint32_t off=LLL_GET_INPUT_DATA_STREAM_OFFSET(c_dt->is);
		if (!_read_object_internal(c_dt,c,&l_sc,e)){
			free(l_sc.m);
			return LLL_RETURN_ERROR;
		}
		if (sc==UINT16_MAX){
			e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
			e->dt.r.off=off-1;
			e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(c_dt->is)-off-1;
			free(l_sc.m);
			return LLL_RETURN_ERROR;
		}
		sc++;
	}
}

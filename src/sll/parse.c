#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/constants.h>
#include <sll/core.h>
#include <sll/ift.h>
#include <sll/object.h>
#include <sll/platform.h>
#include <sll/string.h>
#include <sll/types.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



static sll_object_t* _patch_module(sll_object_t* mo,const import_module_data_t* im_dt){
	sll_object_t* o=_acquire_next_object(im_dt->c_dt);
	while (mo->t==SLL_OBJECT_TYPE_NOP||mo->t==OBJECT_TYPE_CHANGE_STACK){
		if (mo->t==OBJECT_TYPE_CHANGE_STACK){
			mo=mo->dt._p;
		}
		else{
			o->t=SLL_OBJECT_TYPE_NOP;
			o=_acquire_next_object(im_dt->c_dt);
			mo++;
		}
	}
	*o=*mo;
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
		case SLL_OBJECT_TYPE_FUNCTION_ID:
			return mo+1;
		case SLL_OBJECT_TYPE_STRING:
			o->dt.s=*(im_dt->sm+o->dt.s);
			return mo+1;
		case SLL_OBJECT_TYPE_IDENTIFIER:
			for (sll_export_table_length_t i=0;i<im_dt->eiml;i++){
				if ((im_dt->eim+i)->a==o->dt.id){
					o->dt.id=(im_dt->eim+i)->b;
					return mo+1;
				}
			}
			o->dt.id=SLL_IDENTIFIER_ADD_INDEX(o->dt.id,im_dt->off[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]);
			return mo+1;
		case SLL_OBJECT_TYPE_FUNC:
			o->dt.fn.id+=im_dt->f_off;
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_arg_count_t l=o->dt.l.ac;
				mo++;
				while (l){
					l--;
					mo=_patch_module(mo,im_dt);
				}
				return mo;
			}
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_WHILE:
		case SLL_OBJECT_TYPE_LOOP:
			{
				o->dt.l.sc+=im_dt->sc_off;
				sll_arg_count_t l=o->dt.l.ac;
				mo++;
				while (l){
					l--;
					mo=_patch_module(mo,im_dt);
				}
				return mo;
			}
		case SLL_OBJECT_TYPE_COMMA:
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_statement_count_t l=o->dt.sc;
				mo++;
				while (l){
					l--;
					mo=_patch_module(mo,im_dt);
				}
				return mo;
			}
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			o->dt.dbg.fpi=*(im_dt->sm+o->dt.dbg.fpi);
			return _patch_module(mo+1,im_dt);
	}
	sll_arg_count_t l=o->dt.ac;
	mo++;
	while (l){
		l--;
		mo=_patch_module(mo,im_dt);
	}
	return mo;
}



static uint8_t _read_single_char(sll_input_data_stream_t* is,sll_file_offset_t st,sll_error_t* e,sll_char_t* o){
	sll_read_char_t c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
	if (c==SLL_END_OF_DATA){
		e->t=SLL_ERROR_UNMATCHED_QUOTES;
		e->dt.r.off=st;
		e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
		return 1;
	}
	if (c=='\r'||c=='\n'){
		e->t=SLL_ERROR_UNMATCHED_QUOTES;
		e->dt.r.off=st;
		e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
		return 1;
	}
	if (c=='\\'){
		c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
		if (c=='\r'||c=='\n'){
			e->t=SLL_ERROR_UNMATCHED_QUOTES;
			e->dt.r.off=st;
			e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
			return 1;
		}
		if (c==SLL_END_OF_DATA){
			e->t=SLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE;
			e->dt.r.off=st;
			e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
			return 1;
		}
		if (c=='\''||c=='"'||c=='\\'){
			*o=(sll_char_t)c;
			return 2;
		}
		else if (c=='x'){
			c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c==SLL_END_OF_DATA){
				e->t=SLL_ERROR_UNMATCHED_QUOTES;
				e->dt.r.off=st;
				e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
				return 1;
			}
			if (c>96){
				c-=32;
			}
			if (c<48||(c>57&&c<65)||c>70){
				if (c=='\r'||c=='\n'){
					e->t=SLL_ERROR_UNMATCHED_QUOTES;
					e->dt.r.off=st;
					e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
					return 1;
				}
				e->t=SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				return 1;
			}
			sll_char_t v=(c>64?c-55:c-48)<<4;
			c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c==SLL_END_OF_DATA){
				e->t=SLL_ERROR_UNMATCHED_QUOTES;
				e->dt.r.off=st;
				e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
				return 1;
			}
			if (c>96){
				c-=32;
			}
			if (c<48||(c>57&&c<65)||c>70){
				if (c=='\r'||c=='\n'){
					e->t=SLL_ERROR_UNMATCHED_QUOTES;
					e->dt.r.off=st;
					e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-1;
					return 1;
				}
				e->t=SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				return 1;
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
			e->t=SLL_ERROR_UNKNOWN_ESCAPE_CHARACTER;
			e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
			e->dt.r.sz=1;
			return 1;
		}
	}
	*o=(sll_char_t)c;
	return 0;
}



static uint8_t _read_object_internal(sll_compilation_data_t* c_dt,sll_read_char_t c,const extra_compilation_data_t* e_c_dt,sll_error_t* e){
	uint8_t fl=e_c_dt->fl;
	const scope_data_t* l_sc=&(e_c_dt->sc);
	scope_data_t n_l_sc={
		NULL
	};
	const scope_data_t* b_l_sc=NULL;
	sll_input_data_stream_t* is=c_dt->is;
	sll_file_offset_t st_off=0;
	int ec=-1;
	sll_object_t* o=NULL;
	sll_arg_count_t ac=0;
	sll_statement_count_t sc=0;
	sll_array_length_t al=0;
	sll_map_length_t ml=0;
	sll_object_offset_t f_off=0;
	while (c!=SLL_END_OF_DATA){
		if ((c>8&&c<14)||c==' '){
			while ((c>8&&c<14)||c==' '){
				c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
			}
		}
		else if (c==';'){
			while (c!='\n'&&c!='\r'&&c!=SLL_END_OF_DATA){
				c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
			}
		}
		else if (c==')'){
			if (n_l_sc.m){
				free(n_l_sc.m);
			}
			if (!o||ec!=')'){
				e->t=SLL_ERROR_UNMATCHED_CLOSE_PARENTHESES;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				return SLL_RETURN_ERROR;
			}
			if (o->t==SLL_OBJECT_TYPE_ASSIGN&&ac==1&&e_c_dt->nv_dt->sz){
				sll_object_t* id=o+1;
				while (id->t==SLL_OBJECT_TYPE_NOP||id->t==SLL_OBJECT_TYPE_DEBUG_DATA||id->t==OBJECT_TYPE_CHANGE_STACK){
					id=(id->t==OBJECT_TYPE_CHANGE_STACK?id->dt._p:id+1);
				}
				if (id->t==SLL_OBJECT_TYPE_IDENTIFIER&&id==*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)){
					e_c_dt->nv_dt->sz--;
				}
			}
			if (o->t==SLL_OBJECT_TYPE_UNKNOWN){
				o->t=SLL_OBJECT_TYPE_INT;
				o->dt.i=0;
			}
			else if (o->t==SLL_OBJECT_TYPE_ASSIGN){
				if (ac<2){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					o->dt.sc=ac;
				}
				else{
					sll_object_t* a=o+1;
					while (a->t==SLL_OBJECT_TYPE_NOP||a->t==SLL_OBJECT_TYPE_DEBUG_DATA||a->t==OBJECT_TYPE_CHANGE_STACK){
						a=(a->t==OBJECT_TYPE_CHANGE_STACK?a->dt._p:a+1);
					}
					if (a->t!=SLL_OBJECT_TYPE_IDENTIFIER){
						o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
						o->dt.sc=o->dt.ac;
					}
					else{
						o->dt.ac=ac;
					}
				}
			}
			else if (o->t==SLL_OBJECT_TYPE_FUNC){
				if (!ac){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					o->dt.sc=0;
				}
				else{
					sll_arg_count_t i=0;
					sll_object_t* arg=o+1;
					for (;i<ac;i++){
						while (arg->t==SLL_OBJECT_TYPE_NOP||arg->t==SLL_OBJECT_TYPE_DEBUG_DATA||arg->t==OBJECT_TYPE_CHANGE_STACK){
							arg=(arg->t==OBJECT_TYPE_CHANGE_STACK?arg->dt._p:arg+1);
						}
						if (arg->t!=SLL_OBJECT_TYPE_IDENTIFIER){
							break;
						}
						arg++;
					}
					o->dt.fn.ac=ac-i;
					o->dt.fn.id=c_dt->ft.l;
					c_dt->ft.l++;
					void* tmp=realloc(c_dt->ft.dt,c_dt->ft.l*sizeof(sll_function_t*));
					if (!tmp){
						SLL_ASSERT(!"Unable to Reallocate Function Array");
					}
					c_dt->ft.dt=tmp;
					sll_function_t* f=malloc(sizeof(sll_function_t)+i*sizeof(sll_identifier_index_t));
					f->off=f_off;
					f->al=i;
					arg=o+1;
					for (sll_arg_count_t j=0;j<i;j++){
						while (arg->t==SLL_OBJECT_TYPE_NOP||arg->t==SLL_OBJECT_TYPE_DEBUG_DATA||arg->t==OBJECT_TYPE_CHANGE_STACK){
							if (arg->t==OBJECT_TYPE_CHANGE_STACK){
								arg=arg->dt._p;
							}
							else{
								arg->t=SLL_OBJECT_TYPE_NOP;
								arg++;
							}
						}
						SLL_ASSERT(arg->t==SLL_OBJECT_TYPE_IDENTIFIER);
						f->a[j]=arg->dt.id;
						arg->t=SLL_OBJECT_TYPE_NOP;
						arg++;
					}
					*(c_dt->ft.dt+c_dt->ft.l-1)=f;
				}
			}
			else if (o->t==SLL_OBJECT_TYPE_INTERNAL_FUNC){
				if (!ac){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					o->dt.sc=0;
				}
				else{
					sll_object_t* n=o+1;
					while (n->t==SLL_OBJECT_TYPE_NOP||n->t==SLL_OBJECT_TYPE_DEBUG_DATA||n->t==OBJECT_TYPE_CHANGE_STACK){
						n=(n->t==OBJECT_TYPE_CHANGE_STACK?n->dt._p:n+1);
					}
					if (n->t!=SLL_OBJECT_TYPE_STRING){
						o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
						o->dt.sc=ac;
					}
					else{
						n->t=SLL_OBJECT_TYPE_NOP;
						sll_string_t* s=c_dt->st.dt+n->dt.s;
						if (!s->l||s->l>255){
							e->t=SLL_ERROR_INTERNAL_FUNCTION_NAME_TOO_LONG;
							e->dt.r.off=st_off;
							e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st_off;
							return SLL_RETURN_ERROR;
						}
						o->dt.fn.ac=ac-1;
						o->dt.fn.id=sll_lookup_internal_function(e_c_dt->i_ft,s->v);
						if (o->dt.fn.id==SLL_MAX_FUNCTION_INDEX){
							e->t=SLL_ERROR_UNKNOWN_INTERNAL_FUNCTION;
							memcpy(e->dt.str,s->v,s->l);
							e->dt.str[s->l]=0;
							return SLL_RETURN_ERROR;
						}
					}
				}
			}
			else if (o->t==SLL_OBJECT_TYPE_FOR||o->t==SLL_OBJECT_TYPE_WHILE||o->t==SLL_OBJECT_TYPE_LOOP){
				if (!ac){
					o->t=SLL_OBJECT_TYPE_INT;
					o->dt.i=0;
				}
				else if (ac==1){
					o->t=SLL_OBJECT_TYPE_NOP;
				}
				else{
					o->dt.l.ac=ac;
				}
			}
			else if (o->t==SLL_OBJECT_TYPE_COMMA||(fl&(EXTRA_COMPILATION_DATA_IMPORT|EXTRA_COMPILATION_DATA_EXPORT|EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION))){
				o->dt.sc=sc;
			}
			else{
				o->dt.ac=ac;
			}
			return SLL_RETURN_NO_ERROR;
		}
		else if (c=='}'){
			if (!o||ec!='}'){
				e->t=SLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				return SLL_RETURN_ERROR;
			}
			o->dt.sc=sc;
			return SLL_RETURN_NO_ERROR;
		}
		else if (c=='('||c=='{'){
			if (!o){
				sll_insert_debug_object(c_dt,is);
				st_off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				o=_acquire_next_object(c_dt);
				if (c=='{'){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					ec='}';
				}
				else{
					o->t=SLL_OBJECT_TYPE_UNKNOWN;
					ec=')';
				}
			}
			else{
				if (o->t==SLL_OBJECT_TYPE_IF&&(ac&1)){
					SLL_ASSERT(n_l_sc.m);
					free(n_l_sc.m);
					n_l_sc.l_sc=c_dt->_n_sc_id;
					n_l_sc.ml=(n_l_sc.l_sc+65)>>6;
					n_l_sc.m=malloc(n_l_sc.ml*sizeof(uint64_t));
					n_l_sc.m[n_l_sc.ml-1]=0;
					for (scope_data_mask_length_t i=0;i<b_l_sc->ml;i++){
						*(n_l_sc.m+i)=*(b_l_sc->m+i);
					}
					n_l_sc.m[n_l_sc.ml-1]|=1ull<<(n_l_sc.l_sc&63);
					c_dt->_n_sc_id++;
					l_sc=&n_l_sc;
				}
_recurse_array_or_map:;
				if (o->t==SLL_OBJECT_TYPE_ASSIGN&&ac==1&&e_c_dt->nv_dt->sz){
					sll_object_t* id=o+1;
					while (id->t==SLL_OBJECT_TYPE_NOP||id->t==SLL_OBJECT_TYPE_DEBUG_DATA||id->t==OBJECT_TYPE_CHANGE_STACK){
						id=(id->t==OBJECT_TYPE_CHANGE_STACK?id->dt._p:id+1);
					}
					if (id->t==SLL_OBJECT_TYPE_IDENTIFIER&&id==*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)){
						e_c_dt->nv_dt->sz--;
					}
				}
				sll_file_offset_t arg_s=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				if (o->t==SLL_OBJECT_TYPE_UNKNOWN){
					e->t=SLL_ERROR_NO_SYMBOL;
					e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return SLL_RETURN_ERROR;
				}
				extra_compilation_data_t n_e_c_dt={
					fl&EXTRA_COMPILATION_DATA_INSIDE_FUNCTION,
					*l_sc,
					e_c_dt->i_ft,
					e_c_dt->il,
					e_c_dt->nv_dt
				};
				if (!_read_object_internal(c_dt,c,&n_e_c_dt,e)){
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return SLL_RETURN_ERROR;
				}
				if (o->t==SLL_OBJECT_TYPE_ARRAY){
					if (al==SLL_MAX_ARRAY_LENGTH){
						e->t=SLL_ERROR_ARRAY_TOO_LONG;
						e->dt.r.off=arg_s;
						e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
						return SLL_RETURN_ERROR;
					}
					al++;
				}
				else if (o->t==SLL_OBJECT_TYPE_MAP){
					if (ml==SLL_MAX_ARRAY_LENGTH){
						e->t=SLL_ERROR_MAP_TOO_LONG;
						e->dt.r.off=arg_s;
						e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
						return SLL_RETURN_ERROR;
					}
					ml++;
				}
				else if (o->t==SLL_OBJECT_TYPE_COMMA||o->t==SLL_OBJECT_TYPE_OPERATION_LIST){
					if (sc==SLL_MAX_STATEMENT_COUNT){
						e->t=SLL_ERROR_TOO_MANY_STATEMENTS;
						e->dt.r.off=arg_s;
						e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
						return SLL_RETURN_ERROR;
					}
					sc++;
				}
				else{
					if (ac==SLL_MAX_ARG_COUNT){
						e->t=SLL_ERROR_TOO_MANY_ARGUMENTS;
						e->dt.r.off=arg_s;
						e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
						if (n_l_sc.m){
							free(n_l_sc.m);
						}
						return SLL_RETURN_ERROR;
					}
					ac++;
				}
			}
			c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
		}
		else if (o&&o->t==SLL_OBJECT_TYPE_UNKNOWN){
			char str[256];
			sll_string_length_t sz=0;
			do{
				if (sz==255){
					e->t=SLL_ERROR_SYMBOL_TOO_LONG;
					e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
					e->dt.r.sz=sz;
					return SLL_RETURN_ERROR;
				}
				*(str+sz)=(sll_char_t)c;
				sz++;
				c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c==SLL_END_OF_DATA){
					break;
				}
			} while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='{'&&c!='}'));
			if (sz==1){
				if (*str=='!'){
					o->t=SLL_OBJECT_TYPE_NOT;
				}
				else if (*str=='='){
					o->t=SLL_OBJECT_TYPE_ASSIGN;
				}
				else if (*str=='?'){
					o->t=SLL_OBJECT_TYPE_IF;
				}
				else if (*str=='+'){
					o->t=SLL_OBJECT_TYPE_ADD;
				}
				else if (*str=='-'){
					o->t=SLL_OBJECT_TYPE_SUB;
				}
				else if (*str=='*'){
					o->t=SLL_OBJECT_TYPE_MULT;
				}
				else if (*str=='/'){
					o->t=SLL_OBJECT_TYPE_DIV;
				}
				else if (*str=='%'){
					o->t=SLL_OBJECT_TYPE_MOD;
				}
				else if (*str=='&'){
					o->t=SLL_OBJECT_TYPE_BIT_AND;
				}
				else if (*str=='|'){
					o->t=SLL_OBJECT_TYPE_BIT_OR;
				}
				else if (*str=='^'){
					o->t=SLL_OBJECT_TYPE_BIT_XOR;
				}
				else if (*str=='~'){
					o->t=SLL_OBJECT_TYPE_BIT_NOT;
				}
				else if (*str=='<'){
					o->t=SLL_OBJECT_TYPE_LESS;
				}
				else if (*str=='>'){
					o->t=SLL_OBJECT_TYPE_MORE;
				}
				else if (*str=='$'){
					o->t=SLL_OBJECT_TYPE_LENGTH;
				}
				else if (*str==':'){
					o->t=SLL_OBJECT_TYPE_ACCESS;
				}
				else if (*str=='@'){
					o->t=SLL_OBJECT_TYPE_BREAK;
				}
				else if (*str==','){
					o->t=SLL_OBJECT_TYPE_COMMA;
					sc=0;
				}
				else if (*str=='#'){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					sc=0;
					fl|=EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==2){
				if (*str==':'&&*(str+1)=='>'){
					o->t=SLL_OBJECT_TYPE_PRINT;
				}
				else if (*str=='<'&&*(str+1)==':'){
					o->t=SLL_OBJECT_TYPE_INPUT;
				}
				else if (*str=='&'&&*(str+1)=='&'){
					o->t=SLL_OBJECT_TYPE_AND;
				}
				else if (*str=='|'&&*(str+1)=='|'){
					o->t=SLL_OBJECT_TYPE_OR;
				}
				else if (*str=='<'&&*(str+1)=='-'){
					o->t=SLL_OBJECT_TYPE_CALL;
				}
				else if (*str=='-'&&*(str+1)=='>'){
					o->t=SLL_OBJECT_TYPE_FOR;
					o->dt.l.sc=c_dt->_n_sc_id;
				}
				else if (*str=='>'&&*(str+1)=='-'){
					o->t=SLL_OBJECT_TYPE_WHILE;
					o->dt.l.sc=c_dt->_n_sc_id;
				}
				else if (*str=='>'&&*(str+1)=='<'){
					o->t=SLL_OBJECT_TYPE_LOOP;
				}
				else if (*str=='/'&&*(str+1)=='/'){
					o->t=SLL_OBJECT_TYPE_FLOOR_DIV;
				}
				else if (*str=='>'&&*(str+1)=='>'){
					o->t=SLL_OBJECT_TYPE_BIT_RSHIFT;
				}
				else if (*str=='<'&&*(str+1)=='<'){
					o->t=SLL_OBJECT_TYPE_BIT_LSHIFT;
				}
				else if (*str=='<'&&*(str+1)=='='){
					o->t=SLL_OBJECT_TYPE_LESS_EQUAL;
				}
				else if (*str=='='&&*(str+1)=='='){
					o->t=SLL_OBJECT_TYPE_EQUAL;
				}
				else if (*str=='!'&&*(str+1)=='='){
					o->t=SLL_OBJECT_TYPE_NOT_EQUAL;
				}
				else if (*str=='>'&&*(str+1)=='='){
					o->t=SLL_OBJECT_TYPE_MORE_EQUAL;
				}
				else if (*str==':'&&*(str+1)==':'){
					o->t=SLL_OBJECT_TYPE_CAST;
				}
				else if (*str=='%'&&*(str+1)=='%'){
					o->t=SLL_OBJECT_TYPE_REF;
				}
				else if (*str=='@'&&*(str+1)=='@'){
					o->t=((fl&EXTRA_COMPILATION_DATA_INSIDE_FUNCTION)?SLL_OBJECT_TYPE_RETURN:SLL_OBJECT_TYPE_EXIT);
				}
				else if (*str=='-'&&*(str+1)=='-'){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					sc=0;
					fl|=EXTRA_COMPILATION_DATA_IMPORT;
				}
				else if (*str=='#'&&*(str+1)=='#'){
					o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
					sc=0;
					fl|=EXTRA_COMPILATION_DATA_EXPORT;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==3){
				if (*str==','&&*(str+1)==','&&*(str+2)==','){
					o->t=SLL_OBJECT_TYPE_FUNC;
					f_off=c_dt->_s.off-1;
					o->dt.fn.sc=c_dt->_n_sc_id;
					fl|=EXTRA_COMPILATION_DATA_INSIDE_FUNCTION;
				}
				else if (*str=='.'&&*(str+1)=='.'&&*(str+2)=='.'){
					o->t=SLL_OBJECT_TYPE_INTERNAL_FUNC;
				}
				else if (*str=='*'&&*(str+1)=='*'&&*(str+2)=='*'){
					o->t=SLL_OBJECT_TYPE_INLINE_FUNC;
					o->dt.fn.sc=c_dt->_n_sc_id;
					fl|=EXTRA_COMPILATION_DATA_INSIDE_FUNCTION;
				}
				else if (*str=='<'&&*(str+1)=='<'&&*(str+2)=='<'){
					o->t=SLL_OBJECT_TYPE_CONTINUE;
				}
				else if (*str=='@'&&*(str+1)=='@'&&*(str+2)=='@'){
					o->t=SLL_OBJECT_TYPE_EXIT;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else{
_unknown_symbol:
				e->t=SLL_ERROR_UNKNOWN_SYMBOL;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-1;
				e->dt.r.sz=sz;
				return SLL_RETURN_ERROR;
			}
			if (o->t==SLL_OBJECT_TYPE_IF||o->t==SLL_OBJECT_TYPE_FOR||o->t==SLL_OBJECT_TYPE_WHILE||o->t==SLL_OBJECT_TYPE_LOOP||o->t==SLL_OBJECT_TYPE_FUNC||o->t==SLL_OBJECT_TYPE_INLINE_FUNC){
				n_l_sc.l_sc=c_dt->_n_sc_id;
				n_l_sc.ml=(n_l_sc.l_sc+65)>>6;
				n_l_sc.m=malloc(n_l_sc.ml*sizeof(uint64_t));
				n_l_sc.m[n_l_sc.ml-1]=0;
				for (scope_data_mask_length_t i=0;i<l_sc->ml;i++){
					*(n_l_sc.m+i)=*(l_sc->m+i);
				}
				n_l_sc.m[n_l_sc.ml-1]|=1ull<<(n_l_sc.l_sc&63);
				c_dt->_n_sc_id++;
				b_l_sc=l_sc;
				l_sc=&n_l_sc;
			}
		}
		else if (c=='['||c=='<'){
			if (o){
				goto _recurse_array_or_map;
			}
			sll_insert_debug_object(c_dt,is);
			st_off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
			o=_acquire_next_object(c_dt);
			o->t=(c=='['?SLL_OBJECT_TYPE_ARRAY:SLL_OBJECT_TYPE_MAP);
			c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
		}
		else if (c==']'){
			if (!o||o->t!=SLL_OBJECT_TYPE_ARRAY){
				e->t=SLL_ERROR_UNMATCHED_ARRAY_CLOSE_BRACKETS;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				return SLL_RETURN_ERROR;
			}
			o->dt.al=al;
			return SLL_RETURN_NO_ERROR;
		}
		else if (c=='>'){
			if (!o||o->t!=SLL_OBJECT_TYPE_MAP){
				e->t=SLL_ERROR_UNMATCHED_MAP_CLOSE_BRACKETS;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				return SLL_RETURN_ERROR;
			}
			o->dt.ml=ml;
			return SLL_RETURN_NO_ERROR;
		}
		else if (c=='|'){
			c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c!='#'){
				e->t=SLL_ERROR_UNEXPECTED_CHARACTER;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-3;
				e->dt.r.sz=1;
				if (n_l_sc.m){
					free(n_l_sc.m);
				}
				return SLL_RETURN_ERROR;
			}
			int lc=c;
			do{
				lc=c;
				c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c==SLL_END_OF_DATA){
					e->t=SLL_ERROR_UNMATCHED_OPEN_QUOTE;
					e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-3;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return SLL_RETURN_ERROR;
				}
			} while (c!='|'||lc!='#');
			c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
		}
		else{
			sll_insert_debug_object(c_dt,is);
			sll_file_offset_t arg_s=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
			sll_object_t* arg=_acquire_next_object(c_dt);
			if (c=='\''){
				arg->t=SLL_OBJECT_TYPE_CHAR;
				uint8_t err=_read_single_char(is,arg_s,e,&(arg->dt.c));
				if (err==1){
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return SLL_RETURN_ERROR;
				}
				if (!err&&arg->dt.c=='\''){
					e->t=SLL_ERROR_EMPTY_CHAR_STRING;
					e->dt.r.off=arg_s;
					e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return SLL_RETURN_ERROR;
				}
				c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c!='\''){
					e->t=SLL_ERROR_UNTERMINATED_CHAR_STRING;
					e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return SLL_RETURN_ERROR;
				}
				c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
			}
			else if (c=='"'){
				arg->t=SLL_OBJECT_TYPE_STRING;
				sll_string_t s=SLL_ZERO_STRING_STRUCT;
				s.v=malloc(SLL_STRING_ALIGN_LENGTH(0)*sizeof(sll_char_t));
				SLL_STRING_FORMAT_PADDING(s.v,0);
				while (1){
					SLL_STRING_INCREASE(&s);
					uint8_t err=_read_single_char(is,arg_s,e,s.v+s.l);
					if (err==1){
						free(s.v);
						if (n_l_sc.m){
							free(n_l_sc.m);
						}
						return SLL_RETURN_ERROR;
					}
					if (!err&&s.v[s.l]=='"'){
						break;
					}
					s.l++;
				}
				s.v[s.l]=0;
				sll_string_hash(&s);
				arg->dt.s=sll_add_string(&(c_dt->st),&s,1);
				c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
			}
			else if ((c>47&&c<58)||c=='-'){
				int8_t m=1;
				if (c=='-'){
					m=-1;
					c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
					if (c==SLL_END_OF_DATA){
						break;
					}
				}
				sll_integer_t v=0;
				if (c=='0'){
					c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
					if (c==SLL_END_OF_DATA){
						break;
					}
					if (c=='x'||c=='X'){
						c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
						if (c==SLL_END_OF_DATA){
							break;
						}
						do{
							if (c>96){
								c-=32;
							}
							if (c<48||(c>57&&c<65)||c>70){
								e->t=SLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER;
								e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
								e->dt.r.sz=1;
								if (n_l_sc.m){
									free(n_l_sc.m);
								}
								return SLL_RETURN_ERROR;
							}
							v=(v<<4)+(c>64?c-55:c-48);
							c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
							if (c==SLL_END_OF_DATA){
								goto _return_error;
							}
						} while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'));
					}
					else if (c=='o'||c=='O'){
						c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
						if (c==SLL_END_OF_DATA){
							break;
						}
						do{
							if (c<48||c>55){
								e->t=SLL_ERROR_UNKNOWN_OCTAL_CHARCTER;
								e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
								e->dt.r.sz=1;
								if (n_l_sc.m){
									free(n_l_sc.m);
								}
								return SLL_RETURN_ERROR;
							}
							v=(v<<3)+(c-48);
							c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
							if (c==SLL_END_OF_DATA){
								goto _return_error;
							}
						} while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'));
					}
					else if (c=='b'||c=='B'){
						c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
						if (c==SLL_END_OF_DATA){
							break;
						}
						do{
							if (c!=48&&c!=49){
								e->t=SLL_ERROR_UNKNOWN_BINARY_CHARCTER;
								e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
								e->dt.r.sz=1;
								if (n_l_sc.m){
									free(n_l_sc.m);
								}
								return SLL_RETURN_ERROR;
							}
							v=(v<<1)+(c-48);
							c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
							if (c==SLL_END_OF_DATA){
								goto _return_error;
							}
						} while (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'));
					}
					if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}'&&c!='.'&&c!='e'&&c!='E'&&(c<48||c>57))){
						e->t=SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
						e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
						e->dt.r.sz=1;
						if (n_l_sc.m){
							free(n_l_sc.m);
						}
						return SLL_RETURN_ERROR;
					}
				}
				if (c>47&&c<58){
					while (1){
						v=v*10+(c-48);
						c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
						if (c==SLL_END_OF_DATA){
							goto _return_error;
						}
						if ((c>8&&c<14)||c==' '||c=='('||c==')'||c==';'||c=='<'||c=='>'||c=='['||c==']'||c=='{'||c=='}'||c=='.'||c=='e'||c=='E'){
							break;
						}
						if (c<48||c>57){
							e->t=SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
							e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
							e->dt.r.sz=1;
							if (n_l_sc.m){
								free(n_l_sc.m);
							}
							return SLL_RETURN_ERROR;
						}
					}
				}
				if (c=='.'||c=='e'||c=='E'){
					int16_t ex=0;
					if (c=='.'){
						while (1){
							c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
							if (c==SLL_END_OF_DATA){
								break;
							}
							if ((c>8&&c<14)||c==' '||c=='('||c==')'||c==';'||c=='<'||c=='>'||c=='['||c==']'||c=='{'||c=='}'||c=='e'||c=='E'){
								break;
							}
							if (c<48||c>57){
								e->t=SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
								e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
								e->dt.r.sz=1;
								if (n_l_sc.m){
									free(n_l_sc.m);
								}
								return SLL_RETURN_ERROR;
							}
							ex--;
							v=v*10+(c-48);
						}
					}
					if (c=='e'||c=='E'){
						int8_t em=1;
						int16_t ev=0;
						c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
						if (c==SLL_END_OF_DATA){
							break;
						}
						if (c=='-'){
							em=-1;
							c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
							if (c==SLL_END_OF_DATA){
								break;
							}
						}
						else if (c=='+'){
							c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
							if (c==SLL_END_OF_DATA){
								break;
							}
						}
						while (1){
							if ((c>8&&c<14)||c==' '||c=='('||c==')'||c==';'||c=='<'||c=='>'||c=='['||c==']'||c=='{'||c=='}'){
								break;
							}
							if (c<48||c>57){
								e->t=SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
								e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
								e->dt.r.sz=1;
								if (n_l_sc.m){
									free(n_l_sc.m);
								}
								return SLL_RETURN_ERROR;
							}
							ev=ev*10+(c-48);
							c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
							if (c==SLL_END_OF_DATA){
								break;
							}
						}
						ex+=ev*em;
					}
					if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
						e->t=SLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
						e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
						e->dt.r.sz=1;
						if (n_l_sc.m){
							free(n_l_sc.m);
						}
						return SLL_RETURN_ERROR;
					}
					arg->t=SLL_OBJECT_TYPE_FLOAT;
					arg->dt.f=((sll_float_t)v)*pow(5,ex)*pow(2,ex)*m;
				}
				else{
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=v*m;
				}
			}
			else if ((c>64&&c<91)||c=='_'||(c>96&&c<123)){
				sll_char_t str[SLL_STRING_ALIGN_LENGTH(255)];
				uint8_t sz=0;
				do{
					if (sz==255){
						e->t=SLL_ERROR_INTERNAL_STACK_OVERFLOW;
						e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
						e->dt.r.sz=1;
						if (n_l_sc.m){
							free(n_l_sc.m);
						}
						return SLL_RETURN_ERROR;
					}
					str[sz]=(sll_char_t)c;
					sz++;
					c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
					if (c==SLL_END_OF_DATA){
						goto _return_error;
					}
				} while ((c>47&&c<58)||(c>64&&c<91)||c=='_'||(c>96&&c<123));
				if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
					e->t=SLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER;
					e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return SLL_RETURN_ERROR;
				}
				if ((sz==3&&!memcmp(str,"nil",3))||(sz==5&&!memcmp(str,"false",5))){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=0;
				}
				else if (sz==4&&!memcmp(str,"true",4)){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=1;
				}
				else if (sz==8&&!memcmp(str,"int_type",8)){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=SLL_CONSTANT_TYPE_INT;
				}
				else if (sz==10&&!memcmp(str,"float_type",10)){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=SLL_CONSTANT_TYPE_FLOAT;
				}
				else if (sz==9&&!memcmp(str,"char_type",9)){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=SLL_CONSTANT_TYPE_CHAR;
				}
				else if (sz==11&&!memcmp(str,"string_type",11)){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=SLL_CONSTANT_TYPE_STRING;
				}
				else if (sz==10&&!memcmp(str,"array_type",10)){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=SLL_CONSTANT_TYPE_ARRAY;
				}
				else if (sz==11&&!memcmp(str,"handle_type",11)){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=SLL_CONSTANT_TYPE_HANDLE;
				}
				else if (sz==8&&!memcmp(str,"map_type",8)){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=SLL_CONSTANT_TYPE_MAP;
				}
				else if (sz==12&&!memcmp(str,"map_key_type",12)){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=SLL_CONSTANT_TYPE_MAP_KEY;
				}
				else if (sz==14&&!memcmp(str,"map_value_type",14)){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=SLL_CONSTANT_TYPE_MAP_VALUE;
				}
				else if (sz==5&&!memcmp(str,"stdin",5)){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=-1;
				}
				else if (sz==6&&!memcmp(str,"stdout",6)){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=-2;
				}
				else if (sz==6&&!memcmp(str,"stderr",6)){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=-3;
				}
				else{
					sll_string_t n={
						sz,
						0,
						str
					};
					SLL_STRING_FORMAT_PADDING(str,sz);
					sll_string_hash(&n);
					arg->t=SLL_OBJECT_TYPE_IDENTIFIER;
					if (sz<=SLL_MAX_SHORT_IDENTIFIER_LENGTH){
						sll_identifier_list_t* k=c_dt->idt.s+sz-1;
						sll_scope_t mx_sc=SLL_MAX_SCOPE;
						sll_identifier_index_t mx_i=0;
						if (o->t!=SLL_OBJECT_TYPE_FUNC){
							for (sll_identifier_list_length_t i=0;i<k->l;i++){
								sll_identifier_t* si=k->dt+i;
								sll_string_t* si_s=c_dt->st.dt+si->i;
								if (si_s->c!=n.c||si->sc==SLL_MAX_SCOPE||memcmp(str,si_s->v,sz)){
									continue;
								}
								if (si->sc==l_sc->l_sc){
									arg->dt.id=SLL_CREATE_IDENTIFIER(i,sz-1);
									goto _check_exports;
								}
								else if ((l_sc->m[si->sc>>6]&(1ull<<(si->sc&0x3f)))&&(mx_sc==SLL_MAX_SCOPE||si->sc>mx_sc)){
									mx_sc=si->sc;
									mx_i=SLL_CREATE_IDENTIFIER(i,sz-1);
								}
							}
							if (mx_sc!=SLL_MAX_SCOPE){
								arg->dt.id=mx_i;
								goto _check_exports;
							}
							if ((o->t!=SLL_OBJECT_TYPE_ASSIGN||ac)&&!(fl&EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION)){
								e->t=SLL_ERROR_UNKNOWN_IDENTIFIER;
								e->dt.r.off=arg_s;
								e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
								if (n_l_sc.m){
									free(n_l_sc.m);
								}
								return SLL_RETURN_ERROR;
							}
						}
						k->l++;
						k->dt=realloc(k->dt,k->l*sizeof(sll_identifier_t));
						(k->dt+k->l-1)->sc=l_sc->l_sc;
						(k->dt+k->l-1)->i=sll_create_string(&(c_dt->st),str,sz);
						arg->dt.id=SLL_CREATE_IDENTIFIER(k->l-1,sz-1);
						if (o->t==SLL_OBJECT_TYPE_ASSIGN){
							e_c_dt->nv_dt->sz++;
							e_c_dt->nv_dt->dt=realloc(e_c_dt->nv_dt->dt,e_c_dt->nv_dt->sz*sizeof(sll_object_t*));
							*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)=arg;
						}
						goto _identifier_created;
					}
					else{
						sll_scope_t mx_sc=SLL_MAX_SCOPE;
						sll_identifier_index_t mx_i=0;
						if (o->t!=SLL_OBJECT_TYPE_FUNC){
							for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
								sll_identifier_t* k=c_dt->idt.il+i;
								sll_string_t* s=c_dt->st.dt+k->i;
								if (s->c!=n.c||s->l!=sz||k->sc==SLL_MAX_SCOPE||memcmp(str,s->v,sz)){
									continue;
								}
								if (k->sc==l_sc->l_sc){
									arg->dt.id=SLL_CREATE_IDENTIFIER(i,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
									goto _check_exports;
								}
								else if ((l_sc->m[k->sc>>6]&(1ull<<(k->sc&0x3f)))&&(mx_sc==SLL_MAX_SCOPE||k->sc>mx_sc)){
									mx_sc=k->sc;
									mx_i=SLL_CREATE_IDENTIFIER(i,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
								}
							}
							if (mx_sc!=SLL_MAX_SCOPE){
								arg->dt.id=mx_i;
								goto _check_exports;
							}
							if ((o->t!=SLL_OBJECT_TYPE_ASSIGN||ac)&&!(fl&EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION)){
								e->t=SLL_ERROR_UNKNOWN_IDENTIFIER;
								e->dt.r.off=arg_s;
								e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
								if (n_l_sc.m){
									free(n_l_sc.m);
								}
								return SLL_RETURN_ERROR;
							}
						}
						c_dt->idt.ill++;
						c_dt->idt.il=realloc(c_dt->idt.il,c_dt->idt.ill*sizeof(sll_identifier_t));
						(c_dt->idt.il+c_dt->idt.ill-1)->sc=l_sc->l_sc;
						(c_dt->idt.il+c_dt->idt.ill-1)->i=sll_create_string(&(c_dt->st),str,sz);
						arg->dt.id=SLL_CREATE_IDENTIFIER(c_dt->idt.ill-1,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
						if (o->t==SLL_OBJECT_TYPE_ASSIGN){
							e_c_dt->nv_dt->sz++;
							e_c_dt->nv_dt->dt=realloc(e_c_dt->nv_dt->dt,e_c_dt->nv_dt->sz*sizeof(sll_object_t*));
							*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)=arg;
						}
						goto _identifier_created;
					}
_check_exports:;
					for (uint32_t i=0;i<e_c_dt->nv_dt->sz;i++){
						if ((*(e_c_dt->nv_dt->dt+i))->dt.id==arg->dt.id){
							arg->t=SLL_OBJECT_TYPE_INT;
							arg->dt.i=0;
							break;
						}
					}
_identifier_created:;
				}
			}
			else{
				e->t=SLL_ERROR_UNEXPECTED_CHARACTER;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				if (n_l_sc.m){
					free(n_l_sc.m);
				}
				return SLL_RETURN_ERROR;
			}
			if (!o){
				return SLL_RETURN_NO_ERROR;
			}
			if (o->t==SLL_OBJECT_TYPE_ARRAY){
				if (al==SLL_MAX_ARRAY_LENGTH){
					e->t=SLL_ERROR_ARRAY_TOO_LONG;
					e->dt.r.off=arg_s;
					e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
					return SLL_RETURN_ERROR;
				}
				al++;
			}
			else if (o->t==SLL_OBJECT_TYPE_MAP){
				if (ml==SLL_MAX_ARRAY_LENGTH){
					e->t=SLL_ERROR_MAP_TOO_LONG;
					e->dt.r.off=arg_s;
					e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
					return SLL_RETURN_ERROR;
				}
				ml++;
			}
			else if (o->t==SLL_OBJECT_TYPE_COMMA||o->t==SLL_OBJECT_TYPE_OPERATION_LIST){
				if (sc==SLL_MAX_STATEMENT_COUNT){
					e->t=SLL_ERROR_TOO_MANY_STATEMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
					return SLL_RETURN_ERROR;
				}
				sc++;
				if ((fl&EXTRA_COMPILATION_DATA_IMPORT)&&arg->t==SLL_OBJECT_TYPE_STRING){
					if (sc==SLL_MAX_STATEMENT_COUNT){
						e->t=SLL_ERROR_TOO_MANY_STATEMENTS;
						e->dt.r.off=arg_s;
						e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
						return SLL_RETURN_ERROR;
					}
					sc++;
					sll_compilation_data_t im={0};
					if (e_c_dt->il(c_dt->st.dt+arg->dt.s,&im,e)==SLL_RETURN_ERROR){
						return SLL_RETURN_ERROR;
					}
					import_module_data_t im_dt={
						.sm=malloc(im.st.l*sizeof(sll_string_index_t)),
						.f_off=c_dt->ft.l,
						.eiml=im.et.l,
						.sc_off=c_dt->_n_sc_id,
						.c_dt=c_dt
					};
					for (sll_string_index_t i=0;i<im.st.l;i++){
						sll_string_t* s=im.st.dt+i;
						for (sll_string_index_t j=0;j<c_dt->st.l;j++){
							sll_string_t* d=c_dt->st.dt+j;
							if (s->c==d->c&&s->l==d->l&&!memcmp(s->v,d->v,s->l)){
								*(im_dt.sm+i)=j;
								goto _merge_next_string;
							}
						}
						*(im_dt.sm+i)=c_dt->st.l;
						c_dt->st.l++;
						c_dt->st.dt=realloc(c_dt->st.dt,c_dt->st.l*sizeof(sll_string_t));
						sll_string_t* n=c_dt->st.dt+c_dt->st.l-1;
						sll_string_create(s->l,n);
						n->c=s->c;
						memcpy(n->v,s->v,s->l);
_merge_next_string:;
					}
					sll_identifier_list_length_t s_si[SLL_MAX_SHORT_IDENTIFIER_LENGTH];
					for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
						sll_identifier_list_t* il=c_dt->idt.s+i;
						sll_identifier_list_t mil=im.idt.s[i];
						im_dt.off[i]=il->l;
						if (mil.l){
							s_si[i]=il->l;
							il->l+=mil.l;
							void* tmp=realloc(il->dt,il->l*sizeof(sll_identifier_t));
							if (!tmp){
								SLL_ASSERT(!"Unable to Reallocate Fixed-Length Identifier Array",e,SLL_RETURN_ERROR);
							}
							il->dt=tmp;
							for (sll_identifier_list_length_t j=0;j<mil.l;j++){
								(il->dt+s_si[i]+j)->sc=(mil.dt+j)->sc+c_dt->_n_sc_id;
								(il->dt+s_si[i]+j)->i=*(im_dt.sm+(mil.dt+j)->i);
							}
						}
					}
					im_dt.off[SLL_MAX_SHORT_IDENTIFIER_LENGTH]=c_dt->idt.ill;
					sll_identifier_list_length_t si=c_dt->idt.ill;
					if (im.idt.ill){
						c_dt->idt.ill+=im.idt.ill;
						void* tmp=realloc(c_dt->idt.il,c_dt->idt.ill*sizeof(sll_identifier_t));
						if (!tmp){
							SLL_ASSERT(!"Unable to Reallocate Variable-Length Identifier Array",e,SLL_RETURN_ERROR);
						}
						c_dt->idt.il=tmp;
						for (sll_identifier_list_length_t j=0;j<im.idt.ill;j++){
							(c_dt->idt.il+si+j)->sc=(im.idt.il+j)->sc+c_dt->_n_sc_id;
							(c_dt->idt.il+si+j)->i=*(im_dt.sm+(im.idt.il+j)->i);
						}
					}
					im_dt.eim=malloc(im.et.l*sizeof(identifier_pair_t));
					for (sll_export_table_length_t i=0;i<im.et.l;i++){
						sll_identifier_index_t eii=*(im.et.dt+i);
						(im_dt.eim+i)->a=eii;
						sll_scope_t mx_sc=SLL_MAX_SCOPE;
						sll_identifier_index_t mx_i=0;
						sll_identifier_t* ei;
						uint8_t j=SLL_IDENTIFIER_GET_ARRAY_ID(eii);
						if (j==SLL_MAX_SHORT_IDENTIFIER_LENGTH){
							ei=c_dt->idt.il+si+SLL_IDENTIFIER_GET_ARRAY_INDEX(eii);
							for (sll_identifier_list_length_t k=0;k<c_dt->idt.ill;k++){
								sll_identifier_t* n=c_dt->idt.il+k;
								if (n->i!=ei->i){
									continue;
								}
								if (n->sc==l_sc->l_sc){
									ei->sc=SLL_MAX_SCOPE;
									(im_dt.eim+i)->b=SLL_CREATE_IDENTIFIER(k,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
									goto _export_identifier_found;
								}
								if ((l_sc->m[n->sc>>6]&(1ull<<(n->sc&0x3f)))&&(mx_sc==SLL_MAX_SCOPE||n->sc>mx_sc)){
									mx_sc=n->sc;
									mx_i=SLL_CREATE_IDENTIFIER(k,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
								}
							}
						}
						else{
							ei=c_dt->idt.s[j].dt+s_si[j]+SLL_IDENTIFIER_GET_ARRAY_INDEX(eii);
							sll_identifier_list_t* il=c_dt->idt.s+j;
							for (sll_identifier_list_length_t k=0;k<il->l;k++){
								sll_identifier_t* n=il->dt+k;
								if (n->i!=ei->i){
									continue;
								}
								if (n->sc==l_sc->l_sc){
									ei->sc=SLL_MAX_SCOPE;
									(im_dt.eim+i)->b=SLL_CREATE_IDENTIFIER(k,j);
									goto _export_identifier_found;
								}
								if ((l_sc->m[n->sc>>6]&(1ull<<(n->sc&0x3f)))&&(mx_sc==SLL_MAX_SCOPE||n->sc>mx_sc)){
									mx_sc=n->sc;
									mx_i=SLL_CREATE_IDENTIFIER(k,j);
								}
							}
						}
						if (mx_sc!=SLL_MAX_SCOPE){
							ei->sc=SLL_MAX_SCOPE;
							(im_dt.eim+i)->b=mx_i;
						}
						else{
							ei->sc=l_sc->l_sc;
							(im_dt.eim+i)->a=SLL_MAX_IDENTIFIER_INDEX;
						}
_export_identifier_found:;
					}
					sll_function_index_t j=c_dt->ft.l;
					c_dt->ft.l+=im.ft.l;
					c_dt->ft.dt=realloc(c_dt->ft.dt,c_dt->ft.l*sizeof(sll_function_t*));
					for (sll_function_index_t i=0;i<im.ft.l;i++){
						sll_function_t* f=*(im.ft.dt+i);
						sll_function_t* nf=malloc(sizeof(sll_function_t)+f->al*sizeof(sll_identifier_index_t));
						nf->off=(sll_object_offset_t)(f->off+c_dt->_s.off);
						nf->al=f->al;
						memcpy(nf->a,f->a,f->al*sizeof(sll_identifier_index_t));
						*(c_dt->ft.dt+i+j)=nf;
					}
					c_dt->_n_sc_id+=im._n_sc_id;
					_patch_module(im.h,&im_dt);
					free(im_dt.sm);
					free(im_dt.eim);
					sll_free_compilation_data(&im);
				}
				else if ((fl&EXTRA_COMPILATION_DATA_EXPORT)&&arg->t==SLL_OBJECT_TYPE_IDENTIFIER){
					for (sll_export_table_length_t j=0;j<c_dt->et.l;j++){
						if (*(c_dt->et.dt+j)==arg->dt.id){
							goto _skip_export;
						}
					}
					c_dt->et.l++;
					c_dt->et.dt=realloc(c_dt->et.dt,c_dt->et.l*sizeof(sll_identifier_index_t));
					*(c_dt->et.dt+c_dt->et.l-1)=arg->dt.id;
_skip_export:;
				}
			}
			else{
				if (o->t==SLL_OBJECT_TYPE_IF&&(ac&1)){
					SLL_ASSERT(n_l_sc.m);
					free(n_l_sc.m);
					n_l_sc.l_sc=c_dt->_n_sc_id;
					n_l_sc.ml=(n_l_sc.l_sc+65)>>6;
					n_l_sc.m=malloc(n_l_sc.ml*sizeof(uint64_t));
					n_l_sc.m[n_l_sc.ml-1]=0;
					for (scope_data_mask_length_t i=0;i<b_l_sc->ml;i++){
						*(n_l_sc.m+i)=*(b_l_sc->m+i);
					}
					n_l_sc.m[n_l_sc.ml-1]|=1ull<<(n_l_sc.l_sc&63);
					c_dt->_n_sc_id++;
					l_sc=&n_l_sc;
				}
				else if (o->t==SLL_OBJECT_TYPE_ASSIGN&&ac==1&&e_c_dt->nv_dt->sz){
					sll_object_t* id=o+1;
					while (id->t==SLL_OBJECT_TYPE_NOP||id->t==SLL_OBJECT_TYPE_DEBUG_DATA||id->t==OBJECT_TYPE_CHANGE_STACK){
						id=(id->t==OBJECT_TYPE_CHANGE_STACK?id->dt._p:id+1);
					}
					if (id->t==SLL_OBJECT_TYPE_IDENTIFIER&&id==*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)){
						e_c_dt->nv_dt->sz--;
					}
				}
				if (ac==SLL_MAX_ARG_COUNT){
					e->t=SLL_ERROR_TOO_MANY_ARGUMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return SLL_RETURN_ERROR;
				}
				ac++;
			}
		}
	}
	if (!o){
		o=_acquire_next_object(c_dt);
		o->t=SLL_OBJECT_TYPE_INT;
		o->dt.i=0;
		return SLL_RETURN_NO_ERROR;
	}
_return_error:;
	if (o->t==SLL_OBJECT_TYPE_ARRAY){
		e->t=SLL_ERROR_UNMATCHED_ARRAY_OPEN_BRACKETS;
	}
	else if (o->t==SLL_OBJECT_TYPE_MAP){
		e->t=SLL_ERROR_UNMATCHED_MAP_OPEN_BRACKETS;
	}
	else if (o->t==SLL_OBJECT_TYPE_OPERATION_LIST){
		e->t=SLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS;
	}
	else{
		e->t=SLL_ERROR_UNMATCHED_OPEN_PARENTHESES;
	}
	e->dt.r.off=st_off;
	e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st_off;
	if (n_l_sc.m){
		free(n_l_sc.m);
	}
	return SLL_RETURN_ERROR;
}



__SLL_FUNC void sll_init_compilation_data(const sll_char_t* fp,sll_input_data_stream_t* is,sll_compilation_data_t* o){
	o->is=is;
	o->tm=sll_platform_get_current_time();
	o->h=NULL;
	for (uint8_t i=0;i<SLL_MAX_SHORT_IDENTIFIER_LENGTH;i++){
		o->idt.s[i].dt=NULL;
		o->idt.s[i].l=0;
	}
	o->idt.il=NULL;
	o->idt.ill=0;
	o->et.dt=NULL;
	o->et.l=0;
	o->ft.dt=NULL;
	o->ft.l=0;
	o->st.dt=NULL;
	o->st.l=0;
	_init_object_stack(o);
	o->_n_sc_id=1;
	sll_string_length_t i=0;
	while (*(fp+i)){
		i++;
	}
	IGNORE_RESULT(sll_create_string(&(o->st),fp,i));
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_parse_object(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e,sll_object_t** o){
	sll_object_t* a=c_dt->_s.p;
	SLL_ASSERT(a);
	sll_read_char_t c=SLL_READ_FROM_INPUT_DATA_STREAM(c_dt->is);
	if (c==SLL_END_OF_DATA){
		a->t=SLL_OBJECT_TYPE_INT;
		a->dt.i=0;
		return SLL_RETURN_NO_ERROR;
	}
	new_variable_data_t nv_dt={
		NULL,
		0
	};
	extra_compilation_data_t e_c_dt={
		0,
		{
			malloc(sizeof(uint64_t)),
			0,
			1
		},
		i_ft,
		il,
		&nv_dt
	};
	e_c_dt.sc.m[0]=1;
	if (!_read_object_internal(c_dt,c,&e_c_dt,e)){
		free(e_c_dt.sc.m);
		free(nv_dt.dt);
		return SLL_RETURN_ERROR;
	}
	free(e_c_dt.sc.m);
	free(nv_dt.dt);
	*o=a;
	return SLL_RETURN_NO_ERROR;
}



__SLL_FUNC __SLL_CHECK_OUTPUT sll_return_t sll_parse_all_objects(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e){
	c_dt->h=_acquire_next_object(c_dt);
	c_dt->h->t=SLL_OBJECT_TYPE_OPERATION_LIST;
	c_dt->h->dt.sc=0;
	new_variable_data_t nv_dt={
		NULL,
		0
	};
	extra_compilation_data_t e_c_dt={
		0,
		{
			malloc(sizeof(uint64_t)),
			0,
			1
		},
		i_ft,
		il,
		&nv_dt
	};
	e_c_dt.sc.m[0]=1;
	while (1){
		sll_read_char_t c=SLL_READ_FROM_INPUT_DATA_STREAM(c_dt->is);
		if (c==SLL_END_OF_DATA){
			free(e_c_dt.sc.m);
			free(nv_dt.dt);
			return SLL_RETURN_NO_ERROR;
		}
		sll_file_offset_t off=SLL_GET_INPUT_DATA_STREAM_OFFSET(c_dt->is);
		if (!_read_object_internal(c_dt,c,&e_c_dt,e)){
			free(e_c_dt.sc.m);
			free(nv_dt.dt);
			return SLL_RETURN_ERROR;
		}
		if (c_dt->h->dt.sc==SLL_MAX_STATEMENT_COUNT){
			e->t=SLL_ERROR_TOO_MANY_STATEMENTS;
			e->dt.r.off=off-1;
			e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(c_dt->is)-off-1;
			free(e_c_dt.sc.m);
			free(nv_dt.dt);
			return SLL_RETURN_ERROR;
		}
		c_dt->h->dt.sc++;
	}
}

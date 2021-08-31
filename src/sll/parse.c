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



static sll_object_offset_t _patch_module(sll_object_t* mo,const import_module_data_t* im_dt){
	sll_object_offset_t eoff=0;
	sll_object_t* o=im_dt->d+(mo-im_dt->s);
	while (mo->t==SLL_OBJECT_TYPE_NOP){
		eoff++;
		o->t=SLL_OBJECT_TYPE_NOP;
		o++;
		mo++;
	}
	*o=*mo;
	switch (o->t){
		case SLL_OBJECT_TYPE_UNKNOWN:
		case SLL_OBJECT_TYPE_CHAR:
		case SLL_OBJECT_TYPE_INT:
		case SLL_OBJECT_TYPE_FLOAT:
			return eoff+1;
		case SLL_OBJECT_TYPE_STRING:
			o->dt.s=*(im_dt->sm+o->dt.s);
			return eoff+1;
		case SLL_OBJECT_TYPE_IDENTIFIER:
			for (sll_export_table_length_t i=0;i<im_dt->eiml;i++){
				if ((im_dt->eim+i)->a==o->dt.id){
					o->dt.id=(im_dt->eim+i)->b;
					return eoff+1;
				}
			}
			o->dt.id=SLL_IDENTIFIER_ADD_INDEX(o->dt.id,im_dt->off[SLL_IDENTIFIER_GET_ARRAY_ID(o->dt.id)]);
			return eoff+1;
		case SLL_OBJECT_TYPE_FUNC:
			o->dt.fn.id+=im_dt->f_off;
		case SLL_OBJECT_TYPE_INTERNAL_FUNC:
			{
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.l.ac;
				while (l){
					l--;
					off+=_patch_module(mo+off,im_dt);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_FOR:
		case SLL_OBJECT_TYPE_WHILE:
		case SLL_OBJECT_TYPE_LOOP:
			{
				o->dt.l.sc+=im_dt->sc_off;
				sll_object_offset_t off=1;
				sll_arg_count_t l=o->dt.l.ac;
				while (l){
					l--;
					off+=_patch_module(mo+off,im_dt);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_OPERATION_LIST:
			{
				sll_object_offset_t off=1;
				sll_statement_count_t l=o->dt.sc;
				while (l){
					l--;
					off+=_patch_module(mo+off,im_dt);
				}
				return off+eoff;
			}
		case SLL_OBJECT_TYPE_DEBUG_DATA:
			o->dt.dbg.fpi=*(im_dt->sm+o->dt.dbg.fpi);
			return eoff+_patch_module(mo+1,im_dt)+1;
	}
	sll_object_offset_t off=1;
	sll_arg_count_t l=o->dt.ac;
	while (l){
		l--;
		off+=_patch_module(mo+off,im_dt);
	}
	return off+eoff;
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
			return 0;
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
			if (o->t==SLL_OBJECT_TYPE_ASSIGN&&ac==1&&(o+2)->t==SLL_OBJECT_TYPE_IDENTIFIER&&o+2==*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)){
				e_c_dt->nv_dt->sz--;
			}
			if (o->t==SLL_OBJECT_TYPE_UNKNOWN){
				c_dt->_s.off+=sizeof(sll_object_t);
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
					while (a->t==SLL_OBJECT_TYPE_DEBUG_DATA){
						a++;
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
					sll_object_offset_t off=1;
					sll_arg_count_t i=0;
					for (;i<ac;i++){
						while ((o+off)->t==SLL_OBJECT_TYPE_NOP||(o+off)->t==SLL_OBJECT_TYPE_DEBUG_DATA){
							(o+off)->t=SLL_OBJECT_TYPE_NOP;
							off++;
						}
						if ((o+off)->t!=SLL_OBJECT_TYPE_IDENTIFIER){
							break;
						}
						off++;
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
					f->off=(sll_object_offset_t)(o-c_dt->h);
					f->al=i;
					off=1;
					sll_object_offset_t eoff=1;
					for (sll_arg_count_t j=0;j<i;j++){
						while ((o+off)->t==SLL_OBJECT_TYPE_NOP){
							off++;
						}
						SLL_ASSERT((o+off)->t==SLL_OBJECT_TYPE_IDENTIFIER);
						f->a[j]=(o+off)->dt.id;
						off++;
						eoff=off;
					}
					for (off=1;off<eoff;off++){
						(o+off)->t=SLL_OBJECT_TYPE_NOP;
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
					sll_object_offset_t off=1;
					while ((o+off)->t==SLL_OBJECT_TYPE_NOP||(o+off)->t==SLL_OBJECT_TYPE_DEBUG_DATA){
						(o+off)->t=SLL_OBJECT_TYPE_NOP;
						off++;
					}
					if ((o+off)->t!=SLL_OBJECT_TYPE_STRING){
						o->t=SLL_OBJECT_TYPE_OPERATION_LIST;
						o->dt.sc=ac;
					}
					else{
						(o+off)->t=SLL_OBJECT_TYPE_NOP;
						sll_string_t* s=c_dt->st.dt+(o+off)->dt.s;
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
			else if (fl&(EXTRA_COMPILATION_DATA_IMPORT|EXTRA_COMPILATION_DATA_EXPORT|EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION)){
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
				if (!sll_insert_debug_object(c_dt,is,e)){
					return SLL_RETURN_ERROR;
				}
				st_off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				o=(sll_object_t*)(c_dt->_s.ptr+c_dt->_s.off);
				if (c=='{'){
					c_dt->_s.off+=sizeof(sll_object_t);
					if (c_dt->_s.off>=c_dt->_s.sz){
						e->t=SLL_ERROR_INTERNAL_STACK_OVERFLOW;
						e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
						e->dt.r.sz=1;
						return SLL_RETURN_ERROR;
					}
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
				if (o->t==SLL_OBJECT_TYPE_ASSIGN&&ac==1&&(o+2)->t==SLL_OBJECT_TYPE_IDENTIFIER&&o+2==*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)){
					e_c_dt->nv_dt->sz--;
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
				else if (o->t==SLL_OBJECT_TYPE_OPERATION_LIST){
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
			char* str=(char*)(c_dt->_s.ptr+c_dt->_s.off);
			sll_string_length_t sz=0;
_read_symbol:
			if (sz==SLL_MAX_STRING_LENGTH){
				e->t=SLL_ERROR_SYMBOL_TOO_LONG;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
				e->dt.r.sz=sz;
				return SLL_RETURN_ERROR;
			}
			*(str+sz)=(sll_char_t)c;
			sz++;
			if (sz+c_dt->_s.off>=c_dt->_s.sz){
				e->t=SLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				return SLL_RETURN_ERROR;
			}
			c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c==SLL_END_OF_DATA){
				break;
			}
			if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='('&&c!='}'&&c!='{')){
				goto _read_symbol;
			}
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
					o->t=SLL_OBJECT_TYPE_EXIT;
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
					o->dt.fn.sc=c_dt->_n_sc_id;
					fl|=EXTRA_COMPILATION_DATA_INSIDE_FUNCTION;
				}
				else if (*str=='.'&&*(str+1)=='.'&&*(str+2)=='.'){
					o->t=SLL_OBJECT_TYPE_INTERNAL_FUNC;
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
			c_dt->_s.off+=sizeof(sll_object_t);
			if (c_dt->_s.off>=c_dt->_s.sz){
				e->t=SLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				return SLL_RETURN_ERROR;
			}
			if (o->t==SLL_OBJECT_TYPE_IF||o->t==SLL_OBJECT_TYPE_FOR||o->t==SLL_OBJECT_TYPE_WHILE||o->t==SLL_OBJECT_TYPE_LOOP||o->t==SLL_OBJECT_TYPE_FUNC){
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
			if (!sll_insert_debug_object(c_dt,is,e)){
				return SLL_RETURN_ERROR;
			}
			st_off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
			o=(sll_object_t*)(c_dt->_s.ptr+c_dt->_s.off);
			c_dt->_s.off+=sizeof(sll_object_t);
			if (c_dt->_s.off>=c_dt->_s.sz){
				e->t=SLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				return SLL_RETURN_ERROR;
			}
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
			if (!sll_insert_debug_object(c_dt,is,e)){
				if (n_l_sc.m){
					free(n_l_sc.m);
				}
				return SLL_RETURN_ERROR;
			}
			sll_file_offset_t arg_s=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
			sll_object_t* arg=(sll_object_t*)(c_dt->_s.ptr+c_dt->_s.off);
			c_dt->_s.off+=sizeof(sll_object_t);
			if (c_dt->_s.off>=c_dt->_s.sz){
				e->t=SLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
				e->dt.r.sz=1;
				if (n_l_sc.m){
					free(n_l_sc.m);
				}
				return SLL_RETURN_ERROR;
			}
			if (c=='\''){
				arg->t=SLL_OBJECT_TYPE_CHAR;
				if (_read_single_char(is,arg_s,e,&(arg->dt.c))){
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return SLL_RETURN_ERROR;
				}
				if (arg->dt.c=='\''){
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
				sll_string_length_t ln=0;
				sll_char_t* s=(sll_char_t*)(c_dt->_s.ptr+c_dt->_s.off);
				while (1){
					if (_read_single_char(is,arg_s,e,s+ln)){
						if (n_l_sc.m){
							free(n_l_sc.m);
						}
						return SLL_RETURN_ERROR;
					}
					if (*(s+ln)=='"'){
						break;
					}
					ln++;
				}
				arg->dt.s=sll_create_string(&(c_dt->st),s,ln);
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
_hexadecimal:
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
							break;
						}
						if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
							goto _hexadecimal;
						}
					}
					else if (c=='o'||c=='O'){
						c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
						if (c==SLL_END_OF_DATA){
							break;
						}
_octal:
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
							break;
						}
						if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
							goto _octal;
						}
					}
					else if (c=='b'||c=='B'){
						c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
						if (c==SLL_END_OF_DATA){
							break;
						}
_binary:
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
							break;
						}
						if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
							goto _binary;
						}
					}
					else if (c=='.'||c=='e'||c=='E'){
						goto _parse_float;
					}
					else if (c>47&&c<58){
						goto _decimal;
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
				}
				else{
_decimal:
					v=v*10+(c-48);
					c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
					if (c==SLL_END_OF_DATA){
						break;
					}
					if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
						if (c=='.'||c=='e'||c=='E'){
							goto _parse_float;
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
						goto _decimal;
					}
				}
				arg->t=SLL_OBJECT_TYPE_INT;
				arg->dt.i=v*m;
				goto _skip_float_parse;
_parse_float:;
				int16_t ex=0;
				if (c=='.'){
_float:
					c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
					if (c==SLL_END_OF_DATA){
						break;
					}
					if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
						if (c=='e'||c=='E'){
							goto _parse_float_exponent;
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
						goto _float;
					}
				}
				else{
_parse_float_exponent:;
					int8_t em=1;
					int16_t ev=0;
					c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
					if (c==SLL_END_OF_DATA){
						break;
					}
					if (c=='-'){
						em=-1;
					}
					else if (c!='+'){
						goto _add_exponent_char;
					}
_float_exponent:
					c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
					if (c==SLL_END_OF_DATA){
						break;
					}
_add_exponent_char:
					if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
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
						goto _float_exponent;
					}
					ex+=ev*em;
				}
				arg->t=SLL_OBJECT_TYPE_FLOAT;
				arg->dt.f=((sll_float_t)v)*pow(5,ex)*pow(2,ex)*m;
_skip_float_parse:;
			}
			else if ((c>64&&c<91)||c=='_'||(c>96&&c<123)){
				sll_char_t* str=(sll_char_t*)(c_dt->_s.ptr+c_dt->_s.off);
				sll_string_length_t sz=0;
				sll_string_checksum_t cs=0;
_read_identifier:
				*(str+sz)=(sll_char_t)c;
				cs^=(sll_string_checksum_t)c;
				sz++;
				if (sz+c_dt->_s.off>=c_dt->_s.sz){
					e->t=SLL_ERROR_INTERNAL_STACK_OVERFLOW;
					e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return SLL_RETURN_ERROR;
				}
				c=SLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c==SLL_END_OF_DATA){
					break;
				}
				if ((c>47&&c<58)||(c>64&&c<91)||c=='_'||(c>96&&c<123)){
					goto _read_identifier;
				}
				if (c<9||(c>13&&c!=' '&&c!='('&&c!=')'&&c!=';'&&c!='<'&&c!='>'&&c!='['&&c!=']'&&c!='{'&&c!='}')){
					e->t=SLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER;
					e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
					e->dt.r.sz=1;
					if (n_l_sc.m){
						free(n_l_sc.m);
					}
					return SLL_RETURN_ERROR;
				}
				if ((sz==3&&*str=='n'&&*(str+1)=='i'&&*(str+2)=='l')||(sz==5&&*str=='f'&&*(str+1)=='a'&&*(str+2)=='l'&&*(str+3)=='s'&&*(str+4)=='e')){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=0;
				}
				else if (sz==4&&*str=='t'&&*(str+1)=='r'&&*(str+2)=='u'&&*(str+3)=='e'){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=1;
				}
				else if (sz==5&&*str=='s'&&*(str+1)=='t'&&*(str+2)=='d'&&*(str+3)=='i'&&*(str+4)=='n'){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=-1;
				}
				else if (sz==6&&*str=='s'&&*(str+1)=='t'&&*(str+2)=='d'&&*(str+3)=='o'&&*(str+4)=='u'&&*(str+5)=='t'){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=-2;
				}
				else if (sz==6&&*str=='s'&&*(str+1)=='t'&&*(str+2)=='d'&&*(str+3)=='e'&&*(str+4)=='r'&&*(str+5)=='r'){
					arg->t=SLL_OBJECT_TYPE_INT;
					arg->dt.i=-3;
				}
				else{
					arg->t=SLL_OBJECT_TYPE_IDENTIFIER;
					if (sz<=SLL_MAX_SHORT_IDENTIFIER_LENGTH){
						sll_identifier_list_t* k=c_dt->idt.s+sz-1;
						sll_scope_t mx_sc=SLL_MAX_SCOPE;
						sll_identifier_index_t mx_i=0;
						for (sll_identifier_list_length_t i=0;i<k->l;i++){
							sll_identifier_t* si=k->dt+i;
							sll_string_t* si_s=c_dt->st.dt+si->i;
							if (si_s->c!=cs||si->sc==SLL_MAX_SCOPE){
								continue;
							}
							for (sll_string_length_t j=0;j<sz;j++){
								if (*(str+j)!=si_s->v[j]){
									goto _next_short_identifier;
								}
							}
							if (si->sc==l_sc->l_sc){
								arg->dt.id=SLL_CREATE_IDENTIFIER(i,sz-1);
								goto _check_new_variable;
							}
							else if ((l_sc->m[si->sc>>6]&(1ull<<(si->sc&0x3f)))&&(mx_sc==SLL_MAX_SCOPE||si->sc>mx_sc)){
								mx_sc=si->sc;
								mx_i=SLL_CREATE_IDENTIFIER(i,sz-1);
							}
_next_short_identifier:;
						}
						if (mx_sc!=SLL_MAX_SCOPE){
							arg->dt.id=mx_i;
						}
						else{
							if ((o->t!=SLL_OBJECT_TYPE_ASSIGN||ac)&&o->t!=SLL_OBJECT_TYPE_FUNC&&!(fl&EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION)){
								e->t=SLL_ERROR_UNKNOWN_IDENTIFIER;
								e->dt.r.off=arg_s;
								e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
								if (n_l_sc.m){
									free(n_l_sc.m);
								}
								return SLL_RETURN_ERROR;
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
					}
					else{
						sll_scope_t mx_sc=SLL_MAX_SCOPE;
						sll_identifier_index_t mx_i=0;
						for (sll_identifier_list_length_t i=0;i<c_dt->idt.ill;i++){
							sll_identifier_t* k=c_dt->idt.il+i;
							sll_string_t* s=c_dt->st.dt+k->i;
							if (s->c!=cs||s->l!=sz||k->sc==SLL_MAX_SCOPE){
								continue;
							}
							for (sll_string_length_t j=0;j<sz;j++){
								if (*(str+j)!=s->v[j]){
									goto _next_long_identifier;
								}
							}
							if (k->sc==l_sc->l_sc){
								arg->dt.id=SLL_CREATE_IDENTIFIER(i,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
								goto _check_new_variable;
							}
							else if ((l_sc->m[k->sc>>6]&(1ull<<(k->sc&0x3f)))&&(mx_sc==SLL_MAX_SCOPE||k->sc>mx_sc)){
								mx_sc=k->sc;
								mx_i=SLL_CREATE_IDENTIFIER(i,SLL_MAX_SHORT_IDENTIFIER_LENGTH);
							}
_next_long_identifier:;
						}
						if (mx_sc!=SLL_MAX_SCOPE){
							arg->dt.id=mx_i;
						}
						else{
							if ((o->t!=SLL_OBJECT_TYPE_ASSIGN||ac)&&o->t!=SLL_OBJECT_TYPE_FUNC&&!(fl&EXTRA_COMPILATION_DATA_VARIABLE_DEFINITION)){
								e->t=SLL_ERROR_UNKNOWN_IDENTIFIER;
								e->dt.r.off=arg_s;
								e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
								if (n_l_sc.m){
									free(n_l_sc.m);
								}
								return SLL_RETURN_ERROR;
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
					}
_check_new_variable:;
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
			else if (o->t==SLL_OBJECT_TYPE_OPERATION_LIST){
				if (sc==SLL_MAX_STATEMENT_COUNT){
					e->t=SLL_ERROR_TOO_MANY_STATEMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
					return SLL_RETURN_ERROR;
				}
				sc++;
				if ((fl&EXTRA_COMPILATION_DATA_IMPORT)&&arg->t==SLL_OBJECT_TYPE_STRING){
					sll_compilation_data_t im={0};
					if (e_c_dt->il(c_dt->st.dt+arg->dt.s,&im,e)==SLL_RETURN_ERROR){
						return SLL_RETURN_ERROR;
					}
					c_dt->_s.off-=sizeof(sll_object_t)*2;
					arg->t=SLL_OBJECT_TYPE_NOP;
					while (arg->t==SLL_OBJECT_TYPE_NOP||arg->t==SLL_OBJECT_TYPE_DEBUG_DATA){
						arg->t=SLL_OBJECT_TYPE_NOP;
						arg--;
					}
					arg++;
					import_module_data_t im_dt={
						.sm=malloc(im.st.l*sizeof(sll_string_index_t)),
						.f_off=c_dt->ft.l,
						.s=im.h,
						.d=arg,
						.eiml=im.et.l,
						.sc_off=c_dt->_n_sc_id
					};
					for (sll_string_index_t i=0;i<im.st.l;i++){
						sll_string_t* s=im.st.dt+i;
						for (sll_string_index_t j=0;j<c_dt->st.l;j++){
							sll_string_t* d=c_dt->st.dt+j;
							if (s->c!=d->c||s->l!=d->l){
								continue;
							}
							for (sll_string_length_t k=0;k<s->l;k++){
								if (s->v[k]!=d->v[k]){
									goto _check_next_string;
								}
							}
							*(im_dt.sm+i)=j;
							goto _merge_next_string;
_check_next_string:;
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
						nf->off=(sll_object_offset_t)(f->off+(arg-c_dt->h));
						nf->al=f->al;
						for (sll_arg_count_t k=0;k<f->al;k++){
							nf->a[k]=f->a[k];
						}
						*(c_dt->ft.dt+i+j)=nf;
					}
					c_dt->_n_sc_id+=im._n_sc_id;
					sll_object_offset_t im_sz=_patch_module(im.h,&im_dt);
					free(im_dt.sm);
					free(im_dt.eim);
					sll_free_compilation_data(&im);
					c_dt->_s.off+=im_sz*sizeof(sll_object_t);
					if (c_dt->_s.off>=c_dt->_s.sz){
						e->t=SLL_ERROR_INTERNAL_STACK_OVERFLOW;
						e->dt.r.off=0;
						e->dt.r.sz=1;
						return SLL_RETURN_ERROR;
					}
				}
				else if ((fl&EXTRA_COMPILATION_DATA_EXPORT)&&arg->t==SLL_OBJECT_TYPE_IDENTIFIER){
					sc--;
					for (sll_export_table_length_t j=0;j<c_dt->et.l;j++){
						if (*(c_dt->et.dt+j)==arg->dt.id){
							goto _skip_export;
						}
					}
					c_dt->et.l++;
					c_dt->et.dt=realloc(c_dt->et.dt,c_dt->et.l*sizeof(sll_identifier_index_t));
					*(c_dt->et.dt+c_dt->et.l-1)=arg->dt.id;
_skip_export:;
					arg->t=SLL_OBJECT_TYPE_NOP;
					while (arg->t==SLL_OBJECT_TYPE_NOP||arg->t==SLL_OBJECT_TYPE_DEBUG_DATA){
						arg->t=SLL_OBJECT_TYPE_NOP;
						arg--;
					}
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
				else if (o->t==SLL_OBJECT_TYPE_ASSIGN&&ac==1&&(o+2)->t==SLL_OBJECT_TYPE_IDENTIFIER&&o+2==*(e_c_dt->nv_dt->dt+e_c_dt->nv_dt->sz-1)){
					e_c_dt->nv_dt->sz--;
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
		o=(sll_object_t*)(c_dt->_s.ptr+c_dt->_s.off);
		c_dt->_s.off+=sizeof(sll_object_t);
		if (c_dt->_s.off>=c_dt->_s.sz){
			e->t=SLL_ERROR_INTERNAL_STACK_OVERFLOW;
			e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(is)-1;
			e->dt.r.sz=1;
			return SLL_RETURN_ERROR;
		}
		o->t=SLL_OBJECT_TYPE_INT;
		o->dt.i=0;
		return SLL_RETURN_NO_ERROR;
	}
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
	o->_n_sc_id=1;
	sll_string_length_t i=0;
	while (*(fp+i)){
		i++;
	}
	IGNORE_RESULT(sll_create_string(&(o->st),fp,i));
}



__SLL_FUNC __SLL_RETURN sll_parse_object(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e,sll_object_t** o){
	if (!c_dt->_s.ptr){
		e->t=SLL_ERROR_NO_STACK;
		return SLL_RETURN_ERROR;
	}
	sll_object_t* a=(sll_object_t*)(c_dt->_s.ptr+c_dt->_s.off);
	sll_read_char_t c=SLL_READ_FROM_INPUT_DATA_STREAM(c_dt->is);
	if (c==SLL_END_OF_DATA){
		c_dt->_s.off+=sizeof(sll_object_t);
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



__SLL_FUNC __SLL_RETURN sll_parse_all_objects(sll_compilation_data_t* c_dt,sll_internal_function_table_t* i_ft,sll_import_loader_t il,sll_error_t* e){
	if (!c_dt->_s.ptr){
		e->t=SLL_ERROR_NO_STACK;
		return SLL_RETURN_ERROR;
	}
	c_dt->h=(sll_object_t*)(c_dt->_s.ptr+c_dt->_s.off);
	c_dt->_s.off+=sizeof(sll_object_t);
	if (c_dt->_s.off>=c_dt->_s.sz){
		e->t=SLL_ERROR_INTERNAL_STACK_OVERFLOW;
		e->dt.r.off=SLL_GET_INPUT_DATA_STREAM_OFFSET(c_dt->is)-1;
		e->dt.r.sz=1;
		return SLL_RETURN_ERROR;
	}
	c_dt->h->t=SLL_OBJECT_TYPE_OPERATION_LIST;
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
	sll_statement_count_t sc=0;
	while (1){
		sll_read_char_t c=SLL_READ_FROM_INPUT_DATA_STREAM(c_dt->is);
		if (c==SLL_END_OF_DATA){
			c_dt->h->dt.sc=sc;
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
		if (sc==SLL_MAX_STATEMENT_COUNT){
			e->t=SLL_ERROR_TOO_MANY_STATEMENTS;
			e->dt.r.off=off-1;
			e->dt.r.sz=SLL_GET_INPUT_DATA_STREAM_OFFSET(c_dt->is)-off-1;
			free(e_c_dt.sc.m);
			free(nv_dt.dt);
			return SLL_RETURN_ERROR;
		}
		sc++;
	}
}

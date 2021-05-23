#include <lll_lib.h>
#include <_lll_internal.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



uint8_t FAST_COMPARE_STR(char* a,char* b,uint32_t sz){
	for (uint32_t i=0;i<sz;i++){
		if (*(a+i)!=*(b+i)){
			return 0;
		}
	}
	return 1;
}



#define COMPARE_IDENTIFIER_LIST(o,c_dt,sz,str,i,l_sc) \
	if ((sz)==(i)){ \
		lll_identifier_list_t* e=(c_dt)->i_dt.s+((i)-1); \
		uint32_t mx_sc=UINT32_MAX; \
		lll_identifier_index_t mx_i; \
		for (uint32_t j=0;j<e->l;j++){ \
			lll_small_identifier_t* si=e->dt+j; \
			if (FAST_COMPARE_STR((str),si->v,(i))){ \
				if (si->sc==(l_sc)){ \
					LLL_SET_OBJECT_AS_IDENTIFIER(o,LLL_CREATE_IDENTIFIER(j,(i)-1)); \
					goto _identifier_found; \
				} \
				else if (mx_sc==UINT32_MAX||si->sc>mx_sc){ \
					mx_sc=si->sc; \
					mx_i=LLL_CREATE_IDENTIFIER(j,(i)-1); \
				} \
			} \
		} \
		if (mx_sc!=UINT32_MAX){ \
			LLL_SET_OBJECT_AS_IDENTIFIER(o,mx_i); \
			goto _identifier_found; \
		} \
		e->l++; \
		e->dt=realloc(e->dt,e->l*sizeof(lll_small_identifier_t)); \
		(e->dt+e->l-1)->v=malloc((i)*sizeof(char)); \
		char* d=(e->dt+e->l-1)->v; \
		for (uint32_t j=0;j<(i);j++){ \
			*(d+j)=*((str)+j); \
		} \
		(e->dt+e->l-1)->sc=(l_sc); \
		LLL_SET_OBJECT_AS_IDENTIFIER(o,LLL_CREATE_IDENTIFIER(e->l-1,(i)-1)); \
	}



uint8_t _read_single_char(lll_input_data_stream_t* is,char t,uint32_t st,lll_error_t* e){
	int c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
	if (c==LLL_END_OF_DATA){
		e->t=LLL_ERROR_UNMATCHED_QUOTES;
		e->dt.r.off=st;
		e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
		return READ_SINGLE_CHAR_ERROR;
	}
	if (c==t){
		return READ_SINGLE_CHAR_END;
	}
	if (c=='\r'||c=='\n'){
		e->t=LLL_ERROR_UNMATCHED_QUOTES;
		e->dt.r.off=st;
		e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
		return READ_SINGLE_CHAR_ERROR;
	}
	if (c=='\\'){
		c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
		if (c=='\r'||c=='\n'){
			e->t=LLL_ERROR_UNMATCHED_QUOTES;
			e->dt.r.off=st;
			e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
			return READ_SINGLE_CHAR_ERROR;
		}
		if (c==LLL_END_OF_DATA){
			e->t=LLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE;
			e->dt.r.off=st;
			e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
			return READ_SINGLE_CHAR_ERROR;
		}
		if (c=='\''||c=='"'||c=='\\'){
			goto _skip_parse;
		}
		else if (c=='x'){
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c==LLL_END_OF_DATA){
				e->t=LLL_ERROR_UNMATCHED_QUOTES;
				e->dt.r.off=st;
				e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
				return READ_SINGLE_CHAR_ERROR;
			}
			if (c>96){
				c-=32;
			}
			if (c<48||(c>57&&c<65)||c>70){
				if (c=='\r'||c=='\n'){
					e->t=LLL_ERROR_UNMATCHED_QUOTES;
					e->dt.r.off=st;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
					return READ_SINGLE_CHAR_ERROR;
				}
				e->t=LLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
				return READ_SINGLE_CHAR_ERROR;
			}
			uint8_t v=(c>64?c-55:c-48)<<4;
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c==LLL_END_OF_DATA){
				e->t=LLL_ERROR_UNMATCHED_QUOTES;
				e->dt.r.off=st;
				e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
				return READ_SINGLE_CHAR_ERROR;
			}
			if (c>96){
				c-=32;
			}
			if (c<48||(c>57&&c<65)||c>70){
				if (c=='\r'||c=='\n'){
					e->t=LLL_ERROR_UNMATCHED_QUOTES;
					e->dt.r.off=st;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
					return READ_SINGLE_CHAR_ERROR;
				}
				e->t=LLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
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
			e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
			e->dt.r.sz=1;
			return READ_SINGLE_CHAR_ERROR;
		}
	}
_skip_parse:
	*((char*)(_bf+_bf_off))=c;
	_bf_off++;
	if (_bf_off>=_bf_sz){
		e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
		e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
		e->dt.r.sz=1;
		return READ_SINGLE_CHAR_ERROR;
	}
	return READ_SINGLE_CHAR_OK;
}



uint8_t _read_object_internal(lll_compilation_data_t* c_dt,int c,uint32_t l_sc,lll_error_t* e){
	lll_input_data_stream_t* is=c_dt->is;
	uint32_t st_off=UINT32_MAX;
	int ec=-1;
	lll_object_t* o=NULL;
	lll_arg_count_t* ac=NULL;
	lll_statement_count_t* sc=NULL;
	lll_object_modifier_t am=0;
	while (c!=LLL_END_OF_DATA){
		if ((c>8&&c<14)||c==' '){
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			continue;
		}
		if (c==';'){
			while (c!='\n'&&c!='\r'&&c!=LLL_END_OF_DATA){
				c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			}
			continue;
		}
		if (c=='|'){
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c!='#'){
				e->t=LLL_ERROR_UNEXPECTED_CHARACTER;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-3;
				e->dt.r.sz=1;
				return LLL_RETURN_ERROR;
			}
			int lc=c;
			while (c!='|'||lc!='#'){
				lc=c;
				c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c==LLL_END_OF_DATA){
					e->t=LLL_ERROR_UNMATCHED_OPEN_QUOTE;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-3;
					e->dt.r.sz=1;
					return LLL_RETURN_ERROR;
				}
			}
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			continue;
		}
		if (c==')'){
			if (!o||ec!=')'){
				e->t=LLL_ERROR_UNMATCHED_CLOSE_PARENTHESES;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
				return LLL_RETURN_ERROR;
			}
			if (am){
				e->t=LLL_ERROR_UNUSED_MODIFIERS;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
				return LLL_RETURN_ERROR;
			}
			if (LLL_IS_OBJECT_UNKNOWN(o)){
				o->t=LLL_OBJECT_TYPE_NIL;
				_bf_off-=sizeof(lll_arg_count_t);
			}
			else if (LLL_IS_OBJECT_TYPE_MATH_NO_CHAIN(o)){
				if (*ac==1){
					e->t=LLL_ERROR_MATH_OP_NOT_ENOUGH_ARGUMENTS;
					e->dt.r.off=st_off;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st_off-1;
					return LLL_RETURN_ERROR;
				}
				if (*ac>(LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_POW?3:2)){
					e->t=LLL_ERROR_MATH_OP_TOO_MANY_ARGUMENTS;
					e->dt.r.off=st_off;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st_off-1;
					return LLL_RETURN_ERROR;
				}
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
				lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,sizeof(lll_object_t)+sizeof(lll_arg_count_t));
				while (LLL_GET_OBJECT_TYPE(a)==LLL_OBJECT_TYPE_DEBUG_DATA){
					lll_debug_object_t* dbg=(lll_debug_object_t*)a;
					a=LLL_GET_DEBUG_OBJECT_CHILD(dbg,sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg));
				}
				if (LLL_GET_OBJECT_TYPE(a)!=LLL_OBJECT_TYPE_IDENTIFIER){
					e->t=LLL_ERROR_SET_NO_INDENTIFIER;
					e->dt.r.off=st_off;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st_off-1;
					return LLL_RETURN_ERROR;
				}
			}
			return LLL_RETURN_NO_ERROR;
		}
		if (c=='}'){
			if (!o||ec!='}'){
				e->t=LLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
				return LLL_RETURN_ERROR;
			}
			if (am){
				e->t=LLL_ERROR_UNUSED_MODIFIERS;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
				return LLL_RETURN_ERROR;
			}
			return LLL_RETURN_NO_ERROR;
		}
		if (c=='('||c=='{'){
			if (!o){
				if (!lll_insert_debug_object(is,e)){
					return LLL_RETURN_ERROR;
				}
				st_off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				o=(lll_object_t*)(_bf+_bf_off);
				_bf_off+=sizeof(lll_object_t)+(c=='('?sizeof(lll_arg_count_t):sizeof(lll_statement_count_t));
				if (_bf_off>=_bf_sz){
					e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
					e->dt.r.sz=1;
					return LLL_RETURN_ERROR;
				}
				o->t=(c=='('?LLL_OBJECT_TYPE_UNKNOWN:LLL_OBJECT_TYPE_OPERATION_LIST|LLL_OBJECT_TYPE_CONST);
				o->m=0;
				ec=(c=='('?')':'}');
				if (c=='('){
					ac=LLL_GET_OBJECT_ARGUMENT_COUNT(o);
					*ac=0;
				}
				else{
					sc=LLL_GET_OBJECT_STATEMENT_COUNT(o);
					*sc=0;
				}
			}
			else{
				uint32_t arg_s=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				if (LLL_IS_OBJECT_UNKNOWN(o)){
					e->t=LLL_ERROR_NO_SYMBOL;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
					e->dt.r.sz=1;
					return LLL_RETURN_ERROR;
				}
				lll_object_t* so=(lll_object_t*)(_bf+_bf_off);
				if (!_read_object_internal(c_dt,c,l_sc,e)){
					return LLL_RETURN_ERROR;
				}
				if (LLL_GET_OBJECT_TYPE(so)==LLL_OBJECT_TYPE_DEBUG_DATA){
					lll_debug_object_t* dbg=(lll_debug_object_t*)so;
					so=LLL_GET_DEBUG_OBJECT_CHILD(dbg,sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg));
				}
				if (LLL_IS_OBJECT_TYPE_NOT_TYPE(so)&&!LLL_IS_OBJECT_CONST(so)){
					o->t&=~LLL_OBJECT_TYPE_CONST;
				}
				if (LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_OPERATION_LIST){
					if (*sc==UINT16_MAX){
						e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
						e->dt.r.off=arg_s;
						e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
						return LLL_RETURN_ERROR;
					}
					(*sc)++;
				}
				else{
					if (*ac==UINT8_MAX){
						e->t=LLL_ERROR_TOO_MANY_ARGUMENTS;
						e->dt.r.off=arg_s;
						e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
						return LLL_RETURN_ERROR;
					}
					(*ac)++;
				}
			}
		}
		else if (c=='@'){
			if (!o){
				e->t=LLL_ERROR_UNEXPECTED_CHARACTER;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
				return LLL_RETURN_ERROR;
			}
			char* str=(char*)(_bf+_bf_off);
			uint16_t sz=0;
_read_modifier:
			if (sz==UINT16_MAX){
				e->t=LLL_ERROR_MODIFIER_TOO_LONG;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz;
				e->dt.r.sz=sz;
				return LLL_RETURN_ERROR;
			}
			*(str+sz)=c;
			sz++;
			if (sz+_bf_off>=_bf_sz){
				e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
				return LLL_RETURN_ERROR;
			}
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c==LLL_END_OF_DATA){
				break;
			}
			if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='('&&c!='}'&&c!='{')){
				goto _read_modifier;
			}
			sz--;
			str++;
			if (sz==4){
				if (FAST_COMPARE(str,8,b,i,t)){
					if (LLL_IS_OBJECT_UNKNOWN(o)){
						if (o->m&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->dt.r.sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m=(o->m&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_8BIT;
					}
					else{
						if (am&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->dt.r.sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						am=(am&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_8BIT;
					}
				}
				else if (FAST_COMPARE(str,l,i,s,t)){
					if (LLL_IS_OBJECT_UNKNOWN(o)){
						if (o->m&LLL_OBJECT_MODIFIER_OUTPUT_TYPE_MASK){
							e->t=LLL_ERROR_MULTIPLE_OUTPUT_TYPE_MODIFIERS;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->dt.r.sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m|=LLL_OBJECT_MODIFIER_LIST;
					}
					else{
						e->t=LLL_ERROR_UNKNOWN_MODIFIER;
						e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
						e->dt.r.sz=sz+1;
						return LLL_RETURN_ERROR;
					}
				}
				else if (FAST_COMPARE(str,l,a,s,t)){
					if (LLL_IS_OBJECT_UNKNOWN(o)){
						if (o->m&LLL_OBJECT_MODIFIER_OUTPUT_TYPE_MASK){
							e->t=LLL_ERROR_MULTIPLE_OUTPUT_TYPE_MODIFIERS;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->dt.r.sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m|=LLL_OBJECT_MODIFIER_LAST;
					}
					else{
						e->t=LLL_ERROR_UNKNOWN_MODIFIER;
						e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
						e->dt.r.sz=sz+1;
						return LLL_RETURN_ERROR;
					}
				}
				else{
					goto _unknown_modifier;
				}
			}
			else if (sz==5){
				if (FAST_COMPARE(str,f,i,x,e,d)){
					if (LLL_IS_OBJECT_UNKNOWN(o)){
						o->m|=LLL_OBJECT_MODIFIER_FIXED;
					}
					else{
						am|=LLL_OBJECT_MODIFIER_FIXED;
					}
				}
				else if (FAST_COMPARE(str,1,6,b,i,t)){
					if (LLL_IS_OBJECT_UNKNOWN(o)){
						if (o->m&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->dt.r.sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m=(o->m&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_16BIT;
					}
					else{
						if (am&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->dt.r.sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						am=(am&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_16BIT;
					}
				}
				else if (FAST_COMPARE(str,3,2,b,i,t)){
					if (LLL_IS_OBJECT_UNKNOWN(o)){
						if (o->m&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->dt.r.sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m=(o->m&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_32BIT;
					}
					else{
						if (am&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->dt.r.sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						am=(am&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_32BIT;
					}
				}
				else if (FAST_COMPARE(str,6,4,b,i,t)){
					if (LLL_IS_OBJECT_UNKNOWN(o)){
						if (o->m&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->dt.r.sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m=(o->m&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_64BIT;
					}
					else{
						if (am&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->dt.r.sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						am=(am&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_64BIT;
					}
				}
				else if (FAST_COMPARE(str,a,r,r,a,y)){
					if (LLL_IS_OBJECT_UNKNOWN(o)){
						if (o->m&LLL_OBJECT_MODIFIER_OUTPUT_TYPE_MASK){
							e->t=LLL_ERROR_MULTIPLE_OUTPUT_TYPE_MODIFIERS;
							e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->dt.r.sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m|=LLL_OBJECT_MODIFIER_ARRAY;
					}
					else{
						e->t=LLL_ERROR_UNKNOWN_MODIFIER;
						e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
						e->dt.r.sz=sz+1;
						return LLL_RETURN_ERROR;
					}
				}
				else{
					goto _unknown_modifier;
				}
			}
			else if (sz==8){
				if (FAST_COMPARE(str,u,n,s,i,g,n,e,d)){
					if (LLL_IS_OBJECT_UNKNOWN(o)){
						o->m|=LLL_OBJECT_MODIFIER_UNSIGNED;
					}
					else{
						am|=LLL_OBJECT_MODIFIER_UNSIGNED;
					}
				}
				else{
					goto _unknown_modifier;
				}
			}
			else{
_unknown_modifier:
				e->t=(LLL_IS_OBJECT_UNKNOWN(o)?LLL_ERROR_UNKNOWN_OUTPUT_MODIFIER:LLL_ERROR_UNKNOWN_MODIFIER);
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
				e->dt.r.sz=sz+1;
				return LLL_RETURN_ERROR;
			}
			continue;
		}
		else if (o&&LLL_IS_OBJECT_UNKNOWN(o)){
			char* str=(char*)(_bf+_bf_off);
			uint16_t sz=0;
_read_symbol:
			if (sz==UINT16_MAX){
				e->t=LLL_ERROR_SYMBOL_TOO_LONG;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
				e->dt.r.sz=sz;
				return LLL_RETURN_ERROR;
			}
			*(str+sz)=c;
			sz++;
			if (sz+_bf_off>=_bf_sz){
				e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
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
				if (FAST_COMPARE(str,&,&)){
					o->t=LLL_OBJECT_TYPE_AND;
				}
				else if (FAST_COMPARE(str,|,|)){
					o->t=LLL_OBJECT_TYPE_OR;
				}
				else if (FAST_COMPARE(str,i,f)){
					o->t=LLL_OBJECT_TYPE_IF;
				}
				else if (FAST_COMPARE(str,/,/)){
					o->t=LLL_OBJECT_TYPE_FLOOR_DIV;
				}
				else if (FAST_COMPARE(str,/,%)){
					o->t=LLL_OBJECT_TYPE_DIV_MOD;
				}
				else if (FAST_COMPARE(str,*,*)){
					o->t=LLL_OBJECT_TYPE_POW;
				}
				else if (FAST_COMPARE(str,*,/)){
					o->t=LLL_OBJECT_TYPE_ROOT;
				}
				else if (FAST_COMPARE(str,_,/)){
					o->t=LLL_OBJECT_TYPE_LOG;
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
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==3){
				if (FAST_COMPARE(str,s,t,r)){
					o->t=LLL_OBJECT_TYPE_CAST_STRING;
				}
				else if (FAST_COMPARE(str,i,n,t)){
					o->t=LLL_OBJECT_TYPE_CAST_INT;
				}
				else if (FAST_COMPARE(str,p,t,r)){
					o->t=LLL_OBJECT_TYPE_FUNC_PTR;
				}
				else if (FAST_COMPARE(str,f,o,r)){
					o->t=LLL_OBJECT_TYPE_FOR;
					l_sc++;
				}
				else if (FAST_COMPARE(str,*,/,/)){
					o->t=LLL_OBJECT_TYPE_FLOOR_ROOT;
				}
				else if (FAST_COMPARE(str,_,/,/)){
					o->t=LLL_OBJECT_TYPE_FLOOR_LOG;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==4){
				if (FAST_COMPARE(str,c,h,a,r)){
					o->t=LLL_OBJECT_TYPE_CAST_CHAR;
				}
				else if (FAST_COMPARE(str,b,o,o,l)){
					o->t=LLL_OBJECT_TYPE_CAST_BOOL;
				}
				else if (FAST_COMPARE(str,f,u,n,c)){
					o->t=LLL_OBJECT_TYPE_FUNC;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==5){
				if (FAST_COMPARE(str,i,n,t,6,4)){
					o->t=LLL_OBJECT_TYPE_CAST_INT64;
				}
				else if (FAST_COMPARE(str,f,l,o,a,t)){
					o->t=LLL_OBJECT_TYPE_CAST_FLOAT;
				}
				else if (FAST_COMPARE(str,p,r,i,n,t)){
					o->t=LLL_OBJECT_TYPE_FUNC_PRINT;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==6){
				if (FAST_COMPARE(str,t,y,p,e,o,f)){
					o->t=LLL_OBJECT_TYPE_FUNC_TYPEOF;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==7){
				if (FAST_COMPARE(str,f,l,o,a,t,6,4)){
					o->t=LLL_OBJECT_TYPE_CAST_FLOAT64;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else{
_unknown_symbol:
				e->t=LLL_ERROR_UNKNOWN_SYMBOL;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-2;
				e->dt.r.sz=sz;
				return LLL_RETURN_ERROR;
			}
			o->t|=LLL_OBJECT_TYPE_CONST;
			continue;
		}
		else if (c=='\''){
			if (!lll_insert_debug_object(is,e)){
				return LLL_RETURN_ERROR;
			}
			uint32_t arg_s=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
			lll_object_t* arg=(lll_object_t*)(_bf+_bf_off);
			_bf_off+=sizeof(lll_object_t);
			if (_bf_off>=_bf_sz){
				e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
				return LLL_RETURN_ERROR;
			}
			arg->t=LLL_OBJECT_TYPE_CHAR;
			arg->m=am;
			am=0;
			uint8_t ce=_read_single_char(is,'\'',arg_s,e);
			if (ce==READ_SINGLE_CHAR_ERROR){
				return LLL_RETURN_ERROR;
			}
			if (ce==READ_SINGLE_CHAR_END){
				return LLL_RETURN_ERROR;
			}
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c!='\''){
				e->t=LLL_ERROR_UNTERMINATED_CHAR_STRING;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
				return LLL_RETURN_ERROR;
			}
			if (!o){
				return LLL_RETURN_NO_ERROR;
			}
			if (LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_OPERATION_LIST){
				if (*sc==UINT16_MAX){
					e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*sc)++;
			}
			else{
				if (*ac==UINT8_MAX){
					e->t=LLL_ERROR_TOO_MANY_ARGUMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*ac)++;
			}
		}
		else if (c=='"'){
			if (!lll_insert_debug_object(is,e)){
				return LLL_RETURN_ERROR;
			}
			uint32_t arg_s=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
			lll_object_t* arg=(lll_object_t*)(_bf+_bf_off);
			_bf_off+=sizeof(lll_object_t)+sizeof(lll_string_length_t);
			if (_bf_off>=_bf_sz){
				e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
				return LLL_RETURN_ERROR;
			}
			arg->t=LLL_OBJECT_TYPE_STRING;
			arg->m=am;
			am=0;
			uint32_t sz=0;
			while (1){
				uint8_t ce=_read_single_char(is,'"',arg_s,e);
				if (ce==READ_SINGLE_CHAR_ERROR){
					return LLL_RETURN_ERROR;
				}
				if (ce==READ_SINGLE_CHAR_END){
					break;
				}
				sz++;
			}
			LLL_SET_OBJECT_STRING_LENGTH(arg,sz);
			if (!o){
				return LLL_RETURN_NO_ERROR;
			}
			if (LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_OPERATION_LIST){
				if (*sc==UINT16_MAX){
					e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*sc)++;
			}
			else{
				if (*ac==UINT8_MAX){
					e->t=LLL_ERROR_TOO_MANY_ARGUMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*ac)++;
			}
		}
		else if ((c>47&&c<58)||c=='-'){
			if (!lll_insert_debug_object(is,e)){
				return LLL_RETURN_ERROR;
			}
			uint32_t arg_s=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
			lll_object_t* arg=(lll_object_t*)(_bf+_bf_off);
			_bf_off+=sizeof(lll_object_t)+sizeof(int64_t);
			if (_bf_off>=_bf_sz){
				e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
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
			arg->m=am;
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
						e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
						e->dt.r.sz=1;
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
						e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
						e->dt.r.sz=1;
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
						e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
						e->dt.r.sz=1;
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
					arg->t=LLL_OBJECT_TYPE_FLOAT;
					goto _parse_float;
				}
				else if (c>47&&c<58){
					goto _decimal;
				}
				if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='('&&c!='}'&&c!='{')){
					e->t=LLL_ERROR_UNKNOWN_DECIMAL_CHARCTER;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
					e->dt.r.sz=1;
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
						e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
						e->dt.r.sz=1;
						return LLL_RETURN_ERROR;
					}
					goto _decimal;
				}
			}
			if (v>INT32_MAX){
				arg->t|=LLL_OBJECT_TYPE_INT64_FLAG;
				LLL_SET_OBJECT_AS_INT64(arg,v*m);
			}
			else if (v>INT16_MAX){
				_bf_off-=sizeof(int64_t)-sizeof(int32_t);
				arg->t|=LLL_OBJECT_TYPE_INT32_FLAG;
				LLL_SET_OBJECT_AS_INT32(arg,v*m);
			}
			else if (v>INT8_MAX){
				_bf_off-=sizeof(int64_t)-sizeof(int16_t);
				arg->t|=LLL_OBJECT_TYPE_INT16_FLAG;
				LLL_SET_OBJECT_AS_INT16(arg,v*m);
			}
			else{
				_bf_off-=sizeof(int64_t)-sizeof(int8_t);
				LLL_SET_OBJECT_AS_INT8(arg,v*m);
			}
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
						e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
						e->dt.r.sz=1;
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
						e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
						e->dt.r.sz=1;
						return LLL_RETURN_ERROR;
					}
					ev=ev*10+(c-48);
					goto _float_exponent;
				}
				ex+=ev*em;
			}
			if (am&LLL_OBJECT_MODIFIER_SIZE){
				if ((am&LLL_OBJECT_MODIFIER_SIZE_MASK)==LLL_OBJECT_MODIFIER_8BIT){
					e->t=LLL_ERROR_UNSUPPORTED_8BIT_FLOAT_SIZE;
					e->dt.r.off=arg_s;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				else if ((am&LLL_OBJECT_MODIFIER_SIZE_MASK)==LLL_OBJECT_MODIFIER_16BIT){
					e->t=LLL_ERROR_UNSUPPORTED_16BIT_FLOAT_SIZE;
					e->dt.r.off=arg_s;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				else if ((am&LLL_OBJECT_MODIFIER_SIZE_MASK)==LLL_OBJECT_MODIFIER_32BIT){
					_bf_off-=sizeof(double)-sizeof(float);
					arg->t=LLL_OBJECT_TYPE_FLOAT;
					LLL_SET_OBJECT_AS_FLOAT32(arg,((float)v)*powf(5,ex)*powf(2,ex)*m);
					goto _skip_float_parse;
				}
			}
			arg->t=LLL_OBJECT_TYPE_FLOAT|LLL_OBJECT_TYPE_FLOAT64_FLAG;
			LLL_SET_OBJECT_AS_FLOAT64(arg,((double)v)*pow(5,ex)*pow(2,ex)*m);
_skip_float_parse:
			am=0;
			if (!o){
				return LLL_RETURN_NO_ERROR;
			}
			if (LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_OPERATION_LIST){
				if (*sc==UINT16_MAX){
					e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*sc)++;
			}
			else{
				if (*ac==UINT8_MAX){
					e->t=LLL_ERROR_TOO_MANY_ARGUMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*ac)++;
			}
			continue;
		}
		else if ((c>64&&c<91)||c=='_'||(c>96&&c<123)){
			if (!lll_insert_debug_object(is,e)){
				return LLL_RETURN_ERROR;
			}
			uint32_t arg_s=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
			lll_object_t* arg=(lll_object_t*)(_bf+_bf_off);
			_bf_off+=sizeof(lll_object_t);
			if (_bf_off>=_bf_sz){
				e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
				return LLL_RETURN_ERROR;
			}
			arg->t=LLL_OBJECT_TYPE_IDENTIFIER;
			arg->m=am;
			am=0;
			char* str=(char*)(_bf+_bf_off);
			uint32_t sz=0;
_read_identifier:
			*(str+sz)=c;
			sz++;
			if (sz+_bf_off>=_bf_sz){
				e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
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
				e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->dt.r.sz=1;
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
				_bf_off+=sizeof(lll_identifier_index_t);
				if (_bf_off>=_bf_sz){
					e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
					e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
					e->dt.r.sz=1;
					return LLL_RETURN_ERROR;
				}
#if LLL_MAX_SHORT_IDENTIFIER_LENGTH!=15
#error The following code is broken
#endif
				COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,1,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,2,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,3,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,4,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,5,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,6,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,7,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,8,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,9,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,10,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,11,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,12,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,13,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,14,l_sc)
				else COMPARE_IDENTIFIER_LIST(arg,c_dt,sz,str,15,l_sc)
				else{
					uint32_t mx_sc=UINT32_MAX;
					lll_identifier_index_t mx_i;
					for (uint32_t i=0;i<c_dt->i_dt.ill;i++){
						lll_identifier_t* e=*(c_dt->i_dt.il+i);
						if (e->sz!=sz){
							continue;
						}
						if (FAST_COMPARE_STR(str,e->v,sz)){
							if (e->sc==l_sc){
								LLL_SET_OBJECT_AS_IDENTIFIER(arg,LLL_CREATE_IDENTIFIER(i,LLL_MAX_SHORT_IDENTIFIER_LENGTH));
								goto _identifier_found;
							}
							else if (mx_sc==UINT32_MAX||e->sc>mx_sc){
								mx_sc=e->sc;
								mx_i=LLL_CREATE_IDENTIFIER(i,LLL_MAX_SHORT_IDENTIFIER_LENGTH);
							}
						}
					}
					if (mx_sc!=UINT32_MAX){
						LLL_SET_OBJECT_AS_IDENTIFIER(arg,mx_i);
						goto _identifier_found;
					}
					c_dt->i_dt.ill++;
					c_dt->i_dt.il=realloc(c_dt->i_dt.il,c_dt->i_dt.ill*sizeof(lll_identifier_t*));
					lll_identifier_t* n=malloc(sizeof(lll_identifier_t)+sz);
					n->sz=sz;
					n->sc=l_sc;
					for (uint32_t i=0;i<sz;i++){
						n->v[i]=*(str+i);
					}
					*(c_dt->i_dt.il+c_dt->i_dt.ill-1)=n;
					LLL_SET_OBJECT_AS_IDENTIFIER(arg,LLL_CREATE_IDENTIFIER(c_dt->i_dt.ill-1,LLL_MAX_SHORT_IDENTIFIER_LENGTH));
				}
_identifier_found:
				if (o){
					o->t&=~LLL_OBJECT_TYPE_CONST;
				}
			}
			if (!o){
				return LLL_RETURN_NO_ERROR;
			}
			if (LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_OPERATION_LIST){
				if (*sc==UINT16_MAX){
					e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*sc)++;
			}
			else{
				if (*ac==UINT8_MAX){
					e->t=LLL_ERROR_TOO_MANY_ARGUMENTS;
					e->dt.r.off=arg_s;
					e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*ac)++;
			}
			continue;
		}
		else{
			e->t=LLL_ERROR_UNEXPECTED_CHARACTER;
			e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
			e->dt.r.sz=1;
			return LLL_RETURN_ERROR;
		}
		c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
	}
	if (!o){
		o=(lll_object_t*)(_bf+_bf_off);
		_bf_off+=sizeof(lll_object_t);
		if (_bf_off>=_bf_sz){
			e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
			e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
			e->dt.r.sz=1;
			return LLL_RETURN_ERROR;
		}
		o->t=LLL_OBJECT_TYPE_NIL;
		return LLL_RETURN_NO_ERROR;
	}
	e->t=LLL_ERROR_UNMATCHED_OPEN_PARENTHESES;
	e->dt.r.off=st_off;
	e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st_off-1;
	return LLL_RETURN_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT lll_object_t* lll_read_object(lll_compilation_data_t* c_dt,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR_AS_OBJECT(LLL_RETURN_ERROR);
	}
	lll_object_t* o=(lll_object_t*)(_bf+_bf_off);
	int c=LLL_READ_FROM_INPUT_DATA_STREAM(c_dt->is);
	if (c==LLL_END_OF_DATA){
		_bf_off+=sizeof(lll_object_t);
		o->t=LLL_OBJECT_TYPE_NIL;
		return o;
	}
	if (!_read_object_internal(c_dt,c,0,e)){
		return LLL_RETURN_ERROR_AS_OBJECT(LLL_RETURN_ERROR);
	}
	return o;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_read_all_objects(lll_compilation_data_t* c_dt,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	c_dt->h=(lll_object_t*)(_bf+_bf_off);
	_bf_off+=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
	if (_bf_off>=_bf_sz){
		e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
		e->dt.r.off=LLL_GET_INPUT_DATA_STREAM_OFFSET(c_dt->is)-2;
		e->dt.r.sz=1;
		return LLL_RETURN_ERROR;
	}
	c_dt->h->t=LLL_OBJECT_TYPE_OPERATION_LIST;
	lll_statement_count_t* sc=LLL_GET_OBJECT_STATEMENT_COUNT(c_dt->h);
	*sc=0;
	while (1){
		int c=LLL_READ_FROM_INPUT_DATA_STREAM(c_dt->is);
		if (c==LLL_END_OF_DATA){
			return LLL_RETURN_NO_ERROR;
		}
		uint32_t off=LLL_GET_INPUT_DATA_STREAM_OFFSET(c_dt->is);
		if (!_read_object_internal(c_dt,c,0,e)){
			return LLL_RETURN_ERROR;
		}
		if (*sc==UINT16_MAX){
			e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
			e->dt.r.off=off-2;
			e->dt.r.sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(c_dt->is)-off-2;
			return LLL_RETURN_ERROR;
		}
		(*sc)++;
	}
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_load_compiled_object(lll_input_data_stream_t* is,lll_compilation_data_t* c_dt,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	compiled_object_file_t dt;
	if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(&dt),sizeof(compiled_object_file_t))||dt.m!=COMPLIED_OBJECT_FILE_MAGIC_NUMBER||!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,(uint8_t*)(c_dt->fp),dt.fpl)){
		e->t=LLL_ERROR_INVALID_FILE_FORMAT;
		return LLL_RETURN_ERROR;
	}
	c_dt->fp[dt.fpl]=0;
	c_dt->fpl=dt.fpl;
	c_dt->is=NULL;
	c_dt->tm=dt.t;
	c_dt->h=(lll_object_t*)(_bf+_bf_off);
	if (_bf_off+dt.sz>_bf_sz){
		e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
		return LLL_RETURN_ERROR;
	}
	if (!LLL_READ_BUFFER_FROM_INPUT_DATA_STREAM(is,_bf+_bf_off,dt.sz)){
		e->t=LLL_ERROR_INVALID_FILE_FORMAT;
		return LLL_RETURN_ERROR;
	}
	_bf_off+=dt.sz;
	return LLL_RETURN_NO_ERROR;
}

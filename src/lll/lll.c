#ifdef _MSC_VER
#include <windows.h>
#include <intrin.h>
#endif
#include <lll.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



#define CONSTRUCT_CHAR(c) ((#c)[0])
#define CONSTRUCT_WORD(a,b) ((((uint16_t)(b))<<8)|(a))
#define CONSTRUCT_DWORD(a,b,c,d) ((((uint32_t)(d))<<24)|(((uint32_t)(c))<<16)|(((uint32_t)(b))<<8)|(a))
#define CONSTRUCT_QWORD(a,b,c,d,e,f,g,h) ((((uint64_t)(h))<<56)|(((uint64_t)(g))<<48)|(((uint64_t)(f))<<40)|(((uint64_t)(e))<<32)|(((uint64_t)(d))<<24)|(((uint64_t)(c))<<16)|(((uint64_t)(b))<<8)|(a))
#define _FAST_COMPARE_JOIN_(l) FAST_COMPARE_##l
#define _FAST_COMPARE_JOIN(l) _FAST_COMPARE_JOIN_(l)
#define _FAST_COMPARE_COUNT_ARGS(_1,_2,_3,_4,_5,_6,_7,_8,n,...) n
#define FAST_COMPARE(s,...) _FAST_COMPARE_JOIN(_FAST_COMPARE_COUNT_ARGS(__VA_ARGS__,8,7,6,5,4,3,2,1))(s,__VA_ARGS__)
#define FAST_COMPARE_1(s,a) (*(s)==CONSTRUCT_CHAR(a))
#define FAST_COMPARE_2(s,a,b) (*((uint16_t*)(s))==CONSTRUCT_WORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b)))
#define FAST_COMPARE_3(s,a,b,c) (*((uint16_t*)(s))==CONSTRUCT_WORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b))&&*((s)+2)==CONSTRUCT_CHAR(c))
#define FAST_COMPARE_4(s,a,b,c,d) (*((uint32_t*)(s))==CONSTRUCT_DWORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b),CONSTRUCT_CHAR(c),CONSTRUCT_CHAR(d)))
#define FAST_COMPARE_5(s,a,b,c,d,e) (*((uint32_t*)(s))==CONSTRUCT_DWORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b),CONSTRUCT_CHAR(c),CONSTRUCT_CHAR(d))&&*((s)+4)==CONSTRUCT_CHAR(e))
#define FAST_COMPARE_6(s,a,b,c,d,e,f) (*((uint32_t*)(s))==CONSTRUCT_DWORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b),CONSTRUCT_CHAR(c),CONSTRUCT_CHAR(d))&&*((uint16_t*)(s+4))==CONSTRUCT_WORD(CONSTRUCT_CHAR(e),CONSTRUCT_CHAR(f)))
#define FAST_COMPARE_7(s,a,b,c,d,e,f,g) (*((uint32_t*)(s))==CONSTRUCT_DWORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b),CONSTRUCT_CHAR(c),CONSTRUCT_CHAR(d))&&*((uint16_t*)(s+4))==CONSTRUCT_WORD(CONSTRUCT_CHAR(e),CONSTRUCT_CHAR(f))&&*((s)+6)==CONSTRUCT_CHAR(g))
#define FAST_COMPARE_8(s,a,b,c,d,e,f,g,h) (*((uint64_t*)(s))==CONSTRUCT_QWORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b),CONSTRUCT_CHAR(c),CONSTRUCT_CHAR(d),CONSTRUCT_CHAR(e),CONSTRUCT_CHAR(f),CONSTRUCT_CHAR(g),CONSTRUCT_CHAR(h)))
#define READ_SINGLE_CHAR_OK 0
#define READ_SINGLE_CHAR_END 1
#define READ_SINGLE_CHAR_ERROR 2
#define READ_REF_FROM_STACK(o) ((lll_object_t*)(_bf+(*((uint32_t*)LLL_GET_OBJECT_WITH_OFFSET((o),sizeof(lll_object_t))))))
#define ERROR_DISPLAY_TAB_WIDTH 4



#define HIGHLIGHT_COLOR "\x1b[31m"
#define HIGHLIGHT_COLOR_RESET "\x1b[0m"
#ifdef _MSC_VER
#pragma intrinsic(__movsb)
#define REPEAT_BYTE_COPY(d,s,sz) __movsb(d,s,sz)
#define UNREACHABLE() __assume(0)
#define ENABLE_COLOR() \
	uint32_t __tv; \
	do{ \
		SetConsoleOutputCP(CP_UTF8); \
		GetConsoleMode(GetStdHandle(-11),&__tv); \
		SetConsoleMode(GetStdHandle(-11),7); \
	} while (0)
#define DISABLE_COLOR() \
	do{ \
		SetConsoleMode(GetStdHandle(-11),__tv); \
	} while (0)
#else
static inline void REPEAT_BYTE_COPY(unsigned char* d,unsigned char* s,size_t n){
	__asm__("rep movsb":"=D"(d),"=S"(s),"=c"(n):"0"(d),"1"(s),"2"(n):"memory");
}
#define UNREACHABLE() __builtin_unreachable()
#define ENABLE_COLOR()
#define DISABLE_COLOR()
#endif



uint8_t* _bf=NULL;
uint32_t _bf_off;
uint32_t _bf_sz;



int _input_data_stream_file_read(lll_input_data_stream_t* is){
	int o=fgetc((FILE*)(is->ctx));
	if (o==EOF){
		return LLL_END_OF_DATA;
	}
	is->_off++;
	if (o=='\n'){
		is->_lc++;
		is->_loff=is->_off;
	}
	return o;
}



void _input_data_stream_file_restart_line(lll_input_data_stream_t* is,uint32_t lp){
	FILE* f=(FILE*)(is->ctx);
	fseek(f,lp,SEEK_SET);
	int c=fgetc(f);
	while (lp&&c!='\n'&&c!='\r'){
		lp--;
		fseek(f,lp,SEEK_SET);
		c=fgetc(f);
	}
	is->_off=lp;
}



uint8_t _output_data_stream_file_write(lll_output_data_stream_t* os,uint8_t* bf,size_t sz){
	return (fwrite(bf,sizeof(uint8_t),sz,(FILE*)(os->ctx))==sz);
}



uint32_t _print_object_internal(lll_object_t* o,FILE* f){
	uint32_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	if (LLL_IS_OBJECT_TYPE_NOT_INTEGRAL(o)){
		while (LLL_IS_OBJECT_REF(o)){
			o=READ_REF_FROM_STACK(o);
		}
	}
	if (LLL_GET_OBJECT_TYPE(o)>LLL_OBJECT_TYPE_MAX_TYPE&&LLL_GET_OBJECT_TYPE(o)<LLL_OBJECT_TYPE_MIN_EXTRA){
		fputc('(',f);
	}
	if (LLL_GET_OBJECT_TYPE(o)<LLL_OBJECT_TYPE_MIN_EXTRA){
		if (o->m&LLL_OBJECT_MODIFIER_FIXED){
			fprintf(f,"@fixed ");
		}
		if (o->m&LLL_OBJECT_MODIFIER_UNSIGNED){
			fprintf(f,"@unsigned ");
		}
		if (o->m&LLL_OBJECT_MODIFIER_SIZE){
			if ((o->m&LLL_OBJECT_MODIFIER_SIZE_MASK)==LLL_OBJECT_MODIFIER_8BIT){
				fprintf(f,"@8bit ");
			}
			else if ((o->m&LLL_OBJECT_MODIFIER_SIZE_MASK)==LLL_OBJECT_MODIFIER_16BIT){
				fprintf(f,"@16bit ");
			}
			else if ((o->m&LLL_OBJECT_MODIFIER_SIZE_MASK)==LLL_OBJECT_MODIFIER_32BIT){
				fprintf(f,"@32bit ");
			}
			else{
				fprintf(f,"@64bit ");
			}
		}
		if ((o->m&LLL_OBJECT_MODIFIER_OUTPUT_TYPE_MASK)==LLL_OBJECT_MODIFIER_ARRAY){
			fprintf(f,"@array ");
		}
		else if ((o->m&LLL_OBJECT_MODIFIER_OUTPUT_TYPE_MASK)==LLL_OBJECT_MODIFIER_LIST){
			fprintf(f,"@list ");
		}
		else if ((o->m&LLL_OBJECT_MODIFIER_OUTPUT_TYPE_MASK)==LLL_OBJECT_MODIFIER_VECTOR){
			fprintf(f,"@vector ");
		}
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
			fprintf(f,"(unknown)");
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			fprintf(f,"'%c'",LLL_GET_OBJECT_AS_CHAR(o));
			return sizeof(lll_object_t)+eoff+sizeof(char);
		case LLL_OBJECT_TYPE_STRING:
			fputc('"',f);
			lll_string_length_t l=LLL_GET_OBJECT_STRING_LENGTH(o);
			uint32_t off=sizeof(lll_object_t)+l+sizeof(lll_string_length_t);
			char* str=LLL_GET_OBJECT_AS_STRING(o);
			while (l){
				l--;
				char c=*str;
				if (c=='\''||c=='"'||c=='\\'){
					fputc('\\',f);
				}
				else if (c=='\t'){
					fputc('\\',f);
					c='t';
				}
				else if (c=='\n'){
					fputc('\\',f);
					c='n';
				}
				else if (c=='\v'){
					fputc('\\',f);
					c='v';
				}
				else if (c=='\f'){
					fputc('\\',f);
					c='f';
				}
				else if (c=='\r'){
					fputc('\\',f);
					c='r';
				}
				else if (c<32||c>126){
					fputc('\\',f);
					fputc('x',f);
					fputc((((uint8_t)c)>>4)+(((uint8_t)c)>159?87:48),f);
					c=(c&0xf)+((c&0xf)>9?87:48);
				}
				fputc(c,f);
				str++;
			}
			fputc('"',f);
			return off+eoff;
		case LLL_OBJECT_TYPE_INT:
			if (LLL_IS_OBJECT_INT16(o)){
				fprintf(f,"%"PRId16,LLL_GET_OBJECT_AS_INT16(o));
				return sizeof(lll_object_t)+eoff+sizeof(int16_t);
			}
			if (LLL_IS_OBJECT_INT32(o)){
				fprintf(f,"%"PRId32,LLL_GET_OBJECT_AS_INT32(o));
				return sizeof(lll_object_t)+eoff+sizeof(int32_t);
			}
			if (LLL_IS_OBJECT_INT64(o)){
				fprintf(f,"%"PRId64,LLL_GET_OBJECT_AS_INT64(o));
				return sizeof(lll_object_t)+eoff+sizeof(int64_t);
			}
			fprintf(f,"%"PRId8,LLL_GET_OBJECT_AS_INT8(o));
			return sizeof(lll_object_t)+eoff+sizeof(int8_t);
		case LLL_OBJECT_TYPE_FLOAT:
			if (LLL_IS_OBJECT_FLOAT64(o)){
				fprintf(f,"%lf",LLL_GET_OBJECT_AS_FLOAT64(o));
				return sizeof(lll_object_t)+eoff+sizeof(double);
			}
			fprintf(f,"%f",LLL_GET_OBJECT_AS_FLOAT32(o));
			return sizeof(lll_object_t)+eoff+sizeof(float);
		case LLL_OBJECT_TYPE_IDENTIFIER:
			l=LLL_GET_OBJECT_STRING_LENGTH(o);
			off=sizeof(lll_object_t)+l+sizeof(lll_string_length_t);
			str=LLL_GET_OBJECT_AS_STRING(o);
			while (l){
				l--;
				fputc(*str,f);
				str++;
			}
			return off+eoff;
		case LLL_OBJECT_TYPE_NIL:
			fprintf(f,"nil");
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_TRUE:
			fprintf(f,"true");
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_FALSE:
			fprintf(f,"false");
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CAST_CHAR:
			fprintf(f,"char");
			break;
		case LLL_OBJECT_TYPE_CAST_STRING:
			fprintf(f,"str");
			break;
		case LLL_OBJECT_TYPE_CAST_INT:
			fprintf(f,"int");
			break;
		case LLL_OBJECT_TYPE_CAST_INT64:
			fprintf(f,"int64");
			break;
		case LLL_OBJECT_TYPE_CAST_FLOAT:
			fprintf(f,"float");
			break;
		case LLL_OBJECT_TYPE_CAST_FLOAT64:
			fprintf(f,"float64");
			break;
		case LLL_OBJECT_TYPE_CAST_BOOL:
			fprintf(f,"bool");
			break;
		case LLL_OBJECT_TYPE_FUNC_PRINT:
			fprintf(f,"print");
			break;
		case LLL_OBJECT_TYPE_FUNC_PTR:
			fprintf(f,"ptr");
			break;
		case LLL_OBJECT_TYPE_FUNC_TYPEOF:
			fprintf(f,"typeof");
			break;
		case LLL_OBJECT_TYPE_AND:
			fprintf(f,"&&");
			break;
		case LLL_OBJECT_TYPE_OR:
			fprintf(f,"||");
			break;
		case LLL_OBJECT_TYPE_NOT:
			fprintf(f,"!");
			break;
		case LLL_OBJECT_TYPE_SET:
			fprintf(f,"=");
			break;
		case LLL_OBJECT_TYPE_FUNC:
			fprintf(f,"func");
			break;
		case LLL_OBJECT_TYPE_IF:
			fprintf(f,"if");
			break;
		case LLL_OBJECT_TYPE_FOR:
			fprintf(f,"for");
			break;
		case LLL_OBJECT_TYPE_ADD:
			fprintf(f,"+");
			break;
		case LLL_OBJECT_TYPE_SUB:
			fprintf(f,"-");
			break;
		case LLL_OBJECT_TYPE_MULT:
			fprintf(f,"*");
			break;
		case LLL_OBJECT_TYPE_DIV:
			fprintf(f,"/");
			break;
		case LLL_OBJECT_TYPE_FLOOR_DIV:
			fprintf(f,"//");
			break;
		case LLL_OBJECT_TYPE_MOD:
			fprintf(f,"%%");
			break;
		case LLL_OBJECT_TYPE_BIT_AND:
			fprintf(f,"&");
			break;
		case LLL_OBJECT_TYPE_BIT_OR:
			fprintf(f,"|");
			break;
		case LLL_OBJECT_TYPE_BIT_XOR:
			fprintf(f,"^");
			break;
		case LLL_OBJECT_TYPE_BIT_NOT:
			fprintf(f,"~");
			break;
		case LLL_OBJECT_TYPE_DIV_MOD:
			fprintf(f,"/%%");
			break;
		case LLL_OBJECT_TYPE_POW:
			fprintf(f,"**");
			break;
		case LLL_OBJECT_TYPE_ROOT:
			fprintf(f,"*/");
			break;
		case LLL_OBJECT_TYPE_FLOOR_ROOT:
			fprintf(f,"*//");
			break;
		case LLL_OBJECT_TYPE_LOG:
			fprintf(f,"_/");
			break;
		case LLL_OBJECT_TYPE_FLOOR_LOG:
			fprintf(f,"_//");
			break;
		case LLL_OBJECT_TYPE_LESS:
			fprintf(f,"<");
			break;
		case LLL_OBJECT_TYPE_LESS_EQUAL:
			fprintf(f,"<=");
			break;
		case LLL_OBJECT_TYPE_EQUAL:
			fprintf(f,"==");
			break;
		case LLL_OBJECT_TYPE_NOT_EQUAL:
			fprintf(f,"!=");
			break;
		case LLL_OBJECT_TYPE_MORE:
			fprintf(f,">");
			break;
		case LLL_OBJECT_TYPE_MORE_EQUAL:
			fprintf(f,">=");
			break;
		case LLL_OBJECT_TYPE_OPERATION_LIST:
			fputc('{',f);
			off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
			lll_statement_count_t sc=*LLL_GET_OBJECT_STATEMENT_COUNT(o);
			for (lll_statement_count_t i=0;i<sc;i++){
				if (i){
					fputc(' ',f);
				}
				off+=_print_object_internal(LLL_GET_OBJECT_STATEMENT(o,off),f);
			}
			fputc('}',f);
			return off+eoff;
		case LLL_OBJECT_TYPE_DEBUG_DATA:;
			lll_debug_object_t* dbg=(lll_debug_object_t*)o;
			uint32_t i=sizeof(lll_debug_object_t);
			if (dbg->f&LLL_DEBUG_OBJECT_LINE_NUMBER_INT32){
				fprintf(f,"<%"PRIu32":",LLL_GET_DEBUG_OBJECT_DATA_UINT32(dbg,i)+1);
				i+=sizeof(uint32_t);
			}
			else if (dbg->f&LLL_DEBUG_OBJECT_LINE_NUMBER_INT16){
				fprintf(f,"<%"PRIu16":",LLL_GET_DEBUG_OBJECT_DATA_UINT16(dbg,i)+1);
				i+=sizeof(uint16_t);
			}
			else{
				fprintf(f,"<%"PRIu8":",LLL_GET_DEBUG_OBJECT_DATA_UINT8(dbg,i)+1);
				i+=sizeof(uint8_t);
			}
			if (dbg->f&LLL_DEBUG_OBJECT_COLUMN_NUMBER_INT32){
				fprintf(f,"%"PRIu32">",LLL_GET_DEBUG_OBJECT_DATA_UINT32(dbg,i)+1);
				i+=sizeof(uint32_t);
			}
			else if (dbg->f&LLL_DEBUG_OBJECT_COLUMN_NUMBER_INT16){
				fprintf(f,"%"PRIu16">",LLL_GET_DEBUG_OBJECT_DATA_UINT16(dbg,i)+1);
				i+=sizeof(uint16_t);
			}
			else{
				fprintf(f,"%"PRIu8">",LLL_GET_DEBUG_OBJECT_DATA_UINT8(dbg,i)+1);
				i+=sizeof(uint8_t);
			}
			i+=LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
			return i+eoff+_print_object_internal(LLL_GET_DEBUG_OBJECT_CHILD(dbg,i),f);
		default:
			UNREACHABLE();
	}
	uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
	lll_arg_count_t l=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
	while (l){
		l--;
		fputc(' ',f);
		off+=_print_object_internal(LLL_GET_OBJECT_ARGUMENT(o,off),f);
	}
	fputc(')',f);
	return off+eoff;
}



uint8_t _read_single_char(lll_input_data_stream_t* is,char t,uint32_t st,lll_error_t* e){
	int c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
	if (c==LLL_END_OF_DATA){
		e->t=LLL_ERROR_UNMATCHED_QUOTES;
		e->off=st;
		e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
		return READ_SINGLE_CHAR_ERROR;
	}
	if (c==t){
		return READ_SINGLE_CHAR_END;
	}
	if (c=='\r'||c=='\n'){
		e->t=LLL_ERROR_UNMATCHED_QUOTES;
		e->off=st;
		e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
		return READ_SINGLE_CHAR_ERROR;
	}
	if (c=='\\'){
		c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
		if (c=='\r'||c=='\n'){
			e->t=LLL_ERROR_UNMATCHED_QUOTES;
			e->off=st;
			e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
			return READ_SINGLE_CHAR_ERROR;
		}
		if (c==LLL_END_OF_DATA){
			e->t=LLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE;
			e->off=st;
			e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
			return READ_SINGLE_CHAR_ERROR;
		}
		if (c=='\''||c=='"'||c=='\\'){
			goto _skip_parse;
		}
		else if (c=='x'){
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c==LLL_END_OF_DATA){
				e->t=LLL_ERROR_UNMATCHED_QUOTES;
				e->off=st;
				e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
				return READ_SINGLE_CHAR_ERROR;
			}
			if (c>96){
				c-=32;
			}
			if (c<48||(c>57&&c<65)||c>70){
				if (c=='\r'||c=='\n'){
					e->t=LLL_ERROR_UNMATCHED_QUOTES;
					e->off=st;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
					return READ_SINGLE_CHAR_ERROR;
				}
				e->t=LLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER;
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
				e->c=c;
				return READ_SINGLE_CHAR_ERROR;
			}
			uint8_t v=(c>64?c-55:c-48)<<4;
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c==LLL_END_OF_DATA){
				e->t=LLL_ERROR_UNMATCHED_QUOTES;
				e->off=st;
				e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
				return READ_SINGLE_CHAR_ERROR;
			}
			if (c>96){
				c-=32;
			}
			if (c<48||(c>57&&c<65)||c>70){
				if (c=='\r'||c=='\n'){
					e->t=LLL_ERROR_UNMATCHED_QUOTES;
					e->off=st;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st-2;
					return READ_SINGLE_CHAR_ERROR;
				}
				e->t=LLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER;
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
				e->c=c;
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
			e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
			e->sz=1;
			e->c=c;
			return READ_SINGLE_CHAR_ERROR;
		}
	}
_skip_parse:
	*((char*)(_bf+_bf_off))=c;
	_bf_off++;
	if (_bf_off>=_bf_sz){
		e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
		e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
		e->sz=1;
		return READ_SINGLE_CHAR_ERROR;
	}
	return READ_SINGLE_CHAR_OK;
}



uint8_t _read_object_internal(lll_compilation_data_t* c_dt,int c,lll_error_t* e){
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
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-3;
				e->sz=1;
				e->c='|';
				return LLL_RETURN_ERROR;
			}
			int lc=c;
			while (c!='|'||lc!='#'){
				lc=c;
				c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
				if (c==LLL_END_OF_DATA){
					e->t=LLL_ERROR_UNMATCHED_OPEN_QUOTE;
					e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-3;
					e->sz=1;
					return LLL_RETURN_ERROR;
				}
			}
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			continue;
		}
		if (c==')'){
			if (!o||ec!=')'){
				e->t=LLL_ERROR_UNMATCHED_CLOSE_PARENTHESES;
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
				return LLL_RETURN_ERROR;
			}
			if (am){
				e->t=LLL_ERROR_UNUSED_MODIFIERS;
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
				return LLL_RETURN_ERROR;
			}
			if (o->t==LLL_OBJECT_TYPE_UNKNOWN){
				o->t=LLL_OBJECT_TYPE_NIL;
				_bf_off-=sizeof(lll_arg_count_t);
			}
			else if (LLL_IS_OBJECT_TYPE_MATH_NO_CHAIN(o)){
				if (*ac==1){
					e->t=LLL_ERROR_MATH_OP_NOT_ENOUGH_ARGUMENTS;
					e->off=st_off;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st_off-1;
					return LLL_RETURN_ERROR;
				}
				if (*ac>(LLL_GET_OBJECT_TYPE(o)==LLL_OBJECT_TYPE_POW?3:2)){
					e->t=LLL_ERROR_MATH_OP_TOO_MANY_ARGUMENTS;
					e->off=st_off;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st_off-1;
					return LLL_RETURN_ERROR;
				}
			}
			return LLL_RETURN_NO_ERROR;
		}
		if (c=='}'){
			if (!o||ec!='}'){
				e->t=LLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS;
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
				return LLL_RETURN_ERROR;
			}
			if (am){
				e->t=LLL_ERROR_UNUSED_MODIFIERS;
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
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
					e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
					e->sz=1;
					return LLL_RETURN_ERROR;
				}
				o->t=(c=='('?LLL_OBJECT_TYPE_UNKNOWN:LLL_OBJECT_TYPE_OPERATION_LIST);
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
				if (o->t==LLL_OBJECT_TYPE_UNKNOWN){
					e->t=LLL_ERROR_NO_SYMBOL;
					e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
					e->sz=1;
					return LLL_RETURN_ERROR;
				}
				if (!_read_object_internal(c_dt,c,e)){
					return LLL_RETURN_ERROR;
				}
				if (o->t==LLL_OBJECT_TYPE_OPERATION_LIST){
					if (*sc==UINT16_MAX){
						e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
						e->off=arg_s;
						e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
						return LLL_RETURN_ERROR;
					}
					(*sc)++;
				}
				else{
					if (*ac==UINT8_MAX){
						e->t=LLL_ERROR_TOO_MANY_ARGUMENTS;
						e->off=arg_s;
						e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-1;
						return LLL_RETURN_ERROR;
					}
					(*ac)++;
				}
			}
		}
		else if (c=='@'){
			if (!o){
				e->t=LLL_ERROR_UNEXPECTED_CHARACTER;
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
				e->c='@';
				return LLL_RETURN_ERROR;
			}
			char* str=(char*)(_bf+_bf_off);
			uint16_t sz=0;
_read_modifier:
			if (sz==UINT16_MAX){
				e->t=LLL_ERROR_MODIFIER_TOO_LONG;
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz;
				e->sz=sz;
				return LLL_RETURN_ERROR;
			}
			*(str+sz)=c;
			sz++;
			if (sz+_bf_off>=_bf_sz){
				e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
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
					if (o->t==LLL_OBJECT_TYPE_UNKNOWN){
						if (o->m&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m=(o->m&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_8BIT;
					}
					else{
						if (am&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						am=(am&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_8BIT;
					}
				}
				else if (FAST_COMPARE(str,l,i,s,t)){
					if (o->t==LLL_OBJECT_TYPE_UNKNOWN){
						if (o->m&LLL_OBJECT_MODIFIER_OUTPUT_TYPE_MASK){
							e->t=LLL_ERROR_MULTIPLE_OUTPUT_TYPE_MODIFIERS;
							e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m|=LLL_OBJECT_MODIFIER_LIST;
					}
					else{
						e->t=LLL_ERROR_UNKNOWN_MODIFIER;
						e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
						e->sz=sz+1;
						return LLL_RETURN_ERROR;
					}
				}
				else{
					goto _unknown_modifier;
				}
			}
			else if (sz==5){
				if (FAST_COMPARE(str,f,i,x,e,d)){
					if (o->t==LLL_OBJECT_TYPE_UNKNOWN){
						o->m|=LLL_OBJECT_MODIFIER_FIXED;
					}
					else{
						am|=LLL_OBJECT_MODIFIER_FIXED;
					}
				}
				else if (FAST_COMPARE(str,1,6,b,i,t)){
					if (o->t==LLL_OBJECT_TYPE_UNKNOWN){
						if (o->m&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m=(o->m&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_16BIT;
					}
					else{
						if (am&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						am=(am&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_16BIT;
					}
				}
				else if (FAST_COMPARE(str,3,2,b,i,t)){
					if (o->t==LLL_OBJECT_TYPE_UNKNOWN){
						if (o->m&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m=(o->m&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_32BIT;
					}
					else{
						if (am&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						am=(am&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_32BIT;
					}
				}
				else if (FAST_COMPARE(str,6,4,b,i,t)){
					if (o->t==LLL_OBJECT_TYPE_UNKNOWN){
						if (o->m&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m=(o->m&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_64BIT;
					}
					else{
						if (am&LLL_OBJECT_MODIFIER_SIZE){
							e->t=LLL_ERROR_MULTIPLE_SIZE_MODIFIERS;
							e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						am=(am&(~LLL_OBJECT_MODIFIER_SIZE_MASK))|LLL_OBJECT_MODIFIER_SIZE|LLL_OBJECT_MODIFIER_64BIT;
					}
				}
				else if (FAST_COMPARE(str,a,r,r,a,y)){
					if (o->t==LLL_OBJECT_TYPE_UNKNOWN){
						if (o->m&LLL_OBJECT_MODIFIER_OUTPUT_TYPE_MASK){
							e->t=LLL_ERROR_MULTIPLE_OUTPUT_TYPE_MODIFIERS;
							e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m|=LLL_OBJECT_MODIFIER_ARRAY;
					}
					else{
						e->t=LLL_ERROR_UNKNOWN_MODIFIER;
						e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
						e->sz=sz+1;
						return LLL_RETURN_ERROR;
					}
				}
				else{
					goto _unknown_modifier;
				}
			}
			else if (sz==6){
				if (FAST_COMPARE(str,v,e,c,t,o,r)){
					if (o->t==LLL_OBJECT_TYPE_UNKNOWN){
						if (o->m&LLL_OBJECT_MODIFIER_OUTPUT_TYPE_MASK){
							e->t=LLL_ERROR_MULTIPLE_OUTPUT_TYPE_MODIFIERS;
							e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
							e->sz=sz+1;
							return LLL_RETURN_ERROR;
						}
						o->m|=LLL_OBJECT_MODIFIER_VECTOR;
					}
					else{
						e->t=LLL_ERROR_UNKNOWN_MODIFIER;
						e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
						e->sz=sz+1;
						return LLL_RETURN_ERROR;
					}
				}
				else{
					goto _unknown_modifier;
				}
			}
			else if (sz==8){
				if (FAST_COMPARE(str,u,n,s,i,g,n,e,d)){
					if (o->t==LLL_OBJECT_TYPE_UNKNOWN){
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
				e->t=(o->t==LLL_OBJECT_TYPE_UNKNOWN?LLL_ERROR_UNKNOWN_OUTPUT_MODIFIER:LLL_ERROR_UNKNOWN_MODIFIER);
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
				e->sz=sz+1;
				return LLL_RETURN_ERROR;
			}
			continue;
		}
		else if (o&&o->t==LLL_OBJECT_TYPE_UNKNOWN){
			char* str=(char*)(_bf+_bf_off);
			uint16_t sz=0;
_read_symbol:
			if (sz==UINT16_MAX){
				e->t=LLL_ERROR_SYMBOL_TOO_LONG;
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-3;
				e->sz=sz;
				return LLL_RETURN_ERROR;
			}
			*(str+sz)=c;
			sz++;
			if (sz+_bf_off>=_bf_sz){
				e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
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
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-sz-2;
				e->sz=sz;
				return LLL_RETURN_ERROR;
			}
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
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
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
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
				e->c=c;
				return LLL_RETURN_ERROR;
			}
			if (!o){
				return LLL_RETURN_NO_ERROR;
			}
			if (o->t==LLL_OBJECT_TYPE_OPERATION_LIST){
				if (*sc==UINT16_MAX){
					e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
					e->off=arg_s;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*sc)++;
			}
			else{
				if (*ac==UINT8_MAX){
					e->t=LLL_ERROR_TOO_MANY_ARGUMENTS;
					e->off=arg_s;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
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
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
				return LLL_RETURN_ERROR;
			}
			arg->t=LLL_OBJECT_TYPE_STRING;
			arg->m=am;
			am=0;
			uint32_t sz=0;
			while (1){
				uint8_t ec=_read_single_char(is,'"',arg_s,e);
				if (ec==READ_SINGLE_CHAR_ERROR){
					return LLL_RETURN_ERROR;
				}
				if (ec==READ_SINGLE_CHAR_END){
					break;
				}
				sz++;
			}
			LLL_SET_OBJECT_STRING_LENGTH(arg,sz);
			if (!o){
				return LLL_RETURN_NO_ERROR;
			}
			if (o->t==LLL_OBJECT_TYPE_OPERATION_LIST){
				if (*sc==UINT16_MAX){
					e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
					e->off=arg_s;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*sc)++;
			}
			else{
				if (*ac==UINT8_MAX){
					e->t=LLL_ERROR_TOO_MANY_ARGUMENTS;
					e->off=arg_s;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
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
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
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
						e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
						e->sz=1;
						e->c=c;
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
						e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
						e->sz=1;
						e->c=c;
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
						e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
						e->sz=1;
						e->c=c;
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
					e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
					e->sz=1;
					e->c=c;
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
						e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
						e->sz=1;
						e->c=c;
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
						e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
						e->sz=1;
						e->c=c;
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
						e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
						e->sz=1;
						e->c=c;
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
					e->off=arg_s;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				else if ((am&LLL_OBJECT_MODIFIER_SIZE_MASK)==LLL_OBJECT_MODIFIER_16BIT){
					e->t=LLL_ERROR_UNSUPPORTED_16BIT_FLOAT_SIZE;
					e->off=arg_s;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
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
			if (o->t==LLL_OBJECT_TYPE_OPERATION_LIST){
				if (*sc==UINT16_MAX){
					e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
					e->off=arg_s;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*sc)++;
			}
			else{
				if (*ac==UINT8_MAX){
					e->t=LLL_ERROR_TOO_MANY_ARGUMENTS;
					e->off=arg_s;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*ac)++;
			}
			continue;
		}
		else if (c=='$'||(c>64&&c<91)||c=='_'||(c>96&&c<123)){
			if (!lll_insert_debug_object(is,e)){
				return LLL_RETURN_ERROR;
			}
			uint32_t arg_s=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
			lll_object_t* arg=(lll_object_t*)(_bf+_bf_off);
			_bf_off+=sizeof(lll_object_t)+sizeof(lll_string_length_t);
			if (_bf_off>=_bf_sz){
				e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
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
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
				return LLL_RETURN_ERROR;
			}
			c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
			if (c==LLL_END_OF_DATA){
				break;
			}
			if (c=='$'||(c>47&&c<58)||(c>64&&c<91)||c=='_'||(c>96&&c<123)){
				goto _read_identifier;
			}
			if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='('&&c!='}'&&c!='{')){
				e->t=LLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER;
				e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
				e->sz=1;
				e->c=c;
				return LLL_RETURN_ERROR;
			}
			if (sz==3&&FAST_COMPARE(str,n,i,l)){
				_bf_off-=sizeof(lll_string_length_t);
				arg->t=LLL_OBJECT_TYPE_NIL;
			}
			else if (sz==4&&FAST_COMPARE(str,t,r,u,e)){
				_bf_off-=sizeof(lll_string_length_t);
				arg->t=LLL_OBJECT_TYPE_TRUE;
			}
			else if (sz==5&&FAST_COMPARE(str,f,a,l,s,e)){
				_bf_off-=sizeof(lll_string_length_t);
				arg->t=LLL_OBJECT_TYPE_FALSE;
			}
			else{
				_bf_off+=sz;
				LLL_SET_OBJECT_STRING_LENGTH(arg,sz);
			}
			if (!o){
				return LLL_RETURN_NO_ERROR;
			}
			if (o->t==LLL_OBJECT_TYPE_OPERATION_LIST){
				if (*sc==UINT16_MAX){
					e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
					e->off=arg_s;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*sc)++;
			}
			else{
				if (*ac==UINT8_MAX){
					e->t=LLL_ERROR_TOO_MANY_ARGUMENTS;
					e->off=arg_s;
					e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-arg_s-2;
					return LLL_RETURN_ERROR;
				}
				(*ac)++;
			}
			continue;
		}
		else{
			e->t=LLL_ERROR_UNEXPECTED_CHARACTER;
			e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
			e->sz=1;
			e->c=c;
			return LLL_RETURN_ERROR;
		}
		c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
	}
	if (!o){
		o=(lll_object_t*)(_bf+_bf_off);
		_bf_off+=sizeof(lll_object_t);
		if (_bf_off>=_bf_sz){
			e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
			e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
			e->sz=1;
			return LLL_RETURN_ERROR;
		}
		o->t=LLL_OBJECT_TYPE_NIL;
		return LLL_RETURN_NO_ERROR;
	}
	e->t=LLL_ERROR_UNMATCHED_OPEN_PARENTHESES;
	e->off=st_off;
	e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-st_off-1;
	return LLL_RETURN_ERROR;
}



uint32_t _get_object_size(lll_object_t* o){
	uint32_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	if (LLL_IS_OBJECT_TYPE_NOT_INTEGRAL(o)){
		while (LLL_IS_OBJECT_REF(o)){
			o=READ_REF_FROM_STACK(o);
		}
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_TRUE:
		case LLL_OBJECT_TYPE_FALSE:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			return sizeof(lll_object_t)+eoff+sizeof(char);
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_object_t)+eoff+LLL_GET_OBJECT_STRING_LENGTH(o)+sizeof(lll_string_length_t);
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_object_t)+eoff+LLL_GET_OBJECT_INTEGER_WIDTH(o);
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_object_t)+eoff+(LLL_IS_OBJECT_FLOAT64(o)?sizeof(double):sizeof(float));
		case LLL_OBJECT_TYPE_OPERATION_LIST:;
			uint32_t off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
			lll_statement_count_t l=*LLL_GET_OBJECT_STATEMENT_COUNT(o);
			while (l){
				l--;
				off+=_get_object_size(LLL_GET_OBJECT_STATEMENT(o,off));
			}
			return off+eoff;
		case LLL_OBJECT_TYPE_DEBUG_DATA:;
			lll_debug_object_t* dbg=(lll_debug_object_t*)o;
			uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
			return sz+eoff+_get_object_size(LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz));
	}
	uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
	lll_arg_count_t l=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
	while (l){
		l--;
		off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
	}
	return off+eoff;
}



uint32_t _optimize_object_internal(lll_object_t* o,lll_error_t* e){
	uint32_t eoff=0;
_skip_empty:
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	if (LLL_IS_OBJECT_TYPE_NOT_INTEGRAL(o)){
		if (LLL_IS_OBJECT_REF(o)){
			while (LLL_IS_OBJECT_REF(o)){
				o=READ_REF_FROM_STACK(o);
			}
			goto _skip_empty;
		}
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_TRUE:
		case LLL_OBJECT_TYPE_FALSE:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			return sizeof(lll_object_t)+eoff+sizeof(char);
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_object_t)+eoff+sizeof(lll_string_length_t)+LLL_GET_OBJECT_STRING_LENGTH(o);
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_object_t)+eoff+LLL_GET_OBJECT_INTEGER_WIDTH(o);
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_object_t)+eoff+(LLL_IS_OBJECT_FLOAT64(o)?sizeof(double):sizeof(float));
		case LLL_OBJECT_TYPE_OPERATION_LIST:;
			uint32_t off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
			lll_statement_count_t* l=LLL_GET_OBJECT_STATEMENT_COUNT(o);
			for (lll_statement_count_t i=*l;i>0;i--){
				uint32_t st_l=_optimize_object_internal(LLL_GET_OBJECT_STATEMENT(o,off),e);
				lll_object_t* st=LLL_GET_OBJECT_STATEMENT(o,off);
				off+=st_l;
				while (st->t==LLL_OBJECT_TYPE_NOP||LLL_GET_OBJECT_TYPE(st)==LLL_OBJECT_TYPE_DEBUG_DATA){
					if (st->t==LLL_OBJECT_TYPE_NOP){
						st=LLL_GET_OBJECT_AFTER_NOP(st);
						continue;
					}
					lll_debug_object_t* dbg=(lll_debug_object_t*)st;
					st=LLL_GET_DEBUG_OBJECT_CHILD(dbg,sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg));
				}
				if (LLL_IS_OBJECT_TYPE_TYPE(st)){
					(*l)--;
					for (uint32_t j=off-st_l;j<off;j++){
						*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
					}
				}
			}
			if (!(*l)){
				o->t=LLL_OBJECT_TYPE_NIL;
				for (uint32_t i=sizeof(lll_object_t);i<sizeof(lll_object_t)+sizeof(lll_statement_count_t);i++){
					*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,i))=LLL_OBJECT_TYPE_NOP;
				}
				return off+eoff;
			}
			if (*l==1){
				for (uint32_t i=0;i<sizeof(lll_object_t)+sizeof(lll_statement_count_t);i++){
					*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,i))=LLL_OBJECT_TYPE_NOP;
				}
				return off+eoff;
			}
			return off+eoff;
		case LLL_OBJECT_TYPE_DEBUG_DATA:;
			lll_debug_object_t* dbg=(lll_debug_object_t*)o;
			uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
			lll_object_t* c=LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz);
			uint8_t ot=LLL_GET_OBJECT_TYPE(c);
			off=_optimize_object_internal(c,e);
			if (off==UINT32_MAX){
				return UINT32_MAX;
			}
			off+=sz;
			if (LLL_GET_OBJECT_TYPE(c)!=ot){
				for (uint32_t i=0;i<sz;i++){
					*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,i))=LLL_OBJECT_TYPE_NOP;
				}
			}
			return off+eoff;
	}
	uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
	lll_arg_count_t* l=LLL_GET_OBJECT_ARGUMENT_COUNT(o);
	lll_arg_count_t cl=*l;
	lll_arg_count_t i=0;
	while (i<cl){
		uint32_t al=_optimize_object_internal(LLL_GET_OBJECT_ARGUMENT(o,off),e);
		if (al==UINT32_MAX){
			return UINT32_MAX;
		}
		lll_object_t* a=LLL_GET_OBJECT_ARGUMENT(o,off);
		off+=al;
		if (0&&LLL_IS_OBJECT_TYPE_MATH_CHAIN(o)){
			while (LLL_GET_OBJECT_TYPE(a)==LLL_OBJECT_TYPE_DEBUG_DATA){
				lll_debug_object_t* dbg=(lll_debug_object_t*)a;
				a=LLL_GET_DEBUG_OBJECT_CHILD(dbg,sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg));
			}
			switch (LLL_GET_OBJECT_TYPE(a)){
				case LLL_OBJECT_TYPE_CHAR:
					break;
				case LLL_OBJECT_TYPE_STRING:
					break;
				case LLL_OBJECT_TYPE_INT:
					break;
				case LLL_OBJECT_TYPE_FLOAT:
					break;
				case LLL_OBJECT_TYPE_NIL:
					(*l)--;
					for (uint32_t j=off-al;j<off;j++){
						*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
					}
					break;
				case LLL_OBJECT_TYPE_TRUE:
					switch (LLL_GET_OBJECT_TYPE(o)){
						case LLL_OBJECT_TYPE_DIV:
						case LLL_OBJECT_TYPE_FLOOR_DIV:
						case LLL_OBJECT_TYPE_MOD:
							if (!i){
								break;
							}
						case LLL_OBJECT_TYPE_MULT:
							(*l)--;
							for (uint32_t j=off-al;j<off;j++){
								*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
							}
							break;
						case LLL_OBJECT_TYPE_BIT_NOT:
							printf("SET OBJECT TO -2!\n");
							break;
					}
					break;
				case LLL_OBJECT_TYPE_FALSE:
					switch (LLL_GET_OBJECT_TYPE(o)){
						case LLL_OBJECT_TYPE_ADD:
						case LLL_OBJECT_TYPE_SUB:
						case LLL_OBJECT_TYPE_BIT_OR:
						case LLL_OBJECT_TYPE_BIT_XOR:
							(*l)--;
							for (uint32_t j=off-al;j<off;j++){
								*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
							}
							break;
						case LLL_OBJECT_TYPE_DIV:
						case LLL_OBJECT_TYPE_FLOOR_DIV:
						case LLL_OBJECT_TYPE_MOD:
							if (!i){
								goto _set_to_0;
							}
							e->t=LLL_ERROR_DIVISION_BY_ZERO;
							return UINT32_MAX;
						case LLL_OBJECT_TYPE_MULT:
						case LLL_OBJECT_TYPE_BIT_AND:
_set_to_0:
							i++;
							while (i<cl){
								i++;
								off+=_get_object_size(LLL_GET_OBJECT_ARGUMENT(o,off));
							}
							o->t=LLL_OBJECT_TYPE_INT;
							LLL_SET_OBJECT_AS_INT8(o,0);
							for (uint32_t j=sizeof(lll_object_t)+sizeof(int8_t);j<off;j++){
								*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
							}
							return off+eoff;
						case LLL_OBJECT_TYPE_BIT_NOT:
							printf("SET OBJECT TO -1!\n");
							break;
					}
					break;
			}
		}
		i++;
	}
	if (!(*l)&&LLL_IS_OBJECT_TYPE_MATH_CHAIN(o)){
		o->t=LLL_OBJECT_TYPE_INT;
		LLL_SET_OBJECT_AS_INT8(o,0);
		for (uint32_t j=sizeof(lll_object_t)+sizeof(int8_t);j<off;j++){
			*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
		}
		return off+eoff;
	}
	if (*l==1){
		if (LLL_IS_OBJECT_TYPE_COMPARE(o)){
			o->t=LLL_OBJECT_TYPE_TRUE;
			for (uint32_t j=sizeof(lll_object_t);j<off;j++){
				*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
			}
			return off+eoff;
		}
		if (LLL_IS_OBJECT_TYPE_MATH_CHAIN(o)){
			for (uint32_t j=0;j<sizeof(lll_object_t)+sizeof(lll_arg_count_t);j++){
				*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,j))=LLL_OBJECT_TYPE_NOP;
			}
			return off+eoff;
		}
	}
	return off+eoff;
}



uint32_t _remove_debug_data_internal(lll_object_t* o){
	uint32_t eoff=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		eoff+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	if (LLL_IS_OBJECT_TYPE_NOT_INTEGRAL(o)){
		while (LLL_IS_OBJECT_REF(o)){
			o=READ_REF_FROM_STACK(o);
		}
	}
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_TRUE:
		case LLL_OBJECT_TYPE_FALSE:
			return sizeof(lll_object_t)+eoff;
		case LLL_OBJECT_TYPE_CHAR:
			return sizeof(lll_object_t)+eoff+sizeof(char);
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_IDENTIFIER:
			return sizeof(lll_object_t)+eoff+sizeof(lll_string_length_t)+LLL_GET_OBJECT_STRING_LENGTH(o);
		case LLL_OBJECT_TYPE_INT:
			return sizeof(lll_object_t)+eoff+LLL_GET_OBJECT_INTEGER_WIDTH(o);
		case LLL_OBJECT_TYPE_FLOAT:
			return sizeof(lll_object_t)+eoff+(LLL_IS_OBJECT_FLOAT64(o)?sizeof(double):sizeof(float));
		case LLL_OBJECT_TYPE_OPERATION_LIST:;
			uint32_t off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
			lll_statement_count_t l=*LLL_GET_OBJECT_STATEMENT_COUNT(o);
			while (l){
				l--;
				off+=_remove_debug_data_internal(LLL_GET_OBJECT_STATEMENT(o,off));
			}
			return off+eoff;
		case LLL_OBJECT_TYPE_DEBUG_DATA:;
			lll_debug_object_t* dbg=(lll_debug_object_t*)o;
			uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
			for (uint32_t i=0;i<sz;i++){
				*((lll_object_type_t*)LLL_GET_OBJECT_WITH_OFFSET(o,i))=LLL_OBJECT_TYPE_NOP;
			}
			return sz+eoff+_remove_debug_data_internal(LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz));
	}
	uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
	lll_arg_count_t l=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
	while (l){
		l--;
		off+=_remove_debug_data_internal(LLL_GET_OBJECT_ARGUMENT(o,off));
	}
	return off+eoff;
}



uint32_t _remove_padding_internal(lll_object_t* o,uint32_t* rm){
	uint8_t* d=(uint8_t*)o-(*rm);
	uint32_t pad=0;
	while (o->t==LLL_OBJECT_TYPE_NOP){
		pad+=sizeof(lll_object_type_t);
		o=LLL_GET_OBJECT_AFTER_NOP(o);
	}
	uint8_t* s=(uint8_t*)o;
	if (LLL_IS_OBJECT_TYPE_NOT_INTEGRAL(o)){
		while (LLL_IS_OBJECT_REF(o)){
			o=READ_REF_FROM_STACK(o);
		}
	}
	(*rm)+=pad;
	switch (LLL_GET_OBJECT_TYPE(o)){
		case LLL_OBJECT_TYPE_UNKNOWN:
		case LLL_OBJECT_TYPE_NIL:
		case LLL_OBJECT_TYPE_TRUE:
		case LLL_OBJECT_TYPE_FALSE:
			REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t));
			return sizeof(lll_object_t)+pad;
		case LLL_OBJECT_TYPE_CHAR:
			REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t)+sizeof(char));
			return sizeof(lll_object_t)+sizeof(char)+pad;
		case LLL_OBJECT_TYPE_STRING:
		case LLL_OBJECT_TYPE_IDENTIFIER:;
			lll_string_length_t sl=LLL_GET_OBJECT_STRING_LENGTH(o);
			REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t)+sizeof(lll_string_length_t)+sl);
			return sizeof(lll_object_t)+sizeof(lll_string_length_t)+sl+pad;
		case LLL_OBJECT_TYPE_INT:;
			uint32_t w=LLL_GET_OBJECT_INTEGER_WIDTH(o);
			REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t)+w);
			return sizeof(lll_object_t)+w+pad;
		case LLL_OBJECT_TYPE_FLOAT:
			w=(LLL_IS_OBJECT_FLOAT64(o)?sizeof(double):sizeof(float));
			REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t)+w);
			return sizeof(lll_object_t)+w+pad;
		case LLL_OBJECT_TYPE_OPERATION_LIST:;
			uint32_t off=sizeof(lll_object_t)+sizeof(lll_statement_count_t);
			REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t)+sizeof(lll_statement_count_t));
			lll_statement_count_t l=*LLL_GET_OBJECT_STATEMENT_COUNT(o);
			while (l){
				l--;
				off+=_remove_padding_internal(LLL_GET_OBJECT_STATEMENT(o,off),rm);
			}
			return off+pad;
		case LLL_OBJECT_TYPE_DEBUG_DATA:;
			lll_debug_object_t* dbg=(lll_debug_object_t*)o;
			uint32_t sz=sizeof(lll_debug_object_t)+LLL_GET_DEBUG_OBJECT_LINE_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_COLUMN_NUMBER_WIDTH(dbg)+LLL_GET_DEBUG_OBJECT_FILE_OFFSET_WIDTH(dbg);
			REPEAT_BYTE_COPY(d,s,sz);
			return sz+_remove_padding_internal(LLL_GET_DEBUG_OBJECT_CHILD(dbg,sz),rm)+pad;
	}
	uint32_t off=sizeof(lll_object_t)+sizeof(lll_arg_count_t);
	REPEAT_BYTE_COPY(d,s,sizeof(lll_object_t)+sizeof(lll_arg_count_t));
	lll_arg_count_t l=*LLL_GET_OBJECT_ARGUMENT_COUNT(o);
	while (l){
		l--;
		off+=_remove_padding_internal(LLL_GET_OBJECT_ARGUMENT(o,off),rm);
	}
	return off+pad;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_set_internal_stack(uint8_t* bf,uint32_t sz,lll_error_t* e){
	if (sz>=LLL_MAX_INTERNAL_STACK_SIZE){
		e->t=LLL_ERROR_STACK_TOO_BIG;
		return LLL_RETURN_ERROR;
	}
	_bf=bf;
	_bf_off=0;
	_bf_sz=sz;
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT void lll_init_compilation_data(const char* fp,lll_input_data_stream_t* is,lll_compilation_data_t* o){
	o->fpl=0;
	while (*fp){
		o->fp[o->fpl]=*fp;
		o->fpl++;
		fp++;
	}
	o->fp[o->fpl]=0;
	o->is=is;
	o->h=NULL;
}



__LLL_IMPORT_EXPORT void lll_create_input_data_stream(FILE* f,lll_input_data_stream_t* o){
	o->ctx=f;
	o->rf=_input_data_stream_file_read;
	o->rlf=_input_data_stream_file_restart_line;
	o->_lc=0;
	o->_off=0;
	o->_loff=0;
}



__LLL_IMPORT_EXPORT void lll_create_output_data_stream(FILE* f,lll_output_data_stream_t* o){
	o->ctx=f;
	o->wf=_output_data_stream_file_write;
}



__LLL_IMPORT_EXPORT void lll_print_error(lll_input_data_stream_t* is,lll_error_t* e){
	if (e->t==LLL_ERROR_UNKNOWN||e->t>LLL_MAX_SYNTAX_ERROR){
		switch (e->t){
			default:
			case LLL_ERROR_UNKNOWN:
				printf("Unknown Error: %.2"PRIx8"\n",e->t);
				return;
			case LLL_ERROR_NO_STACK:
				printf("No Internal Stack\n");
				return;
			case LLL_ERROR_STACK_TOO_BIG:
				printf("Stack Size Too Big\n");
				return;
			case LLL_ERROR_FAILED_FILE_WRITE:
				printf("Failed to Write to File\n");
				return;
			case LLL_ERROR_DIVISION_BY_ZERO:
				printf("Division By Zero\n");
				return;
		}
	}
	ENABLE_COLOR();
	uint32_t os=e->off;
	uint32_t oe=os+e->sz;
	LLL_INPUT_DATA_STREAM_RESTART_LINE(is,os);
	uint32_t off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is);
	uint32_t s_off=off;
	uint32_t os_tb=0;
	uint32_t oe_tb=0;
	int c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
	char t=0;
	char* sym=NULL;
	char* sp=NULL;
	if (e->t==LLL_ERROR_UNKNOWN_SYMBOL||e->t==LLL_ERROR_UNKNOWN_MODIFIER||e->t==LLL_ERROR_UNKNOWN_OUTPUT_MODIFIER){
		sym=malloc((oe-os+1)*sizeof(char));
		sp=sym;
	}
	while (c!='\n'&&c!='\r'&&c!=LLL_END_OF_DATA){
		if (off==os){
			t=c;
			printf(HIGHLIGHT_COLOR);
		}
		if (off>=os&&off<oe&&sp){
			*sp=c;
			sp++;
		}
		if (c=='\t'){
			if (off<os){
				os_tb+=ERROR_DISPLAY_TAB_WIDTH-1;
			}
			if (off<oe){
				oe_tb+=ERROR_DISPLAY_TAB_WIDTH-1;
			}
			for (uint8_t i=0;i<ERROR_DISPLAY_TAB_WIDTH;i++){
				putchar(' ');
			}
		}
		else{
			putchar(c);
		}
		c=LLL_READ_FROM_INPUT_DATA_STREAM(is);
		off++;
		if (off==oe){
			printf(HIGHLIGHT_COLOR_RESET);
		}
	}
	putchar('\n');
	uint32_t i=s_off;
	for (;i<os+os_tb;i++){
		putchar(' ');
	}
	printf(HIGHLIGHT_COLOR);
	for (;i<oe+oe_tb;i++){
		putchar('~');
	}
	printf(HIGHLIGHT_COLOR_RESET"\n");
	DISABLE_COLOR();
	switch (e->t){
		default:
			printf("Unknown Error: %.2"PRIx8"\n",e->t);
			return;
		case LLL_ERROR_INTERNAL_STACK_OVERFLOW:
			printf("Internal Stack Overflow\n");
			return;
		case LLL_ERROR_UNMATCHED_OPEN_PARENTHESES:
			printf("Unmatched Left Parentheses\n");
			return;
		case LLL_ERROR_UNMATCHED_CLOSE_PARENTHESES:
			printf("Unmatched Right Parentheses\n");
			return;
		case LLL_ERROR_UNMATCHED_OPEN_QUOTE:
			printf("Unmatched Block Quote\n");
			return;
		case LLL_ERROR_UNMATCHED_CURLY_OPEN_BRACKETS:
			printf("Unmatched Left Curly Brackets\n");
			return;
		case LLL_ERROR_UNMATCHED_CURLY_CLOSE_BRACKETS:
			printf("Unmatched Right Curly Brackets\n");
			return;
		case LLL_ERROR_UNMATCHED_QUOTES:
			if (t=='\''){
				printf("Unmatched Single Quotes\n");
			}
			else{
				printf("Unmatched Double Quotes\n");
			}
			return;
		case LLL_ERROR_EMPTY_CHAR_STRING:
			printf("Empty Character String\n");
			return;
		case LLL_ERROR_UNTERMINATED_CHAR_STRING:
			printf("Character String too Long\n");
			return;
		case LLL_ERROR_UNTERMINATED_ESCAPE_SEQUENCE:
			printf("Unterminated Escape Sequence\n");
			return;
		case LLL_ERROR_UNKNOWN_ESCAPE_CHARACTER:
			printf("Unknown Escape Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE:
			printf("Unterminated Hexadecimal Escape Sequence\n");
			return;
		case LLL_ERROR_UNKNOWN_HEXADECIMAL_CHARCTER:
			printf("Unknown Hexadecimal Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNKNOWN_DECIMAL_CHARCTER:
			printf("Unknown Decimal Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNKNOWN_OCTAL_CHARCTER:
			printf("Unknown Octal Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNKNOWN_BINARY_CHARCTER:
			printf("Unknown Binary Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNKNOWN_SYMBOL:
			*sp=0;
			printf("Unknown Symbol: '%s'\n",sym);
			free(sym);
			return;
		case LLL_ERROR_UNKNOWN_MODIFIER:
			*sp=0;
			printf("Unknown Modifier: '%s'\n",sym);
			free(sym);
			return;
		case LLL_ERROR_UNKNOWN_OUTPUT_MODIFIER:
			*sp=0;
			printf("Unknown Output Modifier: '%s'\n",sym);
			free(sym);
			return;
		case LLL_ERROR_UNKNOWN_IDENTIFIER_CHARACTER:
			printf("Unknown Identifier Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNEXPECTED_CHARACTER:
			printf("Unexpected Character: '%c'\n",t);
			return;
		case LLL_ERROR_UNSUPPORTED_8BIT_FLOAT_SIZE:
			printf("8-bit Float not Supported\n");
			return;
		case LLL_ERROR_UNSUPPORTED_16BIT_FLOAT_SIZE:
			printf("16-bit Float not Supported\n");
			return;
		case LLL_ERROR_SYMBOL_TOO_LONG:
			printf("Symbol Too Long\n");
			return;
		case LLL_ERROR_MODIFIER_TOO_LONG:
			printf("Modifier Too Long\n");
			return;
		case LLL_ERROR_NO_SYMBOL:
			printf("Expression Without a Symbol\n");
			return;
		case LLL_ERROR_TOO_MANY_ARGUMENTS:
			printf("Too Many Arguments\n");
			return;
		case LLL_ERROR_TOO_MANY_STATEMENTS:
			printf("Too Many Statements\n");
			return;
		case LLL_ERROR_MATH_OP_NOT_ENOUGH_ARGUMENTS:
			printf("Math Expression Contains not Enough Symbols\n");
			return;
		case LLL_ERROR_MATH_OP_TOO_MANY_ARGUMENTS:
			printf("Math Expression Contains too Many Symbols\n");
			return;
		case LLL_ERROR_MULTIPLE_OUTPUT_TYPE_MODIFIERS:
			printf("Multiple Output Type Modifiers\n");
			return;
		case LLL_ERROR_MULTIPLE_SIZE_MODIFIERS:
			printf("Multiple Size Modifiers\n");
			return;
		case LLL_ERROR_UNUSED_MODIFIERS:
			printf("Unused Modifiers\n");
			return;
	}
}



__LLL_IMPORT_EXPORT void lll_print_object(lll_object_t* o,FILE* f){
	_print_object_internal(o,f);
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_insert_debug_object(lll_input_data_stream_t* is,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	lll_debug_object_t* dbg=(lll_debug_object_t*)(_bf+_bf_off);
	dbg->t=LLL_OBJECT_TYPE_DEBUG_DATA;
	dbg->f=0;
	uint32_t ln=LLL_GET_INPUT_DATA_STREAM_LINE_NUMBER(is);
	uint32_t i=sizeof(lll_debug_object_t);
	if (ln>UINT16_MAX){
		dbg->f|=LLL_DEBUG_OBJECT_LINE_NUMBER_INT32;
		LLL_SET_DEBUG_OBJECT_DATA_UINT32(dbg,i,ln);
		i+=sizeof(uint32_t);
	}
	else if (ln>UINT8_MAX){
		dbg->f|=LLL_DEBUG_OBJECT_LINE_NUMBER_INT16;
		LLL_SET_DEBUG_OBJECT_DATA_UINT16(dbg,i,ln);
		i+=sizeof(uint16_t);
	}
	else{
		dbg->f|=LLL_DEBUG_OBJECT_LINE_NUMBER_INT8;
		LLL_SET_DEBUG_OBJECT_DATA_UINT8(dbg,i,ln);
		i+=sizeof(uint8_t);
	}
	uint32_t cl=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-LLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is)-1;
	if (cl>UINT16_MAX){
		dbg->f|=LLL_DEBUG_OBJECT_COLUMN_NUMBER_INT32;
		LLL_SET_DEBUG_OBJECT_DATA_UINT32(dbg,i,cl);
		i+=sizeof(uint32_t);
	}
	else if (cl>UINT8_MAX){
		dbg->f|=LLL_DEBUG_OBJECT_COLUMN_NUMBER_INT16;
		LLL_SET_DEBUG_OBJECT_DATA_UINT16(dbg,i,cl);
		i+=sizeof(uint16_t);
	}
	else{
		dbg->f|=LLL_DEBUG_OBJECT_COLUMN_NUMBER_INT8;
		LLL_SET_DEBUG_OBJECT_DATA_UINT8(dbg,i,cl);
		i+=sizeof(uint8_t);
	}
	uint32_t ln_off=LLL_GET_INPUT_DATA_STREAM_LINE_OFFSET(is);
	if (ln_off>UINT16_MAX){
		dbg->f|=LLL_DEBUG_OBJECT_FILE_OFFSET_INT32;
		LLL_SET_DEBUG_OBJECT_DATA_UINT32(dbg,i,ln_off);
		i+=sizeof(uint32_t);
	}
	else if (ln_off>UINT8_MAX){
		dbg->f|=LLL_DEBUG_OBJECT_FILE_OFFSET_INT16;
		LLL_SET_DEBUG_OBJECT_DATA_UINT16(dbg,i,ln_off);
		i+=sizeof(uint16_t);
	}
	else{
		dbg->f|=LLL_DEBUG_OBJECT_FILE_OFFSET_INT8;
		LLL_SET_DEBUG_OBJECT_DATA_UINT8(dbg,i,ln_off);
		i+=sizeof(uint8_t);
	}
	_bf_off+=i;
	if (_bf_off>=_bf_sz){
		e->t=LLL_ERROR_INTERNAL_STACK_OVERFLOW;
		e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(is)-2;
		e->sz=1;
		return LLL_RETURN_ERROR;
	}
	return LLL_RETURN_NO_ERROR;
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
	if (!_read_object_internal(c_dt,c,e)){
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
		e->off=LLL_GET_INPUT_DATA_STREAM_OFFSET(c_dt->is)-2;
		e->sz=1;
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
		if (!_read_object_internal(c_dt,c,e)){
			return LLL_RETURN_ERROR;
		}
		if (*sc==UINT16_MAX){
			e->t=LLL_ERROR_TOO_MANY_STATEMENTS;
			e->off=off-2;
			e->sz=LLL_GET_INPUT_DATA_STREAM_OFFSET(c_dt->is)-off-2;
			return LLL_RETURN_ERROR;
		}
		(*sc)++;
	}
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_optimize_object(lll_object_t* o,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	if (_optimize_object_internal(o,e)==UINT32_MAX){
		return LLL_RETURN_ERROR;
	}
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_remove_object_debug_data(lll_object_t* o,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	_remove_debug_data_internal(o);
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_remove_object_padding(lll_object_t* o,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	uint32_t rm=0;
	_remove_padding_internal(o,&rm);
	return LLL_RETURN_NO_ERROR;
}



__LLL_IMPORT_EXPORT __LLL_CHECK_OUTPUT uint8_t lll_write_object(lll_output_data_stream_t* os,lll_object_t* o,lll_error_t* e){
	if (!_bf){
		e->t=LLL_ERROR_NO_STACK;
		return LLL_RETURN_ERROR;
	}
	if (LLL_WRITE_TO_OUTPUT_DATA_STREAM(os,(uint8_t*)o,_get_object_size(o))){
		return LLL_RETURN_NO_ERROR;
	}
	e->t=LLL_ERROR_FAILED_FILE_WRITE;
	return LLL_RETURN_ERROR;
}



#ifdef _MSC_VER
BOOL WINAPI DllMain(HINSTANCE h,DWORD r,LPVOID rs){
	return TRUE;
}
#endif

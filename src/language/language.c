#include <language.h>



#define CONSTRUCT_CHAR(c) ((#c)[0])
#define CONSTRUCT_WORD(a,b) ((((uint16_t)(b))<<8)|(a))
#define CONSTRUCT_DWORD(a,b,c,d) ((((uint32_t)(d))<<24)|(((uint32_t)(c))<<16)|(((uint32_t)(b))<<8)|(a))
#define _FAST_COMPARE_JOIN_(l) FAST_COMPARE_##l
#define _FAST_COMPARE_JOIN(l) _FAST_COMPARE_JOIN_(l)
#define _FAST_COMPARE_COUNT_ARGS(_1,_2,_3,_4,_5,n,...) n
#define FAST_COMPARE(s,...) _FAST_COMPARE_JOIN(_FAST_COMPARE_COUNT_ARGS(__VA_ARGS__,5,4,3,2,1))(s,__VA_ARGS__)
#define FAST_COMPARE_1(s,a) (*(s)==CONSTRUCT_CHAR(a))
#define FAST_COMPARE_2(s,a,b) (*((uint16_t*)(s))==CONSTRUCT_WORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b)))
#define FAST_COMPARE_3(s,a,b,c) (*((uint16_t*)(s))==CONSTRUCT_WORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b))&&*((s)+2)==CONSTRUCT_CHAR(c))
#define FAST_COMPARE_4(s,a,b,c,d) (*((uint32_t*)(s))==CONSTRUCT_DWORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b),CONSTRUCT_CHAR(c),CONSTRUCT_CHAR(d)))
#define FAST_COMPARE_5(s,a,b,c,d,e) (*((uint32_t*)(s))==CONSTRUCT_DWORD(CONSTRUCT_CHAR(a),CONSTRUCT_CHAR(b),CONSTRUCT_CHAR(c),CONSTRUCT_CHAR(d))&&*((s)+4)==CONSTRUCT_CHAR(e))
#define READ_SINGLE_CHAR_OK 0
#define READ_SINGLE_CHAR_END 1
#define READ_SINGLE_CHAR_ERROR 2
#define READ_SINGLE_CHAR_GET_TYPE(f) ((f)&3)
#define READ_SINGLE_CHAR_GET_ERROR(f) ((f)>>2)
#define READ_SINGLE_CHAR_RETURN_ERROR(e) (((e)<<2)|READ_SINGLE_CHAR_ERROR)



uint8_t _bf[INTERNAL_STACK_SIZE];
uint32_t _bf_ptr=0;
uint8_t _fl=0;



#ifdef _MSC_VER
#include <intrin.h>
#pragma intrinsic(__movsw)
#else
static inline void __movsw(unsigned short* d,unsigned short* s,size_t n){
	__asm__("rep movsw":"=D"(d),"=S"(s),"=c"(n):"0"(d),"1"(s),"2"(n):"memory");
}
#define __assume(x) \
	do{ \
		if (!(x)){ \
			__builtin_unreachable(); \
		} \
	} while (0)
#endif



uint32_t _print_object_internal(object_t* o,FILE* f){
	uint32_t off=sizeof(object_t);
	if (IS_OBJECT_TYPE_NOT_TYPE(o)){
		while (IS_OBJECT_REF(o)){
			o=GET_OBJECT_REF(o);
		}
	}
	switch (GET_OBJECT_TYPE(o)){
		case OBJECT_TYPE_UNKNOWN:
			fprintf(f,"(unknown)");
			return off;
		case OBJECT_TYPE_CHAR:
			fprintf(f,"'%c'",GET_OBJECT_AS_CHAR(o));
			return off+sizeof(char);
		case OBJECT_TYPE_STRING:
			fputc('"',f);
			uint32_t l=GET_OBJECT_STRING_LENGTH(o);
			off+=l+sizeof(string_length_t);
			char* str=GET_OBJECT_AS_STRING(o);
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
			return off;
		case OBJECT_TYPE_INT:
			if (IS_OBJECT_INT64(o)){
				fprintf(f,"%ld",GET_OBJECT_AS_INT64(o));
				return off+sizeof(int64_t);
			}
			fprintf(f,"%d",GET_OBJECT_AS_INT32(o));
			return off+sizeof(int32_t);
		case OBJECT_TYPE_FLOAT:
			if (IS_OBJECT_FLOAT64(o)){
				fprintf(f,"%lf",GET_OBJECT_AS_FLOAT64(o));
				return off+sizeof(double);
			}
			fprintf(f,"%f",GET_OBJECT_AS_FLOAT32(o));
			return off+sizeof(float);
		case OBJECT_TYPE_IDENTIFIER:
			l=GET_OBJECT_STRING_LENGTH(o);
			off+=l+sizeof(string_length_t);
			str=GET_OBJECT_AS_STRING(o);
			while (l){
				l--;
				fputc(*str,f);
				str++;
			}
			return off;
		case OBJECT_TYPE_NIL:
			fprintf(f,"nil");
			return off;
		case OBJECT_TYPE_TRUE:
			fprintf(f,"true");
			return off;
		case OBJECT_TYPE_FALSE:
			fprintf(f,"false");
			return off;
		case OBJECT_TYPE_PRINT:
			fprintf(f,"(print");
			break;
		case OBJECT_TYPE_PTR:
			fprintf(f,"(ptr");
			break;
		case OBJECT_TYPE_OPERATION_LIST:
			fprintf(f,"(<op list>");
			break;
		case OBJECT_TYPE_AND:
			fprintf(f,"(&&");
			break;
		case OBJECT_TYPE_OR:
			fprintf(f,"(||");
			break;
		case OBJECT_TYPE_SET:
			fprintf(f,"(=");
			break;
		case OBJECT_TYPE_FUNC:
			fprintf(f,"(func");
			break;
		case OBJECT_TYPE_IF:
			fprintf(f,"(if");
			break;
		case OBJECT_TYPE_ADD:
			fprintf(f,"(+");
			break;
		case OBJECT_TYPE_SUB:
			fprintf(f,"(-");
			break;
		case OBJECT_TYPE_MULT:
			fprintf(f,"(*");
			break;
		case OBJECT_TYPE_DIV:
			fprintf(f,"(/");
			break;
		case OBJECT_TYPE_FLOOR_DIV:
			fprintf(f,"(//");
			break;
		case OBJECT_TYPE_MOD:
			fprintf(f,"(%%");
			break;
		case OBJECT_TYPE_BIT_AND:
			fprintf(f,"(&");
			break;
		case OBJECT_TYPE_BIT_OR:
			fprintf(f,"(|");
			break;
		case OBJECT_TYPE_BIT_XOR:
			fprintf(f,"(^");
			break;
		case OBJECT_TYPE_BIT_NOT:
			fprintf(f,"(!");
			break;
		case OBJECT_TYPE_DIV_MOD:
			fprintf(f,"(/%%");
			break;
		case OBJECT_TYPE_POW:
			fprintf(f,"(**");
			break;
		case OBJECT_TYPE_ROOT:
			fprintf(f,"(*/");
			break;
		case OBJECT_TYPE_FLOOR_ROOT:
			fprintf(f,"(*//");
			break;
		case OBJECT_TYPE_LOG:
			fprintf(f,"(_/");
			break;
		case OBJECT_TYPE_FLOOR_LOG:
			fprintf(f,"(_//");
			break;
		case OBJECT_TYPE_LESS:
			fprintf(f,"(<");
			break;
		case OBJECT_TYPE_LESS_EQUAL:
			fprintf(f,"(<=");
			break;
		case OBJECT_TYPE_EQUAL:
			fprintf(f,"(==");
			break;
		case OBJECT_TYPE_NOT_EQUAL:
			fprintf(f,"(!=");
			break;
		case OBJECT_TYPE_MORE:
			fprintf(f,"(>");
			break;
		case OBJECT_TYPE_MORE_EQUAL:
			fprintf(f,"(>=");
			break;
		default:
			__assume(0);
	}
	off+=sizeof(arg_count_t);
	uint8_t l=GET_OBJECT_ARGUMENT_COUNT(o);
	for (uint8_t i=0;i<l;i++){
		fputc(' ',f);
		off+=_print_object_internal(GET_OBJECT_ARGUMENT(o,off),f);
	}
	fputc(')',f);
	return off;
}



uint64_t _read_single_char(FILE* f,char t){
	int c=fgetc(f);
	if (!c||c==EOF){
		return READ_SINGLE_CHAR_RETURN_ERROR(CREATE_ERROR_CHAR(ERROR_UNMATCHED_QUOTES,t));
	}
	if (c==t){
		return READ_SINGLE_CHAR_END;
	}
	if (c=='\\'){
		c=fgetc(f);
		if (!c||c==EOF){
			return READ_SINGLE_CHAR_RETURN_ERROR(ERROR_UNTERMINATED_ESCAPE_SEQUENCE);
		}
		if (c=='\''||c=='"'||c=='\\'){
		}
		else if (c=='x'){
			c=fgetc(f);
			if (!c||c==EOF){
				return READ_SINGLE_CHAR_RETURN_ERROR(ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE);
			}
			if (c>96){
				c-=32;
			}
			if (c<48||(c>57&&c<65)||c>70){
				return READ_SINGLE_CHAR_RETURN_ERROR(CREATE_ERROR_CHAR(ERROR_UNKNOWN_HEXADECIMAL_CHARCTER,c));
			}
			uint8_t v=(c>64?c-55:c-48)<<4;
			c=fgetc(f);
			if (!c||c==EOF){
				return READ_SINGLE_CHAR_RETURN_ERROR(ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE);
			}
			if (c>96){
				c-=32;
			}
			if (c<48||(c>57&&c<65)||c>70){
				return READ_SINGLE_CHAR_RETURN_ERROR(CREATE_ERROR_CHAR(ERROR_UNKNOWN_HEXADECIMAL_CHARCTER,c));
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
			return READ_SINGLE_CHAR_RETURN_ERROR(CREATE_ERROR_CHAR(ERROR_UNKNOWN_ESCAPE_CHARACTER,c));
		}
	}
	*((char*)(_bf+_bf_ptr))=c;
	_bf_ptr++;
	if (_bf_ptr>=INTERNAL_STACK_SIZE){
		return READ_SINGLE_CHAR_RETURN_ERROR(ERROR_INTERNAL_STACK_OVERFLOW);
	}
	return READ_SINGLE_CHAR_OK;
}



object_t* _read_object_internal(FILE* f,int c){
	object_t* o=NULL;
	goto _no_read_next;
	while ((c=fgetc(f))&&c!=EOF){
_no_read_next:
		if ((c>8&&c<14)||c==' '||c==';'){
			continue;
		}
		if (c==')'){
			if (!o){
				return RETURN_ERROR(ERROR_UNMATCHED_CLOSE_PARENTHESES);
			}
			if (o->t==OBJECT_TYPE_UNKNOWN){
				if (_fl&FEATURE_EMPTY_EXPRESSION){
					o->t=OBJECT_TYPE_NIL;
					_bf_ptr-=sizeof(arg_count_t);
				}
				else{
					return RETURN_ERROR(ERROR_EMPTY_PARENTHESES);
				}
			}
			if (IS_OBJECT_TYPE_MATH_CHAIN_OPERATION(o)){
				uint8_t ac=GET_OBJECT_ARGUMENT_COUNT(o);
				if (!ac){
					o->t=OBJECT_TYPE_INT;
					_bf_ptr=GET_OBJECT_STACK_OFFSET(o)+sizeof(object_t)+sizeof(int32_t);
					SET_OBJECT_AS_INT32(o,0);
					return o;
				}
				if (ac==1){
					object_t* arg0=GET_OBJECT_ARGUMENT(o,sizeof(object_t)+sizeof(arg_count_t));
					uint32_t sz=_bf_ptr-GET_OBJECT_STACK_OFFSET(arg0);
					uint16_t* d=(uint16_t*)(_bf+GET_OBJECT_STACK_OFFSET(o));
					uint16_t* s=(uint16_t*)(_bf+GET_OBJECT_STACK_OFFSET(arg0));
					__movsw(d,s,sz>>1);
					if (sz&1){
						*(d+sz-1)=*(s+sz-1);
					}
					_bf_ptr-=sizeof(object_t)+sizeof(arg_count_t);
				}
			}
			else if (IS_OBJECT_TYPE_MATH_NO_CHAIN_OPERATION(o)){
				uint8_t ac=GET_OBJECT_ARGUMENT_COUNT(o);
				if (ac==1){
					return RETURN_ERROR(CREATE_ERROR_OBJECT(ERROR_MATH_OP_NOT_ENOUGH_ARGUMENTS,o));
				}
				if (ac>(GET_OBJECT_TYPE(o)==OBJECT_TYPE_POW?3:2)){
					return RETURN_ERROR(CREATE_ERROR_OBJECT(ERROR_MATH_OP_TOO_MANY_ARGUMENTS,o));
				}
			}
			return o;
		}
		if (c=='('){
			if (!o){
				o=(object_t*)(_bf+_bf_ptr);
				_bf_ptr+=sizeof(object_t)+sizeof(arg_count_t);
				if (_bf_ptr>=INTERNAL_STACK_SIZE){
					return RETURN_ERROR(ERROR_INTERNAL_STACK_OVERFLOW);
				}
				o->t=OBJECT_TYPE_UNKNOWN;
				RESET_OBJECT_ARGUMENT_COUNT(o);
			}
			else{
				if (o->t==OBJECT_TYPE_UNKNOWN){
					if (_fl&FEATURE_OPERATION_LIST){
						o->t=OBJECT_TYPE_OPERATION_LIST;
					}
					else{
						return RETURN_ERROR(ERROR_NO_SYMBOL);
					}
				}
				object_t* e=_read_object_internal(f,c);
				if (IS_ERROR(e)){
					return e;
				}
				INCREASE_OBJECT_ARGUMENT_COUNT(o);
			}
		}
		else if (o&&o->t==OBJECT_TYPE_UNKNOWN){
			char* str=(char*)(_bf+_bf_ptr);
			uint32_t sz=0;
_read_symbol:
			*(str+sz)=c;
			sz++;
			if (sz+_bf_ptr>=INTERNAL_STACK_SIZE){
				return RETURN_ERROR(ERROR_INTERNAL_STACK_OVERFLOW);
			}
			c=fgetc(f);
			if (!c||c==EOF){
				break;
			}
			if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='(')){
				goto _read_symbol;
			}
			if (sz==1){
				if (FAST_COMPARE(str,=)){
					o->t=OBJECT_TYPE_SET;
				}
				else if (FAST_COMPARE(str,+)){
					o->t=OBJECT_TYPE_ADD;
				}
				else if (FAST_COMPARE(str,-)){
					o->t=OBJECT_TYPE_SUB;
				}
				else if (FAST_COMPARE(str,*)){
					o->t=OBJECT_TYPE_MULT;
				}
				else if (FAST_COMPARE(str,/)){
					o->t=OBJECT_TYPE_DIV;
				}
				else if (FAST_COMPARE(str,%)){
					o->t=OBJECT_TYPE_MOD;
				}
				else if (FAST_COMPARE(str,&)){
					o->t=OBJECT_TYPE_BIT_AND;
				}
				else if (FAST_COMPARE(str,|)){
					o->t=OBJECT_TYPE_BIT_OR;
				}
				else if (FAST_COMPARE(str,^)){
					o->t=OBJECT_TYPE_BIT_XOR;
				}
				else if (FAST_COMPARE(str,!)){
					o->t=OBJECT_TYPE_BIT_NOT;
				}
				else if (FAST_COMPARE(str,<)){
					o->t=OBJECT_TYPE_LESS;
				}
				else if (FAST_COMPARE(str,>)){
					o->t=OBJECT_TYPE_MORE;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==2){
				if (FAST_COMPARE(str,&,&)){
					o->t=OBJECT_TYPE_AND;
				}
				else if (FAST_COMPARE(str,|,|)){
					o->t=OBJECT_TYPE_OR;
				}
				else if (FAST_COMPARE(str,i,f)){
					o->t=OBJECT_TYPE_IF;
				}
				else if (FAST_COMPARE(str,/,/)){
					o->t=OBJECT_TYPE_FLOOR_DIV;
				}
				else if (FAST_COMPARE(str,/,%)){
					o->t=OBJECT_TYPE_DIV_MOD;
				}
				else if (FAST_COMPARE(str,*,*)){
					o->t=OBJECT_TYPE_POW;
				}
				else if (FAST_COMPARE(str,*,/)){
					o->t=OBJECT_TYPE_ROOT;
				}
				else if (FAST_COMPARE(str,_,/)){
					o->t=OBJECT_TYPE_LOG;
				}
				else if (FAST_COMPARE(str,<,=)){
					o->t=OBJECT_TYPE_LESS_EQUAL;
				}
				else if (FAST_COMPARE(str,=,=)){
					o->t=OBJECT_TYPE_EQUAL;
				}
				else if (FAST_COMPARE(str,!,=)){
					o->t=OBJECT_TYPE_NOT_EQUAL;
				}
				else if (FAST_COMPARE(str,>,=)){
					o->t=OBJECT_TYPE_MORE_EQUAL;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==3){
				if (FAST_COMPARE(str,p,t,r)){
					o->t=OBJECT_TYPE_PTR;
				}
				else if (FAST_COMPARE(str,*,/,/)){
					o->t=OBJECT_TYPE_FLOOR_ROOT;
				}
				else if (FAST_COMPARE(str,_,/,/)){
					o->t=OBJECT_TYPE_FLOOR_LOG;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==4){
				if (FAST_COMPARE(str,f,u,n,c)){
					o->t=OBJECT_TYPE_FUNC;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==5){
				if (FAST_COMPARE(str,p,r,i,n,t)){
					o->t=OBJECT_TYPE_PRINT;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else{
_unknown_symbol:
				*(str+sz)=0;
				return RETURN_ERROR(CREATE_ERROR_STRING(ERROR_UNKNOWN_SYMBOL,str));
			}
			goto _no_read_next;
		}
		else if (c=='\''){
			object_t* arg=(object_t*)(_bf+_bf_ptr);
			_bf_ptr+=sizeof(object_t);
			if (_bf_ptr>=INTERNAL_STACK_SIZE){
				return RETURN_ERROR(ERROR_INTERNAL_STACK_OVERFLOW);
			}
			arg->t=OBJECT_TYPE_CHAR;
			uint64_t e=_read_single_char(f,'\'');
			if (READ_SINGLE_CHAR_GET_TYPE(e)==READ_SINGLE_CHAR_ERROR){
				return RETURN_ERROR(READ_SINGLE_CHAR_GET_ERROR(e));
			}
			if (READ_SINGLE_CHAR_GET_TYPE(e)==READ_SINGLE_CHAR_END){
				return RETURN_ERROR(ERROR_EMPTY_CHAR_STRING);
			}
			c=fgetc(f);
			if (c!='\''){
				return RETURN_ERROR(CREATE_ERROR_CHAR(ERROR_UNTERMINATED_CHAR_STRING,c));
			}
			if (!o){
				return arg;
			}
			INCREASE_OBJECT_ARGUMENT_COUNT(o);
		}
		else if (c=='"'){
			object_t* arg=(object_t*)(_bf+_bf_ptr);
			_bf_ptr+=sizeof(object_t)+sizeof(string_length_t);
			if (_bf_ptr>=INTERNAL_STACK_SIZE){
				return RETURN_ERROR(ERROR_INTERNAL_STACK_OVERFLOW);
			}
			arg->t=OBJECT_TYPE_STRING;
			uint32_t sz=0;
			while (1){
				uint64_t e=_read_single_char(f,'"');
				if (READ_SINGLE_CHAR_GET_TYPE(e)==READ_SINGLE_CHAR_ERROR){
					return RETURN_ERROR(READ_SINGLE_CHAR_GET_ERROR(e));
				}
				if (READ_SINGLE_CHAR_GET_TYPE(e)==READ_SINGLE_CHAR_END){
					break;
				}
				sz++;
			}
			SET_OBJECT_STRING_LENGTH(arg,sz);
			if (!o){
				return arg;
			}
			INCREASE_OBJECT_ARGUMENT_COUNT(o);
		}
		else if ((c>47&&c<58)||c=='-'){
			object_t* arg=(object_t*)(_bf+_bf_ptr);
			_bf_ptr+=sizeof(object_t)+sizeof(int64_t);
			if (_bf_ptr>=INTERNAL_STACK_SIZE){
				return RETURN_ERROR(ERROR_INTERNAL_STACK_OVERFLOW);
			}
			int8_t m=1;
			if (c=='-'){
				m=-1;
				c=fgetc(f);
				if (!c||c==EOF){
					break;
				}
			}
			arg->t=OBJECT_TYPE_INT;
			int64_t v=0;
			if (c=='0'){
				c=fgetc(f);
				if (!c||c==EOF){
					break;
				}
				if (c=='x'||c=='X'){
					c=fgetc(f);
					if (!c||c==EOF){
						break;
					}
_hexadecimal:
					if (c>96){
						c-=32;
					}
					if (c<48||(c>57&&c<65)||c>70){
						return RETURN_ERROR(CREATE_ERROR_CHAR(ERROR_UNKNOWN_HEXADECIMAL_CHARCTER,c));
					}
					v=(v<<4)+(c>64?c-55:c-48);
					c=fgetc(f);
					if (!c||c==EOF){
						break;
					}
					if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='(')){
						goto _hexadecimal;
					}
				}
				else if (c=='o'||c=='O'){
					c=fgetc(f);
					if (!c||c==EOF){
						break;
					}
_octal:
					if (c<48||c>55){
						return RETURN_ERROR(CREATE_ERROR_CHAR(ERROR_UNKNOWN_OCTAL_CHARCTER,c));
					}
					v=(v<<3)+(c-48);
					c=fgetc(f);
					if (!c||c==EOF){
						break;
					}
					if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='(')){
						goto _octal;
					}
				}
				else if (c=='b'||c=='B'){
					c=fgetc(f);
					if (!c||c==EOF){
						break;
					}
_binary:
					if (c!=48&&c!=49){
						return RETURN_ERROR(CREATE_ERROR_CHAR(ERROR_UNKNOWN_BINARY_CHARCTER,c));
					}
					v=(v<<1)+(c-48);
					c=fgetc(f);
					if (!c||c==EOF){
						break;
					}
					if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='(')){
						goto _binary;
					}
				}
				else if (c=='.'){
					arg->t=OBJECT_TYPE_FLOAT;
					printf("FLOAT!\n");
					return NULL;
				}
				else if (c>47&&c<58){
					goto _decimal;
				}
				else{
					return RETURN_ERROR(CREATE_ERROR_CHAR(ERROR_UNKNOWN_DECIMAL_CHARCTER,c));
				}
			}
			else{
_decimal:
				v=v*10+(c-48);
				c=fgetc(f);
				if (!c||c==EOF){
					break;
				}
				if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='(')){
					if (c<48||c>57){
						return RETURN_ERROR(CREATE_ERROR_CHAR(ERROR_UNKNOWN_DECIMAL_CHARCTER,c));
					}
					goto _decimal;
				}
			}
			if (v>INT32_MAX){
				arg->t|=OBJECT_TYPE_INT64_FLAG;
				SET_OBJECT_AS_INT64(arg,v*m);
			}
			else{
				_bf_ptr-=sizeof(int64_t)-sizeof(int32_t);
				SET_OBJECT_AS_INT32(arg,v*m);
			}
			if (!o){
				return arg;
			}
			INCREASE_OBJECT_ARGUMENT_COUNT(o);
			goto _no_read_next;
		}
		else if (c=='$'||(c>64&&c<91)||c=='_'||(c>97&&c<112)){
			object_t* arg=(object_t*)(_bf+_bf_ptr);
			_bf_ptr+=sizeof(object_t)+sizeof(string_length_t);
			if (_bf_ptr>=INTERNAL_STACK_SIZE){
				return RETURN_ERROR(ERROR_INTERNAL_STACK_OVERFLOW);
			}
			arg->t=OBJECT_TYPE_IDENTIFIER;
			char* str=(char*)(_bf+_bf_ptr);
			uint32_t sz=0;
_read_identifier:
			*(str+sz)=c;
			sz++;
			if (sz+_bf_ptr>=INTERNAL_STACK_SIZE){
				return RETURN_ERROR(ERROR_INTERNAL_STACK_OVERFLOW);
			}
			c=fgetc(f);
			if (!c||c==EOF){
				break;
			}
			if (c=='$'||(c>47&&c<58)||(c>64&&c<91)||c=='_'||(c>97&&c<112)){
				goto _read_identifier;
			}
			if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='(')){
				return RETURN_ERROR(CREATE_ERROR_CHAR(ERROR_UNKNOWN_IDENTIFIER_CHARACTER,c));
			}
			if (sz==3&&FAST_COMPARE(str,n,i,l)){
				_bf_ptr-=sizeof(string_length_t);
				arg->t=OBJECT_TYPE_NIL;
			}
			else if (sz==4&&FAST_COMPARE(str,t,r,u,e)){
				_bf_ptr-=sizeof(string_length_t);
				arg->t=OBJECT_TYPE_TRUE;
			}
			else if (sz==5&&FAST_COMPARE(str,f,a,l,s,e)){
				_bf_ptr-=sizeof(string_length_t);
				arg->t=OBJECT_TYPE_FALSE;
			}
			else{
				_bf_ptr+=sz;
				SET_OBJECT_STRING_LENGTH(arg,sz);
			}
			if (!o){
				return arg;
			}
			INCREASE_OBJECT_ARGUMENT_COUNT(o);
			goto _no_read_next;
		}
		else{
			return RETURN_ERROR(CREATE_ERROR_CHAR(ERROR_UNEXPECTED_CHARACTER,c));
		}
	}
	return RETURN_ERROR(ERROR_UNMATCHED_OPEN_PARENTHESES);
}



void print_error(error_t e){
	switch (GET_ERROR_TYPE(e)){
		default:
		case ERROR_UNKNOWN:
			printf("Unknown Error: %.8lx %.8lx\n",e>>32,e&0xffffffff);
			return;
		case ERROR_INTERNAL_STACK_OVERFLOW:
			printf("Internal Stack Overflow\n");
			return;
		case ERROR_UNMATCHED_OPEN_PARENTHESES:
			printf("Unmatched Left Parentheses\n");
			return;
		case ERROR_UNMATCHED_CLOSE_PARENTHESES:
			printf("Unmatched Right Parentheses\n");
			return;
		case ERROR_EMPTY_PARENTHESES:
			printf("Empty Expression\n");
			return;
		case ERROR_UNMATCHED_QUOTES:
			{
				char c=GET_ERROR_CHAR(e);
				if (c=='\''){
					printf("Unmatched Single Quotes\n");
				}
				else{
					printf("Unmatched Double Quotes\n");
				}
				return;
			}
		case ERROR_EMPTY_CHAR_STRING:
			printf("Empty Character String\n");
			return;
		case ERROR_UNTERMINATED_CHAR_STRING:
			printf("Character String too Long\n");
			return;
		case ERROR_UNTERMINATED_ESCAPE_SEQUENCE:
			printf("Unterminated Escape Sequence\n");
			return;
		case ERROR_UNKNOWN_ESCAPE_CHARACTER:
			printf("Unknown Escape Character: '%c'\n",GET_ERROR_CHAR(e));
			return;
		case ERROR_UNTERMINATED_HEX_ESCAPE_SEQUENCE:
			printf("Unterminated Hexadecimal Escape Sequence\n");
			return;
		case ERROR_UNKNOWN_HEXADECIMAL_CHARCTER:
			printf("Unknown Hexadecimal Character: '%c'\n",GET_ERROR_CHAR(e));
			return;
		case ERROR_UNKNOWN_DECIMAL_CHARCTER:
			printf("Unknown Decimal Character: '%c'\n",GET_ERROR_CHAR(e));
			return;
		case ERROR_UNKNOWN_OCTAL_CHARCTER:
			printf("Unknown Octal Character: '%c'\n",GET_ERROR_CHAR(e));
			return;
		case ERROR_UNKNOWN_BINARY_CHARCTER:
			printf("Unknown Binary Character: '%c'\n",GET_ERROR_CHAR(e));
			return;
		case ERROR_UNKNOWN_SYMBOL:
			printf("Unknown Symbol: '%s'\n",GET_ERROR_STRING(e));
			return;
		case ERROR_UNKNOWN_IDENTIFIER_CHARACTER:
			printf("Unknown Identifier Character: '%c'\n",GET_ERROR_CHAR(e));
			return;
		case ERROR_UNEXPECTED_CHARACTER:
			printf("Unexpected Character: '%c'\n",GET_ERROR_CHAR(e));
			return;
		case ERROR_NO_SYMBOL:
			printf("Expression Without a Symbol\n");
			return;
		case ERROR_MATH_OP_NOT_ENOUGH_ARGUMENTS:
			printf("Math Expression Contains not Enough Symbols: ");
			print_object(GET_ERROR_OBJECT(e),stdout);
			return;
		case ERROR_MATH_OP_TOO_MANY_ARGUMENTS:
			printf("Math Expression Contains too Many Symbols: ");
			print_object(GET_ERROR_OBJECT(e),stdout);
			return;
	}
}



uint8_t get_feature(uint8_t f){
	return _fl&f;
}



void set_feature(uint8_t f,uint8_t st){
	if (st){
		_fl|=f;
	}
	else{
		_fl&=~f;
	}
}



void print_object(object_t* o,FILE* f){
	_print_object_internal(o,f);
	fputc('\n',f);
}



object_t* read_object(FILE* f){
	int c=fgetc(f);
	if (!c||c==EOF){
		printf("Error! No Bracket ')'\n");
		return NULL;
	}
	return _read_object_internal(f,c);
}

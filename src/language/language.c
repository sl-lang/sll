#include <language.h>



#define CONSTRUCT_WORD(a,b) ((((uint16_t)(b))<<8)|(a))
#define CONSTRUCT_DWORD(a,b,c,d) ((((uint32_t)(d))<<24)|(((uint32_t)(c))<<16)|(((uint32_t)(b))<<8)|(a))
#define READ_SINGLE_CHAR_OK 0
#define READ_SINGLE_CHAR_END 1
#define READ_SINGLE_CHAR_ERROR 2
#define READ_SINGLE_CHAR_GET_TYPE(f) ((f)&3)
#define READ_SINGLE_CHAR_GET_ERROR(f) ((f)>>2)
#define READ_SINGLE_CHAR_RETURN_ERROR(e) (((e)<<2)|READ_SINGLE_CHAR_ERROR)



uint8_t _bf[INTERNAL_STACK_SIZE];
uint32_t _bf_ptr=0;



uint32_t _print_object_internal(object_t* o,FILE* f){
	uint32_t off=sizeof(object_t);
	switch (o->t){
		case OBJECT_TYPE_UNKNOWN:
			fprintf(f,"(unknown)");
			return off;
		case OBJECT_TYPE_PRINT:
			fprintf(f,"(print");
			goto _print_args;
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
			fprintf(f,"%lld",GET_OBJECT_AS_INT(o));
			return off+sizeof(int64_t);
		case OBJECT_TYPE_FLOAT:
			fprintf(f,"%lf",GET_OBJECT_AS_FLOAT(o));
			return off+sizeof(double);
		case OBJECT_TYPE_ADD:
			fprintf(f,"(+");
			goto _print_args;
		case OBJECT_TYPE_SUB:
			fprintf(f,"(-");
			goto _print_args;
		case OBJECT_TYPE_MULT:
			fprintf(f,"(*");
			goto _print_args;
		case OBJECT_TYPE_DIV:
			fprintf(f,"(/");
			goto _print_args;
		case OBJECT_TYPE_FLOOR_DIV:
			fprintf(f,"(//");
			goto _print_args;
		case OBJECT_TYPE_MOD:
			fprintf(f,"(%%");
			goto _print_args;
		case OBJECT_TYPE_POW:
			fprintf(f,"(**");
			goto _print_args;
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
		case OBJECT_TYPE_DIV_MOD:
			fprintf(f,"(/%%");
			goto _print_args;
		default:
			__assume(0);
	}
_print_args:
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
		return READ_SINGLE_CHAR_RETURN_ERROR(ERROR_UNMATCHED_QUOTES);
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
				return RETURN_ERROR(ERROR_EMPTY_PARENTHESES);
			}
			if (OBJECT_TYPE_IS_MATH_CHAIN_OPERATION(o)){
				uint8_t ac=GET_OBJECT_ARGUMENT_COUNT(o);
				if (!ac){
					o->t=OBJECT_TYPE_INT;
					_bf_ptr=GET_OBJECT_STACK_OFFSET(o)+sizeof(object_t);
					*((int64_t*)(_bf+_bf_ptr))=0;
					_bf_ptr+=sizeof(int64_t);
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
			else if (OBJECT_TYPE_IS_MATH_NO_CHAIN_OPERATION(o)){
				uint8_t ac=GET_OBJECT_ARGUMENT_COUNT(o);
				if (ac<2){
					return RETURN_ERROR(ERROR_MATH_OP_NOT_ENOUGH_ARGUMENTS);
				}
				if (ac>2){
					return RETURN_ERROR(ERROR_MATH_OP_TOO_MANY_ARGUMENTS);
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
					return RETURN_ERROR(ERROR_NO_SYMBOL);
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
				if (*str=='='){
					o->t=OBJECT_TYPE_SET;
				}
				else if (*str=='+'){
					o->t=OBJECT_TYPE_ADD;
				}
				else if (*str=='-'){
					o->t=OBJECT_TYPE_SUB;
				}
				else if (*str=='*'){
					o->t=OBJECT_TYPE_MULT;
				}
				else if (*str=='/'){
					o->t=OBJECT_TYPE_DIV;
				}
				else if (*str=='%'){
					o->t=OBJECT_TYPE_MOD;
				}
				else if (*str=='&'){
					o->t=OBJECT_TYPE_BIT_AND;
				}
				else if (*str=='|'){
					o->t=OBJECT_TYPE_BIT_OR;
				}
				else if (*str=='^'){
					o->t=OBJECT_TYPE_BIT_XOR;
				}
				else if (*str=='!'){
					o->t=OBJECT_TYPE_BIT_NOT;
				}
				else if (*str=='<'){
					o->t=OBJECT_TYPE_LESS;
				}
				else if (*str=='>'){
					o->t=OBJECT_TYPE_MORE;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==2){
				if (*((uint16_t*)str)==CONSTRUCT_WORD('&','&')){
					o->t=OBJECT_TYPE_AND;
				}
				else if (*((uint16_t*)str)==CONSTRUCT_WORD('|','|')){
					o->t=OBJECT_TYPE_OR;
				}
				else if (*((uint16_t*)str)==CONSTRUCT_WORD('i','f')){
					o->t=OBJECT_TYPE_IF;
				}
				else if (*((uint16_t*)str)==CONSTRUCT_WORD('/','/')){
					o->t=OBJECT_TYPE_FLOOR_DIV;
				}
				else if (*((uint16_t*)str)==CONSTRUCT_WORD('/','%')){
					o->t=OBJECT_TYPE_DIV_MOD;
				}
				else if (*((uint16_t*)str)==CONSTRUCT_WORD('*','*')){
					o->t=OBJECT_TYPE_POW;
				}
				else if (*((uint16_t*)str)==CONSTRUCT_WORD('*','/')){
					o->t=OBJECT_TYPE_ROOT;
				}
				else if (*((uint16_t*)str)==CONSTRUCT_WORD('/','_')){
					o->t=OBJECT_TYPE_LOG;
				}
				else if (*((uint16_t*)str)==CONSTRUCT_WORD('<','=')){
					o->t=OBJECT_TYPE_LESS_EQUAL;
				}
				else if (*((uint16_t*)str)==CONSTRUCT_WORD('=','=')){
					o->t=OBJECT_TYPE_EQUAL;
				}
				else if (*((uint16_t*)str)==CONSTRUCT_WORD('!','=')){
					o->t=OBJECT_TYPE_NOT_EQUAL;
				}
				else if (*((uint16_t*)str)==CONSTRUCT_WORD('>','=')){
					o->t=OBJECT_TYPE_MORE_EQUAL;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==3){
				if (*((uint16_t*)str)==CONSTRUCT_WORD('p','t')&&*(str+2)=='r'){
					o->t=OBJECT_TYPE_PTR;
				}
				else if (*((uint16_t*)str)==CONSTRUCT_WORD('l','e')&&*(str+2)=='t'){
					o->t=OBJECT_TYPE_LET;
				}
				else if (*((uint16_t*)str)==CONSTRUCT_WORD('*','/')&&*(str+2)=='/'){
					o->t=OBJECT_TYPE_FLOOR_ROOT;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==4){
				if (*((uint32_t*)str)==CONSTRUCT_DWORD('f','u','n','c')){
					o->t=OBJECT_TYPE_FUNC;
				}
				else if (*((uint32_t*)str)==CONSTRUCT_DWORD('g','o','t','o')){
					o->t=OBJECT_TYPE_GOTO;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==5){
				if (*((uint32_t*)str)==CONSTRUCT_DWORD('p','r','i','n')&&*(str+4)=='t'){
					o->t=OBJECT_TYPE_PRINT;
				}
				else if (*((uint32_t*)str)==CONSTRUCT_DWORD('w','h','i','l')&&*(str+4)=='e'){
					o->t=OBJECT_TYPE_WHILE;
				}
				else if (*((uint32_t*)str)==CONSTRUCT_DWORD('l','a','b','e')&&*(str+4)=='l'){
					o->t=OBJECT_TYPE_LABEL;
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
				uint64_t e=_read_single_char(f,'\"');
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
			_bf_ptr+=sizeof(object_t);
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
			*((int64_t*)(_bf+_bf_ptr))=v*m;
			_bf_ptr+=sizeof(int64_t);
			if (_bf_ptr>=INTERNAL_STACK_SIZE){
				return RETURN_ERROR(ERROR_INTERNAL_STACK_OVERFLOW);
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
			if (sz==3&&*((uint16_t*)str)==CONSTRUCT_WORD('n','i')&&*(str+2)=='l'){
				_bf_ptr-=sizeof(string_length_t);
				arg->t=OBJECT_TYPE_NIL;
			}
			else if (sz==4&&*((uint32_t*)str)==CONSTRUCT_DWORD('t','r','u','e')){
				_bf_ptr-=sizeof(string_length_t);
				arg->t=OBJECT_TYPE_TRUE;
			}
			else if (sz==5&&*((uint32_t*)str)==CONSTRUCT_DWORD('f','a','l','s')&&*(str+4)=='e'){
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
	switch (e){
		default:
			printf("Unknown Error: %llx\n",e);
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

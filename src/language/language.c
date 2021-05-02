#include <language.h>



#define CONSTRUCT_WORD(a,b) ((((uint16_t)(b))<<8)|(a))
#define CONSTRUCT_DWORD(a,b,c,d) ((((uint32_t)(d))<<24)|(((uint32_t)(c))<<16)|(((uint32_t)(b))<<8)|(a))
#define READ_SINGLE_CHAR_OK 0
#define READ_SINGLE_CHAR_END 1
#define READ_SINGLE_CHAR_ERROR 2



uint8_t _bf[INTERNAL_STACK_SIZE];
uint32_t _bf_ptr=0;



uint32_t print_object(object_t* o,FILE* f){
	uint32_t off=sizeof(object_t);
	switch (o->t){
		case OBJECT_TYPE_PRINT:
			fprintf(f,"(print");
			goto _print_args;
		case OBJECT_TYPE_CHAR:
			fprintf(f,"'%c'",GET_OBJECT_AS_CHAR(o));
			return off+sizeof(char);
		case OBJECT_TYPE_STRING:
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
		default:
			__assume(0);
	}
_print_args:
	for (uint8_t i=0;i<o->l;i++){
		fputc(' ',f);
		off+=print_object(GET_OBJECT_ARGUMENT(o,off),f);
	}
	fputc(')',f);
	return off;
}



uint8_t _read_single_char(FILE* f,char t){
	int c=fgetc(f);
	if (!c||c==EOF){
		printf("Unmatched quotes! (%c)\n",t);
		return READ_SINGLE_CHAR_ERROR;
	}
	if (c==t){
		return READ_SINGLE_CHAR_END;
	}
	if (c=='\\'){
		c=fgetc(f);
		if (!c||c==EOF){
			printf("Unterminated Escape!\n");
			return READ_SINGLE_CHAR_ERROR;
		}
		printf("ESCAPE: %c\n",c);
		return READ_SINGLE_CHAR_ERROR;
	}
	*((char*)(_bf+_bf_ptr))=c;
	_bf_ptr++;
	if (_bf_ptr>=INTERNAL_STACK_SIZE){
		printf("Internal Stack Overflow!\n");
		return READ_SINGLE_CHAR_ERROR;
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
				printf("error: no start bracket!\n");
				return NULL;
			}
			return o;
		}
		if (c=='('){
			if (!o){
				o=(object_t*)(_bf+_bf_ptr);
				_bf_ptr+=sizeof(object_t);
				if (_bf_ptr>=INTERNAL_STACK_SIZE){
					goto _overflow;
				}
				o->t=OBJECT_TYPE_UNKNOWN;
				o->l=0;
			}
			else{
				if (!_read_object_internal(f,c)){
					return NULL;
				}
				o->l++;
			}
		}
		else if (!o){
			printf("No Object Start ('(') Before Data!\n");
			return NULL;
		}
		else if (o->t==OBJECT_TYPE_UNKNOWN){
			char* str=(char*)(_bf+_bf_ptr);
			uint32_t sz=0;
_read_str:
			*(str+sz)=c;
			sz++;
			if (sz+_bf_ptr>=INTERNAL_STACK_SIZE){
				goto _overflow;
			}
			c=fgetc(f);
			if (!c||c==EOF){
				break;
			}
			if (c<9||(c>13&&c!=' '&&c!=';'&&c!=')'&&c!='(')){
				goto _read_str;
			}
			if (sz==1){
				if (*str=='+'){
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
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==2){
				if (*((uint16_t*)str)==CONSTRUCT_WORD('/','/')){
					o->t=OBJECT_TYPE_FLOOR_DIV;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else if (sz==5){
				if (*((uint32_t*)str)==CONSTRUCT_DWORD('p','r','i','n')&&*(str+4)=='t'){
					o->t=OBJECT_TYPE_PRINT;
				}
				else{
					goto _unknown_symbol;
				}
			}
			else{
_unknown_symbol:
				*(str+sz)=0;
				printf("Unknown Symbol: %s\n",str);
				return NULL;
			}
			goto _no_read_next;
		}
		else if (c=='\''){
			object_t* arg=(object_t*)(_bf+_bf_ptr);
			_bf_ptr+=sizeof(object_t);
			if (_bf_ptr>=INTERNAL_STACK_SIZE){
				goto _overflow;
			}
			arg->t=OBJECT_TYPE_CHAR;
			uint8_t e=_read_single_char(f,'\'');
			if (e==READ_SINGLE_CHAR_ERROR){
				return NULL;
			}
			if (e==READ_SINGLE_CHAR_END){
				printf("No Charcter in Char Type!\n");
				return NULL;
			}
			c=fgetc(f);
			if (c!='\''){
				printf("Unterminated Char Type!\n");
				return NULL;
			}
			o->l++;
		}
		else if ((c>47&&c<58)||c=='-'){
			object_t* arg=(object_t*)(_bf+_bf_ptr);
			_bf_ptr+=sizeof(object_t);
			if (_bf_ptr>=INTERNAL_STACK_SIZE){
				goto _overflow;
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
						printf("Unexpected charatcer: %c\n",c);
						return NULL;
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
						printf("Unexpected charatcer: %c\n",c);
						return NULL;
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
						printf("Unexpected charatcer: %c\n",c);
						return NULL;
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
					goto _unknown_character;
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
						printf("Unexpected charatcer: %c\n",c);
						return NULL;
					}
					goto _decimal;
				}
			}
			*((int64_t*)(_bf+_bf_ptr))=v*m;
			_bf_ptr+=sizeof(int64_t);
			if (_bf_ptr>=INTERNAL_STACK_SIZE){
				goto _overflow;
			}
			o->l++;
			goto _no_read_next;
		}
		else{
_unknown_character:
			printf("Unrecognised Character: %c\n",c);
			return NULL;
		}
	}
	printf("Error! No Bracket ')'\n");
	return NULL;
_overflow:
	printf("Internal Stack Overflow!\n");
	return NULL;
}



object_t* read_object(FILE* f){
	int c=fgetc(f);
	if (!c||c==EOF){
		printf("Error! No Bracket ')'\n");
		return NULL;
	}
	return _read_object_internal(f,c);
}

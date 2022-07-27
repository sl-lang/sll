#include <sll/_internal/intrinsics.h>
#include <sll/_internal/string.h>
#include <sll/_internal/string_format.h>
#include <sll/allocator.h>
#include <sll/api/string.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/gc.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>
#include <stdio.h>



#define SETUP_AND_JUSTIFT_RIGHT \
	if (p<sz){ \
		p=sz; \
	} \
	sll_string_increase(out,p); \
	p-=sz; \
	if (p&&!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){ \
		sll_set_memory(out->data+out->length,p,'0'); \
		out->length+=p; \
	}



static const sll_size_t _string_pow_10[]={1ull,10ull,100ull,1000ull,10000ull,100000ull,1000000ull,10000000ull,100000000ull,1000000000ull,10000000000ull,100000000000ull,1000000000000ull,10000000000000ull,100000000000000ull,1000000000000000ull,10000000000000000ull,100000000000000000ull,1000000000000000000ull,10000000000000000000ull};



static void _format_string(unsigned int f,unsigned int w,unsigned int p,sll_string_t* string,sll_string_t* out){
	sll_string_length_t l=string->length;
	if (f&STRING_FORMAT_FLAG_PERCISION){
		l=p;
	}
	sll_string_increase(out,(l>w?l:w));
	w=(w<l?0:w-l);
	if (w&&!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
		sll_set_memory(out->data+out->length,w,' ');
		out->length+=w;
	}
	sll_copy_data(string->data,l,out->data+out->length);
	sll_free_string(string);
	out->length+=l;
	if (w&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
		sll_set_memory(out->data+out->length,w,' ');
		out->length+=w;
	}
}



__SLL_EXTERNAL void sll_string_format(const sll_char_t* format,sll_string_t* out,...){
	va_list va;
	va_start(va,out);
	sll_var_arg_list_t dt;
	SLL_VAR_ARG_INIT_C(&dt,&va);
	sll_string_format_list(format,sll_string_length(format),&dt,out);
	va_end(va);
}



__SLL_EXTERNAL void sll_string_format_list(const sll_char_t* format,sll_string_length_t format_length,sll_var_arg_list_t* va,sll_string_t* out){
	if (!format_length){
		SLL_INIT_STRING(out);
		return;
	}
	STRING_INIT_STACK(out);
	sll_string_length_t i=format_length;
	while (i){
		if (*format!='%'){
			sll_string_increase(out,1);
			out->data[out->length]=*format;
			out->length++;
			i--;
			format++;
			continue;
		}
		i--;
		format++;
		const sll_char_t* b=format;
		sll_string_length_t bi=i;
		if (!i){
			sll_string_increase(out,1);
			out->data[out->length]='%';
			out->length++;
			break;
		}
		unsigned int f=0;
		while (1){
			if (*format=='0'){
				f|=STRING_FORMAT_FLAG_PAD_ZERO;
			}
			else if (*format=='+'){
				f|=STRING_FORMAT_FLAG_SIGN;
			}
			else if (*format==' '){
				f|=STRING_FORMAT_FLAG_SPACE_SIGN;
			}
			else if (*format=='-'){
				f|=STRING_FORMAT_FLAG_JUSTIFY_LEFT;
			}
			else if (*format=='#'){
				f|=STRING_FORMAT_FLAG_ALTERNATIVE_FORM;
			}
			else{
				break;
			}
			i--;
			format++;
		}
		unsigned int w=0;
		unsigned int p=0;
		if (*format=='*'){
			int a=(int)sll_var_arg_get_int(va);
			if (a<0){
				f|=STRING_FORMAT_FLAG_JUSTIFY_LEFT;
				a=-a;
			}
			w=(unsigned int)a;
			i--;
			format++;
		}
		else{
			while (*format>47&&*format<58){
				w=w*10+((*format)-48);
				i--;
				format++;
			}
		}
		if (*format=='.'){
			f|=STRING_FORMAT_FLAG_PERCISION;
			i--;
			format++;
			if (*format=='*'){
				int a=(int)sll_var_arg_get_int(va);
				p=(a<0?0:(unsigned int)a);
				i--;
				format++;
			}
			else{
				while (*format>47&&*format<58){
					p=p*10+((*format)-48);
					i--;
					format++;
				}
			}
		}
		if (*format=='h'){
			format++;
			if (*format=='h'){
				f|=STRING_FORMAT_FLAG_HH_BITS;
				i-=2;
				format++;
			}
			else{
				f|=STRING_FORMAT_FLAG_H_BITS;
				i--;
			}
		}
		else if (*format=='l'){
			format++;
			if (*format=='l'){
				f|=STRING_FORMAT_FLAG_LL_BITS;
				i-=2;
				format++;
			}
			else{
				f|=STRING_FORMAT_FLAG_L_BITS;
				i--;
			}
		}
		else if (*format=='j'){
			f|=STRING_FORMAT_FLAG_LL_BITS;
			i--;
			format++;
		}
		if (*format=='%'){
			sll_string_increase(out,1);
			out->data[out->length]='%';
			out->length++;
		}
		else if (*format=='c'){
			if (!w){
				w=1;
			}
			sll_string_increase(out,w);
			w--;
			if (w&&!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
				sll_set_memory(out->data+out->length,w,' ');
				out->length+=w;
			}
			out->data[out->length]=sll_var_arg_get_char(va);
			out->length++;
			if (w&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
				sll_set_memory(out->data+out->length,w,' ');
				out->length+=w;
			}
		}
		else if (*format=='f'){
			sll_char_t buffer[256];
			sll_string_t s;
			sll_string_from_pointer_length(buffer,snprintf((char*)buffer,256,"%.16lf",sll_var_arg_get_float(va)),&s);
			_format_string(f,w,p,&s,out);
		}
		else if (*format=='s'){
			sll_string_t s;
			sll_var_arg_get_string(va,&s);
			_format_string(f,w,p,&s,out);
		}
		else if (*format=='S'){
			sll_object_t obj=sll_var_arg_get_object(va);
			sll_string_t s;
			sll_api_string_convert(&obj,1,&s);
			SLL_RELEASE(obj);
			_format_string(f,w,p,&s,out);
		}
		else if (*format=='d'||*format=='o'||*format=='u'||*format=='x'||*format=='X'){
			if (*format=='X'){
				f|=STRING_FORMAT_FLAG_UPPERCASE;
			}
			sll_size_t n;
			if (*format=='d'){
				sll_integer_t sn;
				if (f&STRING_FORMAT_FLAG_HH_BITS){
					sn=sll_var_arg_get_int(va);
					if (sn<-0x80){
						sn=-0x80;
					}
					else if (sn>0x7f){
						sn=0x7f;
					}
				}
				else if (f&STRING_FORMAT_FLAG_H_BITS){
					sn=sll_var_arg_get_int(va);
					if (sn<-0x8000){
						sn=-0x8000;
					}
					else if (sn>0x7fff){
						sn=0x7fff;
					}
				}
				else if (f&STRING_FORMAT_FLAG_L_BITS){
					sn=sll_var_arg_get_int(va);
					if (sn<-0x80000000ll){
						sn=-0x80000000ll;
					}
					else if (sn>0x7fffffff){
						sn=0x7fffffff;
					}
				}
				else if (f&STRING_FORMAT_FLAG_LL_BITS){
					sn=sll_var_arg_get_int(va);
				}
				else{
					sn=sll_var_arg_get_int(va);
					if (va->type==SLL_VAR_ARG_LIST_TYPE_C){
						if (sn<-0x80000000ll){
							sn=-0x80000000ll;
						}
						else if (sn>0x7fffffff){
							sn=0x7fffffff;
						}
					}
				}
				if ((f&(STRING_FORMAT_FLAG_SIGN|STRING_FORMAT_FLAG_SPACE_SIGN))||sn<0){
					sll_string_increase(out,1);
					out->data[out->length]=43+((sn<0)<<1);
					out->length++;
				}
				n=(sll_size_t)(sn<0?-sn:sn);
			}
			else{
				if (f&STRING_FORMAT_FLAG_HH_BITS){
					n=sll_var_arg_get_int(va)&0xff;
				}
				else if (f&STRING_FORMAT_FLAG_H_BITS){
					n=sll_var_arg_get_int(va)&0xffff;
				}
				else if (f&STRING_FORMAT_FLAG_L_BITS){
					n=sll_var_arg_get_int(va)&0xffffffff;
				}
				else if (f&STRING_FORMAT_FLAG_LL_BITS){
					n=sll_var_arg_get_int(va);
				}
				else{
					n=sll_var_arg_get_int(va);
					if (va->type==SLL_VAR_ARG_LIST_TYPE_C){
						n&=0xffffffff;
					}
				}
				if (f&(STRING_FORMAT_FLAG_SIGN|STRING_FORMAT_FLAG_SPACE_SIGN)){
					sll_string_increase(out,1);
					out->data[out->length]='+';
					out->length++;
				}
			}
			if (!(f&STRING_FORMAT_FLAG_PERCISION)){
				p=1;
			}
			if (!p&&(f&STRING_FORMAT_FLAG_PERCISION)&&!n){
				if ((f&STRING_FORMAT_FLAG_ALTERNATIVE_FORM)&&*format=='o'&&!p){
					p=1;
				}
				sll_string_increase(out,p);
				sll_set_memory(out->data+out->length,p,'0');
				out->length+=p;
			}
			else{
				if ((f&STRING_FORMAT_FLAG_ALTERNATIVE_FORM)&&*format=='o'){
					sll_string_increase(out,1);
					out->data[out->length]='0';
					out->length++;
					if (p>0){
						p--;
					}
				}
				else if ((f&STRING_FORMAT_FLAG_ALTERNATIVE_FORM)&&(*format=='x'||*format=='X')){
					sll_string_increase(out,2);
					out->data[out->length]='0';
					out->data[out->length+1]=(f&STRING_FORMAT_FLAG_UPPERCASE?'X':'x');
					out->length+=2;
					if (p>1){
						p-=2;
					}
					else{
						p=0;
					}
				}
				if (!n){
					if (!p){
						p=1;
					}
					sll_string_increase(out,p);
					sll_set_memory(out->data+out->length,p,'0');
					out->length+=p;
				}
				else if (*format=='o'){
					sll_string_length_t sz=FIND_LAST_SET_BIT(n)/3+1;
					SETUP_AND_JUSTIFT_RIGHT;
					sll_string_length_t j=out->length+sz;
					do{
						j--;
						out->data[j]=(n&7)+48;
						n>>=3;
					} while (n);
					out->length+=sz;
					if (p&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(out->data+out->length,p,'0');
						out->length+=p;
					}
				}
				else if (*format!='x'&&*format!='X'){
					sll_string_length_t sz=((FIND_LAST_SET_BIT(n)+2)*1233)>>12;
					if (n>=_string_pow_10[sz]){
						sz++;
					}
					SETUP_AND_JUSTIFT_RIGHT;
					sll_string_length_t j=out->length+sz;
					do{
						j--;
						out->data[j]=(n%10)+48;
						n/=10;
					} while (n);
					out->length+=sz;
					if (p&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(out->data+out->length,p,'0');
						out->length+=p;
					}
				}
				else{
					sll_string_length_t sz=(FIND_LAST_SET_BIT(n)>>2)+1;
					SETUP_AND_JUSTIFT_RIGHT;
					sll_string_length_t j=out->length+sz;
					sll_char_t e=((f&STRING_FORMAT_FLAG_UPPERCASE)?55:87);
					do{
						j--;
						out->data[j]=(n&15)+((n&15)>9?e:48);
						n>>=4;
					} while (n);
					out->length+=sz;
					if (p&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(out->data+out->length,p,'0');
						out->length+=p;
					}
				}
			}
		}
		else if (*format=='p'){
			sll_string_increase(out,16);
			addr_t ptr=(addr_t)sll_var_arg_get(va);
			sll_string_length_t j=64;
			do{
				j-=4;
				sll_char_t c=(ptr>>j)&15;
				out->data[out->length]=c+(c>9?87:48);
				out->length++;
			} while (j);
		}
		else{
			sll_string_increase(out,1);
			out->data[out->length]='%';
			out->length++;
			i=bi;
			format=b;
			continue;
		}
		i--;
		format++;
	}
	sll_allocator_move((void**)(&(out->data)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(out);
}

#include <sll/_internal/intrinsics.h>
#include <sll/_internal/string_format.h>
#include <sll/allocator.h>
#include <sll/api/string.h>
#include <sll/common.h>
#include <sll/data.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/var_arg.h>
#include <stdarg.h>



#define SETUP_AND_JUSTIFT_RIGHT \
	if (p<sz){ \
		p=sz; \
	} \
	sll_string_increase(o,p); \
	p-=sz; \
	if (p&&!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){ \
		sll_set_memory(o->v+o->l,p,'0'); \
		o->l+=p; \
	}



static const sll_size_t _string_pow_of_10[]={1ull,10ull,100ull,1000ull,10000ull,100000ull,1000000ull,10000000ull,100000000ull,1000000000ull,10000000000ull,100000000000ull,1000000000000ull,10000000000000ull,100000000000000ull,1000000000000000ull,10000000000000000ull,100000000000000000ull,1000000000000000000ull,10000000000000000000ull};



__SLL_EXTERNAL void sll_string_format(const sll_char_t* t,sll_string_t* o,...){
	va_list va;
	va_start(va,o);
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_C,
		{
			.c=&va
		}
	};
	sll_string_format_list(t,sll_string_length_unaligned(t),&dt,o);
	va_end(va);
}



__SLL_EXTERNAL void sll_string_format_list(const sll_char_t* t,sll_string_length_t sl,sll_var_arg_list_t* va,sll_string_t* o){
	sll_string_create(0,o);
	if (!sl){
		return;
	}
	sll_allocator_move((void**)(&(o->v)),SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
	sll_string_length_t i=sl;
	while (i){
		if (*t!='%'){
			sll_string_increase(o,1);
			o->v[o->l]=*t;
			o->l++;
			i--;
			t++;
			continue;
		}
		i--;
		t++;
		const sll_char_t* b=t;
		sll_string_length_t bi=i;
		if (!i){
			sll_string_increase(o,1);
			o->v[o->l]='%';
			o->l++;
			break;
		}
		unsigned int f=0;
		while (1){
			if (*t=='0'){
				f|=STRING_FORMAT_FLAG_PAD_ZERO;
			}
			else if (*t=='+'){
				f|=STRING_FORMAT_FLAG_SIGN;
			}
			else if (*t==' '){
				f|=STRING_FORMAT_FLAG_SPACE_SIGN;
			}
			else if (*t=='-'){
				f|=STRING_FORMAT_FLAG_JUSTIFY_LEFT;
			}
			else if (*t=='#'){
				f|=STRING_FORMAT_FLAG_ALTERNATIVE_FORM;
			}
			else{
				break;
			}
			i--;
			t++;
		}
		unsigned int w=0;
		unsigned int p=0;
		if (*t=='*'){
			int a=(int)sll_var_arg_get_int(va);
			if (a<0){
				f|=STRING_FORMAT_FLAG_JUSTIFY_LEFT;
				a=-a;
			}
			w=(unsigned int)a;
			i--;
			t++;
		}
		else{
			while (*t>47&&*t<58){
				w=w*10+((*t)-48);
				i--;
				t++;
			}
		}
		if (*t=='.'){
			f|=STRING_FORMAT_FLAG_PERCISION;
			i--;
			t++;
			if (*t=='*'){
				int a=(int)sll_var_arg_get_int(va);
				p=(a<0?0:(unsigned int)a);
				i--;
				t++;
			}
			else{
				while (*t>47&&*t<58){
					p=p*10+((*t)-48);
					i--;
					t++;
				}
			}
		}
		if (*t=='h'){
			t++;
			if (*t=='h'){
				f|=STRING_FORMAT_FLAG_HH_BITS;
				i-=2;
				t++;
			}
			else{
				f|=STRING_FORMAT_FLAG_H_BITS;
				i--;
			}
		}
		else if (*t=='l'){
			t++;
			if (*t=='l'){
				f|=STRING_FORMAT_FLAG_LL_BITS;
				i-=2;
				t++;
			}
			else{
				f|=STRING_FORMAT_FLAG_L_BITS;
				i--;
			}
		}
		else if (*t=='j'){
			f|=STRING_FORMAT_FLAG_LL_BITS;
			i--;
			t++;
		}
		if (*t=='%'){
			sll_string_increase(o,1);
			o->v[o->l]='%';
			o->l++;
		}
		else if (*t=='c'){
			if (!w){
				w=1;
			}
			sll_string_increase(o,w);
			w--;
			if (w&&!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
				sll_set_memory(o->v+o->l,w,' ');
				o->l+=w;
			}
			o->v[o->l]=sll_var_arg_get_char(va);
			o->l++;
			if (w&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
				sll_set_memory(o->v+o->l,w,' ');
				o->l+=w;
			}
		}
		else if (*t=='s'){
			sll_string_t s;
			sll_var_arg_get_string(va,&s);
			sll_string_length_t l=s.l;
			if (f&STRING_FORMAT_FLAG_PERCISION){
				l=p;
			}
			sll_string_increase(o,(l>w?l:w));
			w=(w<l?0:w-l);
			if (w&&!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
				sll_set_memory(o->v+o->l,w,' ');
				o->l+=w;
			}
			sll_copy_data(s.v,l,o->v+o->l);
			sll_free_string(&s);
			o->l+=l;
			if (w&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
				sll_set_memory(o->v+o->l,w,' ');
				o->l+=w;
			}
		}
		else if (*t=='S'){
			sll_object_t* obj=(sll_object_t*)sll_var_arg_get(va);
			sll_string_t s;
			if (obj){
				sll_api_string_convert(&obj,1,&s);
			}
			else{
				sll_string_create(0,&s);
			}
			sll_string_length_t l=s.l;
			if (f&STRING_FORMAT_FLAG_PERCISION){
				l=p;
			}
			sll_string_increase(o,(l>w?l:w));
			w=(w<l?0:w-l);
			if (w&&!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
				sll_set_memory(o->v+o->l,w,' ');
				o->l+=w;
			}
			sll_copy_data(s.v,l,o->v+o->l);
			sll_free_string(&s);
			o->l+=l;
			if (w&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
				sll_set_memory(o->v+o->l,w,' ');
				o->l+=w;
			}
		}
		else if (*t=='d'||*t=='o'||*t=='u'||*t=='x'||*t=='X'){
			if (*t=='X'){
				f|=STRING_FORMAT_FLAG_UPPERCASE;
			}
			sll_size_t n;
			if (*t=='d'){
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
					if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
						if (sn<-0x80000000ll){
							sn=-0x80000000ll;
						}
						else if (sn>0x7fffffff){
							sn=0x7fffffff;
						}
					}
				}
				if ((f&(STRING_FORMAT_FLAG_SIGN|STRING_FORMAT_FLAG_SPACE_SIGN))||sn<0){
					sll_string_increase(o,1);
					o->v[o->l]=43+((sn<0)<<1);
					o->l++;
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
					if (va->t==SLL_VAR_ARG_LIST_TYPE_C){
						n&=0xffffffff;
					}
				}
				if (f&(STRING_FORMAT_FLAG_SIGN|STRING_FORMAT_FLAG_SPACE_SIGN)){
					sll_string_increase(o,1);
					o->v[o->l]='+';
					o->l++;
				}
			}
			if (!(f&STRING_FORMAT_FLAG_PERCISION)){
				p=1;
			}
			if (!p&&(f&STRING_FORMAT_FLAG_PERCISION)&&!n){
				if ((f&STRING_FORMAT_FLAG_ALTERNATIVE_FORM)&&*t=='o'&&!p){
					p=1;
				}
				sll_string_increase(o,p);
				sll_set_memory(o->v+o->l,p,'0');
				o->l+=p;
			}
			else{
				if ((f&STRING_FORMAT_FLAG_ALTERNATIVE_FORM)&&*t=='o'){
					sll_string_increase(o,1);
					o->v[o->l]='0';
					o->l++;
					if (p>0){
						p--;
					}
				}
				else if ((f&STRING_FORMAT_FLAG_ALTERNATIVE_FORM)&&(*t=='x'||*t=='X')){
					sll_string_increase(o,2);
					o->v[o->l]='0';
					o->v[o->l+1]=(f&STRING_FORMAT_FLAG_UPPERCASE?'X':'x');
					o->l+=2;
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
					sll_string_increase(o,p);
					sll_set_memory(o->v+o->l,p,'0');
					o->l+=p;
				}
				else if (*t=='o'){
					sll_string_length_t sz=FIND_LAST_SET_BIT(n)/3+1;
					SETUP_AND_JUSTIFT_RIGHT;
					sll_string_length_t j=o->l+sz;
					do{
						j--;
						o->v[j]=(n&7)+48;
						n>>=3;
					} while (n);
					o->l+=sz;
					if (p&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
				}
				else if (*t!='x'&&*t!='X'){
					sll_string_length_t sz=((FIND_LAST_SET_BIT(n)+2)*1233)>>12;
					if (n>=_string_pow_of_10[sz]){
						sz++;
					}
					SETUP_AND_JUSTIFT_RIGHT;
					sll_string_length_t j=o->l+sz;
					do{
						j--;
						o->v[j]=(n%10)+48;
						n/=10;
					} while (n);
					o->l+=sz;
					if (p&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
				}
				else{
					sll_string_length_t sz=(FIND_LAST_SET_BIT(n)>>2)+1;
					SETUP_AND_JUSTIFT_RIGHT;
					sll_string_length_t j=o->l+sz;
					sll_char_t e=((f&STRING_FORMAT_FLAG_UPPERCASE)?55:87);
					do{
						j--;
						o->v[j]=(n&15)+((n&15)>9?e:48);
						n>>=4;
					} while (n);
					o->l+=sz;
					if (p&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
				}
			}
		}
		else if (*t=='p'){
			sll_string_increase(o,16);
			addr_t ptr=(addr_t)sll_var_arg_get(va);
			sll_string_length_t j=64;
			do{
				j-=4;
				sll_char_t c=(ptr>>j)&15;
				o->v[o->l]=c+(c>9?87:48);
				o->l++;
			} while (j);
		}
		else{
			sll_string_increase(o,1);
			o->v[o->l]='%';
			o->l++;
			i=bi;
			t=b;
			continue;
		}
		i--;
		t++;
	}
	sll_allocator_move((void**)(&(o->v)),SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(o);
}

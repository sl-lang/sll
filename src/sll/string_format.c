#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>



static const uint64_t _string_pow_of_10[]={1ull,10ull,100ull,1000ull,10000ull,100000ull,1000000ull,10000000ull,100000000ull,1000000000ull,10000000000ull,100000000000ull,1000000000000ull,10000000000000ull,100000000000000ull,1000000000000000ull,10000000000000000ull,100000000000000000ull,1000000000000000000ull,10000000000000000000ull};



__SLL_EXTERNAL void sll_string_format(const sll_char_t* t,sll_string_t* o,...){
	va_list va;
	va_start(va,o);
	sll_string_format_list(t,va,o);
	va_end(va);
}



__SLL_EXTERNAL void sll_string_format_list(const sll_char_t* t,va_list va,sll_string_t* o){
	sll_string_create(0,o);
	o->v=sll_memory_move(o->v,SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
	while (*t){
		if (*t!='%'){
			sll_string_increase(o,1);
			o->v[o->l]=*t;
			o->l++;
			t++;
			continue;
		}
		t++;
		const sll_char_t* b=t;
		if (!(*t)){
			sll_string_increase(o,1);
			o->v[o->l]='%';
			o->l++;
			break;
		}
		uint16_t f=0;
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
			t++;
		}
		unsigned int w=0;
		unsigned int p=0;
		if (*t=='*'){
			int a=va_arg(va,int);
			if (a<0){
				f|=STRING_FORMAT_FLAG_JUSTIFY_LEFT;
				a=-a;
			}
			w=(unsigned int)a;
			t++;
		}
		else{
			while (*t>47&&*t<58){
				w=w*10+((*t)-48);
				t++;
			}
		}
		if (*t=='.'){
			f|=STRING_FORMAT_FLAG_PERCISION;
			t++;
			if (*t=='*'){
				int a=va_arg(va,int);
				if (a<0){
					a=0;
				}
				p=(unsigned int)a;
				t++;
			}
			else{
				while (*t>47&&*t<58){
					p=p*10+((*t)-48);
					t++;
				}
			}
		}
		if (*t=='h'){
			t++;
			if (*t=='h'){
				f|=STRING_FORMAT_FLAG_HH_BITS;
				t++;
			}
			else{
				f|=STRING_FORMAT_FLAG_H_BITS;
			}
		}
		else if (*t=='l'){
			t++;
			if (*t=='l'){
				f|=STRING_FORMAT_FLAG_LL_BITS;
				t++;
			}
			else{
				f|=STRING_FORMAT_FLAG_L_BITS;
			}
		}
		else if (*t=='j'||*t=='t'||*t=='z'){
			f|=STRING_FORMAT_FLAG_LL_BITS;
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
			if (!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
				sll_set_memory(o->v+o->l,w,' ');
				o->l+=w;
			}
			o->v[o->l]=(sll_char_t)va_arg(va,int);
			o->l++;
			if (f&STRING_FORMAT_FLAG_JUSTIFY_LEFT){
				sll_set_memory(o->v+o->l,w,' ');
				o->l+=w;
			}
		}
		else if (*t=='s'){
			const sll_char_t* s=va_arg(va,const sll_char_t*);
			if (!s){
				sll_string_increase(o,6);
				sll_copy_data(SLL_CHAR("(null)"),6,o->v+o->l);
				o->l+=6;
			}
			else{
				sll_string_length_t l=p;
				if (!(f&STRING_FORMAT_FLAG_PERCISION)){
					l=sll_string_length_unaligned(s);
				}
				sll_string_increase(o,(l>w?l:w));
				w=(w<l?0:w-l);
				if (!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
					sll_set_memory(o->v+o->l,w,' ');
					o->l+=w;
				}
				sll_copy_data(s,l,o->v+o->l);
				o->l+=l;
				if (f&STRING_FORMAT_FLAG_JUSTIFY_LEFT){
					sll_set_memory(o->v+o->l,w,' ');
					o->l+=w;
				}
			}
		}
		else if (*t=='d'||*t=='i'||*t=='o'||*t=='u'||*t=='x'||*t=='X'){
			if (*t=='X'){
				f|=STRING_FORMAT_FLAG_UPPERCASE;
			}
			uint64_t n;
			if (*t=='d'||*t=='i'){
				int64_t sn;
				if (f&STRING_FORMAT_FLAG_HH_BITS){
					sn=va_arg(va,signed int);
					if (sn<-0x80){
						sn=-0x80;
					}
					else if (sn>0x7f){
						sn=0x7f;
					}
				}
				else if (f&STRING_FORMAT_FLAG_H_BITS){
					sn=va_arg(va,signed int);
					if (sn<-0x8000){
						sn=-0x8000;
					}
					else if (sn>0x7fff){
						sn=0x7fff;
					}
				}
				else if (f&STRING_FORMAT_FLAG_L_BITS){
					sn=va_arg(va,signed long int);
				}
				else if (f&STRING_FORMAT_FLAG_LL_BITS){
					sn=va_arg(va,signed long long int);
				}
				else{
					sn=va_arg(va,signed int);
				}
				if ((f&(STRING_FORMAT_FLAG_SIGN|STRING_FORMAT_FLAG_SPACE_SIGN))||sn<0){
					sll_string_increase(o,1);
					o->v[o->l]=(sn<0?'-':'+');
					o->l++;
				}
				n=(uint64_t)(sn<0?-sn:sn);
			}
			else{
				if (f&STRING_FORMAT_FLAG_HH_BITS){
					n=va_arg(va,unsigned int)&0xff;
				}
				else if (f&STRING_FORMAT_FLAG_H_BITS){
					n=va_arg(va,unsigned int)&0xffff;
				}
				else if (f&STRING_FORMAT_FLAG_L_BITS){
					n=va_arg(va,unsigned long int);
				}
				else if (f&STRING_FORMAT_FLAG_LL_BITS){
					n=va_arg(va,unsigned long long int);
				}
				else{
					n=va_arg(va,unsigned int);
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
					uint8_t sz=(FIND_LAST_SET_BIT(n)+2)/3;
					if (p<sz){
						p=sz;
					}
					sll_string_increase(o,p);
					p-=sz;
					if (!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
					uint8_t i=sz*3;
					do{
						i-=3;
						o->v[o->l]=48+((n>>i)&7);
						o->l++;
					} while (i);
					if (f&STRING_FORMAT_FLAG_JUSTIFY_LEFT){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
				}
				else if (*t!='x'&&*t!='X'){
					uint8_t sz=((FIND_LAST_SET_BIT(n)+1)*1233)>>12;
					uint64_t pw=_string_pow_of_10[sz];
					if (n<pw){
						sz--;
						pw=_string_pow_of_10[sz];
					}
					if (p<sz){
						p=sz;
					}
					sll_string_increase(o,p);
					p-=sz;
					if (!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
					do{
						o->v[o->l]=48+((n/pw)%10);
						o->l++;
						sz--;
						pw/=10;
					} while (sz);
					if (f&STRING_FORMAT_FLAG_JUSTIFY_LEFT){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
				}
				else{
					uint8_t sz=(FIND_LAST_SET_BIT(n)+3)>>2;
					if (p<sz){
						p=sz;
					}
					sll_string_increase(o,p);
					p-=sz;
					if (!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
					uint8_t i=sz<<2;
					sll_char_t e=((f&STRING_FORMAT_FLAG_UPPERCASE)?55:87);
					do{
						i-=4;
						sll_char_t c=(n>>i)&0xf;
						o->v[o->l]=c+(c>9?e:48);
						o->l++;
					} while (i);
					if (f&STRING_FORMAT_FLAG_JUSTIFY_LEFT){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
				}
			}
		}
		else if (*t=='p'){
			sll_string_increase(o,sizeof(void*)<<1);
			uint64_t ptr=(uint64_t)va_arg(va,void*);
			for (int8_t i=(sizeof(void*)<<3)-4;i>=0;i-=4){
				sll_char_t c=(ptr>>i)&15;
				o->v[o->l]=c+(c>9?87:48);
				o->l++;
			}
		}
		else{
			sll_string_increase(o,1);
			o->v[o->l]='%';
			o->l++;
			t=b;
			continue;
		}
		t++;
	}
	o->v=sll_memory_move(o->v,SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(o);
}

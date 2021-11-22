#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>



#define FLAGS_PAD_ZERO 1
#define FLAGS_SIGN 2
#define FLAGS_SPACE_SIGN 4
#define FLAGS_JUSTIFY_LEFT 8
#define FLAGS_ALTERNATIVE_FORM 16
#define FLAGS_PERCISION 32
#define FLAGS_HH_BITS 64
#define FLAGS_H_BITS 128
#define FLAGS_L_BITS 256
#define FLAGS_LL_BITS 512
#define FLAGS_UPPERCASE 1024



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
				f|=FLAGS_PAD_ZERO;
			}
			else if (*t=='+'){
				f|=FLAGS_SIGN;
			}
			else if (*t==' '){
				f|=FLAGS_SPACE_SIGN;
			}
			else if (*t=='-'){
				f|=FLAGS_JUSTIFY_LEFT;
			}
			else if (*t=='#'){
				f|=FLAGS_ALTERNATIVE_FORM;
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
				f|=FLAGS_JUSTIFY_LEFT;
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
			f|=FLAGS_PERCISION;
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
				f|=FLAGS_HH_BITS;
				t++;
			}
			else{
				f|=FLAGS_H_BITS;
			}
		}
		else if (*t=='l'){
			t++;
			if (*t=='l'){
				f|=FLAGS_LL_BITS;
				t++;
			}
			else{
				f|=FLAGS_L_BITS;
			}
		}
		else if (*t=='j'||*t=='t'||*t=='z'){
			f|=FLAGS_LL_BITS;
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
			if (!(f&FLAGS_JUSTIFY_LEFT)){
				sll_set_memory(o->v+o->l,w,' ');
				o->l+=w;
			}
			o->v[o->l]=(sll_char_t)va_arg(va,int);
			o->l++;
			if (f&FLAGS_JUSTIFY_LEFT){
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
				if (!(f&FLAGS_PERCISION)){
					l=sll_string_length_unaligned(s);
				}
				sll_string_increase(o,(l>w?l:w));
				w=(w<l?0:w-l);
				if (!(f&FLAGS_JUSTIFY_LEFT)){
					sll_set_memory(o->v+o->l,w,' ');
					o->l+=w;
				}
				sll_copy_data(s,l,o->v+o->l);
				o->l+=l;
				if (f&FLAGS_JUSTIFY_LEFT){
					sll_set_memory(o->v+o->l,w,' ');
					o->l+=w;
				}
			}
		}
		else if (*t=='d'||*t=='i'||*t=='o'||*t=='u'||*t=='x'||*t=='X'){
			if (*t=='X'){
				f|=FLAGS_UPPERCASE;
			}
			uint64_t n;
			if (*t=='d'||*t=='i'){
				int64_t sn;
				if (f&FLAGS_HH_BITS){
					sn=va_arg(va,signed int);
					if (sn<-0x80){
						sn=-0x80;
					}
					else if (sn>0x7f){
						sn=0x7f;
					}
				}
				else if (f&FLAGS_H_BITS){
					sn=va_arg(va,signed int);
					if (sn<-0x8000){
						sn=-0x8000;
					}
					else if (sn>0x7fff){
						sn=0x7fff;
					}
				}
				else if (f&FLAGS_L_BITS){
					sn=va_arg(va,signed long int);
				}
				else if (f&FLAGS_LL_BITS){
					sn=va_arg(va,signed long long int);
				}
				else{
					sn=va_arg(va,signed int);
				}
				if ((f&(FLAGS_SIGN|FLAGS_SPACE_SIGN))||sn<0){
					sll_string_increase(o,1);
					o->v[o->l]=(sn<0?'-':'+');
					o->l++;
				}
				n=(uint64_t)(sn<0?-sn:sn);
			}
			else{
				if (f&FLAGS_HH_BITS){
					n=va_arg(va,unsigned int)&0xff;
				}
				else if (f&FLAGS_H_BITS){
					n=va_arg(va,unsigned int)&0xffff;
				}
				else if (f&FLAGS_L_BITS){
					n=va_arg(va,unsigned long int);
				}
				else if (f&FLAGS_LL_BITS){
					n=va_arg(va,unsigned long long int);
				}
				else{
					n=va_arg(va,unsigned int);
				}

				if (f&(FLAGS_SIGN|FLAGS_SPACE_SIGN)){
					sll_string_increase(o,1);
					o->v[o->l]='+';
					o->l++;
				}
			}
			if (!(f&FLAGS_PERCISION)){
				p=1;
			}
			if (!p&&(f&FLAGS_PERCISION)&&!n){
				if ((f&FLAGS_ALTERNATIVE_FORM)&&*t=='o'&&!p){
					p=1;
				}
				sll_string_increase(o,p);
				sll_set_memory(o->v+o->l,p,'0');
				o->l+=p;
			}
			else{
				if ((f&FLAGS_ALTERNATIVE_FORM)&&*t=='o'){
					sll_string_increase(o,1);
					o->v[o->l]='0';
					o->l++;
					if (p>0){
						p--;
					}
				}
				else if ((f&FLAGS_ALTERNATIVE_FORM)&&(*t=='x'||*t=='X')){
					sll_string_increase(o,2);
					o->v[o->l]='0';
					o->v[o->l+1]=(f&FLAGS_UPPERCASE?'X':'x');
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
					uint8_t sz=0;
					uint64_t nv=n;
					while (nv){
						nv>>=3;
						sz++;
					}
					if (p<sz){
						p=sz;
					}
					sll_string_increase(o,p);
					p-=sz;
					if (!(f&FLAGS_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
					uint8_t i=sz*3;
					do{
						i-=3;
						o->v[o->l]=48+((n>>i)&7);
						o->l++;
					} while (i);
					if (f&FLAGS_JUSTIFY_LEFT){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
				}
				else if (*t!='x'&&*t!='X'){
					uint8_t sz=1;
					uint64_t pw=10;
					while (pw<n+1){
						sz++;
						if (sz==20){
							break;
						}
						pw*=10;
					}
					if (p<sz){
						p=sz;
					}
					sll_string_increase(o,p);
					p-=sz;
					if (!(f&FLAGS_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
					if (sz!=20){
						pw/=10;
					}
					do{
						o->v[o->l]=48+((n/pw)%10);
						o->l++;
						sz--;
						pw/=10;
					} while (sz);
					if (f&FLAGS_JUSTIFY_LEFT){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
				}
				else{
					uint8_t sz=0;
					uint64_t nv=n;
					while (nv>0){
						nv>>=4;
						sz++;
					}
					if (p<sz){
						p=sz;
					}
					sll_string_increase(o,p);
					p-=sz;
					if (!(f&FLAGS_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
					uint8_t i=sz<<2;
					sll_char_t e=((f&FLAGS_UPPERCASE)?55:87);
					do{
						i-=4;
						sll_char_t c=(n>>i)&0xf;
						o->v[o->l]=c+(c>9?e:48);
						o->l++;
					} while (i);
					if (f&FLAGS_JUSTIFY_LEFT){
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

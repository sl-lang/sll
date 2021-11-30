#include <sll/_sll_internal.h>
#include <sll/common.h>
#include <sll/memory.h>
#include <sll/string.h>
#include <sll/types.h>
#include <sll/util.h>
#include <sll/var_arg.h>
#include <stdarg.h>
#include <stdint.h>



static const uint64_t _string_pow_of_10[]={1ull,10ull,100ull,1000ull,10000ull,100000ull,1000000ull,10000000ull,100000000ull,1000000000ull,10000000000ull,100000000000ull,1000000000000ull,10000000000000ull,100000000000000ull,1000000000000000ull,10000000000000000ull,100000000000000000ull,1000000000000000000ull,10000000000000000000ull};



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_format(const sll_char_t* t,sll_string_t* o,...){
	va_list va;
	va_start(va,o);
	sll_var_arg_list_t dt={
		SLL_VAR_ARG_LIST_TYPE_C,
		{
			.c=&va
		}
	};
	sll_bool_t so=sll_string_format_list(t,sll_string_length_unaligned(t),&dt,o);
	va_end(va);
	return so;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_bool_t sll_string_format_list(const sll_char_t* t,sll_string_length_t sl,sll_var_arg_list_t* va,sll_string_t* o){
	if (!sll_string_create(0,o)){
		return 0;
	}
	if (!sl){
		return 1;
	}
	o->v=sll_memory_move(o->v,SLL_MEMORY_MOVE_DIRECTION_TO_STACK);
	sll_string_length_t i=sl;
	while (i){
		if (*t!='%'){
			if (!sll_string_increase(o,1)){
				sll_deallocate(o->v);
				return 0;
			}
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
			if (!sll_string_increase(o,1)){
				sll_deallocate(o->v);
				return 0;
			}
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
			if (!sll_string_increase(o,1)){
				sll_deallocate(o->v);
				return 0;
			}
			o->v[o->l]='%';
			o->l++;
		}
		else if (*t=='c'){
			if (!w){
				w=1;
			}
			if (!sll_string_increase(o,w)){
				sll_deallocate(o->v);
				return 0;
			}
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
			if (!sll_string_increase(o,(l>w?l:w))){
				sll_deallocate(o->v);
				return 0;
			}
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
			uint64_t n;
			if (*t=='d'){
				int64_t sn;
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
					if (!sll_string_increase(o,1)){
						sll_deallocate(o->v);
						return 0;
					}
					o->v[o->l]=(sn<0?'-':'+');
					o->l++;
				}
				n=(uint64_t)(sn<0?-sn:sn);
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
					if (!sll_string_increase(o,1)){
						sll_deallocate(o->v);
						return 0;
					}
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
				if (!sll_string_increase(o,p)){
					sll_deallocate(o->v);
					return 0;
				}
				sll_set_memory(o->v+o->l,p,'0');
				o->l+=p;
			}
			else{
				if ((f&STRING_FORMAT_FLAG_ALTERNATIVE_FORM)&&*t=='o'){
					if (!sll_string_increase(o,1)){
						sll_deallocate(o->v);
						return 0;
					}
					o->v[o->l]='0';
					o->l++;
					if (p>0){
						p--;
					}
				}
				else if ((f&STRING_FORMAT_FLAG_ALTERNATIVE_FORM)&&(*t=='x'||*t=='X')){
					if (!sll_string_increase(o,2)){
						sll_deallocate(o->v);
						return 0;
					}
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
					if (!sll_string_increase(o,p)){
						sll_deallocate(o->v);
						return 0;
					}
					sll_set_memory(o->v+o->l,p,'0');
					o->l+=p;
				}
				else if (*t=='o'){
					uint8_t sz=(FIND_LAST_SET_BIT(n)+2)/3;
					if (p<sz){
						p=sz;
					}
					if (!sll_string_increase(o,p)){
						sll_deallocate(o->v);
						return 0;
					}
					p-=sz;
					if (p&&!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
					uint8_t j=sz*3;
					do{
						j-=3;
						o->v[o->l]=48+((n>>j)&7);
						o->l++;
					} while (j);
					if (p&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
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
					if (!sll_string_increase(o,p)){
						sll_deallocate(o->v);
						return 0;
					}
					p-=sz;
					if (p&&!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
					while (1){
						o->v[o->l]=48+((n/pw)%10);
						o->l++;
						if (sz<=1){
							break;
						}
						sz--;
						pw/=10;
					}
					if (p&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
				}
				else{
					uint8_t sz=(FIND_LAST_SET_BIT(n)+3)>>2;
					if (p<sz){
						p=sz;
					}
					if (!sll_string_increase(o,p)){
						sll_deallocate(o->v);
						return 0;
					}
					p-=sz;
					if (p&&!(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
					uint8_t j=sz<<2;
					sll_char_t e=((f&STRING_FORMAT_FLAG_UPPERCASE)?55:87);
					do{
						j-=4;
						sll_char_t c=(n>>j)&15;
						o->v[o->l]=c+(c>9?e:48);
						o->l++;
					} while (j);
					if (p&&(f&STRING_FORMAT_FLAG_JUSTIFY_LEFT)){
						sll_set_memory(o->v+o->l,p,'0');
						o->l+=p;
					}
				}
			}
		}
		else if (*t=='p'){
			if (!sll_string_increase(o,16)){
				sll_deallocate(o->v);
				return 0;
			}
			uint64_t ptr=(uint64_t)sll_var_arg_get(va);
			for (int8_t j=60;j>=0;j-=4){
				sll_char_t c=(ptr>>j)&15;
				o->v[o->l]=c+(c>9?87:48);
				o->l++;
			}
		}
		else{
			if (!sll_string_increase(o,1)){
				sll_deallocate(o->v);
				return 0;
			}
			o->v[o->l]='%';
			o->l++;
			i=bi;
			t=b;
			continue;
		}
		i--;
		t++;
	}
	o->v=sll_memory_move(o->v,SLL_MEMORY_MOVE_DIRECTION_FROM_STACK);
	sll_string_calculate_checksum(o);
	return 1;
}

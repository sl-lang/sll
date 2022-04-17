#include <sll/_internal/intrinsics.h>
#include <sll/common.h>
#include <sll/string.h>
#include <sll/types.h>



static const sll_size_t _int_pow_10[]={1ull,10ull,100ull,1000ull,10000ull,100000ull,1000000ull,10000000ull,100000000ull,1000000000ull,10000000000ull,100000000000ull,1000000000000ull,10000000000000ull,100000000000000ull,1000000000000000ull,10000000000000000ull,100000000000000000ull,1000000000000000000ull,10000000000000000000ull};



__SLL_EXTERNAL __SLL_API_CALL void sll_api_int_to_bin(sll_integer_t a,sll_string_t* out){
	if (!a){
		sll_string_from_char('0',out);
		return;
	}
	sll_bool_t neg=(a<0);
	sll_size_t v=(neg?-a:a);
	sll_string_create(FIND_LAST_SET_BIT(v)+neg+1,out);
	if (neg){
		out->v[0]='-';
	}
	sll_string_length_t i=out->l;
	do{
		i--;
		out->v[i]=(v&1)+48;
		v>>=1;
	} while (v);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_int_to_dec(sll_integer_t a,sll_string_t* out){
	if (!a){
		sll_string_from_char('0',out);
		return;
	}
	sll_bool_t neg=(a<0);
	sll_size_t v=(neg?-a:a);
	sll_string_length_t l=((FIND_LAST_SET_BIT(v)+2)*1233)>>12;
	if (v<_int_pow_10[l]){
		l--;
	}
	l+=neg+1;
	sll_string_create(l,out);
	if (neg){
		out->v[0]='-';
	}
	do{
		l--;
		out->v[l]=(v%10)+48;
		v/=10;
	} while (v);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_int_to_hex(sll_integer_t a,sll_string_t* out){
	if (!a){
		sll_string_from_char('0',out);
		return;
	}
	sll_bool_t neg=(a<0);
	sll_size_t v=(neg?-a:a);
	sll_string_create((FIND_LAST_SET_BIT(v)>>2)+neg+1,out);
	if (neg){
		out->v[0]='-';
	}
	sll_string_length_t i=out->l;
	do{
		i--;
		out->v[i]=(v&15)+((v&15)>9?87:48);
		v>>=4;
	} while (v);
	sll_string_calculate_checksum(out);
}



__SLL_EXTERNAL __SLL_API_CALL void sll_api_int_to_oct(sll_integer_t a,sll_string_t* out){
	if (!a){
		sll_string_from_char('0',out);
		return;
	}
	sll_bool_t neg=(a<0);
	sll_size_t v=(neg?-a:a);
	sll_string_create(FIND_LAST_SET_BIT(v)/3+neg+1,out);
	if (neg){
		out->v[0]='-';
	}
	sll_string_length_t i=out->l;
	do{
		i--;
		out->v[i]=(v&7)+48;
		v>>=3;
	} while (v);
	sll_string_calculate_checksum(out);
}

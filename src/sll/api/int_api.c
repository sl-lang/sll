#include <sll/_sll_internal.h>
#include <sll/api.h>
#include <sll/common.h>
#include <sll/types.h>



__API_FUNC(int_to_bin){
	if (!a){
		sll_string_from_char('0',out);
		return;
	}
	sll_bool_t neg=(a<0);
	uint64_t v=(neg?-a:a);
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



__API_FUNC(int_to_hex){
	if (!a){
		sll_string_from_char('0',out);
		return;
	}
	sll_bool_t neg=(a<0);
	uint64_t v=(neg?-a:a);
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



__API_FUNC(int_to_oct){
	if (!a){
		sll_string_from_char('0',out);
		return;
	}
	sll_bool_t neg=(a<0);
	uint64_t v=(neg?-a:a);
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

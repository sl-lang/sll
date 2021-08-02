#include <lll/_lll_internal.h>
#include <lll/common.h>
#include <lll/string.h>
#include <lll/types.h>



__LLL_FUNC __LLL_RETURN_COMPARE lll_string_compare(const lll_string_t* a,const lll_string_t* b){
	if (a->l!=b->l){
		return (a->l<b->l?LLL_COMPARE_RESULT_BELOW:LLL_COMPARE_RESULT_ABOVE);
	}
	for (lll_string_length_t i=0;i<a->l;i++){
		lll_char_t c=a->v[i];
		lll_char_t d=b->v[i];
		if (c!=d){
			if (c<d){
				return LLL_COMPARE_RESULT_BELOW;
			}
			else if (c>d){
				return LLL_COMPARE_RESULT_ABOVE;
			}
		}
	}
	return LLL_COMPARE_RESULT_EQUAL;
}



__LLL_FUNC __LLL_RETURN_COMPARE lll_string_compare_ascii(const lll_string_t* a,const char* b){
	lll_string_length_t i=0;
	while (*b){
		lll_char_t c=a->v[i];
		lll_char_t d=*b;
		if (c!=d){
			if (c<d){
				return LLL_COMPARE_RESULT_BELOW;
			}
			else if (c>d){
				return LLL_COMPARE_RESULT_ABOVE;
			}
		}
		i++;
		b++;
	}
	return LLL_COMPARE_RESULT_EQUAL;
}



__LLL_FUNC __LLL_RETURN_COMPARE lll_string_compare_ignore_case(const lll_string_t* a,const lll_string_t* b){
	if (a->l!=b->l){
		return (a->l<b->l?LLL_COMPARE_RESULT_BELOW:LLL_COMPARE_RESULT_ABOVE);
	}
	for (lll_string_length_t i=0;i<a->l;i++){
		lll_char_t c=a->v[i];
		lll_char_t d=b->v[i];
		if (c>64&&c<91){
			c+=32;
		}
		if (d>64&&d<91){
			d+=32;
		}
		if (c!=d){
			if (c<d){
				return LLL_COMPARE_RESULT_BELOW;
			}
			else if (c>d){
				return LLL_COMPARE_RESULT_ABOVE;
			}
		}
	}
	return LLL_COMPARE_RESULT_EQUAL;
}



__LLL_FUNC __LLL_RETURN_COMPARE lll_string_compare_ignore_case_ascii(const lll_string_t* a,const char* b){
	lll_string_length_t i=0;
	while (*b){
		lll_char_t c=a->v[i];
		lll_char_t d=*b;
		if (c>64&&c<91){
			c+=32;
		}
		if (d>64&&d<91){
			d+=32;
		}
		if (c!=d){
			if (c<d){
				return LLL_COMPARE_RESULT_BELOW;
			}
			else if (c>d){
				return LLL_COMPARE_RESULT_ABOVE;
			}
		}
		i++;
		b++;
	}
	return LLL_COMPARE_RESULT_EQUAL;
}

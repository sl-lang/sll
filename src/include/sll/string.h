#ifndef __SLL_STRING_H__
#define __SLL_STRING_H__ 1
#include <sll/common.h>
#include <sll/types.h>
#include <stdlib.h>
#include <string.h>



#define SLL_ZERO_STRING(s) \
	do{ \
		(s)->l=0; \
		(s)->c=0; \
		(s)->v=NULL; \
	} while (0)
#define SLL_STRING_FROM_STATIC(s,o) \
	do{ \
		(o)->l=sizeof(s)/sizeof(sll_char_t)-1; \
		(o)->v=SLL_CHAR((s)); \
		sll_string_hash((o)); \
	} while (0)
#define SLL_ZERO_STRING_STRUCT {0,0,NULL}
#define SLL_STRING_ESCAPE(c) ((c)=='\t'||(c)=='\n'||(c)=='\v'||(c)=='\f'||(c)=='\r'||(c)=='\"'||(c)=='\''||(c)=='\\')
#define SLL_STRING_HEX_ESCAPE(c) ((c)<32||(c)>126)
#define SLL_STRING_ALIGN 8
#define SLL_STRING_ALIGN_LENGTH(l) (((l)+SLL_STRING_ALIGN)&(-SLL_STRING_ALIGN))
#define SLL_STRING_FORMAT_PADDING(v,l) memset((v)+(l),0,SLL_STRING_ALIGN_LENGTH((l))-(l))
#define SLL_STRING_INCREASE(s) \
	do{ \
		if (!(((s)->l+1)&(SLL_STRING_ALIGN-1))){ \
			(s)->v=realloc((s)->v,SLL_STRING_ALIGN_LENGTH((s)->l+1)*sizeof(sll_char_t)); \
			SLL_STRING_FORMAT_PADDING((s)->v,(s)->l+1); \
		} \
	} while (0)
#define SLL_CHAR(x) ((sll_char_t*)(x))
#define SLL_STRING_COMBINE_CHECKSUMS(a,l,b) (((sll_string_checksum_t)(a))^((((sll_string_checksum_t)(b))>>(((l)&3)<<3))|(((sll_string_checksum_t)(b))<<((4-((l)&3))<<3))))
#define SLL_STRING_COMBINE_CHECKSUMS_FAST(a,l,b) (((sll_string_checksum_t)(a))^sll_rotate_bits((b),((l)&3)<<3))



__SLL_FUNC void sll_string_and(const sll_string_t* s,sll_char_t v,sll_string_t* o);



__SLL_FUNC void sll_string_and_string(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



__SLL_FUNC void sll_string_clone(const sll_string_t* s,sll_string_t* d);



__SLL_FUNC void sll_string_combinations(const sll_string_t* a,const sll_string_t* b,sll_array_t* o);



__SLL_FUNC sll_string_length_t sll_string_count(const sll_string_t* a,const sll_string_t* b);



__SLL_FUNC void sll_string_create(sll_string_length_t l,sll_string_t* o);



__SLL_FUNC void sll_string_duplicate(const sll_string_t* s,sll_integer_t v,sll_string_length_t e,sll_string_t* o);



__SLL_FUNC void sll_string_from_data(sll_runtime_object_t** v,sll_string_length_t vl,sll_string_t* o);



__SLL_FUNC void sll_string_from_pointer(const sll_char_t* s,sll_string_t* o);



__SLL_FUNC void sll_string_hash(sll_string_t* s);



__SLL_FUNC void sll_string_join(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



__SLL_FUNC sll_string_length_t sll_string_length(const sll_char_t* s);



__SLL_FUNC void sll_string_lower_case(const sll_string_t* s,sll_string_t* o);



__SLL_FUNC void sll_string_or(const sll_string_t* s,sll_char_t v,sll_string_t* o);



__SLL_FUNC void sll_string_or_string(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



__SLL_FUNC void sll_string_remove(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



__SLL_FUNC void sll_string_replace(const sll_string_t* s,const sll_string_t* k,const sll_string_t* v,sll_string_t* o);



__SLL_FUNC void sll_string_title_case(const sll_string_t* s,sll_string_t* o);



__SLL_FUNC void sll_string_upper_case(const sll_string_t* s,sll_string_t* o);



__SLL_FUNC void sll_string_to_array(const sll_string_t* s,sll_array_t* o);



__SLL_FUNC void sll_string_xor(const sll_string_t* s,sll_char_t v,sll_string_t* o);



__SLL_FUNC void sll_string_xor_string(const sll_string_t* a,const sll_string_t* b,sll_string_t* o);



#endif

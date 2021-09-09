#ifndef __SLL_STRING_H__
#define __SLL_STRING_H__ 1
#include <sll/common.h>
#include <sll/types.h>



#define SLL_SAME_STRING(a,b) ((a)->v==(b)->v)
#define SLL_ZERO_STRING(s) \
	do{ \
		(s)->l=0; \
		(s)->c=0; \
		(s)->v=NULL; \
	} while (0)
#define SLL_STRING_FROM_STATIC(s,o) \
	do{ \
		(o)->l=sizeof(s)-1; \
		(o)->v=(sll_char_t*)(s); \
		sll_string_hash((o)); \
	} while (0)
#define SLL_ZERO_STRING_STRUCT {0,0,NULL}
#define SLL_STRING_ESCAPE(c) ((c)=='\t'||(c)=='\n'||(c)=='\v'||(c)=='\f'||(c)=='\r'||(c)=='\"'||(c)=='\''||(c)=='\\')
#define SLL_STRING_HEX_ESCAPE(c) ((c)<32||(c)>126)
#define SLL_STRING_ALIGN 8
#define SLL_STRING_ALIGN_LENGTH(l) (((l)+SLL_STRING_ALIGN)&(-SLL_STRING_ALIGN))
#define SLL_STRING_INCREASE(s) \
	do{ \
		if (!(((s)->l+1)&(SLL_STRING_ALIGN-1))){ \
			(s)->v=realloc((s)->v,SLL_STRING_ALIGN_LENGTH((s)->l+1)*sizeof(sll_char_t)); \
		} \
	} while (0)



__SLL_FUNC void sll_string_create(sll_string_length_t l,sll_string_t* o);



__SLL_FUNC void sll_string_clone(const sll_string_t* s,sll_string_t* d);



__SLL_FUNC void sll_string_hash(sll_string_t* s);



#endif

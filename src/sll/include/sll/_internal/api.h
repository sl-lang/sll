#ifndef __SLL__INTERNAL_API_H__
#define __SLL__INTERNAL_API_H__ 1
#include <sll/common.h>
#include <sll/types.h>



typedef struct _INTERNAL_FUNCTION{
	const sll_char_t* nm;
	const sll_internal_function_pointer_t f;
	const sll_char_t* fmt;
} internal_function_t;



extern const sll_function_index_t _ifunc_size;
extern const internal_function_t* _ifunc_data;



#endif

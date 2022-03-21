#ifndef __SLL__INTERNAL_API_H__
#define __SLL__INTERNAL_API_H__ 1
#include <sll/generated/api.h>
#include <sll/common.h>
#include <sll/types.h>



#define __API_FUNC(nm) __SLL_EXTERNAL __SLL_API_TYPE_sll_api_##nm sll_api_##nm(__SLL_API_ARGS_sll_api_##nm)



typedef struct __INTERNAL_FUNCTION{
	const sll_char_t* nm;
	const sll_internal_function_pointer_t f;
} internal_function_t;



#endif

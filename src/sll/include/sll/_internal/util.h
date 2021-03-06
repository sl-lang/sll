#ifndef __SLL__INTERNAL_UTIL_H__
#define __SLL__INTERNAL_UTIL_H__ 1
#include <sll/_size_types.h>
#include <sll/types.h>



#define WRAP_ARRAY_INDEX(idx,len,var) \
	sll_integer_t var=(idx); \
	WRAP_ARRAY_INDEX_VAR(var,(len));
#define WRAP_ARRAY_INDEX_VAR(idx,len) \
	do{ \
		sll_integer_t __len=(len); \
		if (idx<0){ \
			idx+=__len; \
			if (idx<0){ \
				idx=-1; \
			} \
		} \
		else if (idx>=__len){ \
			idx=__len; \
		} \
	} while (0)



typedef union _F32_DATA{
	__SLL_F32 value;
	__SLL_U32 data;
} f32_data_t;



typedef union _F64_DATA{
	__SLL_F64 value;
	__SLL_U64 data;
} f64_data_t;



#endif

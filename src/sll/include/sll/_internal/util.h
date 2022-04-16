#ifndef __SLL__INTERNAL_UTIL_H__
#define __SLL__INTERNAL_UTIL_H__ 1
#include <sll/_size_types.h>
#include <sll/types.h>



#define MAX_CLEANUP_TABLE_SIZE 512

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



typedef union _FLOAT_DATA{
	float v;
	__SLL_U32 dt;
} float_data_t;



typedef union _DOUBLE_DATA{
	double v;
	__SLL_U64 dt;
} double_data_t;



#endif

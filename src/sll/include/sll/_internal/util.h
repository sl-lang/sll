#ifndef __SLL_INTERNAL_UTIL_H__
#define __SLL_INTERNAL_UTIL_H__ 1
#include <sll/_size_types.h>
#include <sll/types.h>



#define MAX_CLEANUP_TABLE_SIZE 512

#define WRAP_ARRAY_INDEX(idx,len,var) \
	sll_integer_t var; \
	do{ \
		sll_integer_t __idx=(idx); \
		sll_integer_t __len=(len); \
		var=__idx-__idx/__len*__len; \
		if (var<0){ \
			var+=__len; \
		} \
	} while (0)
#define WRAP_ARRAY_INDEX_VAR(idx,len) \
	do{ \
		sll_integer_t __len=(len); \
		idx=idx-idx/__len*__len; \
		if (idx<0){ \
			idx+=__len; \
		} \
	} while (0)



typedef union __FLOAT_DATA{
	float v;
	__SLL_U32 dt;
} float_data_t;



typedef union __DOUBLE_DATA{
	double v;
	__SLL_U64 dt;
} double_data_t;



#endif

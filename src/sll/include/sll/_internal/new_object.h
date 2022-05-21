#ifndef __SLL__INTERNAL_NEW_OBJECT_H__
#define __SLL__INTERNAL_NEW_OBJECT_H__ 1
#include <sll/_size_types.h>



#define NEW_OBJECT_FLAG_NO_ACQUIRE 1
#define NEW_OBJECT_FLAG_ARRAY 2
#define NEW_OBJECT_FLAG_DEREF 4



typedef __SLL_U8 object_creation_flags_t;



typedef struct _STRUCT_OFFSET_BUILDER{
	sll_size_t* off;
	sll_arg_count_t l;
	void** fn;
	sll_arg_count_t fnl;
} struct_offset_builder_t;



#endif

#ifndef __SLL__INTERNAL_NEW_OBJECT_H__
#define __SLL__INTERNAL_NEW_OBJECT_H__ 1
#include <sll/_size_types.h>



#define NEW_OBJECT_FLAG_NO_ACQUIRE 1
#define NEW_OBJECT_FLAG_ARRAY 2
#define NEW_OBJECT_FLAG_DEREF 4



typedef __SLL_U8 object_creation_flags_t;



typedef struct _STRUCT_OFFSET_BUILDER{
	sll_size_t* offset_data;
	sll_arg_count_t offset_count;
	void** converter_function_data;
	sll_arg_count_t converter_function_count;
} struct_offset_builder_t;



#endif

#ifndef __SLL__INTERNAL_NEW_OBJECT_H__
#define __SLL__INTERNAL_NEW_OBJECT_H__ 1



#define NEW_OBJECT_FLAG_NO_ACQUIRE 1
#define NEW_OBJECT_FLAG_ARRAY 2



typedef struct __STRUCT_OFFSET_BUILDER{
	sll_size_t* off;
	sll_arg_count_t l;
	void** fn;
	sll_arg_count_t fnl;
} struct_offset_builder_t;



#endif

#ifndef __SLL__INTERNAL_PARSE_ARGS_H__
#define __SLL__INTERNAL_PARSE_ARGS_H__ 1
#include <sll/types.h>



#define ARG_OUTPUT_TYPE_C 0
#define ARG_OUTPUT_TYPE_ARRAY 1

#define PARSE_ARGS_FLAG_ARRAY 1
#define PARSE_ARGS_FLAG_REF 2
#define PARSE_ARGS_FLAG_CONST 4



typedef struct _ARG_STATE{
	sll_size_t sz;
	sll_object_t* dt[];
} arg_state_t;



typedef struct _ARG_OUTPUT_ARRAY_DATA{
	void* ptr;
	sll_size_t sz;
} arg_output_array_data_t;



typedef union _ARG_OUTPUT_DATA{
	va_list* c;
	arg_output_array_data_t arr;
} arg_output_data_t;



typedef struct _ARG_OUTPUT{
	sll_bool_t t;
	arg_output_data_t dt;
} arg_output_t;



sll_arg_count_t _parse_arg_count(const sll_char_t* t,__SLL_U16 ret,bitmap_t** regs,sll_size_t* o);



sll_arg_state_t _parse_args_raw(const sll_char_t* t,sll_object_t*const* al,sll_arg_count_t all,arg_output_t* va);



#endif

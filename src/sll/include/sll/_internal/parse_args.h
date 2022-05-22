#ifndef __SLL__INTERNAL_PARSE_ARGS_H__
#define __SLL__INTERNAL_PARSE_ARGS_H__ 1
#include <sll/_size_types.h>
#include <sll/gc.h>
#include <sll/parse_args.h>
#include <sll/types.h>
#include <stdarg.h>



#define ARG_OUTPUT_TYPE_C 0
#define ARG_OUTPUT_TYPE_ARRAY 1

#define PARSE_ARGS_FLAG_ARRAY 1
#define PARSE_ARGS_FLAG_REF 2
#define PARSE_ARGS_FLAG_CONST 4
#define PARSE_ARGS_FLAG_NULL 8



typedef __SLL_U8 arg_parse_flags_t;



typedef struct _ARG_STATE{
	sll_size_t length;
	sll_object_t* data[];
} arg_state_t;



typedef struct _ARG_OUTPUT_ARRAY_DATA{
	void* pointer;
	sll_size_t count;
} arg_output_array_data_t;



typedef union _ARG_OUTPUT_DATA{
	va_list* c;
	arg_output_array_data_t array;
} arg_output_data_t;



typedef struct _ARG_OUTPUT{
	sll_bool_t type;
	arg_output_data_t data;
} arg_output_t;



sll_arg_count_t _parse_arg_count(const sll_char_t* t,__SLL_U16 ret,bitmap_t** regs,sll_size_t* o);



sll_arg_state_t _parse_args_raw(const sll_char_t* t,sll_object_t*const* al,sll_arg_count_t all,arg_output_t* va);



#endif

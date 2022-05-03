#ifndef __SLL__INTERNAL_API_CALL_H__
#define __SLL__INTERNAL_API_CALL_H__ 1
#include <sll/_internal/common.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/gc.h>
#include <sll/ift.h>
#include <sll/parse_args.h>
#include <sll/string.h>
#include <sll/types.h>



#define RETURN_VALUE_GET_TYPE(r) ((r)&0xff)
#define RETURN_VALUE_FLAG_ERROR 256



typedef union _API_RETURN_VALUE{
	sll_bool_t b;
	sll_integer_t i;
	sll_char_t c;
	sll_complex_t d;
	sll_number_t x;
	sll_string_t s;
	sll_array_t a;
	sll_map_t m;
	sll_object_t* o;
	sll_error_t err;
} api_return_value_t;



sll_object_t* _call_api_func(sll_function_index_t fn,sll_object_t*const* al,sll_arg_count_t all);



__SLL_API_CALL sll_float_t _call_api_func_assembly(api_return_value_t* ret,const bitmap_t* regs,const void* data,sll_arg_count_t ac,sll_internal_function_pointer_t fn);



void _parse_api_call_format(const sll_char_t* fmt,sll_internal_function_t* o);



#endif

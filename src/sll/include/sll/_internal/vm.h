#ifndef __SLL__INTERNAL_VM_H__
#define __SLL__INTERNAL_VM_H__ 1
#include <sll/_internal/thread.h>
#include <sll/types.h>



#define EXECUTE_FUNCTION_NO_AUDIT_TERMINATE SLL_EXECUTE_FUNCTION_RESERVED0



typedef union _API_RETURN_VALUE{
	sll_bool_t b;
	sll_integer_t i;
	sll_char_t c;
	sll_complex_t d;
	sll_string_t s;
	sll_array_t a;
	sll_map_t m;
	sll_object_t* o;
} api_return_value_t;



__SLL_API_CALL sll_float_t _call_api_func(api_return_value_t* ret,const bitmap_t* regs,const void* data,sll_arg_count_t ac,sll_internal_function_pointer_t fn);



void _call_function(thread_data_t* thr,sll_function_index_t fn,sll_arg_count_t ac,sll_bool_t fr);



#endif

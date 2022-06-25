#ifndef __SLL__INTERNAL_API_ATEXIT_H__
#define __SLL__INTERNAL_API_ATEXIT_H__ 1
#include <sll/gc.h>
#include <sll/types.h>



typedef struct _ATEXIT_FUNCTION{
	sll_integer_t function;
	sll_arg_count_t arg_count;
	sll_object_t* args[];
} atexit_function_t;



void _atexit_execute(void);



#endif

#ifndef __SLL__INTERNAL_ATEXIT_H__
#define __SLL__INTERNAL_ATEXIT_H__ 1
#include <sll/types.h>



typedef struct __ATEXIT_FUNCTION{
	sll_integer_t fn;
	sll_arg_count_t all;
	sll_object_t* al[];
} atexit_function_t;



void _atexit_execute(void);



#endif

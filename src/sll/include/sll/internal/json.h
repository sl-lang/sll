#ifndef __SLL_INTERNAL_JSON_H__
#define __SLL_INTERNAL_JSON_H__ 1
#include <sll/types.h>



#define JSON_NUMBER_INT 0
#define JSON_NUMBER_FLOAT 1



typedef union __JSON_NUMBER{
	sll_integer_t i;
	sll_float_t f;
} json_number_t;



#endif

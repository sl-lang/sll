#ifndef __SLL__INTERNAL_JSON_H__
#define __SLL__INTERNAL_JSON_H__ 1
#include <sll/types.h>



#define JSON_NUMBER_INT 0
#define JSON_NUMBER_FLOAT 1



typedef union _JSON_NUMBER{
	sll_integer_t int_;
	sll_float_t float_;
} json_number_t;



#endif

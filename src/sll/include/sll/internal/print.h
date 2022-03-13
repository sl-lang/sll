#ifndef __SLL_INTERNAL_PRINT_H__
#define __SLL_INTERNAL_PRINT_H__ 1
#include <sll/file.h>
#include <sll/types.h>



#define STATIC_STRING_LEN(s) (sizeof(s)/sizeof(char)-1)
#define PRINT_STATIC_STRING(s,wf) sll_file_write((wf),(s),STATIC_STRING_LEN(s),NULL)



void _print_char(sll_char_t c,sll_file_t* wf);



void _print_float(sll_float_t v,sll_file_t* wf);



void _print_int(sll_integer_t v,sll_file_t* wf);



#endif

#ifndef __SLL__INTERNAL_PRINT_H__
#define __SLL__INTERNAL_PRINT_H__ 1
#include <sll/file.h>
#include <sll/types.h>



#define STATIC_STRING_LEN(s) (sizeof(s)/sizeof(char)-1)
#define PRINT_STATIC_STRING(s,wf) sll_file_write((wf),(s),STATIC_STRING_LEN(s),NULL)



#endif

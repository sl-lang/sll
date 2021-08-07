#ifndef __LLL_STRING_H__
#define __LLL_STRING_H__ 1
#include <lll/common.h>
#include <lll/types.h>



__LLL_FUNC void lll_reset_heap(void);



__LLL_FUNC lll_string_t* lll_string_create(lll_string_length_t l);



__LLL_FUNC void lll_string_release(lll_string_t* s);



#endif

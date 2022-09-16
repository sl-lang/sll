#ifndef __FONT_TRUE_TYPE_H__
#define __FONT_TRUE_TYPE_H__ 1
#include <font/common.h>
#include <sll.h>



__FONT_API_CALL void font_api_true_type_create(const sll_string_t* data,sll_array_t* out);



__FONT_API_CALL void font_api_true_type_find(const sll_string_t* name,sll_string_t* out);



#endif

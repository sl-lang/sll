#ifndef __CLIB_FUNCTION_H__
#define __CLIB_FUNCTION_H__ 1
#include <clib/common.h>
#include <sll.h>



__CLIB_API_CALL sll_object_t* clib_api_call_function(void* address,sll_bool_t return_type,const sll_array_t* args);



#endif

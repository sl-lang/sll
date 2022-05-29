#ifndef __CLIB_LIBRARY_H__
#define __CLIB_LIBRARY_H__ 1
#include <clib/common.h>
#include <sll.h>



__CLIB_API_CALL sll_library_handle_t clib_api_get_sll_library_handle(void);



__CLIB_API_CALL sll_library_handle_t clib_api_load_library(const sll_string_t* str);



#endif

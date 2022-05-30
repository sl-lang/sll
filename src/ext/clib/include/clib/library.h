#ifndef __CLIB_LIBRARY_H__
#define __CLIB_LIBRARY_H__ 1
#include <clib/common.h>
#include <sll.h>



__CLIB_API_CALL sll_library_handle_t clib_api_get_sll_library_handle(void);



__CLIB_API_CALL sll_error_t clib_api_load_library(const sll_string_t* path,sll_library_handle_t* out);



__CLIB_API_CALL void* clib_api_lookup_symbol(sll_library_handle_t lib,const sll_string_t* name);



__CLIB_API_CALL sll_bool_t clib_api_unload_library(sll_library_handle_t lib);



#endif

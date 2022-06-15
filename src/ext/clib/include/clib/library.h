#ifndef __CLIB_LIBRARY_H__
#define __CLIB_LIBRARY_H__ 1
#include <clib/common.h>
#include <sll.h>



__CLIB_API_CALL sll_library_handle_t clib_api_library_get_sll_handle(void);



__CLIB_API_CALL sll_error_t clib_api_library_load(const sll_string_t* name,sll_library_handle_t* out);



__CLIB_API_CALL void* clib_api_library_lookup_symbol(sll_library_handle_t lib,const sll_string_t* name);



__CLIB_API_CALL sll_error_t clib_api_library_unload(sll_library_handle_t lib);



#endif

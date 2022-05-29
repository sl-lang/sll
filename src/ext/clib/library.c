#include <clib/common.h>
#include <sll.h>
#include <stddef.h>



__CLIB_API_CALL sll_library_handle_t clib_api_get_sll_library_handle(void){
	return sll_platform_load_library(NULL,NULL);
}



__CLIB_API_CALL sll_library_handle_t clib_api_load_library(const sll_string_t* str){
	sll_error_t err;
	sll_library_handle_t out=sll_platform_load_library(str->data,&err);
	return (out?out:(void*)(~err));
}

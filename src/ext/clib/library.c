#include <clib/common.h>
#include <sll.h>
#include <stddef.h>



__CLIB_API_CALL sll_library_handle_t clib_api_get_sll_library_handle(void){
	return sll_platform_load_library(NULL,NULL);
}



__CLIB_API_CALL sll_error_t clib_api_load_library(const sll_string_t* path,sll_library_handle_t* out){
	sll_error_t err;
	*out=sll_platform_load_library(path->data,&err);
	sll_audit(SLL_CHAR("clib.library.load"),SLL_CHAR("si"),path,*out);
	return err;
}



__CLIB_API_CALL void* clib_api_lookup_symbol(sll_library_handle_t lib,const sll_string_t* name){
	void* addr=sll_platform_lookup_symbol(lib,name->data);
	sll_audit(SLL_CHAR("clib.library.lookup"),SLL_CHAR("isi"),lib,name,addr);
	return addr;
}



__CLIB_API_CALL sll_bool_t clib_api_unload_library(sll_library_handle_t lib){
	sll_audit(SLL_CHAR("clib.library.unload"),SLL_CHAR("i"),lib);
	return sll_platform_unload_library(lib)==SLL_NO_ERROR;
}
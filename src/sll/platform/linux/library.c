#include <sll/api/sys.h>
#include <sll/common.h>
#include <sll/types.h>
#include <dlfcn.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_library_handle_t sll_platform_load_library(const sll_char_t* fp){
	void* o=dlopen((char*)fp,RTLD_NOW);
	return (!o?SLL_UNKNOWN_LIBRARY_HANDLE:o);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_lookup_symbol(sll_library_handle_t h,const sll_char_t* nm){
	return dlsym(h,(char*)nm);
}



__SLL_EXTERNAL void sll_platform_unload_library(sll_library_handle_t h){
	dlclose(h);
}

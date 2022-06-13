#include <sll/_internal/common.h>
#include <sll/_internal/error.h>
#include <sll/api/path.h>
#include <sll/api/sys.h>
#include <sll/common.h>
#include <sll/error.h>
#include <sll/platform/library.h>
#include <sll/platform/path.h>
#include <sll/string.h>
#include <sll/types.h>
#include <dlfcn.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_library_file_path(sll_library_handle_t h,sll_char_t* fp,sll_string_length_t fpl,sll_error_t* err){
	ERROR_PTR_RESET;
	sll_char_t bf[SLL_API_MAX_FILE_PATH_LENGTH]={0};
	if (!dlinfo(h,RTLD_DI_ORIGIN,bf)){
		sll_string_length_t l=sll_string_length(bf);
		if (l>=fpl){
			l=fpl-1;
		}
		sll_copy_data(bf,l,fp);
		return l;
	}
	ERROR_PTR(sll_error_from_string_pointer(SLL_CHAR(dlerror())));
	return 0;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_library_handle_t sll_platform_load_library(const sll_char_t* fp,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!fp){
		void* out=dlopen((char*)(sll_library_file_path->data),RTLD_NOW|RTLD_GLOBAL|RTLD_NOLOAD);
		SLL_CRITICAL(!dlclose(out));
		return out;
	}
	void* out=dlopen((char*)fp,RTLD_NOW);
	if (!out){
		ERROR_PTR(sll_error_from_string_pointer(SLL_CHAR(dlerror())));
	}
	return out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_lookup_symbol(sll_library_handle_t h,const sll_char_t* nm){
	return dlsym(h,(char*)nm);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_unload_library(sll_library_handle_t h){
	return (dlclose(h)?sll_error_from_string_pointer(SLL_CHAR(dlerror())):SLL_NO_ERROR);
}

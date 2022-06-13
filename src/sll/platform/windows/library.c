#include <windows.h>
#include <sll/_internal/error.h>
#include <sll/_internal/platform.h>
#include <sll/api/path.h>
#include <sll/api/sys.h>
#include <sll/common.h>
#include <sll/platform/library.h>
#include <sll/error.h>
#include <sll/types.h>



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_string_length_t sll_platform_get_library_file_path(sll_library_handle_t h,sll_char_t* fp,sll_string_length_t fpl,sll_error_t* err){
	SLL_UNIMPLEMENTED();
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_library_handle_t sll_platform_load_library(const sll_char_t* fp,sll_error_t* err){
	ERROR_PTR_RESET;
	if (!fp){
		return _win_dll_handle;
	}
	HMODULE out=LoadLibraryExA(fp,NULL,0);
	if (!out){
		ERROR_PTR_SYSTEM;
	}
	return out;
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT void* sll_platform_lookup_symbol(sll_library_handle_t h,const sll_char_t* nm){
	return GetProcAddress(h,nm);
}



__SLL_EXTERNAL __SLL_CHECK_OUTPUT sll_error_t sll_platform_unload_library(sll_library_handle_t h){
	return (FreeLibrary(h)?SLL_NO_ERROR:sll_platform_get_error());
}

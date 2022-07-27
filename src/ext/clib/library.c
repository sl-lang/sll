#include <clib/common.h>
#include <sll.h>
#include <stddef.h>



__CLIB_API_CALL void clib_api_library_get_loaded_list(sll_array_t* out){
	sll_size_t count;
	const sll_loaded_library_t*const* data=sll_get_loaded_libraries(&count);
	sll_object_t tmp=sll_new_object(SLL_CHAR("{si}"),data,count,0,SLL_OFFSETOF(sll_loaded_library_t,name),SLL_OFFSETOF(sll_loaded_library_t,handle));
	*out=tmp->data.array;
	if (!sll_destroy_object(tmp)){
		SLL_INIT_ARRAY(out);
	}
}



__CLIB_API_CALL sll_library_handle_t clib_api_library_get_sll_handle(void){
	return sll_platform_load_library(NULL,NULL);
}



__CLIB_API_CALL sll_error_t clib_api_library_load(const sll_string_t* name,sll_array_t* out){
	sll_error_t err;
	sll_library_handle_t lib=sll_platform_load_library(name->data,&err);
	if (err==SLL_NO_ERROR){
		sll_char_t buffer[SLL_API_MAX_FILE_PATH_LENGTH];
		sll_string_length_t len=sll_platform_get_library_file_path(lib,buffer,SLL_API_MAX_FILE_PATH_LENGTH,&err);
		if (err==SLL_NO_ERROR){
			sll_new_object_array(SLL_CHAR("iS"),out,lib,buffer,len);
			sll_audit(SLL_CHAR("clib.library.load"),SLL_CHAR("siS"),name,lib,buffer,len);
			return SLL_NO_ERROR;
		}
	}
	sll_audit(SLL_CHAR("clib.library.load.error"),SLL_CHAR("si"),name,err);
	return err;
}



__CLIB_API_CALL void* clib_api_library_lookup_symbol(sll_library_handle_t lib,const sll_string_t* name){
	void* addr=sll_platform_lookup_symbol(lib,name->data);
	sll_audit(SLL_CHAR("clib.library.lookup"),SLL_CHAR("isi"),lib,name,addr);
	return addr;
}



__CLIB_API_CALL sll_error_t clib_api_library_unload(sll_library_handle_t lib){
	sll_audit(SLL_CHAR("clib.library.unload"),SLL_CHAR("i"),lib);
	return sll_platform_unload_library(lib);
}

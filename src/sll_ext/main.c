#include <sll_ext/common.h>
#include <sll_ext/version.h>
#include <sll.h>



static sll_object_t* _external_func(const sll_object_t*const* al,sll_arg_count_t all){
	SLL_LOG("External function call!");
	return SLL_FROM_INT(1);
}



__SLL_EXT_EXTERNAL sll_bool_t __sll_load(sll_version_t v){
	if (v!=SLL_EXT_VERSION){
		SLL_LOG("Invalid version!");
		return 0;
	}
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:test_func"),_external_func,0);
	SLL_LOG("Loaded extension library for v"SLL_EXT_VERSION_STRING"!");
	return 1;
}



__SLL_EXT_EXTERNAL void __sll_unload(void){
	SLL_LOG("Successfully unloaded extension library");
}

#include <sll_ext/common.h>
#include <sll_ext/debug.h>
#include <sll_ext/version.h>
#include <sll.h>



__SLL_EXT_EXTERNAL sll_bool_t __sll_load(sll_version_t v){
	if (v!=SLL_EXT_VERSION){
		SLL_LOG("Version mismatch!");
		return 0;
	}
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_get_call_stack"),sll_ext_api_debug_get_call_stack,0);
	sll_register_internal_function(sll_current_runtime_data->ift,SLL_CHAR("sll_ext:debug_set_type"),sll_ext_api_debug_set_type,0);
	return 1;
}



__SLL_EXT_EXTERNAL void __sll_unload(void){
}
